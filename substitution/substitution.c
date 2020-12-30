#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool isKeyValid(string key);
bool alphabeticOnly(string key);
bool isCorrectLength(string key);
bool containsOnlyOnce(string key);
void encrypt(string plaintext, string key);
char getEncryptedChar(char plainChar, string key);

int main(int argc, string argv[]) 
{
    //accept a single command-line argument, the key to use for the substitution.
    if (argc == 2 && isKeyValid(argv[1]))
    {
        string key = argv[1];
        string plaintext = get_string("plaintext: ");
        // I struggled to return the array of chars so I just invoke the function and print there
        printf("ciphertext: ");
        encrypt(plaintext, key);
    }
    else 
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}

bool isKeyValid(string key)
{
    //containing 26 characters
    if (isCorrectLength(key) && alphabeticOnly(key) && containsOnlyOnce(key))
    {  
        return true;
    }
    else
    {
        printf("Invalid key.\n");
        return false;
    }
}

//is key 26 chars long
bool isCorrectLength(string key)
{
    int i = strlen(key);
    //containing 26 characters
    if (strlen(key) == 26)
    {
        return true;
    }
    else
    {
        printf("Key supposed to be 26 characters long.\n");
        return false;
    }
}

//containing any character that is not an alphabetic character
bool alphabeticOnly(string key) 
{
    for (int i = 0; i < 26; i++)
    {
        //convert to ascii only once
        int ascii = (int) key[i];
        //check if non-alphabetical
        if (ascii < 65 || (ascii > 90 && ascii < 97) || (ascii > 122)) 
        {
            printf("Key must only contain alphabetic characters.\n");
            return false;
        }
    }
    //if all chars in key alphabetical
    return true;
}

bool containsOnlyOnce(string key)
{
    for (int i = 0; i < 26; i++) 
    {
        for (int j = i + 1; j < 26; j++)
        {
            //check for every char in key if it is not occuring later once more
            if ((int) key[i] == (int) key[j])
            {
                printf("Key must not contain repeated characters.\n");
                return false;
            }
        }
    }
    return true;
}

void encrypt(string plaintext, string key)
{
    //initialize empty string array for encrypted
    int lenPlainText = strlen(plaintext);
    char encrypted[lenPlainText + 1];
    
    //iterate though chars of plaintext
    
    for (int i = 0; i <= lenPlainText; i++)
    {
        //make subs of each char by char in key if letters
        if (isalpha(plaintext[i]))
        {
            encrypted[i] = (char) getEncryptedChar(plaintext[i], key);
        }
        
        //neither upper nor lowercase letter
        else
        {
            //dont change the char
            encrypted[i] = (char) plaintext[i];
        }
    }
    //put in terminating char
    encrypted[lenPlainText] = '\0';
    //return encrypted[lenPlainText];
    
    //I dont know how to return an array of chars so I just print it right now.
    for (int i = 0; encrypted[i] != '\0'; i++)
    {
        printf("%c", encrypted[i]);
    }
    printf("\n");
}

char getEncryptedChar(char plainChar, string key)
{
    //convert input char to lowercase
    char lowerChar = tolower(plainChar);
    
    //look up in key
    int orderInKey = (int) lowerChar - (int) 'a';
    char cypherChar = (char) key[orderInKey];
    
    //preserve original case of letter
    if (islower(plainChar)) 
    {
        return tolower(cypherChar);
    }
    else
    {
        return toupper(cypherChar);
    }
}