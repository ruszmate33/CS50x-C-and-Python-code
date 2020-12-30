# import get_string from cs50 package
from cs50 import get_string


def main():
    # prompt the user for a of text
    text = get_string("Text: ")
    # calculate cli value with helper functions
    cli = calcCLI(countNumLetters(text), countNumWords(text), countSentences(text))

    # from 1 to 16: output "Grade X" where X is the grade level computed by the Coleman-Liau formula, rounded to the nearest integer
    if (cli >= 1 and cli <= 16):
        print("Grade", cli)

    # 16 or higher, output "Grade 16+"
    elif (cli > 16):
        print("Grade 16+")

    # If the index number is less than 1, your program should output "Before Grade 1"
    elif (cli < 1):
        print("Before Grade 1")


# count the number of letters in the text
def countNumLetters(text):
    numLetters = 0
    for letter in text:
        # calculate ascii code only once
        ascii = ord(letter)
        # Letters can be any uppercase or lowercase alphabetic characters, but shouldn’t include any punctuation, digits, or other symbols
        if ((ascii >= 65 and ascii <= 90) or (ascii >= 97 and ascii <= 122)):
            numLetters += 1

    return numLetters


# count the number of words in the text
def countNumWords(text):
    numSpaces = 0
    for letter in text:
        # calculate ascii code only once
        ascii = ord(letter)
        # words separated by a space
        if (ascii == 32):
            numSpaces += 1

    # characters separated by spaces should count as a word
    return numSpaces + 1


# count the number of sentences in the text
def countSentences(text):
    numSentences = 0

    for letter in text:
        # calculate ascii only once
        ascii = ord(letter)
        # sequence of characters that ends with a . or a ! or a ? a sentence
        if (ascii == 46 or ascii == 63 or ascii == 33):
            numSentences += 1
    return numSentences


# Coleman-Liau formula, rounded to the nearest integer
def calcCLI(letters, words, sentences):
    # letters and senstences PER 100 words in text
    l = 100 * float(letters) / float(words)
    s = 100 * float(sentences) / float(words)
    index = round(0.0588 * l - 0.296 * s - 15.8)
    return index


# call main function after defining helpers
main()