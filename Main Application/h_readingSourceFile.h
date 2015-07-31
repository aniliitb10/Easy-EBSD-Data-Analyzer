/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_readingSourceFile.h
*	date added: July 30, 2015
*	Copyright © 2015 Anil Kumar

*	This file is part of Easy EBSD Data Analyzer.

    Easy EBSD Data Analyzer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    Easy EBSD Data Analyzer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Easy EBSD Data Analyzer.  If not, see <http://www.gnu.org/licenses/>.
    
***************************************************************************************/


#pragma once
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "myAlgorithms.h"

using namespace std;

#define bufferSize 200
#define phaseNameSize 80

//to skip warnings for using fopen etc by Visual Studio
#ifdef _MSC_VER 
#pragma warning( disable : 4996)
#endif

unsigned int countNoOfLines(const char *filepath)
{
	char buffer[bufferSize];
	int i = 0;
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) fatal_here("in counting number of lines: Error code: 002\n");
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if (buffer[0] == '#') continue;
		else
		{
			++i;
			break;
		}
	}
	//following while loop is to avoid the if block of 1st while loop
	while (fgets(buffer, sizeof(buffer), fp) != NULL) //just to get speed and avoid the extra if block!
		++i;
	
	fclose(fp);
	return i;
}

void correctedNoOfLines(const char *filepath, unsigned int &DATA_SIZE)
{
	char buffer[bufferSize], phaseName[phaseNameSize];
	int edge, id, counter = 0;
	double temp = 0;
	unsigned int superI = 0;

	vector <double> info(8);

	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) fatal_here("in correcting number of lines: Error code: 004\n");
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if (buffer[0] == '#') continue;
		else
		{
			//here it meets the 1st row of data, ignores till finds the character "#"
			++superI;
			break;
		}
	}
	//following while loop is to avoid the if block of 1st while loop
	while ((fgets(buffer, sizeof(buffer), fp) != NULL) && counter < 2) //just to get speed and avoid the extra if block!
	{
		sscanf(buffer, "%lf	%lf %lf %lf %lf %lf %lf %lf %d %d %[^\n]%*c",
						&info[0], &info[1], &info[2], &info[3], &info[4], &info[5], &info[6], &info[7], \
						&id, &edge, phaseName);
		if (info[4] > temp)
			++counter;
		temp = info[4];
		++superI;
	}

	fclose(fp);
	--superI; //because during the break, scanning of 3rd row was in progress (after 1st and 2nd rows)
	DATA_SIZE = (DATA_SIZE / superI)*superI; //DATA_SIZE redefined
}

void readSourceFile(const char *filepath, const int &DATA_SIZE, vector <vector <double >> &euler, vector <double> &x_coord,
	vector <double> &y_coord, vector <double> &ci, vector <int> &grain_id)
{
	char buffer[bufferSize], phaseName[phaseNameSize];
	int i = 0, edge;
	double iq, fit;
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) fatal_here("in counting number of lines: Error code: 005\n");
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if (buffer[0] == '#') continue;
		else
		{
			
			sscanf(buffer, "%lf	%lf %lf %lf %lf %lf %lf %lf %d %d %[^\n]%*c",
				&euler[i][0], &euler[i][1], &euler[i][2], &x_coord[i], &y_coord[i], &iq, &ci[i], &fit, \
				&grain_id[i], &edge, phaseName);
			++i;
			break;
		}
	}
	//following while loop is to avoid the if block of 1st while loop
	while ((fgets(buffer, sizeof(buffer), fp) != NULL) && i < DATA_SIZE) //just to get speed and avoid the extra if block!
	{
		sscanf(buffer, "%lf	%lf %lf %lf %lf %lf %lf %lf %d %d %[^\n]%*c",
			&euler[i][0], &euler[i][1], &euler[i][2], &x_coord[i], &y_coord[i], &iq, &ci[i], &fit, \
			&grain_id[i], &edge, phaseName);
		++i;
	}

	fclose(fp);
}

void notice()
{
	printf("**********************************************************************\n");
	printf("Copyright (C) 2015 Anil Kumar\n\n");
	printf("This program is free software: you can redistribute it and/or modify\n");
	printf("it under the terms of the GNU General Public License as published by\n");
	printf("the Free Software Foundation, either version 3 of the License, or\n"); 
	printf("any later version.\n\n");
	printf("This program is distributed in the hope that it will be useful,\n");
	printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	printf("GNU General Public License for more details.\n\n");
	printf("You should have received a copy of the GNU General Public License\n");
	printf("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
	printf("**********************************************************************\n\n\n\n\n\n\n\n\n");
}