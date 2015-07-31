/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: myAlgorithms.h
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
#include <cstdio>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include "h_math.h"
#include "gnu_plot.h"
#include <cstring>

using namespace std;

#define b_tol 0.1
#define m_tol 0.001

//to skip warnings for using fopen etc by Visual Studio
#ifdef _MSC_VER 
#pragma warning( disable : 4996)
#endif

void reArrange(const vector <double> &values, const double min_, vector <int> &y_axis, vector <double> &x_axis, const int &bins)
{
	
	double max_ = *max_element(values.begin(), values.end());
	double x_range = (max_ - min_)/ bins; //usually this means (max_-0)/bins;
	double l = x_range / 2.0;
	x_axis[0] = min_ + l;
	for (int i = 1; i < bins; ++i)
		x_axis[i] = x_axis[i - 1] + x_range;

	double values_size = values.size();
	
	for (int i = 0; i < values_size; ++i)
		for (int j = 0; j < bins; ++j)
			if (values[i] >= (x_axis[j] - l - m_tol) && values[i] < (x_axis[j] + l + m_tol))
			{
				y_axis[j] += 1;
				break;
			}
	
	for (unsigned int i = 0; i < values_size; ++i)
	{
		if (values[i] < ((x_axis[0] - l) - m_tol))
			y_axis[0] += 1;
		else if (values[i] > (((x_axis[bins - 1] + l) + m_tol)))
			y_axis[bins - 1] += 1;
	}

	//done here
}

void reArrangeLog(const vector <double> &values, const double min_, vector <int> &y_axis, vector <double> &x_axis, const int &bins)
{

	//min_ here can't be <= 0
	if (min_ <= 0)
	{
		cout << "Invalid input to reArrangeLog function\n\n";
		system("pause");
		return;
	}


	double max_ = *max_element(values.begin(), values.end());
	double x_range = (log10(max_) - log10(min_)) / bins; //usually this means (max_-0)/bins;
	double l = x_range / 2.0;
	x_axis[0] = pow(10,(log10(min_ ) + l));
	for (int i = 1; i < bins; ++i)
		x_axis[i] = pow(10,(log10(x_axis[i - 1]) + x_range));

	unsigned int values_size = values.size();
	
	for (unsigned int i = 0; i < values_size; ++i)
		for (int j = 0; j < bins; ++j)
			if (values[i] >= (pow(10, (log10(x_axis[j]) - l)) - m_tol) && values[i] < (pow(10, (log10(x_axis[j]) + l)) + m_tol))
			{
				y_axis[j] += 1;
				break;
			}

	for (unsigned int i = 0; i < values_size; ++i)
	{
		if (values[i] < (pow(10, (log10(x_axis[0]) - l)) - m_tol))
			y_axis[0] += 1;
		else if (values[i] > (pow(10, (log10(x_axis[bins - 1]) + l)) + m_tol))
			y_axis[bins - 1] += 1;
	}

}



void reArrangePartLinear(const double &percent, const int &firstBins, const vector <double> &values, const double min_,\
	vector <int> &y_axis, vector <double> &x_axis, const int &bins)
{
	/*
	*	Everything named as "first" correspond to the first part of the binning
	*	Similarly, everything named "second" correspond to the 2nd part of the binning
	*	Both, putting values in x and y axes have been done separately for both the binnings
	*	There might be another way, but this looks more manageable
	*/

	double max_ = *max_element(values.begin(), values.end());
	double x_first_range = ((max_ - min_)* percent / 100)/firstBins; //usually this means (max_-0)/bins;
	double first_l = x_first_range / 2.0;
	x_axis[0] = min_ + first_l;
	for (int i = 1; i < firstBins; ++i)
		x_axis[i] = x_axis[i - 1] + x_first_range;

	double values_size = values.size();
	
	for (int i = 0; i < values_size; ++i)
		for (int j = 0; j < firstBins; ++j)
			if (values[i] >= (x_axis[j] - first_l - m_tol) && values[i] < (x_axis[j] + first_l + m_tol))
				y_axis[j] += 1;

	
	double x_second_range = (max_ - min_ - ((max_ - min_)* percent / 100)) / (bins-firstBins);
	//the gone range ((max_ - min_)* percent / 100);
	double second_l = x_second_range / 2.0;
	x_axis[firstBins] = (min_ + ((max_ - min_)* percent / 100))+ second_l; //the new minima (min_ + ((max_ - min_)* percent / 100))
	for (int i = (1 + firstBins); i < bins; ++i)
		x_axis[i] = x_axis[i - 1] + x_second_range;


	for (int i = 0; i < values_size; ++i)
		for (int j = firstBins; j < bins; ++j)
			if (values[i] >= (x_axis[j] - second_l - m_tol) && values[i] < (x_axis[j] + second_l + m_tol))
				y_axis[j] += 1;


	for (unsigned int i = 0; i < values_size; ++i)
	{
		if (values[i] < ((x_axis[0] - first_l) - m_tol))
			y_axis[0] += 1;
		else if ((values[i] >= (x_axis[firstBins - 1] + first_l + m_tol)) && (values[i] < ((x_axis[firstBins] - second_l - m_tol))))
			y_axis[firstBins - 1] += 1; //could go to y_axis[firstBins] too
		else if (values[i] >= (x_axis[bins - 1] + second_l + m_tol))
			y_axis[bins - 1] += 1;
	}
	//x_axis[bins - 1] -= 0.1; //to restore to the original value.
	//done here
}


void open_existing_file(fstream &inFile, const char* filename, const char* errorCode)
{
	//first checking whether file really exists or not
	/*struct _stat buf;
	int result = _stat(filename, &buf);
	if (result != 0)
	{
		printf("Something went wrong while opening the file: %s", filename);
		perror("\n");
		printf("Error code: %s\n", errorCode);
		system("pause");
		exit(0);
	}
*/
	inFile.open(filename, std::fstream::in);
	if (!inFile)
	{
		printf("Unable to open the file %s ", filename);
		perror(" ");
		printf("Error code: %s\n", errorCode);
		system("pause");
		exit(0);
	}
}


void create_new_file(fstream &outFile, const char* filename, const char* errorCode)
{
	outFile.open(filename, std::fstream::out);
	if (!outFile)
	{
		perror("Something went wrong");
		printf("Error code: %s\n", errorCode);
		system("pause");
		exit(0);
	}
}

void fatal_here(char* message)
{
	char error_message[100];
	strcpy(error_message, "[!!] Fatal Error ");
	strncat(error_message, message, 83);
	perror(error_message);
	system("pause");
	exit(0);
}

void reArrangeAll(const vector<vector<double>> &userResponse, const vector <double> &values, vector <double> &x_axis, \
	vector <int> &y_axis, const int &option, const double &min_)
{
	int bins = static_cast<int>(userResponse[option][3] + m_tol);
	int adOpt_response = static_cast<int>(userResponse[option + 4][0] + m_tol);
	
	if ((adOpt_response == 0) || (adOpt_response == 1))
	{
		reArrange(values, min_, y_axis, x_axis, bins);
	}
	else if (adOpt_response == 2)
	{
		reArrangeLog(values, (min_ + 0.000001), y_axis, x_axis, bins); //to make the min_ +ve, log of only +ve is defined
	}
	else if (adOpt_response == 3)
	{
		reArrangePartLinear(userResponse[option + 4][1], static_cast<int>(userResponse[option + 4][2]), \
			values, min_, y_axis, x_axis, bins);
	}
}


void writeMyFile(const vector <double> &avgAndDev, const char* fileName, const vector <double> &x_axis, \
	const vector <int> &y_axis, const char* errorCode, const char* heading, const char* col1, const char* col2)
{
	unsigned int bins = y_axis.size();
	fstream writeFile;
	create_new_file(writeFile, fileName, errorCode);

	writeFile << "#\n#\n# Chart:  "<< heading <<"\n#\n";
	writeFile << "# Column 1: "<< col1 <<"\n";
	writeFile << "# Column 2: "<< col2 <<"\n";

	for (unsigned int i = 0; i < bins; ++i)
		writeFile << x_axis[i] << " " << y_axis[i] << "\n";

	writeFile << "# Average number: " << avgAndDev[0] << "\n\n";
	writeFile << "# Standrard deviation number: " << avgAndDev[1] << "\n";
	
	//finding the total no of grains accounted!
	int sum = 0;
	for (unsigned int i = 0; i < y_axis.size(); ++i)
		sum += y_axis[i];
	writeFile << "\n#\n# Total no of grains accounted here is: " << sum;
	writeFile.close();
}


void gnuPLotAll(const int &response, const char* readFileName, const double &max_, const double &min_, const int &bins, \
	const char* xLabel, const char* yLabel, const char* title, const char* writeFileName)
{
	if ((response == 1) || (response == 0))
	{
		gnu_plot(readFileName, (max_ - min_) / bins*0.9, xLabel, yLabel, title, writeFileName);
	}
	else if (response == 2)
	{
		gnu_plot_logx(readFileName, max_, (min_ + 0.000001), bins, xLabel, yLabel, title, writeFileName);
	}
	else if (response == 3)
	{
		gnu_linePlot(readFileName, xLabel, yLabel, xLabel, writeFileName);
	}
}