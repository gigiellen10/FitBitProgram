#include "fitbit.h"


/* Programmer: Genevieve Kochel
Date: 01/14/23
Assignment description: The purpose of this program is to read, filter and store data from a fitbit
smartwatch over a 24 hr period. The data will then be used to compute the total calories burned,
distance walked in miles, floors walked, steps walked, the average heartrate, max steps walked in a minute,
and max length of consecutive poor sleep.
File purpose: This file contains main(), the driver of the program
*/

int main(void)
{

	FitbitData records[1440]; // array uninitialized, may change???
	char filteredLines[MAX_ROWS][MAX_COLS] = { {""} }; // dupStr is variable to hold duplicate of unedited string

	char target[50] = "", lastTime[50] = "", token[50] = "", timeToken[50] = "";
	int linesRead = 0, rowIndex = 0, filteredRow = 0, recordsIndex = 0; // filteredRow controls row of filtered data array

	FILE* infile = fopen("FitbitData.csv", "r");

	// check if FitbitData opened successfully
	if (infile != NULL)
	{
		char line[MAX_COLS] = "";
		for (; fgets(line, MAX_COLS, infile) != NULL; ++linesRead) // read until end of file (fgets() returns NULL when at the end)
		{
			
			char dupStr[100] = ""; // initialize here so dupStr is reset to empty at every iteration
			int invalid = 0;
			// get and store target
			if (linesRead == 0)
			{
				strtok(line, ",");
				strcpy(target, strtok(NULL, ","));
			}
			else // linesRead not == 0 aka other data besides line 1
			{
				
				// make a copy of line[] to store before editing for future use
				strcpy(dupStr, _strdup(line));

				// cleanse any data/empty fields with invalid data -> '#'
				invalid = correct_data(line, ",");


				// determine if first field matches target patient/if duplicate entry -> based on time
				reset_array(token, 50); // reset token array to null chars
				reset_array(timeToken, 50); // reset timeToken array to null chars

				strcpy(token, strtok(line, ","));
				strcpy(timeToken, strtok(NULL, ","));

				// store data if not duplicate and matches target patient
				if (strcmp(token, target) == 0 && strcmp(timeToken, lastTime) != 0)
				{
					reset_array(lastTime, 50); // reset last time array
					strcpy(lastTime, timeToken);
					strcpy(records[recordsIndex].patient, token); // store patient number
					strcpy(records[recordsIndex].minute, timeToken); // store time
					records[recordsIndex].calories = atof(strtok(NULL, ",")); // calories
					records[recordsIndex].distance = atof(strtok(NULL, ",")); // distance
					records[recordsIndex].floors = atoi(strtok(NULL, ",")); // floors
					records[recordsIndex].heartRate = atoi(strtok(NULL, ",")); // heartRate
					records[recordsIndex].steps = atoi(strtok(NULL, ",")); // steps
					records[recordsIndex].sleepLevel = atoi(strtok(NULL, "\n")); // sleepLevel


					++recordsIndex; // increase index in records[] struct array
				}
				else  // invalid line, so store in filteredArr[][]
				{
					invalid = 1;
				}

				if (invalid) // if found that line contained any invalid data, store in invalid array for printing at end
				{
					strcpy(filteredLines[filteredRow], dupStr);
					++filteredRow;
				}

			}

			reset_array(line, 100); // reset line array

		}

	
	}

	/* Perform computations */
	
	double totalCalories = 0, totalDistance = 0;
	unsigned int totalFloors = 0, avgHeartRate = 0, totalSteps = 0, maxSteps = 0, maxPoorSleep = 0;
	char maxMinutes[][100] = { {""} }, startRange[50] = "", endRange[50] = ""; // array stores minute strings where max steps occured

	calculateTotals(records, &totalCalories, &totalDistance, &totalFloors, &avgHeartRate, &totalSteps);
	maxSteps = calculateMaxSteps(records, maxMinutes);
	maxPoorSleep = calculatePoorSleep(records, startRange, endRange);




	/* Print Results */

	FILE* outfile = fopen("Results.csv", "w"); // open outfile

	if (outfile != NULL) // check if Results file opened successfully
	{
		int i = 0;
	
		// print results to screen and Results.csv

		fputs("Total Calories,Total Distance,Total Floors,Total Steps, Avg Heartrate, Max Steps(minute(s)), Sleep\n", outfile);
		fprintf(outfile, "%lf,%lf,%u,%u,%u,%u(%s),%s:%s\n", totalCalories, totalDistance, totalFloors, totalSteps, avgHeartRate, 
			maxSteps, maxMinutes[0], startRange, endRange);
		fputs("\n********** Original data deduped, filtered or cleansed **********\n", outfile);
		
		printf("Total Calories,Total Distance,Total Floors,Total Steps, Avg Heartrate, Max Steps(minute(s)), Sleep\n");
		printf("%lf,%lf,%u,%u,%u,%u(%s),%s:%s\n", totalCalories, totalDistance, totalFloors, totalSteps, avgHeartRate,
			maxSteps, maxMinutes[0], startRange, endRange);
		printf("\n******** Original data deduped, filtered or cleansed *********\n");
		for (; i < filteredRow; ++i)
		{
			fputs(filteredLines[i], outfile);
			printf("%s", filteredLines[i]);

		}
	}

	fclose(infile);
	fclose(outfile);

	return 0;
}