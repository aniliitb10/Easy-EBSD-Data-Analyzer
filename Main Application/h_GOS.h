/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_GOS.h
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
#include "myAlgorithms.h"
#include "h_grain_map.h"
#include "h_math.h"
#include "gnu_plot.h"

using namespace std;



extern vector <int> id, size;


void GOS_final(const vector <vector <double>> &euler, const vector <double> &ci, const int &DATA_SIZE,
	const double &min_ci, const int &bins, const vector <double> &x_coord, const vector < vector < double>> &userResponse);


void GOS(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci)
{

	//the only reason to pass euler is to calculate dis_array, if it hasn't yet been calcuated
	//-dis_array is used in labelling

	if (isSame(userResponse[0][0], 0) && isSame(userResponse[1][0], 0) && isSame(userResponse[2][0], 0) && \
		isSame(userResponse[3][0], 0) && isSame(userResponse[4][0], 0))
		dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);
	else
		; // this means dis_array has already been calculated either in option 1,2,3 or 4;



	if (isSame(userResponse[0][0], 0) && isSame(userResponse[4][0], 0))
	{
		size.resize(DATA_SIZE);
		id.resize(DATA_SIZE);

		cout << "Initializing id and size vectors....";

		for (int a = 0; a < DATA_SIZE; a++)
		{
			size[a] = 1;
			id[a] = a;
		}
		labelling(x_coord, dis_array, DATA_SIZE, userResponse[5][1], ci);
	}

	else
		;//that means, it has already been called and calculated!

	int bins = static_cast<int>(userResponse[5][3] + 0.05);
	double min_ci = userResponse[5][4];

	GOS_final(euler, ci, DATA_SIZE, min_ci, bins, x_coord, userResponse);

}


void GOS_final(const vector <vector <double>> &euler, const vector <double> &ci, const int &DATA_SIZE, const double &min_ci, \
	const int &bins, const vector <double> &x_coord, const vector < vector < double>> &userResponse)
{

	vector <int> count(DATA_SIZE);
	vector <double> devFromAve(DATA_SIZE); //of every pixel
	vector < vector<double>> tempMatrix(3, vector<double>(3));
	vector <unsigned long long> sumOfIndices(DATA_SIZE);

	//finding the geometric center

	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if (ci[i] >= min_ci)
		{
			sumOfIndices[id[i]] += i; //just summing the indices at their respective root;
			count[id[i]] += 1;
		}
	}

	vector <int> avgOfIndices(DATA_SIZE);
	int tempIndex = 0;
	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if ((ci[i] >= min_ci) && (count[i] >= 1) && (id[i] == i)) //that means if i goes through, i is root here; i = id[i]
		{
			tempIndex = static_cast<int>(sumOfIndices[i] / count[i]);
			avgOfIndices[i] = ((id[tempIndex] == i) ? tempIndex : i); //checking if i == id[i]
		}
	}


	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if ((ci[i] >= min_ci) && (count[id[i]] > 1)) //all grains having size > 1
		{
			devFromAve[i] = abs(misorientation(min_ci, min_ci, euler[i], euler[avgOfIndices[id[i]]])); //mind the change: abs

		}// in degree, because the result by paid software is in degrees
	}


	vector<double> sumDevFromAvgAtRoot(DATA_SIZE); //the sum will be stored "at root", need to go through all pixels
	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if ((ci[i] >= min_ci) && (count[id[i]] > 1)) // Not just root but all having size > 1
		{
			sumDevFromAvgAtRoot[id[i]] += devFromAve[i];
		}
	}

	vector <double> avgSumDevFromAvgAtRoot; //need to go though just root// anyway rest all count[i] are zero
	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if ((ci[i] >= min_ci) && (count[i] > 1) && (id[i] == i)) //only root will pass this condition
		{
			avgSumDevFromAvgAtRoot.push_back(sumDevFromAvgAtRoot[i] / (count[i]));
		}
	}

	vector <double> xgos(bins);
	vector <int> ygos(bins);
	double max_ = *max_element(avgSumDevFromAvgAtRoot.begin(), avgSumDevFromAvgAtRoot.end());
	double min_ = *min_element(avgSumDevFromAvgAtRoot.begin(), avgSumDevFromAvgAtRoot.end());
	int gos_response = static_cast<int>(userResponse[9][0]);
	
	reArrangeAll(userResponse, avgSumDevFromAvgAtRoot, xgos, ygos, 5, min_); //mind the change here

	vector<double> col(2);
	col = avgAndDev(avgSumDevFromAvgAtRoot);

	writeMyFile(col, "GOS.txt", xgos, ygos, "009", "Grain Orientation Spread", "Spread [degrees]", "Number");

	gnuPLotAll(gos_response, "GOS.txt", max_, min_, bins, "Angle (in degrees)", "Frequencies", "GOS plot", "GOS plot.png");

	cout << "\nFinished with GOS calculations\n\n";


	//avgOfIndices.clear();
	sumOfIndices.clear();
	count.clear();
	devFromAve.clear();
	sumDevFromAvgAtRoot.clear();
	avgSumDevFromAvgAtRoot.clear();
	col.clear();

}

