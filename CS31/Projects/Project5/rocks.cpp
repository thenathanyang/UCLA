#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <cstring>
#include <cctype>
#include "utilities.h"
using namespace std;

// computer picks secret word of 4 to 6 letters
// rock: letter in word and same position
// pebble: letter in word but not in same position
// score: number of probe words needed to get secret word; includes the one that gets the secret one

int manageOneRound(const char words[][7], int nWords, int wordnum);
int isNotLower(const char probeWord[200]);
int isWordValid(const char words[][7], char secretWord[7], int nWords);

int main()
{
	char wordList[10000][7];
	int nWords = loadWords(wordList, 10000);
	int numRounds = 0;
	int score = 0;
	double totalScore = 0;
	double average = 0;
	int minScore = 0;
	int maxScore = 0;

	if (nWords < 1)		// Checks to see if words were probably loaded
	{
		cout << "No words were loaded, so I can't play the game." << endl;
		return 1;
	}

	cout << "How many rounds do you want to play? ";		// Asks user how many rounds he/she wants to p lay
	cin >> numRounds;
	cin.ignore(10000, '\n');

	if (numRounds < 0)			// Tells user number of rounds must be positive if number entered is negative
	{
		cout << "The number of rounds must be positive.";
		return 1;
	}

	for (int currentRound = 1; currentRound <= numRounds; currentRound++)
	{
		int wordnum = randInt(0, nWords - 1);	// Selects random word
		int len = strlen(wordList[wordnum]);

		cout << endl;
		cout << "Round " << currentRound << endl;		// Executes one round of the game
		cout << "The secret word is " << len << " letters long." << endl;
		score = manageOneRound(wordList, nWords, wordnum);
		if (score == 1)
			cout << "You got it in " << score << " try." << endl;
		else
			cout << "You got it in " << score << " tries." << endl;

		totalScore += score;
		average = totalScore / currentRound;		// Calculates total average score

		if (currentRound == 1)		// Sets min and maxx score to score for first round
		{
			minScore = score;
			maxScore = score;
		}
		if (score < minScore)		// Sets minScore to the lowest score
			minScore = score;
		if (score > maxScore)		// Sets maxScore to the highest score
			maxScore = score;

		cout.setf(ios::fixed);		// Sets average to print out with two decimal places
	    cout.precision(2);

		cout << "Average: " << average << ", minimum: " << minScore << ", maximum: " << maxScore << endl;
	}
	return 0;
}

int manageOneRound(const char words[][7], int nWords, int wordnum)
{
	if (nWords < 0 || wordnum < 0 || wordnum >= nWords)
		return -1;

	char probeWord[200];
	char secretWord[7];
	strcpy(secretWord, words[wordnum]);
	int roundComplete = 0;
	int score = 0;

	while(!roundComplete)
	{
		int rocks = 0;
		int pebbles = 0;

		cout << "Probe Word: ";			// Asks user to enter a probe word
		cin.getline(probeWord, 200);

		if (strlen(probeWord) < 4 || strlen(probeWord) > 6 || isNotLower(probeWord))	// Makes sure that probeWord is between 4 to 6 lower case characters
		{
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
			continue;
		}

		int q = isWordValid(words, probeWord, nWords);		// Determines if probeWord is a valid word
		if (q == 0)
		{
			cout << "I don't know that word." << endl;
			continue;
		}

		score++;

		if (!strcmp(secretWord, probeWord))		// If probeWord matches secretWord, round is complete
		{
			roundComplete = 1;
			break;
		}


		int probeLen = strlen(probeWord);
		int secretLen = strlen(secretWord);
		char probeWordCopy[200];
		char secretWordCopy[200];
		strcpy(probeWordCopy, probeWord);
		strcpy(secretWordCopy, secretWord);

		for (int p = 0; p < min(probeLen, secretLen); p++) 
		{
			if (probeWordCopy[p] == secretWordCopy[p]) 
			{
				rocks++;					// Add one to rocks for each character that is in both probeWord and secretWord at the same position
				secretWordCopy[p] = 0;		// Sets the character at that position to 0 in both probeWord and secretWord
				probeWordCopy[p] = 0;
			}
		}

		for (int k = 0; k < probeLen; k++)
		{
			for (int i = 0; i < secretLen; i++)
			{
				if (!(probeWordCopy[k] == 0 || secretWordCopy[i] == 0) && probeWordCopy[k] == secretWordCopy[i])
				{

					pebbles++;		// Add one to pebbles for each character that is in both probeWord and secretWord but are not at the same position
					secretWordCopy[i] = 0;	// Sets the character at that position to 0 in both probeWord and secretWord
					probeWordCopy[k] = 0;
					break;			// Breaks out of innner for loop in order to prevent double checking
				}
			}
		}

		cout << "Rocks: " << rocks << ", Pebbles: " << pebbles << endl;		// Prints out number of rocks and pebbles for each probeWord
	}

	return score;		// Returns the score of that round
}


int isNotLower(const char probeWord[200])		// Checks if probeWord is lower case or not
{
	int notLower = 0;

	for (int x = 0; probeWord[x] != '\0'; x++)
	{
		if (!islower(probeWord[x]))
		{
			notLower = 1;
		}
	}
	return notLower;
}

int isWordValid(const char words[][7], char probeWord[200], int nWords)		// Checks if probeWord is a valid word in words
{
		for (int y = 0; y < nWords; y++)
		{
			if (strcmp(probeWord, words[y]) == 0)
			{
				return 1;
			}
		}
		return 0;
}

