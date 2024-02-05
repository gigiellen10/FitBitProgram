#include "fitbit.h"

/* Programmer: Genevieve Kochel
Date: 01/14/23
Assignment description: The purpose of this program is to read, filter and store data from a fitbit
smartwatch over a 24 hr period. The data will then be used to compute the total calories burned,
distance walked in miles, floors walked, steps walked, the average heartrate, max steps walked in a minute,
and max length of consecutive poor sleep.
File purpose: This file contains all of the function definitions for the program
*/



/* Function: int correct_data(char arr[], char* delim)
Description: Identifies empty fields (',,' or ',\0') in string (arr[]) based on a delimeter string and performs data cleansing on 
the empty fields...inserts -1 in most cases (two seperate characters). 
Date created: 01/18/23
Date last modified: 01/18/23
Accepts: A string (character array), and a delimeter string. 
Returns: Function returns 0 if no empty fields were found, 1 if empty fields were
found and cleansed. This return value is used to identify which lines to store in filteredData[][] array 
Preconditions: The character array put in argument 1 must be null-terminated. This array must be defined in main().
Postconditions: Upon finding an empty field (2 consecutive delimeters), the function must shift all elements in the array
to the right by 2 places, and fill those places in the character array with, first, a '-' and second, a '1' to represent -1 */
int correct_data(char arr[], char* delim)
{
	int cleansed = 0, index = 0;

	// search array for consecutive delimeters until reach null character
	for (; arr[index] != '\0'; ++index)
	{
		if ((arr[index] == *delim && arr[index + 1] == *delim) || (arr[index] == *delim && arr[index + 1] == '\n')) // found empty field
		{
			cleansed = 1; // indicate that empty field was found
			int startIndex = index, endIndex = index; // save index of first delimeter out of 2; 1st index to save start, 2nd to find end of string
			for (; arr[endIndex] != '\0'; ++endIndex); // find end/null char in array, set to endIndex
			// starting at the end, shift everything over by 1 until startIndex and endIndex are equal
			for (; endIndex != startIndex; --endIndex)
			{
				arr[endIndex + 2] = arr[endIndex];

			}

			arr[startIndex + 1] = '-'; // replace empty field with -1 value...could be without quotes????? check this
			arr[startIndex + 2] = '1';
			
		}	
	}

	return cleansed;

}

/* Function: void reset_array(char arr[], int size)
Description: Resets any array to contain all null characters
Date Created: 01/18/23
* Date Last Modified: 01/18/23
Accepts: An array of characters and the size of that character array
Returns: None, void
Preconditions: The array must be defined in main().
Postconditions: The array must be edited to reflect all '\0' in every index */
void reset_array(char arr[], int size)
{
	int index = 0;

	for (; index < size; ++index)
	{
		arr[index] = '\0'; // reset array to all null characters
	}
}


/* Function: void calculateTotals()
Description: Calculates the totals for calories, distance, floors, heartRate and totalSteps. 
Date Created: 01/18/23
* Date Last Modified: 01/18/23
Accepts: A records array of type FitbitData, 2 pointers to doubles for the calories and distance, and 3 pointers to unsigned ints
that reprent total floors, heartrate and total steps
Returns: None, void
Preconditions: The records[] array must be filled with accurate fitbit data and any empty fields must contain a (). The indirect values of all the pointers
must be 0 before calling the function.
Postconditions: The totals for calories, distance, floors, heartrate, and total steps must be indirectly returned via the pointers 
to thier respective variables */
void calculateTotals(FitbitData records[], double* totalCalories, double* totalDistance, unsigned int* totalFloors, unsigned int* avgHeartRate,
	unsigned int* totalSteps)
{
	int i = 0, validHeartData = 0, invalid = 0; // heartData indicates how many valid heartrates were added to the total HR sum to calculate avg
	double sumCals = 0, sumDistance = 0;
	unsigned int sumFloors = 0, sumHeartRate = 0, sumSteps = 0;

	for (; i < 1440; ++i)
	{
		
		if (records[i].calories != -1) // if not invalid data/empty field; ADD CONDITION FOR INVALID
		{
			sumCals += records[i].calories; 
		}
		if (records[i].distance != -1) // if not invalid data/empty field
		{
			sumDistance += records[i].distance;
		}
		if (records[i].floors != 4294967295) // if not invalid data/empty field
		{
			sumFloors += records[i].floors;
		}
		if (records[i].heartRate != 4294967295) // if not invalid data/empty field
		{
			sumHeartRate += records[i].heartRate;
			++validHeartData;
		}
		if (records[i].steps != 4294967295) // if not invalid data/empty field
		{
			sumSteps += records[i].steps;
		}
	}

	// return the sums via ptrs
	*totalCalories = sumCals;
	*totalDistance = sumDistance;
	*totalFloors = sumFloors;
	*avgHeartRate = sumHeartRate / (unsigned int)validHeartData;
	*totalSteps = sumSteps;

}

/* Function: unsigned int calculateMax(FitbitData records[], char maxMinutes[][100])
* Date Created: 01/18/23
* Date Last Modified: 01/18/23
Description: Calculates the maximum number of steps taken in a minute. If the max appears in multiple minutes, the 
function reflects those minute strings in the maxMinutes array
Accepts: A records array of type FitbitData and a 2d maxMinutes array.
Returns: The maximum steps taken in a minute.
Preconditions: The records[] array must be filled with accurate fitbit data and any empty fields must contain a ()
Postconditions: The maxMinutes array must reflect the strings of the minutes where the maximum steps appeared in the records */
unsigned int calculateMaxSteps(FitbitData records[], char maxMinutes[][100])
{
	int i = 0, maxMinutesRow = 0;
	unsigned int max = 0;

	// determine max
	for (; i < 1440; ++i)
	{
		if (records[i].steps != 4294967295 && max < records[i].steps) // not invalid and new max
		{
			max = records[i].steps; // found new max # steps
		}
	}

	// find and store minutes where max occurs if multiple times
	for (i = 0; i < 1440; ++i)
	{
		if (records[i].steps == max)
		{
			strcpy(maxMinutes[maxMinutesRow], records[i].minute);
			++maxMinutesRow;
		}
	}

	return max;
}

/* Function: unsigned int calculatePoorSleep(FitbitData records[], char maxMinutes[][100])
* Date Created: 01/18/23
Description: Calculates the maximum sum of consecutive poor sleep. Reports the range of minutes that the max poor sleep occurs within.
Accepts: A records array of type FitbitData and a 2d rangeMinutes array.
Returns: The maximum sum of a range of poor sleep (sleep level > 1)
Preconditions: The records[] array must be filled with accurate fitbit data and any empty fields must contain a ()
Postconditions: The rangeMinutes array must reflect the strings of the starting and ending minutes containing the max sum of poor sleep */
unsigned int calculatePoorSleep(FitbitData records[], char startRange[], char endRange[])
{
	unsigned int currSum = 0, maxSum = 0;
	char start[50] = "", end[50] = "";

	for (int i = 0; i < 1440; ++i)
	{
		if (records[i].sleepLevel != 4294967295 && records[i].sleepLevel > 1)
		{
			currSum += records[i].sleepLevel;
			if (records[i - 1].sleepLevel <= 1) // start of consecutive range
			{
				reset_array(start, 50); // found new start so reset
				strcpy(start, records[i].minute);
			}
			
		}
		else // not considered poor sleep...end of consecutive range
		{
			currSum = 0; // reset sum
		}

		if (currSum > maxSum) // end of consecutive range, i! so does not go outside of bounds
		{
			maxSum = currSum; // found new maxSum
			reset_array(end, 50); // found new end so reset end array
			strcpy(end, records[i].minute);
		}

	}

	strcpy(startRange, start);
	strcpy(endRange, end);
	return maxSum;

}
