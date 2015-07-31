/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: DDP_back_end.cpp
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


#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>
#include <vector>
#include "h_grain_map.h"
#include "h_grain_bound.h"
#include "h_twin_bound.h"
#include "h_KAM.h" 
#include "h_GAM.h" 
#include "h_GOS.h"
#include "h_GSD.h"
#include "h_plot.h"
#include "h_math.h"
#include "myAlgorithms.h"
#include "gnu_plot.h"
#include "h_readingSourceFile.h"


using namespace std;



void grain_map(const vector <vector <double>> &euler, const vector <double> &x_coord, const vector <double> &y_coord, const int DATA_SIZE,\
	vector <vector <double>> &dis_array, const vector <vector<double>> &userResponse, const vector <double> &ci);
void grain_bound(const vector < vector < double>> &euler, vector <vector <double>> &dis_array, const vector <double> &x_coord, 
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci);
void twin_bound(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci);
void KAM(vector <vector <double>> &dis_array, const vector <vector<double>> &euler, const vector <double> &x_coord, const vector <double> &y_coord, \
	const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci);
void GAM(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci);
void GOS(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci);
void GSD(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord,
	const int &DATA_SIZE, const vector <vector <double>> &userResponse, const vector <int> &grain_id, const vector <double> &ci);

void open_existing_file(fstream &inFile, const char* filename, const char* errorCode);
void create_new_file(fstream &outFile, const char* filename, const char* errorCode);
unsigned int countNoOfLines(const char *filepath);
void correctedNoOfLines(const char *filepath, unsigned int &DATA_SIZE);
void readSourceFile(const char *filepath, const int &DATA_SIZE, vector <vector <double >> &euler, vector <double> &x_coord,
	vector <double> &y_coord, vector <double> &ci, vector <int> &grain_id);
void notice();
//global variables
double min_ci;



int main(int argc, char* argv[])
{

	notice();
	cout << "The current working directory is:\n" << argv[0] << "\n\n";
	

	//working on calculating the total runtime here
	time_t time_begin, time_bet, time_end;
	int time_diff_sec;
	time(&time_begin);
	

	char filepath[80];
	unsigned int DATA_SIZE = 0;
	fstream infile1;
	open_existing_file(infile1, "filepath.txt", "001");

	infile1.getline(filepath, 200);
	//infile1 >> DATA_SIZE; //now there is no longer any data_size entry in this file
	infile1.close(); // done with the file that stores the address and no of lines to be processed.

	
	DATA_SIZE = countNoOfLines(filepath);
	printf("The total number of lines is: %d\n\n", DATA_SIZE);
	correctedNoOfLines(filepath, DATA_SIZE);
	printf("The corrected number of lines is: %d\n\n", DATA_SIZE);

	fstream infile2;
	open_existing_file(infile2, filepath, "005");
	//start from here

	cout << "Declaring vectors....";
	
	vector <vector <double >> euler(DATA_SIZE, vector <double>(3));
	vector <double> x_coord(DATA_SIZE);
	vector <double> y_coord(DATA_SIZE);
	//ector <double> iq(DATA_SIZE);
	vector <double> ci(DATA_SIZE);
	//vector <double> fit(DATA_SIZE);
	vector <int> grain_id(DATA_SIZE);
	//vector <int> edge(DATA_SIZE);
	vector <vector <double>> dis_array(DATA_SIZE, vector <double>(6));
	//vector <double> gam(DATA_SIZE);
	//string  phase_name1, phase_name2, phase_name3, phase_name4;

	
	// don't forget to clear these vectors

	printf("\n\nStoring data...");
	readSourceFile(filepath, DATA_SIZE, euler, x_coord, y_coord, ci, grain_id);




	time(&time_bet);
	time_diff_sec = static_cast<int>(difftime(time_bet, time_begin));
	printf("\n\nTime taken to store the data is is:%2d minutes %2d seconds\n\n", (time_diff_sec / 60), (time_diff_sec % 60));
	

	
	//======================================= Reading the response of the user ========================================
	int j = 0;
	fstream infile3;
	open_existing_file(infile3, "response.txt", "006");

	
	vector <vector<double>> userResponse(11, vector <double>(5)); //initialized too, mind the change here 7 to 11;
	while (infile3 && (j < 11)) //what if there are just 7 lines, infile is NULL or what ??
	{
		infile3 >> userResponse[j][0] >> userResponse[j][1] >> userResponse[j][2] >> userResponse[j][3] >> userResponse[j][4];
		j++;
	}


	min_ci = userResponse[0][4];
	userResponse[2][0] = 0; //as no more twin boundary calculations!

	//============================ done with reading the UserResponse in array double userResponse ==========================
	//becasue decimal are never stored exactly, it is advisable to use a function to compare them
	//calling different functions, based on the options of userInterface
	if (isSame(userResponse[0][0], 1))
		grain_map(euler, x_coord, y_coord, DATA_SIZE, dis_array, userResponse, ci);
	if (isSame(userResponse[1][0], 1))
		grain_bound(euler, dis_array, x_coord, y_coord, DATA_SIZE, userResponse, ci);
	if (isSame(userResponse[2][0], 1))
		twin_bound(dis_array, euler, x_coord, y_coord, DATA_SIZE, userResponse, ci);
	if (isSame(userResponse[3][0], 1))
		KAM(dis_array, euler, x_coord, y_coord, DATA_SIZE, userResponse, ci);
	if (isSame(userResponse[4][0], 1))
		GAM(dis_array, euler, x_coord, DATA_SIZE, userResponse, ci);
	if (isSame(userResponse[5][0], 1))
		GOS(dis_array, euler, x_coord, y_coord, DATA_SIZE, userResponse, ci);
	if (isSame(userResponse[6][0], 1))
		GSD(dis_array, euler, x_coord, DATA_SIZE, userResponse, grain_id, ci);
	

//=========================================== Following steps should be at the end of everything ========================
	
	cout << "\nClearing vectors .......";
	
	euler.clear();
	x_coord.clear();
	y_coord.clear();
	//iq.clear();
	ci.clear();
	//fit.clear();
	grain_id.clear();
	//edge.clear();
	dis_array.clear();
	
	cout << "\n";

	//and here the time calculation finalizes
	time(&time_end);
	time_diff_sec = static_cast<int>(difftime(time_end, time_begin));
	printf("\n\nThe total time taken is:%2d minutes %2d seconds\n\n", (time_diff_sec / 60), (time_diff_sec % 60));
	
	system("pause");
	return 0;
}
