#include <cs50.h>
#include <stdio.h>

bool isValid(long number);
int calcCheckSum(long number);
bool isAMEX(long number);
bool isMASTERCARD(long number);
bool isVISA(long number);

//check if checksum of creditcard number is valid and determines what type of card
int main(void)
{
    long number = get_long("Number: ");
    if (isValid(number))
    { 
        if (isAMEX(number)) 
        {
            printf("AMEX\n");
        }
        else if (isMASTERCARD(number)) 
        {
            printf("MASTERCARD\n");
        }
        else if (isVISA(number)) 
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }

    }
    else 
    {
        printf("INVALID\n");
    }
}

int calcCheckSum(long number)
{
    int sumOtherDigits = 0;
    int sumOddDigits = 0;
    while (number > 0)
    {
        int digit = number % 10;
        sumOddDigits += digit;
        number = number / 10;
        digit = number % 10;
        //multiply the other digit by 2
        digit *= 2;
        //extract both digits of the product
        int last = digit % 10;
        int first = digit / 10;
        //add to sum
        sumOtherDigits += first + last;
        number = number / 10;
    }
    //adds the odd and other digits
    int checkSum = sumOddDigits + sumOtherDigits;

    return checkSum;
}

//check criteria for AMEX (not of checksum!)
bool isAMEX(long number) 
{
    //15digits starts with 34 or 37
    if ((number < 380000000000000 && number >= 370000000000000) || ((number >= 340000000000000) && (number < 350000000000000)))
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

//check criteria for MASTERCARD (not of checksum!)
bool isMASTERCARD(long number) 
{
    //16digits starts with 51, 52, 53, 54
    if (number < 5600000000000000 && number >= 5100000000000000)
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

//check criteria for VISA (not of checksum!)
bool isVISA(long number) 
{
    //13 or 16 digits, starts with 4
    if ((number < 5000000000000 && number >= 4000000000000) || (number < 5000000000000000 && number >= 4000000000000000))
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

//checks if checksum ends 0
bool isValid(long number) 
{
    bool valid = false;
    if (calcCheckSum(number) % 10 == 0)
    {
        valid = true;
    }
    
    return valid;
}
