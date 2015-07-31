/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_grain_bound.h
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
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "h_plot.h"
#include "h_math.h"


using namespace std;


//function definitions
void bound_calc(const vector <vector <double>> &dis_array, const vector <double> &x_coord, const vector<double> &y_coord, \
	const int DATA_SIZE, const double orient_angle, const vector <double> &ci);

void plot_bound(vector <double> xb, vector <double> yb, const double x_max, const double y_max, \
	const double step_size, const int DATA_SIZE, const char* name = "bound_plot.bmp");
//xb and yb are supposed to be changed in this function..so no reference, hence no const!


//global variables
vector <double> xb, yb;
extern double min_ci;


void grain_bound(const vector < vector < double>> &euler, vector <vector <double>> &dis_array, const vector <double> &x_coord, 
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci)
{
	//the only reason to pass euler is to calculate dis_array, if it hasn't yet been calculated!
	//checking whethere we need to calculate dis_array or not
	
	if (isSame(userResponse[0][0], 0))
		dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);
	else
		; // this means dis_array has alraedy been calculate in option 1;
	
	bound_calc(dis_array, x_coord, y_coord, DATA_SIZE, userResponse[1][1], ci);

	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double y_max = *max_element(y_coord.begin(), y_coord.end());
	const double step_size = x_coord[1] - x_coord[0];

	plot_bound(xb, yb, x_max, y_max, step_size, DATA_SIZE, "grain_bound_plot.bmp");
}


void bound_calc(const vector <vector <double>> &dis_array, const vector <double> &x_coord, const vector<double> &y_coord, \
	const int DATA_SIZE, const double orient_angle, const vector <double> &ci)
{
	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double step_size = x_coord[1] - x_coord[0];

	for (int i = 1; i < DATA_SIZE; ++i)
	{
		if (ci[i] >= min_ci)
		{
			double t = dis_array[i][2];
			double u = dis_array[i][1];
			double v = dis_array[i][0];

			if ((x_coord[i] < (x_max - (step_size - 0.05) / 2)) && (x_coord[i] > (step_size - 0.05))) //every column except 1st, 2nd and last
			{
				if (i > static_cast <int> (x_max / step_size)) //every row except 1st row
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
					}
					if (u > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					}
					if (v > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					}
				}
				else //taking care of 1st row 
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
					}
				}

			}
			else if ((x_coord[i] < (step_size + 0.05) / 2) && (x_coord[i] > (step_size - 0.05) / 2)) //2nd column of grid
			{
				//there is no 2nd column in 1st row, so no unnecessary conditions checks like
				//-if (i > static_cast <int> (x_max / step_size))
				if (u > orient_angle)
				{
					xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
				}
				if (v > orient_angle)
				{
					xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
				}
			}
			else if (x_coord[i] < (step_size - 0.05) / 2) //1st column of the grid 
			{
				if (u > orient_angle)
				{
					xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
				}
				//no arrangement for 1st column, 1st row i.e. (i == 0), notice the initialization of i of for loop
			}
			else if (x_coord[i] > (x_max - (step_size - 0.05) / 2)) //last column of the grid
			{
				if (i > static_cast<int> (x_max / step_size)) //every row except 1st row
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
					}
					if (v > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					}
				}
				else
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
					}
				}
			}
		}
	}

	printf("Grain Boundary length:\t%02.03lf cm\n\n", xb.size()*step_size / sqrt(3) / 10000);
	printf("\nDone with grain boundary length calculations\n\n");

}