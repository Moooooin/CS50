// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table : 26³
const unsigned int N = 26 * 26 * 26;

// Hash table
node *table[N];

// size of dictionary (added by me for an easier size() function)
int dSize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Go to linked list of hash value of word
    node *cursor = table[hash(word)];

    // Iterate through each node and check if word matches
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    // Took first 3 instead of just 1 letter
    unsigned int bucket = 0;

    for (int i = 0; i < 3 && word[i] != '\0'; i++)
    {
        int pow = 1;
        for (int j = 0; j < i; j++)
        {
            pow *= 26;
        }

        bucket = pow * (toupper(word[i]) - 'A');
    }

    return bucket % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open file
    FILE *wörterbuch = fopen(dictionary, "r");
    if (wörterbuch == NULL)
    {
        return false;
    }

    // Initialize every node of table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char wort[LENGTH + 1];

    // Hash every word and store it in hashtable
    while (fscanf(wörterbuch, "%s", wort) == 1)
    {
        // Create new node
        node *neu = malloc(sizeof(node));
        if (neu == NULL)
        {
            return false;
        }

        // Copy word into node and add node to hashtable
        strcpy(neu->word, wort);
        int ll = hash(wort);

        neu->next = table[ll];
        table[ll] = neu;

        // Update dictionary size variable
        dSize++;
    }

    fclose(wörterbuch);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Iterate through every linked list in table and free every node
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *cursor = table[i];
            node *tmp = cursor;

            while (cursor != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    }
    return true;
}