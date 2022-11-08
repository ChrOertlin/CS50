// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "dictionary.h"



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void freeNodes(struct node *head);
// TODO: Choose number of buckets in hash table
const unsigned int N = 4001;

// Hash table
node *table[N];
int nWords = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // get the hash of the word and lookup in hash
    if(table[hash(word)] != NULL)
    {
        // go through each element of the linked list
        node *temp = table[hash(word)];
        while(temp != NULL)
        {
            if(strcasecmp(word,temp->word) == 0){
                return true;
            }
            temp = temp->next;
        }
    }
    return false;
}
// Hash function (citation below)
// Title: djb2 hash function
// Author: Bernstein, Daniel J.
// Hashes word to a number
unsigned int hash(const char *word)
{
     // Random number
    unsigned long hash = 5381;

    // Stores ASCII values
    int c = *word;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }

    // Get a value within the max size of the hash table
    hash = hash % N;
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // open dictorionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        unload();
    }
    else
    {
        // read in each word
        char word[45];
        while(fscanf(dict, "%s", word) != EOF)
        {
            nWords++;
            // assign memory to a new node
            node *new_entry = malloc(sizeof(node));
            if(new_entry == NULL)
            {
                printf("Could not assign memory to new entry.\n");
                return false;
            }
            // initialize a new node with word and NULL pointer
            strcpy(new_entry->word, word);
            new_entry->next = NULL;
            // add the node to the hash table
            //if there is nothing at the point, just add the first node
            if(table[hash(word)] == NULL)
            {
                table[hash(word)] = new_entry;
            }
            // if there is already something at that spot, add it to the linked ist
            else
            {
                // add the new word att the beginning of the list and make sure pointer to next is there
                new_entry->next = table[hash(word)];
                table[hash(word)] = new_entry;
            }
            // free the new entry from memory
            //free(new_entry);
        }
     fclose(dict);
     return true;
    }
    fclose(dict);
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return nWords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for(int i = 0; i < N; i++)
    {
        freeNodes(table[i]);
    }
    return true;
}

void freeNodes(struct node *head)
{
    while(head != NULL)
    {
        struct node *tmp = head;
        head = head->next;
        free(tmp);
    }
}