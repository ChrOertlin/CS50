import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
    # TODO: Read database file into a variable
    dbfile = sys.argv[1]
    db = []
    with open(dbfile) as file:
        reader = csv.DictReader(file)
        for r in reader:
            db.append(r)
    # TODO: Read DNA sequence file into a variable
    seqfile = sys.argv[2]

    with open(seqfile) as file:
        reader = csv.reader(file)
        for r in reader:
            seq = "".join(r)

    # TODO: Find longest match of each STR in DNA sequence
    profile = dict()
    for k in db[0].keys():
        if k != "name":
            profile[k] = longest_match(seq, k)

    # TODO: Check database for matching profiles
    for entry in db:
        matches = 0
        for k in profile.keys():
            if int(profile.get(k)) == int(entry.get(k)):
                matches += 1
        if matches == len(profile.keys()):
            return print(entry.get("name"))

    return print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
