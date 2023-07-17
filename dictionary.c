// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Number of words loaded from dictionary
static int word_counter = 0;

// Hash table
node *table[N];

// Changeable pointer to use to add nodes to lists
node *n = NULL;

// Changeable pointer to search through lists
node *tmp = NULL;



////////////////////////////////////////////////////// FUNCTIONS //////////////////////////////////////////////////////



// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char WORD[LENGTH + 1];
    strcpy(WORD, word);                             // create copy of word
    for (int i = 0, l = strlen(word); i < l; i++)   // convert word copy to all lowercase letters
    {
        WORD[i] = tolower(WORD[i]);
    }

    tmp = table[hash(WORD)];                        // make pointer point to start of relevant list

    while (tmp != NULL)                             // until we reach end of list...
    {
        if (strcmp(tmp->word, WORD) == 0)
        {
            return true;                            // compare word copy to each word field of the list
        }
        else
        {
            tmp = tmp->next;                        // if words don't match go to next node in list
        }
    }

    return false;
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    return (word[0] - 97);
}



// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");                    // open dictionary
    if (file == NULL)
    {
        return false;
    }

    int in_file;                                            // keeps track of whether word was successfully scanned into node

    for (int i = 0; i < N; i++)
    {
        table[0] = NULL;                                    // makes sure all buckets have NULL pointers to begin with
    }

    while (1)
    {
        n = malloc(sizeof(node));                           // malloc new node
        if (n == NULL)
        {
            return false;
        }
        n->next = NULL;                                     // initialize node pointer to NULL to prevent memory issues and to indicate end of list later on

        in_file = fscanf(file, "%s", n->word);              // scan new word into new node
        if (in_file == EOF)                                 // check whether we have reached EOF, if so break
        {
            free(n);
            break;
        }

        n->next = table[hash(n->word)];                     // make next field in new node point to start of relevant list
        table[hash(n->word)] =
            n;                                              // make relevant bucket/list point to new node which is the new start of the list

        word_counter++;                                     // add one to the word counter (words loaded into memory from dictionary)
    }

    fclose(file);

    return true;
}



// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}



// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int counter;

    for (int i = 0; i < N; i++)
    {
        tmp = table[i];
        while (table[i] != NULL)
        {
            tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }

    return true;
}