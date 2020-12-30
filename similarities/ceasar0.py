#import
import cs50
import sys

def main():
    #if not 2 command line arguments, return with an error
    if len(sys.argv) != 2:
        print("Usage: ./caesar k")
        return 1

    #pass in key via command line
    key = int(sys.argv[1])

    #get the plaintext
    plaintext = cs50.get_string("plaintext: ")

    #initialize output
    output =""

    #iterate through plaintext
    for char in plaintext:
        if char.isalpha():
            if char.islower():
                positionA = 97
            else:
                positionA = 65
            #shift char
            p = ord(char) - positionA
            c = (p + key) % 26
            c += positionA
            #convert back to string
            char = chr(c)
        #append char to output
        output += char

    print(output)

if __name__ == "__main__":
    main()
