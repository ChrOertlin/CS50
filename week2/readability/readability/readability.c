#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


// Return "Grade X" where X is the score according to the formula
// If X is 16 or higher return "Grade 16+"
// If X is below 1 return "Before Grade 1"

// Declaration of functions
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void determine_grade(float L, float S, float W);
// main
int main(void)
{
// Promp for a string of text from the user
    string userInput = get_string("Text: ");
    //printf("%s\n", userInput);
    int L = count_letters(userInput);
    int W = count_words(userInput);
    int S = count_sentences(userInput);
    determine_grade(L, S, W);
}


// helper functions
int count_letters(string text)
{
    // Count the number of letters
    int letterCount = 0;
    // I.e. A-Z and a-z everything else is not considered a letter
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // convert to lowercase and check if it falls in the dedicated ASCII table range for lower case
        if ((int) tolower(text[i]) >= 97 && (int) tolower(text[i]) <= 122)
        {
            letterCount ++;
        }
    }
    return letterCount;
}

int count_words(string text)
{
    // assume groupings of letters a-z A-Z separated by spaces is a word
    // This boils down to counting spaces in the sentence
    // Initialize with 1 since a sentence has to have at least one word without a space
    int spaceCount = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // convert to lowercase and check if it falls in the dedicated ASCII table range for lower case
        if ((int) text[i] == 32)
        {
            spaceCount ++;
        }
    }
    return spaceCount;
}

int count_sentences(string text)
{
    // period (.), exlamation mark ! or question mark ? indicate the end of a sentence
    int sentenceCount = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // convert to lowercase and check if it falls in the dedicated ASCII table range for lower case
        if ((int) text[i] == 33 | (int) text[i] == 46 | (int) text[i] == 63)
        {
            sentenceCount ++;
        }
    }
    return sentenceCount;
}


void determine_grade(float L, float S, float W)
{
    //coleman-liau index for grading readability of texts
    // formula: index = (0.0588 * L) - (0.296 * S) - 15.8
    // L average number of letters per 100 words in the text
    // S the average number of sentences per 100 words in a text

    float avgLetterPerHunW = L / (W / 100);
    float avgSentencePerHunW = S / (W / 100);
    int index = (int) round(0.0588 * avgLetterPerHunW - 0.296 * avgSentencePerHunW - 15.8);
    if (index >= 1 && index < 16)
    {
        printf("Grade %i\n", index);
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade 16+\n");
    }
}