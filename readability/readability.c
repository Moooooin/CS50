#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int letters(string s);
int words(string s);
int sentences(string s);

int main(void)
{
    // Prompt Text
    string t = get_string("Text: ");

    // Calculate Grade level
    int grade = round(0.0588 * (letters(t) / (words(t) / 100.0)) - 0.296 * (sentences(t) / (words(t) / 100.0)) - 15.8);

    // Print grade
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int letters(string s)
{
    int n = strlen(s);
    int c = 0;

    for (int i = 0; i < n; i++)
    {
        // Determine if current char is letter
        char buchstabe = tolower(s[i]);

        if (buchstabe >= 'a' && buchstabe <= 'z')
        {
            // Increase Count of letters
            c++;
        }
    }

    return c;
}

int words(string s)
{
    int n = strlen(s);
    int c = 0;

    for (int i = 0; i < n; i++)
    {
        char buchstabe = s[i];

        // Determine if current char is space
        if (buchstabe == ' ')
        {
            // Increase Count of words
            c++;
        }
    }

    // Make sure last word is added too
    if (c > 0)
    {
        c++;
    }

    return c;
}

int sentences(string s)
{
    int n = strlen(s);
    int c = 0;

    for (int i = 0; i < n; i++)
    {
        char buchstabe = s[i];

        // Determine if current char is . or ! or ?
        if (buchstabe == '.' || buchstabe == '!' || buchstabe == '?')
        {
            // Increase Count of sentences
            c++;
        }
    }

    return c;
}