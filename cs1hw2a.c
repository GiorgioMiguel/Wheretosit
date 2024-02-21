/*
COP 3502 Assignment 2 PART A "Where to Sit?"
© Giorgio Torregrosa 2024
*/

//including proper libs/definitions  
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining a struct to hold information about each attendee
typedef struct  
{
	char name[20];
	char enemy[10][20];
	int holding;
	int totalEnemies;
} Attendee;

//two functions from class (swap/permute) and a third function (validPerm) I created to validate permutations.
void permute(Attendee* attendee, int l, int r);
void swap(Attendee* attendeeA, Attendee* attendeeB);
int validPerm(Attendee* attendees, int size);

//total amount of valid permutations
int totalPerms = 0;

//start main
int main(int argc, char *argv[])
{
	//local variables used to process information from input files to an array of structs w/ a file pointer
	int totalAttendees = 0;
	int totalPairs = 0;
	Attendee Attendees[10] = { NULL };
	FILE* fPtr = NULL;
	fPtr = fopen(argv[1], "r");
	if (!fPtr) //if file fails to open return exit failure
	{
		return EXIT_FAILURE;
	}

	//scan in the total number of movie goers and the total pairs of enemies
	if (fscanf(fPtr, "%d %d", &totalAttendees, &totalPairs) != 2) 
	{
		return EXIT_FAILURE;//exit if more than two items were scanned in
	}

	//loop through all the movie goers and assign them to the attendees array of structs 
	for (int i = 0; i < totalAttendees; i++)
	{
		if (fscanf(fPtr, "%19s %d", &Attendees[i].name, &Attendees[i].holding) != 2) 
		{
			return EXIT_FAILURE;//exit if more than two items were scanned in
		}
		strcpy(Attendees[i].enemy[0], "None");//intialize all movie goers enemies to none
	}
	
	//creating temp variables
	char target[20] = { NULL };
	char enemy[20] = { NULL };
	//scanning in the information from the given file
	for (int i = 0; i < totalPairs; i++)
	{
		if (fscanf(fPtr,"%19s %19s", &target, &enemy) != 2)//at each iteration two names will be collected from the file. 
		{
			return EXIT_FAILURE;
		}
		for (int j = 0; j < totalAttendees; j++)//this loop will assign enemies to elements in my array of structs
		{
			if (strcmp(Attendees[j].name, target) == 0)//if target matches a name in my array of structs, assign them enemy as one of their enemies.
			{
				strcpy(Attendees[j].enemy[Attendees[j].totalEnemies], enemy);
				Attendees[j].totalEnemies++;
			}
		
			if (strcmp(Attendees[j].name, enemy) == 0)//if enemy matches a name in my array of structs, assign them target as one of their enemies.
			{
				strcpy(Attendees[j].enemy[Attendees[j].totalEnemies], target);
				Attendees[j].totalEnemies++;
				break;
			}
		}
	}
	fclose(fPtr);
	//PERMUTE
	permute(Attendees, 0, totalAttendees - 1);
	printf("\n\ntotal valid permutations: %d\n\n", totalPerms);
	
	return 0;
}//end part A

int validPerm(Attendee* attendees, int size)
{
	for (int i = 0; i < size; i++)
	{
		//popcorn check
		if (attendees[i].holding != 1)//check to see if the current position has popcorn 
		{
			if (i == 0) //cannot check left, only right
			{
				if (attendees[i + 1].holding != 1) //if right doesnt have popcorn, permutation is invalid
				{
					return 0; //invalid permutation
				}
			}
			else if (i == size - 1)//cannot check right, only left
			{
				if (attendees[i - 1].holding != 1) //if right doesnt have popcorn, permutation is invalid
				{
					return 0; //invalid permutation
				}
			}
			else if (i != 0 && i != size - 1) //check left and right
			{
				if (attendees[i - 1].holding == 0 && attendees[i + 1].holding == 0)
				{
					return 0; //invalid permutation
				}
			}
		}
		
			//enemy check
		if (strcmp(attendees[i].enemy, "None") != 0)//check to see if the current position has an enemy 
		{
			for (int k = 0; k < attendees[i].totalEnemies; k++) {
				if (i == 0) //cannot check left, only right
				{
					if (strcmp(attendees[i].enemy[k], attendees[i + 1].name) == 0) //if person to the right is the enemy of current person
					{
						return 0; //invalid permutation
					}
				}
				else if (i == size - 1)//cannot check right, only left
				{
					
					if (strcmp(attendees[i].enemy[k], attendees[i - 1].name) == 0) //if right doesnt have popcorn, permutation is invalid
					{
						return 0; //invalid permutation
					}	
				}
				else if (i != 0 && i != size - 1) //check left and right
				{
					if (strcmp(attendees[i].enemy[k], attendees[i - 1].name) == 0 || strcmp(attendees[i].enemy[k], attendees[i + 1].name) == 0)
					{
						return 0; //invalid permutation
					}
				}
			}
		}
	}//end looping

	return 1;//if all checks were passed then return 1 for valid permutation

}//end validity check

//swap function from class
void swap(Attendee* attendeeA, Attendee* attendeeB) {
	Attendee temp = *attendeeA;
	*attendeeA = *attendeeB;
	*attendeeB = temp;
}

//permute function from class
void permute(Attendee* attendee, int l, int r) {

	int i;
	int size = r + 1;

	if (l == r && validPerm(attendee, size) == 1)
	{
		totalPerms++;
	}
	else 
	{
		for (i = l; i <= r; i++) 
		{
			swap(&attendee[l], &attendee[i]);
			permute(attendee, l + 1, r);
			swap(&attendee[l], &attendee[i]); // Swap back for backtracking
		}
	}
}