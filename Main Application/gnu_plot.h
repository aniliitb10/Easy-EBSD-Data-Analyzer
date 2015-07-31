/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: gnu_plot.h
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
#include <cstdio>
#include <iostream>

void gnu_plot(const char *data_file_name,const double &width, const char *x_label, const char *y_label, \
	const char *title = "", const char *ouput_file_name = "plot.png")
{
	/*x_label = "testing-xlabel-automated";
	char *y_label = "testing-ylabel-automated";
	char *_title = "testing-title-automated";*/
	FILE* pipe = _popen("C:/gnuplot/bin/pgnuplot.exe", "w"); //mind the change here "-"
	if (pipe != NULL)
	{
		// Output to PNG, with Verdana 8pt font
		fprintf(pipe, "set terminal pngcairo nocrop enhanced font \"verdana,8\" size 720,500\n");
		// Don't show the legend in the chart
		fprintf(pipe, "set nokey\n");
		// Thinner, filled bars----------------------------------still need to work on this, it has to be automatic;
		fprintf(pipe, "set boxwidth %lf\n", width);
		fprintf(pipe, "set style fill solid 0.9\n");
		//fprintf(pipe, "\n");
		//Set a title and X label and Y label
		fprintf(pipe, "set title \"%s\"font \",14\" tc rgb \"#606060\"\n",title);
		//may be it means just same font, different font size
		fprintf(pipe, "set ylabel \"%s\"\n", y_label);
		fprintf(pipe, "set xlabel \"%s\"\n", x_label);
		//fprintf(pipe, "set yrange [0 to 1800]\n");
		//fprintf(pipe, "set xrange [0 to 6]\n"); //--------------------------------------need to automate it!
		//Show human-readable Y-axis. E.g. "100 k" instead of 100000.
		//fprintf(pipe, "set format y \"%.0s %c\"\n");//----------------------------not working!!
		//Replace small stripes on the Y-axis with a horizontal gridlines
		fprintf(pipe, "set tic scale 0\n");
		fprintf(pipe, "set grid ytics lc rgb \"#505050\"\n");
		fprintf(pipe, "set output \"%s\"\n", ouput_file_name);
		fprintf(pipe, "plot \"%s\" using 1:2 with boxes lt rgb \"#406090\"\n", data_file_name);
		fprintf(pipe, "replot\n");
		fprintf(pipe, "set terminal win\n");
		fflush(pipe);
	}
	else
		puts("Couldn't open the application\n");
	_pclose(pipe);
}


void gnu_plot_logx(const char *data_file_name, const double &max_, const double &min_, const int &bins, const char *x_label, \
	const char *y_label, const char *title = "", const char *ouput_file_name = "plot.png")
{
	/*x_label = "testing-xlabel-automated";
	char *y_label = "testing-ylabel-automated";
	char *_title = "testing-title-automated";*/
	double width = (log10(max_) - log10(min_)) / bins * 0.9;

	FILE* pipe = _popen("C:/gnuplot/bin/pgnuplot.exe", "w"); //mind the change here "-"
	if (pipe != NULL)
	{
		// Output to PNG, with Verdana 8pt font
		fprintf(pipe, "set terminal pngcairo nocrop enhanced font \"verdana,8\" size 720,500\n");
		// Don't show the legend in the chart
		fprintf(pipe, "set logscale x 10\n");
		//logscale x axis
		fprintf(pipe, "set nokey\n");
		// Thinner, filled bars----------------------------------still need to work on this, it has to be automatic;
		fprintf(pipe, "set boxwidth %lf\n", width);
		fprintf(pipe, "set style fill solid 0.9\n");
		//fprintf(pipe, "\n");
		//Set a title and X label and Y label
		fprintf(pipe, "set title \"%s\"font \",14\" tc rgb \"#606060\"\n", title);
		//may be it means just same font, different font size
		fprintf(pipe, "set ylabel \"%s\"\n", y_label);
		fprintf(pipe, "set xlabel \"%s\"\n", x_label);
		//fprintf(pipe, "set yrange [0 to 1800]\n");
		//fprintf(pipe, "set xrange [0 to 6]\n"); //--------------------------------------need to automate it!
		//Show human-readable Y-axis. E.g. "100 k" instead of 100000.
		//fprintf(pipe, "set format y \"%.0s %c\"\n");//----------------------------not working!!
		//Replace small stripes on the Y-axis with a horizontal gridlines
		fprintf(pipe, "set tic scale 0\n");
		fprintf(pipe, "set grid ytics lc rgb \"#505050\"\n");
		fprintf(pipe, "set output \"%s\"\n", ouput_file_name);
		fprintf(pipe, "plot [%lf:%lf] \"%s\" using 1:2 with boxes lt rgb \"#406090\"\n", min_, max_, data_file_name);
		fprintf(pipe, "replot\n");
		fprintf(pipe, "unset logscale x\n");
		fprintf(pipe, "set terminal win\n");
		fflush(pipe);
	}
	else
		puts("Couldn't open the application\n");
	_pclose(pipe);
}


void gnu_linePlot(const char *data_file_name, const char *x_label, const char *y_label, const char *title = "", \
	const char *ouput_file_name = "plot.png")
{
	/*x_label = "testing-xlabel-automated";
	char *y_label = "testing-ylabel-automated";
	char *_title = "testing-title-automated";*/
	FILE* pipe = _popen("C:/gnuplot/bin/pgnuplot.exe", "w"); //mind the change here "-"
	if (pipe != NULL)
	{
		// Output to PNG, with Verdana 8pt font
		fprintf(pipe, "set terminal pngcairo nocrop enhanced font \"verdana,8\" size 720,500\n");
		// Don't show the legend in the chart
		fprintf(pipe, "set nokey\n");
		//fprintf(pipe, "\n");
		//Set a title and X label and Y label
		fprintf(pipe, "set title \"%s\"font \",14\" tc rgb \"#606060\"\n", title);
		//may be it means just same font, different font size
		fprintf(pipe, "set ylabel \"%s\"\n", y_label);
		fprintf(pipe, "set xlabel \"%s\"\n", x_label);
		
		fprintf(pipe, "set tic scale 0\n");
		fprintf(pipe, "set grid ytics lc rgb \"#505050\"\n");
		fprintf(pipe, "set output \"%s\"\n", ouput_file_name);
		fprintf(pipe, "plot \"%s\" using 1:2 with lp lw 2 lt rgb \"#406090\"\n", data_file_name);
		fprintf(pipe, "replot\n");
		fprintf(pipe, "set terminal win\n");
		fflush(pipe);
	}
	else
		puts("Couldn't open the application\n");
	_pclose(pipe);
}
