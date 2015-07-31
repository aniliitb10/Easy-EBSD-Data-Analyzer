/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_GAM.h
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
#include <tuple>
#include <fstream>
#include "h_plot.h"
#include "h_math.h"
#include "myAlgorithms.h"
#include "h_bar_plot.h"
#include "gnu_plot.h"

using namespace std;

//#define range 0.127996

extern vector <int> id, size;
extern double min_ci;



void GAM_grain(const vector <double> &gam, const vector <double> &m, const int &DATA_SIZE, 
	const int &min_grain_size, vector <double> &gam_final);

void GAM_calc(const vector <vector <double>> &dis_array, const vector <double> &x_coord, const int &DATA_SIZE,
	vector <double> &gam, vector <double> &m, const vector <double> &ci);

tuple <double, int> GAM_data_calc(const vector <vector <double>> &dis_array, const int &num, const int &i,
	const int &p = 10, const int &q = 10, const int &r = 10, const int &s = 10);

//void gnu_plot(char* data_file_name, const double& width, char *x_label, char *y_label, char *title, char *ouput_file_name);

//void GAM_dis(const vector <double> &gam_final, vector <double> &xgam, vector <double> &ygam, const int &bins);
//-for plotting GAM distribution in bar graph form

void GAM(vector <vector <double>> &dis_array, const vector <vector <double>> &euler, const vector <double> &x_coord, \
	const int DATA_SIZE, const vector <vector<double>> &userResponse, const vector <double> &ci)
{
	//the only reason to pass euler is to calculate dis_array, if it hasn't yet been calcuated 
	//=================checking whether we need to calculate dis_array or not=============================================

	if (isSame(userResponse[0][0], 0) && isSame(userResponse[1][0], 0) && isSame(userResponse[2][0], 0) &&\
		isSame(userResponse[3][0], 0))
		dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);
	else
		; // this means dis_array has already been calculated either in option 1,2,3 or 4;


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
		labelling(x_coord, dis_array, DATA_SIZE, userResponse[4][1], ci);
	}
		
	else
		;//that means, it has already been called and calculated!
	
	//const int max_root = *max_element(id.begin(), id.end());

	vector <double> gam(DATA_SIZE), m(DATA_SIZE), gam_final;
	int bins = static_cast<int>(userResponse[4][3] + 0.05);
	int min_grain_size = static_cast<int>(userResponse[4][2]);
	vector <double> xgam(bins);
	vector <int> ygam(bins);
	//gam is just the sum of all the misorientation of all the neighbouring pairs

	GAM_calc(dis_array, x_coord, DATA_SIZE, gam, m, ci); //function call and writes the values in a file

	//GAM_grain(gam, m, DATA_SIZE, bins, gam_final); //blunder mistake
	GAM_grain(gam, m, DATA_SIZE, min_grain_size, gam_final);

	double min_ = *min_element(gam_final.begin(), gam_final.end());
	double max_ = *max_element(gam_final.begin(), gam_final.end());
	int gam_response = static_cast<int>(userResponse[8][0]);

	
	reArrangeAll(userResponse, gam_final, xgam, ygam, 4, min_);

	vector<double> col(2);
	col = avgAndDev(gam_final);

	writeMyFile(col, "GAM.txt", xgam, ygam, "008", "Grain Average Misorientation", "Diameter [microns]", "Number");

	gnuPLotAll(gam_response, "GAM.txt", max_, min_, bins, "Angle (in degrees)", "Frequencies", "GAM plot", "GAM plot.png");

	cout << "\nFinished with GAM calculations\n\n";
	
	gam.clear();
	m.clear();
	xgam.clear();
	ygam.clear();
	gam_final.clear();
	
}


void GAM_calc(const vector <vector <double>> &dis_array, const vector <double> &x_coord, const int &DATA_SIZE,
	vector <double> &gam, vector <double> &m, const vector <double> &ci)
{
	
	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double step_size = x_coord[1] - x_coord[0];
	const int num = static_cast <int>(x_max / step_size);
	
	//just a check:


	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if (ci[i] >= min_ci)
		{
			if (x_coord[i] < (x_max - step_size + 0.05) && x_coord[i] > (step_size - 0.05)) //every point on the grid except 1st two and last two columns
			{
				if (i >static_cast<int>(x_max / step_size) && (i < DATA_SIZE - static_cast<int>(x_max / step_size + 1))) // every row except bottom and top
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i);

				else if (i > DATA_SIZE - static_cast<int> (x_max / step_size + 1)) //top row, only 4 neighbours
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 4, 5);

				else if (i < static_cast<int> (x_max / step_size)) //bottom row, 4 neighbours
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 0, 1);
			}
			else if ((x_coord[i] < ((step_size + 0.05) / 2)) && (x_coord[i] > ((step_size - 0.05) / 2))) //only 2nd column of grid, 5 neighbours
			{
				if (i >= (x_max / step_size) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except bottom and top rows
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 2);

				else if (i > DATA_SIZE - static_cast<int>(x_max / step_size + 1)) // the top row (last row of raster scan)
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 2, 4, 5);

				//there is no first row in the 2nd column
			}
			else if ((x_coord[i] < (x_max - ((step_size - 0.05) / 2))) && (x_coord[i] > x_max - ((step_size + 0.05) / 2))) //2nd last column of grid
			{
				if ((i > (x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1))))
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 3);

				else if (i >(DATA_SIZE - static_cast<int> (x_max / step_size + 1))) //case when last row starts from 2nd column, ends at 2nd last column
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 3, 4, 5);

				//there is no 1st row in the 2nd last column
			}
			else if (x_coord[i] < ((step_size - 0.05) / 2)) //considering the case of 1st column
			{
				if ((i > static_cast<int>(x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1))))//except top and bottom rows
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 0, 2, 4);

				else if (i == (DATA_SIZE - static_cast<int>(x_max / step_size + 1))) //exactly first element of the last (top) row
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 0, 2, 4, 5);

				else if (i == 0) //first row, first element
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 0, 1, 2, 4);
			}
			else if (x_coord[i] >(x_max - ((step_size - 0.05) / 2))) // last column of hexagonal grid
			{
				if ((i > (x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except top and bottom rows
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 1, 3, 5);

				else if (i == (DATA_SIZE - 1)) //last column, last row
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 1, 3, 4, 5);

				else if (i == static_cast<int>(x_max / step_size)) //last column, first row
					tie(gam[i], m[i]) = GAM_data_calc(dis_array, num, i, 0, 1, 3, 5);
			}
		}
	}
}


tuple <double, int> GAM_data_calc(const vector <vector <double>> &dis_array, const int &num, const int &i,
	const int &p, const int &q, const int &r, const int &s)
{

	double count = 0;
	double sum = 0;

	if (p != 0 && q != 0 && r != 0 && s != 0)
	{
		if (id[i] == id[i - (num + 1)])// && dis_array[i][0] < misorient)
		{
			sum += dis_array[i][0];
			++count;
		}
	}

	if (p != 1 && q != 1 && r != 1 && s != 1)
	{
		if (id[i] == id[i - num])// && dis_array[i][1] < misorient)
		{
			sum += dis_array[i][1];
			++count;
		}
	}

	if (p != 2 && q != 2 && r != 2 && s !=2)
	{
		if (id[i] == id[i - 1])// && dis_array[i][2] < misorient)
		{
			sum += dis_array[i][2];
			++count;
		}
	}

	if (p != 3 && q != 3 && r != 3 && s != 3 )
	{
		if (id[i] == id[i + 1])// && dis_array[i][3] < misorient)
		{
			sum += dis_array[i][3];
			++count;
		}
	}

	if (p != 4 && q != 4 && r != 4 && s != 4)
	{
		if (id[i] == id[i + num])// && dis_array[i][4] < misorient)
		{
			sum += dis_array[i][4];
			++count;
		}
	}

	if (p != 5 && q != 5 && r != 5 && s != 5)
	{
		if (id[i] == id[i + num + 1])// && dis_array[i][5] < misorient)
		{
			sum += dis_array[i][5];
			++count;
		}
	}

	return make_tuple(sum, count);

}

void GAM_grain(const vector <double> &gam, const vector <double> &m, const int &DATA_SIZE, const int &min_grain_size,
	vector <double> &gam_final)
{
	
	//int max_root = *max_element(id.begin(), id.end());
	vector <double> gam_total(DATA_SIZE), gam_n(DATA_SIZE);
	for (int i = 0; i < DATA_SIZE; ++i)
	{
		//if (size[i] >= min_grain_size)
		{
			int root_i = id[i]; //basically id[] stores the root values
			gam_total[root_i] += gam[i];
			gam_n[root_i] += m[i];
		}//for details refer fig 2.8 and the formula beneath
	}

	//vector <int> idFromGAM;

	
	for (int i = 0; i < DATA_SIZE; ++i) //final step of GAM_grain calculation!
		if (gam_n[i] != 0 && size[i] >= min_grain_size)// && gam_total[i] > 0) another blunder mistake
		{
			gam_final.push_back(gam_total[i] / (gam_n[i]));
			//idFromGAM.push_back(i);
		}
			
}
