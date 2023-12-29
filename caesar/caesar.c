#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool checkKey(string k, int c);
string encrypt(string s, int key);

int main(int argc, string argv[])
{
    // Remind user for correct usage when mistake made
    if (!checkKey(argv[1], argc))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert key to integer
    int key = atoi(argv[1]);

    // Prompt for text to convert
    string t = get_string("Plaintext:  ");

    // Encrypt and print
    string e = encrypt(t, key);
    printf("ciphertext: %s\n", e);
    return 0;
}

string encrypt(string s, int key)
{
    int l = strlen(s);

    for (int i = 0; i < l; i++)
    {
        // Get current character
        char buchstabe = s[i];

        if (buchstabe >= 'a' && buchstabe)
        {
            // If lowercase letter : encrypt
            int e = (int) buchstabe + key;

            // Go back to a if switching over z
            while (e > 122)
            {
                e -= 26;
            }

            s[i] = (char) e;
        }
        else if (buchstabe >= 'A' && buchstabe <= 'Z')
        {
            // If uppercase letter : encrypt
            int e = (int) buchstabe + key;

            // Go back to a if switching over z
            while (e > 90)
            {
                e -= 26;
            }

            s[i] = (char) e;
        }
        else
        {
            // If no letter : don't encrypt
            s[i] = buchstabe;
        }
    }

    return s;
}

bool checkKey(string k, int c)
{
    // Check for correct argument
    if (c != 2)
    {
        return false;
    }

    // Check if key is number
    int l = strlen(k);

    for (int i = 0; i < l; i++)
    {
        if (k[i] < '0' || k[i] > '9')
        {
            return false;
        }
    }

    return true;
}