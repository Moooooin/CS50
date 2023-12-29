#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);
void encode(string s);

int main(void)
{
    // Prompt for message
    string m = get_string("Message: ");

    // Encode and print the message
    encode(m);
}

void encode(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        // Convert current letter to decimal
        int dezimal = (int) s[i];
        int m = BITS_IN_BYTE - 1;

        // Convert decimal to binary
        for (int j = 0; j < BITS_IN_BYTE; j++)
        {
            int n = pow(2, m);

            // Bulbify
            if (dezimal / n >= 1)
            {
                print_bulb(1);
                dezimal -= n;
            }
            else
            {
                print_bulb(0);
            }

            m--;
        }

        printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
