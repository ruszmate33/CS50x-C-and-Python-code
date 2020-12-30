import sys
import csv


def main():

    # if not 2 command line arguments, return with an error
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return 1

    # open DNA sequence and read its contents into memory
    textfile = sys.argv[2]
    textfile = open(textfile)
    sequence = textfile.read()

    # open csv with people STR data
    table = sys.argv[1]
    csvfile = open(table, "r")
    tableAsDict = csv.DictReader(csvfile, delimiter=',')

    # get the metching person if any
    print(getMatchingPerson(sequence, tableAsDict))

    # close text file and csv
    textfile.close()
    csvfile.close()

    # upon success return 0
    return 0


def getMatchingPerson(sequence, tableAsDict):

    # no exact match return this
    matchingPerson = "No match"

    # extract STRs from csv file
    shortTRs = tableAsDict.fieldnames[1:]

    # dictionary of STR vs maxReapeats in a given sequence
    shortTRdict = makeShortTRdict(shortTRs, sequence)

    # go though each person (row) of table
    for person in tableAsDict:
        # go though all shortTandemRepeats
        for shortTR in shortTRs:
            # check if all STRs matching in a given person
            if (int(person[shortTR]) != shortTRdict[shortTR]):
                # if not continue with another person
                break

            # last shortTR was also identical
            if shortTR == shortTRs[-1]:
                matchingPerson = person['name']

    return matchingPerson


def makeShortTRdict(shortTRs, sequence):
    shortTRdict = {}

    # dictionary for all STRs and their max count
    for shortTR in shortTRs:
        shortTRdict[shortTR] = countSTRs(shortTR, sequence)

    return shortTRdict


def countSTRs(shortTR, sequence):
    # initialize number of STRs to 0
    numSTR = 0
    highestSTR = 0
    isRepeated = False
    lastStartingPosition = 0
    i = 0

    # compute the longest run of consecutive repeats of the STR in the DNA sequenceuntil EOF
    while ((i + len(shortTR)) < len(sequence)):
        # not in a repeated STR region
        while (not isRepeated and (i + len(shortTR)) < len(sequence)):
            # count if matching and leave this loop for the loop with repeating region
            if sequence[i: (i + len(shortTR))] == shortTR:
                numSTR += 1
                if numSTR > highestSTR:
                    highestSTR = numSTR
                isRepeated = True
                break
            else:
                # check one base firther for shortTRs
                i += 1

        # in a repeated STR region
        while (isRepeated and (i + len(shortTR)) < len(sequence)):
            # check for distance of whole shortTR away for repeats
            i += len(shortTR)
            if sequence[i: (i + len(shortTR))] == shortTR:
                numSTR += 1
                if numSTR > highestSTR:
                    highestSTR = numSTR
            # not repeating go back to loop with single increments
            else:
                isRepeated = False
                numSTR = 0
                break

    return highestSTR


main()