#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
pair sorted_pairs[MAX * (MAX - 1) / 2];
int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool alreadySeen(int j, int trackRank, int ranks[]);
bool hasEntry(int i, int j);
bool createsCycle(int source, int target);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
        printf("\n");

    }


    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // ranks holds position of candidates in ranked order
    // preferences is a matrix where each row represent a candidate (i.e. row 1 is candidate 1)
    // go through the candidate count -1 since the last ranked is not preferred above any other
    int trackRank = 0;
    for (int i = 0; i < candidate_count; i ++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // set horizontal to zero
            if (i == j)
            {
                preferences[i][j] = 0;
            }
            // if it is the first rank update whole row except itself an previous candidates
            if (trackRank > 0 && alreadySeen(j, trackRank, ranks) == false && ranks[i] != j)
            {
                preferences[ranks[i]][j] += 1;
            }
            // If it is the first rank update the whole row except the position of candidate self
            if (trackRank == 0 && ranks[i] != j)
            {
                preferences[ranks[i]][j] += 1;
            }
        }
        trackRank += 1;
    }
    return;
}

bool alreadySeen(int j, int trackRank, int ranks[])
{
    bool seen = false;
    for (int i = 0; i < trackRank; i++)
    {
        if (j == ranks[i])
        {
            seen = true;
        }
    }
    return seen;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] != 0 && preferences[i][j] != preferences[j][i] && hasEntry(i, j) == false)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                }
                else
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                }
                pair_count++;
            }
        }
    }
    return;
}

bool hasEntry(int i, int j)
{
    for (int p = 0; p < pair_count; p++)
    {
        if ((pairs[p].winner == i && pairs[p].loser == j) || (pairs[p].winner == j && pairs[p].loser == i))
        {
            return true;
        }
    }
    return false;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // strength of victory is  seen in preferences
    int stronger;
    for (int i = 0; i < pair_count - 1; i++)
    {
        stronger = i;
        for (int j = i + 1; j < pair_count; j++)
            if (preferences[pairs[j].winner][pairs[j].loser] >= preferences[pairs[i].winner][pairs[i].loser])
            {
                stronger = j;
            }
        int tmpWin = pairs[i].winner;
        int tmpLose = pairs[i].loser;
        pairs[i].winner = pairs[stronger].winner;
        pairs[i].loser = pairs[stronger].loser;
        pairs[stronger].winner = tmpWin;
        pairs[stronger].loser = tmpLose;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // pairs is sorted so that first pair is strongest winner
    // lock[pairs.winner][pairs.loser] as true
    for (int i = 0; i < pair_count; i++)
    {
        if (createsCycle(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = 1;
        }
    }
    return;
}

bool createsCycle(int source, int target)
{
    // check if adding a 1 in place would create a cycle
    // check if the target of the target pionts to itself
    if (target == source)
    {
        return true;
    }
    // first check if the target has a target
    for (int j = 0; j < candidate_count; j++)
    {
        //check if target row has an edge
        if (locked[target][j] == 1)
        {
            // is this edge source
            if (j == source)
            {
                return true;
            }
            else
            {
                // Recursive call for other possible links
                if (createsCycle(source, j))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // check for a locked column with only 0s
    bool winner;
    int source;
    // we need a candidate with 0 in all columns
    for (int i = 0; i < candidate_count; i++)
    {
        winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == 1)
            {
                winner = false;
            }
        }
        if (winner == true)
        {
            source = i;
        }
    }
    printf("%s\n", candidates[source]);
    return;
}