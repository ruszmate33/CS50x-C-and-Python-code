#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // If name is a match for the name of a valid candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            // update the global preferences array
            preferences[voter][rank] = i;
            return true;
        }
    }
    //preference was not recorded
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // update the number of votes each candidate has at this stage in the runoff
    // each stage in the runoff, every voter effectively votes for their top-preferred candidate who has not already been eliminated
    for (int voter = 0; voter < voter_count; voter++)
    {
        for (int rank = 0; rank < candidate_count; rank++)
        {
            //pull out the candidateNum at position voter-rank
            int candNum = preferences[voter][rank]; 
            //at first valid candidate per voter
            if (!candidates[candNum].eliminated)
            {
                //increment votes and continue with next voter
                candidates[candNum].votes++;
                break;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // any candidate has more than half of the vote, wins
    for (int candNum = 0; candNum < candidate_count; candNum++)
    {
        if (candidates[candNum].votes > voter_count / 2)
        {
            //fprintf( stdout, candidates[candNum].name);
            printf("%s\n", candidates[candNum].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //  minimum vote total for any candidate who is still in the election
    //initialize to every voter voting for the same
    int minVotes = voter_count;
    
    for (int candNum = 0; candNum < candidate_count; candNum++)
    {
        //get what is the smalles number of #1 votes
        if (!candidates[candNum].eliminated)
        {
            
            if (candidates[candNum].votes < minVotes)
            {
                minVotes = candidates[candNum].votes;
            }
        }
    }
    return minVotes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // every candidate remaining in the election has the same number of votes
    for (int candNum = 0; candNum < candidate_count; candNum++)
    {
        //remaining in election
        if (!candidates[candNum].eliminated)
        {
            //in a second loop compare each candidate with each other
            for (int candNum2 = candNum + 1; candNum2 < candidate_count; candNum2++)
            {
                //remaining in election
                if (!candidates[candNum2].eliminated)
                {
                    if (candidates[candNum].votes != candidates[candNum2].votes)
                    {
                        return false;
                    }
                }
            }
        }
    }
    //all possible combinations of remaining candidates were equal
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    //takes minimum number of votes that anyone in the election currently has
    
    //eliminate the candidate (or candidates) who have min number of votes
    for (int candNum = 0; candNum < candidate_count; candNum++)
    {
        //get what is the smalles number of #1 votes
        if (!candidates[candNum].eliminated)
        {
            
            if (candidates[candNum].votes == min)
            {
                candidates[candNum].eliminated = true;
            }
        }
    }
    return;
}
