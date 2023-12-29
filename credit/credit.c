#include <cs50.h>
#include <stdio.h>
#include <math.h>

long getNumber(void);
string checkDN(int l, int f, int sum);

int main(void)
{
    //get credit card number
    long n = getNumber();
    long c = n;

    //calculate digits and check the algorythm
    int length = 0;
    int sum = 0;
    while (c > 1)
    {
        int digit = c % 10; //get the current digit of n / c
        c /= 10;
        length++;

        //multiply every second number and add to sum
        if (length % 2 == 0)
        {
            if (digit * 2 < 9)
            {
                sum += (digit * 2);
            }
            else
            {
                sum += ((digit * 2) % 10 + 1);
            }
        }
        else
        {
            //add every other number to sum
            sum += digit;
        }
    }

    //get first two digits
    int f = (int)((double)n / pow(10, (length - 2))) % 100;

    //exit if length doesn't match
    if (length > 16)
    {
        printf("INVALID\n");
        return 0;
    }
    else if (length < 13)
    {
        printf("INVALID\n");
        return 0;
    }

    //calculate end result and print it
    string result = checkDN(length, f, sum);
    printf("%s\n", result);
}


//functions :

long getNumber(void)
{
    long n;

    //prompt for number larger than 1
    do
    {
        n = get_long("Credit card number: ");
    }
    while (n < 1);

    return n;
}


string checkDN(int l, int f, int sum)
{
    if (sum % 10 == 0)
    {
        //check digits and start-numbers
        if (l == 15 && (f == 34 || f == 37)) //American Express
        {
            return "AMEX";
        }
        else if (l == 16 && (f <= 55 && f >= 51)) //Mastercard
        {
            return "MASTERCARD";
        }
        else if ((l == 13 || l == 16) && (f <= 49 && f >= 40)) //Visa
        {
            return "VISA";
        }
        else //Invalidate everything else
        {
            return "INVALID";
        }
    }
    else
    {
        return "INVALID";
    }
}