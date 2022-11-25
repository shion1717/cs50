// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N] = {NULL};

// Track size of dictionary
int sizedict;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Returns true if word is in the dictionary, false otherwise

    // Hash word to obtain a hash value
    int loc = hash(word);

    // Access linked list at that index in the hash table
    node *cursor = 0;
    cursor = table[loc];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    // Read strings from file one at a time
    char dictword[LENGTH + 1];
    while (fscanf(file, "%s", dictword) != EOF)
    {
        // Update global variable for dictionary size
        sizedict++;

        // Create a new node for each word
        node *n = calloc(1, sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, dictword);

        // Hash word to obtain a hash value
        int loc = hash(n->word);

        // Insert node into hash table at that location
        if (table[loc] == NULL)
        {
            table[loc] = n;
        }
        else
        {
            n->next = table[loc];
            table[loc] = n;
        }
    }
    fclose(file);
    return true;
    //return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return sizedict;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = 0;
        cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
    //return false;
}
