/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_twin_bound.h
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
#include "h_plot.h"
#include "h_math.h"

using namespace std;

//function declaration


void twin_bound_calc1(const vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const double orient_angle, const double tol1, const double tol2,
	const vector <double> &ci);
void twin_bound_calc2(const vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const double orient_angle, const double tol1, const double tol2,
	const vector <double> &ci);

void matrix_mult(const vector<vector<double>> &matrix2, const vector<double> &matrix1, vector<double> &result);

double k1_calc(const vector <double> &euler1, const vector <double> &euler2);

//global variables 

vector <double> xc, yc;
extern vector <double> xb, yb;
extern double min_ci;



void twin_bound(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci)
{

	//checking whether we need to calculate dis_array or not

	if (isSame(userResponse[0][0], 0) && isSame(userResponse[1][0], 0))
		dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);
	else
		; // this means dis_array has alraedy been calculate either in option 1 or in option 2;


	if (isSame(userResponse[1][0], 0))
		twin_bound_calc1(dis_array, euler, x_coord, y_coord, DATA_SIZE, userResponse[2][1], userResponse[2][2], userResponse[2][3], ci);
	else if (isSame(userResponse[1][0], 1))
		twin_bound_calc2(dis_array, euler, x_coord, y_coord, DATA_SIZE, userResponse[2][1], userResponse[2][2], userResponse[2][3], ci);
	//avoiding re-calculation of xb and yb


	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double y_max = *max_element(y_coord.begin(), y_coord.end());
	const double step_size = x_coord[1] - x_coord[0];

	plot_bound(xc, yc, x_max, y_max, step_size, DATA_SIZE, "twin_bound_plot.bmp");
}


void twin_bound_calc1(const vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const double orient_angle, const double tol1, const double tol2,
	const vector <double> &ci)
{
	xb.erase(xb.begin(), xb.end());
	yb.erase(yb.begin(), yb.end());
	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double step_size = x_coord[1] - x_coord[0];


	for (int i = 1; i < DATA_SIZE; ++i)
	{
		if (ci[i] >= min_ci)
		{
			double t = dis_array[i][2];
			double u = dis_array[i][1];
			double v = dis_array[i][0];
			double angle = 0;

			if ((x_coord[i] < (x_max - (step_size - 0.05) / 2)) && (x_coord[i] > (step_size - 0.05))) //every column except 1st, 2nd and last
			{
				if (i > static_cast <int> (x_max / step_size)) //every row except 1st row
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						if (abs(60 - k1_calc(euler[i], euler[i - 1])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
					if (u > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						if (abs(60 - k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size)])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						}
					}
					if (v > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						if (abs(60 - k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size + 1)])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						}
					}
				}
				else //taking care of 1st row 
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						if (abs(60 - k1_calc(euler[i], euler[i - 1])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
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
					if (abs(60 - k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size)])) <= tol2)
					{
						xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					}
				}
				if (v > orient_angle)
				{
					xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					if (abs(60 - k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size + 1)])) <= tol2)
					{
						xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					}
				}
			}
			else if (x_coord[i] < (step_size - 0.05) / 2) //1st column of the grid 
			{
				if (u > orient_angle)
				{
					xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					if (abs(60 - k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size)])) <= tol2)
					{
						xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					}
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
						if (abs(60 - k1_calc(euler[i], euler[i - 1])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
					if (v > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						if (abs(60 - k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size + 1)])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						}
					}
				}
				else
				{
					if (t > orient_angle)
					{
						xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						if (abs(60 - k1_calc(euler[i], euler[i - 1])) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
				}
			}
		}
	}

	printf("Grain Boundary length:\t%02.03lf cm\n\n", xb.size()*step_size / sqrt(3) / 10000);
	printf("Twin Boundary length:\t%02.03lf cm\n\n", xc.size()*step_size / sqrt(3) / 10000);
	printf("Twin boundary fraction: %02.03lf\n\n", static_cast<double>(xc.size()) / static_cast<double>(xb.size()));

}

void twin_bound_calc2(const vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const vector <double> &y_coord, const int DATA_SIZE, const double orient_angle, const double tol1, const double tol2,
	const vector <double> &ci)
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
			double angle = 0;

			if ((x_coord[i] < (x_max - (step_size - 0.05) / 2)) && (x_coord[i] > (step_size - 0.05))) //every column except 1st, 2nd and last
			{
				if (i > static_cast <int> (x_max / step_size)) //every row except 1st row
				{
					if (t > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);*/
						if (t <= (60 + tol1) && t >= (60 - tol1) && k1_calc(euler[i], euler[i - 1]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
					if (u > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);*/
						if (u <= (60 + tol1) && u >= (60 - tol1) && k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size)]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						}
					}
					if (v > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);*/
						if (v <= (60 + tol1) && v >= (60 - tol1) && k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size + 1)]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						}
					}
				}
				else //taking care of 1st row 
				{
					if (t > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);*/
						if (t <= (60 + tol1) && t >= (60 - tol1) && k1_calc(euler[i], euler[i - 1]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
				}

			}
			else if ((x_coord[i] < (step_size + 0.05) / 2) && (x_coord[i] > (step_size - 0.05) / 2)) //2nd column of grid
			{
				//there is no 2nd column in 1st row, so no unnecessary conditions checks like
				//-if (i > static_cast <int> (x_max / step_size))
				if (u > orient_angle)
				{
					/*xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);*/
					if (u <= (60 + tol1) && u >= (60 - tol1) && k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size)]) <= tol2)
					{
						xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					}
				}
				if (v > orient_angle)
				{
					/*xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);*/
					if (v <= (60 + tol1) && v >= (60 - tol1) && k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size + 1)]) <= tol2)
					{
						xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
					}
				}
			}
			else if (x_coord[i] < (step_size - 0.05) / 2) //1st column of the grid 
			{
				if (u > orient_angle)
				{
					/*xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);*/
					if (u <= (60 + tol1) && u >= (60 - tol1) && k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size)]) <= tol2)
					{
						xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size)]) / 2);
						yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size)]) / 2);
					}
				}
				//no arrangement for 1st column, 1st row i.e. (i == 0), notice the initialization of i of for loop
			}
			else if (x_coord[i] > (x_max - (step_size - 0.05) / 2)) //last column of the grid
			{
				if (i > static_cast<int> (x_max / step_size)) //every row except 1st row
				{
					if (t > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);*/
						if (t <= (60 + tol1) && t >= (60 - tol1) && k1_calc(euler[i], euler[i - 1]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
					if (v > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);*/
						if (v <= (60 + tol1) && v >= (60 - tol1) && k1_calc(euler[i], euler[i - static_cast<int>(x_max / step_size + 1)]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - static_cast<int>(x_max / step_size + 1)]) / 2);
						}
					}
				}
				else
				{
					if (t > orient_angle)
					{
						/*xb.push_back((x_coord[i] + x_coord[i - 1]) / 2);
						yb.push_back((y_coord[i] + y_coord[i - 1]) / 2);*/
						if (t <= (60 + tol1) && t >= (60 - tol1) && k1_calc(euler[i], euler[i - 1]) <= tol2)
						{
							xc.push_back((x_coord[i] + x_coord[i - 1]) / 2);
							yc.push_back((y_coord[i] + y_coord[i - 1]) / 2);
						}
					}
				}
			}
		}
	}

	printf("Grain Boundary length:\t%02.03lf cm\n\n", xb.size()*step_size / sqrt(3) / 10000);
	printf("Twin Boundary length:\t%02.03lf cm\n\n", xc.size()*step_size / sqrt(3) / 10000);
	printf("Twin boundary fraction: %02.03lf\n\n", static_cast<double>(xc.size()) / static_cast<double>(xb.size()));
}


double k1_calc(const vector <double> &euler1, const vector <double> &euler2)
{
	double p1 = euler1[0];
	double p = euler1[1];
	double p2 = euler1[2];
	double min_angle = 120; //initializing with some random angle


	vector<double> quat1(4), quat2(4);
	eulerToQuat(euler1, quat1);
	eulerToQuat(euler2, quat2);

	vector <vector<double>> mat = { { 1 / sqrt(3), 1 / sqrt(3), 1 / sqrt(3) }, { -1 / sqrt(3), 1 / sqrt(3), 1 / sqrt(3) }, \
	{ 1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3) }, { 1 / sqrt(3), 1 / sqrt(3), -1 / sqrt(3) } };

	vector <vector <double>> vectorsRotated1(4, vector <double>(3)), vectorsRotated2(4, vector <double>(3));

	for (int i = 0; i < 4; ++i)
	{
		quatRot(quat1, mat[i], vectorsRotated1[i]);
		quatRot(quat2, mat[i], vectorsRotated2[i]);
	}

	double cos_beta = 0, beta = 0;


	vector <double> cos_beta_values;
	/*for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cos_beta = abs(dot(vectorsRotated1[i], vectorsRotated2[j]));
			cos_beta_values.push_back(cos_beta);
		}
	}*/

	for (int j = 0; j < 4; ++j)
	{
		cos_beta = (dot(vectorsRotated1[j], vectorsRotated2[j])); //mind the change here- no more abs()
		cos_beta_values.push_back(cos_beta);
	}

	beta = acos(*max_element(cos_beta_values.begin(), cos_beta_values.end())) * 180 / pi;
	
	if (beta < 90)
		;//then fine
	else
		beta = 180 - beta;

	cos_beta_values.clear();
	return beta;
}
