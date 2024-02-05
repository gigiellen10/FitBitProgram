/* Programmer: Genevieve Kochel
Date: 01/14/23
Assignment description: The purpose of this program is to read, filter and store data from a fitbit
smartwatch over a 24 hr period. The data will then be used to compute the total calories burned, 
distance walked in miles, floors walked, steps walked, the average heartrate, max steps walked in a minute,
and max length of consecutive poor sleep.
File purpose: This file contains guard code, all #includes and #defines, struct definitions and function
prototypes for the program
*/

#ifndef FITBIT_H
#define FITBIT_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COLS 100
#define MAX_ROWS 1450


typedef enum sleep
{
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

typedef struct fitbit
{
	char patient[10];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;

int correct_data(char arr[], char* delim);
void reset_array(char arr[], int size);
void calculateTotals(FitbitData records[], double* totalCalories, double* totalDistance, unsigned int* totalFloors, unsigned int* avgHeartRate,
	unsigned int* totalSteps);
unsigned int calculateMaxSteps(FitbitData records[], char maxMinutes[][100]); 
unsigned int calculatePoorSleep(FitbitData records[], char startRange[], char endRange[]);



#endif
