/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_plot.h
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
#include <string>
#include <algorithm>
#include <cmath>
#include "EasyBMP.h"
#include "EasyBMP_BMP.h"
#include "EasyBMP_DataStructures.h"
#include "EasyBMP_VariousBMPutilities.h"



void plot_func(BMP &image, int l, int m, double p, double q, double r);

using namespace std;



void plot_bound(vector <double> xb, vector <double> yb, const double x_max, const double y_max, \
	const double step_size, const int DATA_SIZE, const char* name)
{
	BMP bound;
	vector <int> v1 = { 255, 255, 255 };
	vector <vector <int>> rgb;
	rgb.resize(2 * DATA_SIZE, v1);
	int xb_size = xb.size();
	for (int i = 0; i < xb_size; ++i)
	{
		xb[i] = static_cast<int> (2 * (xb[i] / step_size));
		yb[i] = static_cast<int> (2 * (yb[i] / (sqrt(3)*step_size)));
	}//initial coloring

	int a = static_cast<int>(2 * x_max / step_size) + 1;
	int b = static_cast<int>(2 * y_max / (sqrt(3)*step_size)) + 1;
	
	//modifying the pixel dimensions, from square to rectangle
	if (a < b)
		bound.SetDPI(static_cast<int>(96 * static_cast<double>(a) / b), 96);
	else if (a > b )
		bound.SetDPI(96, static_cast<int>(96 * static_cast<double>(b) / a));


	bound.SetSize(a + 1, b + 1);

	for (int j = 0; j < b; ++j)
	{
		if (j % 2 == 0)
		{
			for (int i = 0; i < a; i = i + 2)
			{
				plot_func(bound, i, j, rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][0],
				rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][1], rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][2]);
				/*bound(i, j)->Red = rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][0];
				bound(i, j)->Green = rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][1];
				bound(i, j)->Blue = rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][2];*/
			}
		}
		else
		{
			for (int i = 1; i < a; i = i + 2)
			{
				plot_func(bound, i, j, rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][0],
					rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][1], rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][2]);
				/*bound(i, j)->Red = rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][0];
				bound(i, j)->Green = rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][1];
				bound(i, j)->Blue = rgb[i + j*(static_cast<int>(2 * x_max / step_size) + 1)][2];*/
			}
		}
	}

	int l, m;

	for (int i = 0; i < xb_size; ++i)
	{
		l = static_cast<int> (xb[i]);
		m = static_cast<int> (yb[i]);
		bound(l, m)->Red = 0;
		bound(l, m)->Green = 0;
		bound(l, m)->Blue = 255; //coloring the bounderies blue
	}
	
	bound.WriteToFile(name);

	cout << "Grain boundary plot has been created\n";
}
//(kam, xmax, ymax, stepsize, sn)


void plot_func(BMP &image, int l, int m, double p, double q, double r)
{
	image(l, m)->Red = static_cast<int>(p);
	image(l, m)->Green = static_cast<int>(q);
	image(l, m)->Blue = static_cast<int>(r);
}


void BMP_plot(const int &x, const int &y, const int &DATA_SIZE, const vector<vector<int>> &color, char* fileName = "grain map.bmp")
{
	BMP image;
	int count = 0;
	int a = 2 * x;
	int b = y;
	if (a < b)
		image.SetDPI(static_cast<int>(96 * static_cast<double>(a) / b), 96);
	else if (a > b)
		image.SetDPI(96, static_cast<int>(96 * static_cast<double>(b) / a));

	image.SetSize(a + 1, b + 1);

	for (int j = 0; ((j < b) && (count <DATA_SIZE)); j += 2) //notice the increament by 2 here
	{
		for (int i = 0; ((i < a) && (count <DATA_SIZE)); i += 2) //notice increament by 2 here
		{
			image(i, j)->Red = color[count][0];
			image(i, j)->Green = color[count][1];
			image(i, j)->Blue = color[count][2];

			image(i + 1, j)->Red = color[count][0];
			image(i + 1, j)->Green = color[count][1];
			image(i + 1, j)->Blue = color[count][2];

			++count;
		}

		/*
		- here this second "for loop block" is meant for the rows like 2nd from bottom
		- which are just one point shorter than bottom most
		- look at the maxima that i can take, it's a-2; a is already 2*x
		*/

		for (int i = 0; ((i < (a-2)) && (count < DATA_SIZE)); i += 2) //notice increament by 2 here
		{
			image(i, j + 1)->Red = color[count][0];
			image(i, j + 1)->Green = color[count][1];
			image(i, j + 1)->Blue = color[count][2];

			image(i + 1, j + 1)->Red= color[count][0];
			image(i + 1, j + 1)->Green = color[count][1];
			image(i + 1, j + 1)->Blue = color[count][2];

			++count;
		}
	}
	
	image.WriteToFile(fileName);
	cout << "Done with new plot here\n\n";

}