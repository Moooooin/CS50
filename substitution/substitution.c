#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool validKey(string key, int n);
string encrypt(string s, string key);

int main(int argc, string argv[])
{
    // Check if key is valid
    if (!validKey(argv[1], argc))
    {
        return 1;
    }

    // Get text to encrypt
    string t = get_string("plaintext:  ");

    // Encrypt text
    string e = encrypt(t, argv[1]);

    // Print encrypted text
    printf("ciphertext: %s\n", e);

    return 0;
}

string encrypt(string s, string key)
{
    int l = strlen(s);

    for (int i = 0; i < l; i++)
    {
        // Check if current character is letter
        if (tolower(s[i]) >= 'a' && tolower(s[i]) <= 'z')
        {
            // Get place of letter in alphabet
            int place = (int) toupper(s[i]) - 65;

            // Encrypt letter and keep case
            if (isupper(s[i]))
            {
                s[i] = toupper(key[place]);
            }
            else
            {
                s[i] = tolower(key[place]);
            }
        }
    }

    return s;
}

bool validKey(string key, int n)
{
    // Check if key is provided and
    // Check for no additional arguments
    if (n != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return false;
    }

    int l = strlen(key);

    // Check for correct length
    if (l != 26)
    {
        printf("Please use 26 letters in the key.\n");
        return false;
    }

    // Check if only alphabetical values are used
    for (int i = 0; i < l; i++)
    {
        if (key[i] < 'A' || key[i] > 'z')
        {
            printf("Please only use letters in the key.\n");
            return false;
        }
        else if (key[i] > 'Z' && key[i] < 'a')
        {
            printf("Please only use letters in the key.\n");
            return false;
        }

        // Check if a letter is used more than once
        char c1 = tolower(key[i]);

        for (int j = 0; j < i; j++)
        {
            char c2 = tolower(key[j]);

            if (c1 == c2)
            {
                printf("Please only use each letter once.\n");
                return false;
            }
        }
    }

    return true;
}