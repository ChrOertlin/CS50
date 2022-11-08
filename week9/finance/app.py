import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd, calculate_price, calculate_difference, calculate_addition, isValid

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userInfo = db.execute(
        "SELECT * FROM users INNER JOIN portfolio ON users.id = portfolio.user_id INNER JOIN companies ON portfolio.symbol = companies.symbol WHERE users.id = ?", session["user_id"])
    # For all company stocks get their current price and calculate current values
    if len(userInfo) != 0:
        valueInShares = 0
        for row in userInfo:
            row.update({"pricePerShare": lookup(row["symbol"])["price"]})
            row.update({"totalPrice": row["shares"] * row["pricePerShare"]})
            valueInShares += row["totalPrice"]
        return render_template("index.html", renderPortfolio=True, userInfo=userInfo, sumTotal=valueInShares + userInfo[0]["cash"], cash=userInfo[0]["cash"])
    else:
        userInfo = db.execute("SELECT * FROM users WHERE users.id = ?", session["user_id"])
        return render_template("index.html", renderPortfolio=False, cash=userInfo[0]["cash"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # set vars
        transactionType = "buy"
        if lookup(request.form.get("symbol")) is None:
            return apology("Requested Stock not found.",400)
        stock = lookup(request.form.get("symbol"))

        # check the shares input
        try:
            nShares = int(request.form.get("shares"))
        except:
            return apology("Please enter a number of shares > 0 for purchase.", 400)

        if nShares < 1:
            return apology("Please enter a number of shares > 0 for purchase.", 400)
        totalPrice = calculate_price(stock["price"], nShares)

        # Check if requested stock exists
        if stock is None:
            return apology("Requested Stock not found", 403)

        # Get the user who wants to buy shares
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # check if the buy price exceeds user's available money
        if user[0]["cash"] < totalPrice:
            return apology("Buy price exceeds available cash")

        # update user cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", calculate_difference(totalPrice,user[0]["cash"]), session["user_id"])

        # add company to company list if not already there
        company = db.execute("SELECT * FROM companies WHERE symbol = ?",
        stock["symbol"])
        if len(company) != 1:
            db.execute("INSERT INTO companies (symbol, companyName) VALUES(?, ?)",
            stock["symbol"], stock["name"])

        # add shares to user portfolio
        userPf = db.execute("SELECT * FROM portfolio WHERE symbol = ? AND user_id = ?",
        stock["symbol"], session["user_id"])
        if len(userPf) != 1:
            db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES(?, ?, ?)",
            session["user_id"], stock["symbol"], nShares)
        else:
            # calculate new shares total
            newShares = userPf[0]["shares"] + nShares
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
            newShares, session["user_id"], stock["symbol"])

        # add transaction to history
        db.execute("INSERT INTO history (user_id, symbol, shares, pricePerShare, transactionType, date) VALUES (?, ?, ?, ?, ?, ?)",
                session["user_id"],
                stock["symbol"],
                nShares,
                stock["price"],
                transactionType,
                datetime.datetime.now())
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute(
        "SELECT * FROM history INNER JOIN companies ON history.symbol = companies.symbol WHERE user_id = ?", session["user_id"])
    for elem in history:
        elem.update({"totalPrice": elem["shares"] * elem["pricePerShare"]})
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        lookupReturn = lookup(request.form.get("symbol"))
        # Check whether the stock symbol has been found by the api
        if lookupReturn is None:
            return apology("Stock symbol not found", 400)
        else:
            return render_template("quoted.html", quote=lookupReturn)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Check if password has been repeated
        elif not request.form.get("confirmation"):
            return apology("please repeat your password", 400)
        # Check if both password fields have the same value
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # check if password contains numbers / letters / special characters
        elif not isValid(request.form.get("password")):
            return apology("Password needs to contain letters, numbers and at least one special character $#@!*!")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        # Ensure username exists and password is correct
        if len(rows) == 1:
            return apology("username already exists", 400)

        # If user does not exist already insert username and password into the finance.db
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        #login user
        session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", request.form.get("username"))[0]["id"]

        # Redirect user to home page
        return redirect("/")
    # If the user clicks register show the register page
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # set vars
        transactionType = "sell"
        if lookup(request.form.get("symbol")) is None:
            return apology("Requested Stock not found.",400)
        stock = lookup(request.form.get("symbol"))

        # Check if the number of shares are positive
        try:
            nShares = int(request.form.get("shares"))
        except:
            return apology("Please enter a number of shares > 0 for purchase.", 400)

        if nShares < 1:
            return apology("Please enter a number of shares > 0 for purchase.", 400)

        totalPrice = calculate_price(stock["price"], nShares)
        # Check if requested stock exists
        if stock is None:
            return apology("Requested Stock not found", 403)

        # retrieve user portfolio userPf = db.execute()
        userPf = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?",
        session["user_id"], stock["symbol"])
        # check if the user has any shares from this company
        if len(userPf) != 1:
            return apology("No shares found")
        # Check if the user has the amount of shares to be sold
        if nShares > userPf[0]["shares"]:
            return apology("You do not own the amount of shares you want to sell.")

        # remove shares from portfolio
        residualShares = userPf[0]["shares"] - nShares
        # if there are no shares left remove company from portfolio
        if residualShares <= 0:
            db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])

        elif residualShares != 0:
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                residualShares, session["user_id"], stock["symbol"])

        # Update cash with price
        # Get the user who wants to buy shares
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        db.execute("UPDATE users SET cash = ? WHERE id = ?", calculate_addition(
            float(user[0]["cash"]), totalPrice), session["user_id"])

        # add transaction to history
        db.execute("INSERT INTO history (user_id, symbol, shares, pricePerShare, transactionType, date) VALUES (?, ?, ?, ?, ?, ?)",
            session["user_id"],
            stock["symbol"],
            nShares,
            stock["price"],
            transactionType,
            datetime.datetime.now())
        return redirect("/")

    else:
        stocks = db.execute("SELECT * FROM portfolio WHERE user_id = ?",
        session["user_id"])
        return render_template("sell.html",stocks = stocks)
