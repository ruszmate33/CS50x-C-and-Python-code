import cs50
import sys

def main():
    #if not 2 command line arguments, return with an error
    if len(sys.argv) != 2:
        print("Usage: python vigenere k")
        return 1

    #pass in key via command line
    key = sys.argv[1]

    #if keyword contains nonalphabetical characters, return with error as well
    for char in key:
        if not char.isalpha():
            print("Usage: python vigenere k")
            return 1

    #get the plaintext
    plaintext = cs50.get_string("plaintext: ")

    #initialize output
    ciphertext =""

    #initialize counter to key
    i = 0

    #iterate through plaintext
    for num in range(len(plaintext)):
        char = plaintext[num]

        #get key
        i = i % len(key)
        char_key = key[i]
        if char_key.isupper():
            k = ord(char_key) - 65
        else:
            k = ord(char_key) - 97


        #shift to get ciphertext
        if char.isalpha():
            #increment counter of key
            i += 1
            if char.islower():
                positionA = 97
            else:
                positionA = 65
            #shift char
            p = ord(char) - positionA
            c = (p + k) % 26
            c += positionA
            #convert back to string
            char = chr(c)
        #append char to output
        ciphertext += char

    print(ciphertext)

if __name__ == "__main__":
    main()