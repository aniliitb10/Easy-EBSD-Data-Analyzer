/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_KAM.h
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
#include <algorithm>
#include <fstream>
#include <Windows.h>
#include <tuple>
#include "h_plot.h"
#include "h_math.h"
#include "myAlgorithms.h"
#include "gnu_plot.h"
using namespace std;

extern vector <int> id, size;
extern double min_ci;

//function definitions
//void time_spent(SYSTEMTIME t0);

void KAM_calc(const vector <vector <double>> &dis_array, const vector <double> &x_coord, const int &DATA_SIZE,
	const double &max_dis_angle, vector <double> &kam);


double KAM_data_calc(const double &misorient, const vector <vector <double>> &dis_array, const int &i,
	const int &p=10, const int &q=10, const int &r=10, const int &s=10); //default values

void kam_dis(const vector <double> &kam, const vector <vector <double>> &userResponse);


void gnu_plot(char* data_file_name, const double& width, char *x_label, char *y_label, char *title, char *ouput_file_name);

//global variables




void KAM(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector<double> &ci)
{
	//the only reason to pass euler is to calculate dis_array, if it hasn't yet been calcuated 
	//the only reason to pass y_coord is to be able to pass y_max for plot_kam 
	//-(maintain coherancy by passing y_max instead of y_max itself)
	//=================checking whether we need to calculate dis_array and ID or not=============================================
	
	if (isSame(userResponse[0][0], 0) && isSame(userResponse[1][0], 0) && isSame(userResponse[2][0], 0))
		dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);
	else
		; // this means dis_array has already been calculate either in option 1, 2 or 3;
	
	if (isSame(userResponse[0][0], 0))
	{
		size.resize(DATA_SIZE);
		id.resize(DATA_SIZE);

		cout << "Initializing id and size vectors....";

		for (int a = 0; a < DATA_SIZE; a++)
		{
			size[a] = 1;
			id[a] = a;
		}
		labelling(x_coord, dis_array, DATA_SIZE, userResponse[3][1], ci);
	}

	else
		;//that means, it has already been called and calculated!


	vector <double> kam(DATA_SIZE);

	KAM_calc(dis_array, x_coord, DATA_SIZE, userResponse[3][1] , kam); //function call and writes the values in a file
	
	kam_dis(kam, userResponse);

	kam.clear();
}

void KAM_calc(const vector <vector <double>> &dis_array, const vector <double> &x_coord, const int &DATA_SIZE, 
	const double &max_dis_angle, vector <double> &kam)
{
	double x_max = *max_element(x_coord.begin(), x_coord.end());
	double step_size = x_coord[1] - x_coord[0];
	//int num = static_cast<int> (x_max / step_size);

	for (int i = 0; i < DATA_SIZE; ++i)
	{
		//if (ci[i] >= 0.1) //not needed!
		{
			if (x_coord[i] < (x_max - step_size + 0.05) && x_coord[i] > (step_size - 0.05)) //every point on the grid except 1st two and last two columns
			{
				if (i >static_cast<int>(x_max / step_size) && (i < DATA_SIZE - static_cast<int>(x_max / step_size + 1))) // every row except bottom and top
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i);

				else if (i > DATA_SIZE - static_cast<int> (x_max / step_size + 1)) //top row, only 4 neighbours
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 4, 5);

				else if (i < static_cast<int> (x_max / step_size)) //bottom row, 4 neighbours
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 0, 1);
			}
			else if ((x_coord[i] < ((step_size + 0.05) / 2)) && (x_coord[i] > ((step_size - 0.05) / 2))) //only 2nd column of grid, 5 neighbours
			{
				if (i >= (x_max / step_size) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except bottom and top rows
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 2);

				else if (i > DATA_SIZE - static_cast<int>(x_max / step_size + 1)) // the top row (last row of raster scan)
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 2, 4, 5);

				//there is no first row in the 2nd column
			}
			else if ((x_coord[i] < (x_max - ((step_size - 0.05) / 2))) && (x_coord[i] > x_max - ((step_size + 0.05) / 2))) //2nd last column of grid
			{
				if ((i > (x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1))))
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 3);

				else if (i >(DATA_SIZE - static_cast<int> (x_max / step_size + 1))) //case when last row starts from 2nd column, ends at 2nd last column
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 3, 4, 5);

				//there is no 1st row in the 2nd last column
			}
			else if (x_coord[i] < ((step_size - 0.05) / 2)) //considering the case of 1st column
			{
				if ((i > static_cast<int>(x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1))))//except top and bottom rows
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 0, 2, 4);

				else if (i == (DATA_SIZE - static_cast<int>(x_max / step_size + 1))) //exactly first element of the last (top) row
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 0, 2, 4, 5);

				else if (i == 0) //first row, first element
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 0, 1, 2, 4);
			}
			else if (x_coord[i] >(x_max - ((step_size - 0.05) / 2))) // last column of hexagonal grid
			{
				if ((i > (x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except top and bottom rows
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 1, 3, 5);

				else if (i == (DATA_SIZE - 1)) //last column, last row
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 1, 3, 4, 5);

				else if (i == static_cast<int>(x_max / step_size)) //last column, first row
					kam[i] = KAM_data_calc(max_dis_angle, dis_array, i, 0, 1, 3, 5);
			}
		}
	}

}




double KAM_data_calc(const double &misorient, const vector <vector <double>> &dis_array, const int &i,
	const int &p, const int &q, const int &r, const int &s) //function used inside kam_calculation
{
	double count = 0;
	double sum = 0;

	if (p != 0 && q != 0 && r != 0 && s != 0)
	{
		//if (id[i] == id[i - (num + 1)])// && 
		if (dis_array[i][0] <= misorient)
		{
			sum += dis_array[i][0];
			++count;
		}
	}

	if (p != 1 && q != 1 && r != 1 && s != 1)
	{
		//if (id[i] == id[i - num])// && 
		if (dis_array[i][1] <= misorient)
		{
			sum += dis_array[i][1];
			++count;
		}
	}

	if (p != 2 && q != 2 && r != 2 && s != 2)
	{
		//if (id[i] == id[i - 1])// && 
		if (dis_array[i][2] <= misorient)
		{
			sum += dis_array[i][2];
			++count;
		}
	}

	if (p != 3 && q != 3 && r != 3 && s != 3)
	{
		//if (id[i] == id[i + 1])// && 
		if (dis_array[i][3] <= misorient)
		{
			sum += dis_array[i][3];
			++count;
		}
	}

	if (p != 4 && q != 4 && r != 4 && s != 4)
	{
		//if (id[i] == id[i + num])// && 
		if (dis_array[i][4] <= misorient)
		{
			sum += dis_array[i][4];
			++count;
		}
	}

	if (p != 5 && q != 5 && r != 5 && s != 5)
	{
		//if (id[i] == id[i + num + 1])// && 
		if (dis_array[i][5] <= misorient)
		{
			sum += dis_array[i][5];
			++count;
		}
	}

	if (count != 0)
		if (sum / count > 5.0)
			cout << "\n" << i << endl;



	if (count == 0)
		return 0;
	else return sum / count;
}


void kam_dis(const vector <double> &kam,  const vector <vector <double>> &userResponse)
{
	 //using push_back!, not sure about sizes => yeah, sure about sizes!
	int bins = static_cast<int>(userResponse[3][3]);
	vector <double> xkam(static_cast<int>(bins + 0.05));
	vector <int> ykam(static_cast<int>(bins + 0.05));

	//reArrange(kam, 0.00, ykam, xkam, bins);
	
	
	double min_ = *min_element(kam.begin(), kam.end());
	double max_ = *max_element(kam.begin(), kam.end());
	int kam_response = static_cast<int>(userResponse[7][0]);

	reArrangeAll(userResponse, kam, xkam, ykam, 3, min_);

	vector<double> col(2);
	col = avgAndDev(kam);

	writeMyFile(col, "KAM.txt", xkam, ykam, "007", "Kernel Average Misorientation", "Avg Angle [degrees]", "Number");

	gnuPLotAll(kam_response, "KAM.txt", max_, min_, bins, "Angle (in degrees)", "Frequencies", "KAM plot", "KAM plot.png");

	cout << "\nFinished with KAM calculations\n\n";;
	
	xkam.clear();
	ykam.clear();
	col.clear();
}

