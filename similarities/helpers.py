from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    setA = set(a.splitlines())
    setB = set(b.splitlines())
    return list(setA & setB)


def sentences(a, b):
    """Return sentences in both a and b"""
    setA = set(sent_tokenize(a))
    setB = set(sent_tokenize(b))
    return list(setA & setB)


#separates into substrings with length n
def sub(text, n, substrings):
    j = 0
    while ((j+n) <= len(text)):
        print(text[j:(j+n)])
        #adds to a set
        substrings.add(text[j:(j+n)])
        j += 1


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    substringsA = set()
    substringsB = set()

    #calls helperfunction
    sub(a, n, substringsA)
    sub(b, n, substringsB)

    return list(substringsA & substringsB)
