# TODO
from cs50 import get_string

def main():
    userInput = get_string("Number: ")
    revInput = userInput[::-1]
    totalSum = 0
    pos = 1
    for i in range(len(revInput)):
        d = int(revInput[i])
        if pos % 2 == 0:
            product = d * 2
            if product < 10:
                totalSum += product
            else:
                for c in range(len(str(product))):
                    totalSum += int(str(product)[c])
        else:
            totalSum += d
        pos += 1
    if totalSum % 10 == 0:
        checkmanufacturer(userInput)
    else:
        print("INVALID")

def checkmanufacturer(input):

    if len(input) in [13,16] and input[0] == "4":
        print("VISA")

    elif len(input) == 15 and input[0:2] in ["34","37"]:
        print("AMEX")

    elif len(input) == 16 and input[0:2] in ["51","52","53","54","55"]:
        print("MASTERCARD")

    else:
        print("INVALID")

main()


