import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def calculate_price(sharePrice, nShares):
    return sharePrice * nShares


def calculate_difference(buyPrice,userCash):
    return userCash - buyPrice


def calculate_addition(cash,totalToAdd):
    return cash + totalToAdd


def isValid(pw):
    numbers  = set("".join(chr(c) for c in range(48, 58, 1)))
    special_chars = set("$#@!*?")
    upper = set("".join(chr(c) for c in range(65, 91, 1)))
    lower = set("".join(chr(c) for c in range(97, 123, 1)))
    if len(numbers.intersection(pw)) != 0 and len(special_chars.intersection(pw)) != 0 and len(upper.intersection(pw)) != 0 and len(lower.intersection(pw)) != 0:
        return True
    return False



