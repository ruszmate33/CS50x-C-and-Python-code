def main():
    # promt user input
    number = int(input("Number: "))

    # check validity and type of card
    if (isValid(number)):
        if (isAMEX(number)):
            print("AMEX")
        elif (isMASTERCARD(number)):
            print("MASTERCARD")
        elif (isVISA(number)):
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")

    return 0


# calculates checksum
def calcCheckSum(number):
    sumOtherDigits = 0
    sumOddDigits = 0

    while (number > 0):
        digit = number % 10
        sumOddDigits += digit
        number = number // 10
        digit = number % 10
        # multiply the other digit by 2
        digit *= 2
        # extract both digits of the product
        last = digit % 10
        first = digit // 10
        # add to sum
        sumOtherDigits += first + last
        number = number // 10

    # adds the odd and other digits
    checkSum = sumOddDigits + sumOtherDigits

    return checkSum


# check if number fullfills given criteria (like checksum)
def isValid(number):
    if (calcCheckSum(number) % 10 == 0):
        return True
    else:
        return False


# check criteria for AMEX (not of checksum!)
def isAMEX(number):
    # 15digits starts with 34 or 37
    if ((number < 380000000000000 and number >= 370000000000000) or ((number >= 340000000000000) and (number < 350000000000000))):
        return True
    else:
        return False


# check criteria for MASTERCARD (not of checksum!)
def isMASTERCARD(number):
    # 16digits starts with 51, 52, 53, 54
    if (number < 5600000000000000 and number >= 5100000000000000):
        return True
    else:
        return False


# check criteria for VISA (not of checksum!)
def isVISA(number):
    # 13 or 16 digits, starts with 4
    if ((number < 5000000000000 and number >= 4000000000000) or (number < 5000000000000000 and number >= 4000000000000000)):
        return True
    else:
        return False


# call main after declaring all helper functions
main()