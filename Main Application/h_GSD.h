/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_GSD.h
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
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "myAlgorithms.h"
#include "h_bar_plot.h"
#include "gnu_plot.h"

using namespace std;

extern vector <int> id, size;
extern double min_ci;



void GSD_Calculation(const int &DATA_SIZE, const int &bins, const vector <double> &ci, const int &min_grain_size, \
	const double &step_size, const vector <vector <double>> &userResponse);
//void GSD_calc(vector <double> &xgsd, vector <double> &ygsd, const int &min_grain_size, const int &bins, const int &DATA_SIZE,
	//const double &step_size);


void gnu_plot(char* data_file_name, const double& widh, char *x_label, char *y_label, char *title, char *ouput_file_name);

void create_new_file(fstream &outFile, const char* filename, const char* errorCode);



void GSD(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord,
	const int &DATA_SIZE, const vector <vector <double>> &userResponse, const vector <int> &grain_id, const vector <double> &ci)
{
	//the only reason to pass euler is to calculate dis_array, if it hasn't yet been calcuated
	//-dis_array is used in labelling
	
	if (isSame(userResponse[0][0], 0) && isSame(userResponse[1][0], 0) && isSame(userResponse[2][0], 0) && \
		isSame(userResponse[3][0], 0) && isSame(userResponse[4][0], 0) && isSame(userResponse[5][0], 0))
		dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);
	else
		; // this means dis_array has already been calculated either in option 1,2,3 or 4;
	
	if (isSame(userResponse[0][0], 0) && isSame(userResponse[4][0], 0) && isSame(userResponse[5][0], 0))
	{
		size.resize(DATA_SIZE);
		id.resize(DATA_SIZE);

		cout << "Initializing id and size vectors....";

		for (int a = 0; a < DATA_SIZE; a++)
		{
			size[a] = 1;
			id[a] = a;
		}
		labelling(x_coord, dis_array, DATA_SIZE, userResponse[6][1], ci);
	}

	else
		;//that means, it has already been called and calculated!

	//done with the calculation of dis_array and id and size

	//unsigned int max_root = *max_element(id.begin(), id.end());
	const double step_size = x_coord[1] - x_coord[0];
	
	
	//######################################### completely different calculation, only using grain_id ###################################
	int max_id = *max_element(grain_id.begin(), grain_id.end());
	vector <int> size_(max_id+1);
	vector <double> diameter;
	int min_grain_size = static_cast<int>(userResponse[6][2] + 0.05);
	int bins = static_cast<int>(userResponse[6][3] + 0.05);
	

	for (int i = 0; i < DATA_SIZE; ++i)
		if (ci[i] >= min_ci)
			size_[grain_id[i]] += 1;         //change here, from size_[grain_id[i]-1] to size_[grain_id[i]] as there is id=0 also 

	for (int i = 0; i <= max_id; ++i) //change here, equality sign added 
		if (size_[i] >= min_grain_size )// && ci[i] >= 0; ci[i] and size_[i] don't represent the same pixel!! loss of Random Numbering!
			diameter.push_back(1.05*step_size*sqrt(size_[i]));

	vector<double> xgsd(bins);
	vector <int> ygsd(bins);
	double min_ = *min_element(diameter.begin(), diameter.end());
	double max_ = *max_element(diameter.begin(), diameter.end());
	int gsd_response = static_cast<int>(userResponse[10][0]);

	reArrangeAll(userResponse, diameter, xgsd, ygsd, 6, min_);

	vector<double> col(2);
	col = avgAndDev(diameter);

	writeMyFile(col, "GSD gid.txt", xgsd, ygsd, "010", "Grain Size(diameter)", "Diameter [microns]", "Number");

	gnuPLotAll(gsd_response, "GSD gid.txt", max_, min_, bins, "Angle (in degrees)", "Frequencies", "GSD plot", "GSD gid plot.png");

	size_.clear();
	diameter.clear();
	col.clear();
	//############################################ done computing with grain_id ##########################################################

	GSD_Calculation(DATA_SIZE, bins, ci, min_grain_size, step_size, userResponse);

}



void GSD_Calculation(const int &DATA_SIZE, const int &bins, const vector <double> &ci, const int &min_grain_size, \
	const double &step_size, const vector <vector <double>> &userResponse)
{
	vector <int> size_(DATA_SIZE);
	vector <double> diameter;
	for (int i = 0; i < DATA_SIZE; ++i)
		if (ci[i] >= min_ci)// && ci[id[i]] >= 0.1) //now size will not contain low ci pixels! ----- as id[i] = i for low ci pixels
			size_[id[i]] += 1;

	for (int i = 0; i <DATA_SIZE; ++i) //change here, equality sign added 
		if ((size_[i] >= min_grain_size) && (ci[i] >= min_ci)) // if ci < 0.1 then size_[i] === 0, so unnecessary condition!
			diameter.push_back(1.05*step_size*sqrt(size_[i]));

	vector<double> xgsd(bins);
	vector <int> ygsd(bins);
	double min_ = *min_element(diameter.begin(), diameter.end());
	double max_ = *max_element(diameter.begin(), diameter.end());
	int gsd_response = static_cast<int>(userResponse[10][0]);

	reArrangeAll(userResponse, diameter, xgsd, ygsd, 6, min_);

	vector<double> col(2);
	col = avgAndDev(diameter);

	writeMyFile(col, "GSD.txt", xgsd, ygsd, "010", "Grain Size(diameter)", "Diameter [microns]", "Number");

	gnuPLotAll(gsd_response, "GSD.txt", max_, min_, bins, "Angle (in degrees)", "Frequencies", "GSD plot", "GSD plot.png");

	cout << "\nFinished with GSD calculations\n\n";

	size_.clear();
	diameter.clear();
	col.clear();
}

