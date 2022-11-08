#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 Wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 Wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    // loop through string
    int scoreSum = 0;
    int arrPos = 0;
    for (int i = 0,  n = strlen(word); i < n; i++)
    {
        // convert to lower and subtract the ascii decimal from the int so an a should become 0
        // b should become 98-97 = 1 etc.
        // This will correspond to the array position type
        arrPos = (int) tolower(word[i]) - 97;
        if (arrPos >= 0 && arrPos <= 26)
        {
            //sum the pointsda
            scoreSum += POINTS[arrPos];
        }
    }
    return scoreSum;
}
