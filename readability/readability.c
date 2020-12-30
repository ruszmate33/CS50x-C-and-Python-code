#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int countNumLetters(string text);
int countNumWords(string text);
int countSentences(string text);
int calcCLI(int letters, int words, int sentences);

int main(void)
{
    // prompt the user for a string of text
    string text = get_string("Text: ");
    
    //calculate cli value with helper functions
    int cli = calcCLI(countNumLetters(text), countNumWords(text), countSentences(text));
    
    //from 1 to 16: output "Grade X" where X is the grade level computed by the Coleman-Liau formula, rounded to the nearest integer
    if (cli >= 1 && cli <= 16) 
    {
        printf("Grade %i\n", cli);    
    }
    //16 or higher, output "Grade 16+"
    else if (cli > 16) 
    {
        printf("Grade 16+\n");
    }
    //If the index number is less than 1, your program should output "Before Grade 1"
    else if (cli < 1)
    {
        printf("Before Grade 1\n");
    }
}

//count the number of letters in the text
int countNumLetters(string text)
{
    int numLetters = 0;
    
    for (int i = 0; i < strlen(text); i++)
    {  
        //calculate ascii code only once
        int ascii = (int) text[i];
        //Letters can be any uppercase or lowercase alphabetic characters, but shouldn’t include any punctuation, digits, or other symbols
        if ((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122))
        {
            numLetters++;
        }
    }
    return numLetters;
}

//count the number of words in the text
int countNumWords(string text)
{
    int numSpaces = 0;
    
    for (int i = 0; i < strlen(text); i++)
    {
        //calculate ascii code only once
        int ascii = (int) text[i];
        //words separated by a space
        if (ascii == 32)
        {
            numSpaces++;
        }
    }
    //characters separated by spaces should count as a word
    return numSpaces + 1;
}

//count the number of sentences in the text
int countSentences(string text)
{
    int numSentences = 0;
    
    for (int i = 0; i < strlen(text); i++)
    {
        //calculate ascii only once
        int ascii = (int) text[i];
        //sequence of characters that ends with a . or a ! or a ? a sentence
        if (ascii == 46 || ascii == 63 || ascii == 33)
        {
            numSentences++;
        }
    }
    return numSentences;
}

//Coleman-Liau formula, rounded to the nearest integer
int calcCLI(int letters, int words, int sentences) 
{
    //letters and senstences PER 100 words in text
    float L = 100 * (float) letters / (float) words;
    float S = 100 * (float) sentences / (float) words;
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    return index;
}