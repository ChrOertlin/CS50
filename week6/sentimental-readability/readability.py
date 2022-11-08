# TODO
from cs50 import get_string
def main():
    # get input
    userInput = get_string("Text: ")
    # initialize values
    L = 0
    W = 1
    S = 0
    # count occurences
    for i in range(len(userInput)):
        if ord(userInput[i].lower()) >= 97 and ord(userInput[i].lower()) <= 122:
            L += 1
        elif ord(userInput[i]) == 32:
            W += 1
        elif ord(userInput[i]) in [33,46,63]:
            S += 1
    # coleman liau index
    avgLetterPerHunW = float(L / (W / 100))
    avgSentencePerHunW = float(S / (W / 100))
    index = round((0.0588 * avgLetterPerHunW) - (0.296 * avgSentencePerHunW) - 15.8)
    if index >= 1 and index < 16:
        print(f"Grade {index}")
    elif index < 1:
        print(f"Before Grade 1")
    else:
        print("Grade 16+")



main()