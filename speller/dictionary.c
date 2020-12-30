// Implements a dictionary's functionality
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

//for strcasecmp etc.
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int HashTableSize = 131072;

//global variable for sum of words in dictionary
unsigned int totalWords = 0;

// Hash table
node *table[HashTableSize];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // hash the word
    int hashValue = hash(word);

    //access linked list at hash index
    node *trv = table[hashValue];

    //traverse list, looking for the word
    while (trv != NULL)
    {
        if (strcasecmp(word, trv -> word) == 0)
        {
            return true;
        }
        else
        {
            //continue travesing
            trv = trv -> next;
        }
    }
    // if got end of list, not in the dictionary
    return false;
}

// Hashes word to a number
//djb2 by Dan Bernstein
//found at https://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = tolower(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % HashTableSize;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s\n", dictionary);
        return false;
    }


    //read strings one at a time from dictionary
    while (true)
    {
        //create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fprintf(stderr, "Run out of memory.\n");
            return false;
        }

        //put word into the node
        fscanf(file, "%s", n -> word);
        //set next to NULL, if next node is unknown
        n -> next = NULL;

        //clean up memory if end of file
        if (feof(file))
        {
            free(n);
            fclose(file);
            break;
        }

        //count word
        totalWords++;

        //hash word to obtain a hash value
        int hashNum = hash(n -> word);

        // put word into the linked list of hash table
        //insert node into hash table at that location

        //insert very first node, it is empty if the header does not point anywhere
        if (table[hashNum] == NULL)
        {
            table[hashNum] = n;
            //also make sure this node points to NULL (might be redundant)
        }
        // there is one word at least in cell
        else
        {
            n -> next = table[hashNum]; // new node points at head of list
            table[hashNum] = n; // head of list points at new node
        }
    }
    return true;
}

// Returns number of words in dictionary
unsigned int size(void)
{
    return totalWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //iterate over all possible hash values
    for (int i = 0; i < HashTableSize; i++)
    {
        //access linked list at hash index
        node *cursor = table[i];


        //traverse list, go to the end of it
        while (cursor != NULL)
        {
            node *tmp = cursor;
            //continue travesing
            cursor = cursor -> next;
            //one pointer one step ahead, while the other frees node
            free(tmp);
        }
    }
    // success
    return true;
}