/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_grain_map.h
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
#include <cmath>
#include <ctime>
#include <fstream>
#include "h_plot.h"
#include "h_math.h"

using namespace std;

//#define pi 3.14159265359


void mapping_(const vector <vector<double>> &euler, const int &DATA_SIZE, vector <int> &color_, const vector <double> &v);
void union_pix(int a, int b);
int find(int a);
void matrix_mult(double matrix1[][3], double matrix2[][3], double result[][3]);
void matrix_transpose(double matrix[][3]);
double misorientation(const double ci1, const double ci2, const vector <double> &euler1, const vector <double> &euler2);
void dis_array_calc(const vector <vector <double>> &euler, const vector <double> &x_coord, const int DATA_SIZE, vector <vector <double>> &dis_array, 
	const vector <double> &ci);
void labelling(const vector <double> &x_coord, vector <vector <double>> &dis_array, const int DATA_SIZE, const double orient_angle,
	const vector <double> &ci);
void plot_map(const double x_max, const double y_max, const double step_size, const vector<int> &label, const int DATA_SIZE);

void create_new_file(fstream &outFile, const char* filename, const char* errorCode);
void grain_map_plot(const vector <int> &id, const vector<double> &x_coord, const vector<double> &y_coord,
	const vector<double> &ci, const double &min_ci, const int &DATA_SIZE, const double &y_max, const double &step_size);

void grain_mapping(const vector <vector<double>> &euler, const vector<double> &x_coord, const vector<double> &y_coord,
	const vector<double> &ci, const double &min_ci, const int &DATA_SIZE, const double &y_max, const double &step_size);

//global variables
vector <int> id, size;
extern double min_ci;


// here starts the declaration of functions
void grain_map(const vector <vector <double>> &euler, const vector <double> &x_coord, const vector <double> &y_coord, const int DATA_SIZE, \
	vector <vector <double>> &dis_array, const vector <vector<double>> &userResponse, const vector <double> &ci)
{
	/*cout << "I am in h_grain_map.h\nAnd here is a check:\n" << endl;
	cout << "\nJust a check: Row 457869 Column 3: " << euler[457868][2] << endl;
	cout << "\nJust a check: Row 472452 Column 5: " << y_coord[472451] << endl;*/

	
	cout << "Calculating dis-orientation array:\n";
	
	dis_array_calc(euler, x_coord, DATA_SIZE, dis_array, ci);


	size.resize(DATA_SIZE);
	id.resize(DATA_SIZE);

	cout << "Initializing id and size vectors....";

	for (int a = 0; a < DATA_SIZE; a++)
	{
		size[a] = 1;
		id[a] = a;
	}

	cout << "Now inside labelling\n";
	labelling(x_coord, dis_array, DATA_SIZE, userResponse[0][1], ci);
	

	
	double x_max = *max_element(x_coord.begin(), x_coord.end());
	double y_max = *max_element(y_coord.begin(), y_coord.end());
	double step_size = x_coord[1] - x_coord[0];


	grain_mapping(euler, x_coord, y_coord, ci, min_ci, DATA_SIZE, y_max, step_size);
	//grain_map_plot(id, x_coord, y_coord, ci, min_ci, DATA_SIZE, y_max, step_size);
	//plot_map(x_max, y_max, step_size, id, DATA_SIZE);

}

void dis_array_calc(const vector <vector <double>> &euler, const vector <double> &x_coord, const int DATA_SIZE, 
	vector <vector <double>> &dis_array, const vector <double> &ci)
{
	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double step_size = x_coord[1] - x_coord[0];
	

	for (long int i = 0; i < DATA_SIZE; i++)
	{
		//if (ci[i] >= 0.1) //not needed
		{
			if (x_coord[i] < (x_max - step_size + 0.05) && x_coord[i] > (step_size - 0.05)) //region with 6 nearest neighbours including bottom and top row
			{
				if (i > static_cast<int>(x_max / step_size) && (i < DATA_SIZE - static_cast<int>(x_max / step_size + 1))) // every row except bottom and top
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size)], euler[i], euler[i + static_cast<int>(x_max / step_size)]);
					dis_array[i][5] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size + 1)], euler[i], euler[i + static_cast<int>(x_max / step_size +  1)]);
				}
				else if (i > DATA_SIZE - static_cast<int>(x_max / step_size + 1) && i < (DATA_SIZE - 1)) //taking care of top row
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = 0.0; //becuase we are dealing with top row!
					dis_array[i][5] = 0.0; //becuase we are dealing with top row!
				}

				else if (i < (DATA_SIZE - 1))   //taking care of bottom row
				{
					dis_array[i][0] = 0.0; //becuase we are dealing with bottom row!
					dis_array[i][1] = 0.0; //becuase we are dealing with bottom row!
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size)], euler[i], euler[i + static_cast<int>(x_max / step_size)]);
					dis_array[i][5] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size + 1)], euler[i], euler[i + static_cast<int>(x_max / step_size +  1)]);
				}

			}
			else if ((x_coord[i] < ((step_size + 0.05) / 2)) && (x_coord[i] > ((step_size - 0.05) / 2))) //only 2nd column of hexagonal grid
			{
				if (i >= (x_max / step_size) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except bottom and top rows 
				{
					//==== just replaced > by >= to incorporate i = 726 (i.e. point no 727, 2nd row, 2nd column)
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = 0.0; //because there is no neighbour exactly behind, for the 2nd column
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size)], euler[i], euler[i + static_cast<int>(x_max / step_size)]);
					dis_array[i][5] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size + 1)], euler[i], euler[i + static_cast<int>(x_max / step_size +  1)]);
				}

				else if (i > DATA_SIZE - static_cast<int>(x_max / step_size + 1)) // the top row (last row of raster scan)
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = 0.0; //because there is no neighbour exactly behind, for the 2nd column
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = 0.0; //becuase we are dealing with top row!
					dis_array[i][5] = 0.0; //becuase we are dealing with top row!
				}
				//there is no first row in the 2nd column 
			}
			else if ((x_coord[i] < (x_max - ((step_size - 0.05) / 2))) && (x_coord[i] > x_max - ((step_size + 0.05) / 2))) //2nd last column of hexagonal grid
			{
				if ((i > (x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except bottom and top rows
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = 0.0; // because there is no neighbour in front of, 2nd last column
					dis_array[i][4] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size)], euler[i], euler[i + static_cast<int>(x_max / step_size)]);
					dis_array[i][5] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size + 1)], euler[i], euler[i + static_cast<int>(x_max / step_size +  1)]);
				}

				else if (i >(DATA_SIZE - static_cast<int> (x_max / step_size + 1))) //considering the case when last row starts from 2nd column, hence ends at 2nd last column; top row
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = 0.0; // no neighbour in front
					dis_array[i][4] = 0.0; // no row above
					dis_array[i][5] = 0.0; // no row above
				}
				//modified:: not a good idea, as the user can also define the DATA_SIZE randomly, in that case it iDATA_SIZE't necessary that (i == (DATA_SIZE - 1))
				//will be the last column of hexagonal grid : bug found
			}

			else if (x_coord[i] < ((step_size - 0.05) / 2)) //considering the case of 1st column
			{
				if ((i > static_cast<int>(x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except top and bottom rows
				{
					dis_array[i][0] = 0.0; //no neighbours behind in the row below
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = 0.0; //no neighbours exactly behind
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = 0.0; //no neighbours behind in the row above
					dis_array[i][5] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size + 1)], euler[i], euler[i + static_cast<int>(x_max / step_size +  1)]);
				}
				else if (i == (DATA_SIZE - static_cast<int>(x_max / step_size + 1))) //exactly first element of the last (top) row
				{
					dis_array[i][0] = 0.0; //no neighbours behind in the row below
					dis_array[i][1] = dis_array[i - static_cast<int>(x_max / step_size)][4];
					dis_array[i][2] = 0.0; //no neighbours exactly behind
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = 0.0; //no row above
					dis_array[i][5] = 0.0; //no row above
				}

				else if (i == 0) //first row, first element
				{
					dis_array[i][0] = 0.0; //no row below
					dis_array[i][1] = 0.0; //no row below
					dis_array[i][2] = 0.0; //no neighbours exactly behind
					dis_array[i][3] = misorientation(ci[i], ci[i+1], euler[i], euler[i + 1]);
					dis_array[i][4] = 0.0; //no neighbours behind in the row above
					dis_array[i][5] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size + 1)], euler[i], euler[i + static_cast<int>(x_max / step_size +  1)]);
				}
			}
			else if (x_coord[i] > (x_max - ((step_size - 0.05) / 2))) // considering the last column of hexagonal grid
			{
				if ((i > (x_max / step_size)) && (i < (DATA_SIZE - static_cast<int>(x_max / step_size + 1)))) //every row except top and bottom rows
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = 0.0; //no row ahead
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = 0.0; //no row ahead
					dis_array[i][4] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size)], euler[i], euler[i + static_cast<int>(x_max / step_size)]);
					dis_array[i][5] = 0.0; //no row ahead
				}
				else if (i == (DATA_SIZE - 1)) //last column, last row, assuming last row starts at 0.00
				{
					dis_array[i][0] = dis_array[i - static_cast<int>(x_max / step_size + 1)][5];
					dis_array[i][1] = 0.0; //no row ahead
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = 0.0; //no row ahead 
					dis_array[i][4] = 0.0; //no row above
					dis_array[i][5] = 0.0; //no row above
				}
				else if (i == static_cast<int>(x_max / step_size)) //last column, first row, assuming first row starts at 0.00
				{
					dis_array[i][0] = 0.0; //no row below
					dis_array[i][1] = 0.0; //no row below
					dis_array[i][2] = dis_array[i - 1][3];
					dis_array[i][3] = 0.0; //no row ahead
					dis_array[i][4] = misorientation(ci[i], ci[i + static_cast<int>(x_max / step_size)], euler[i], euler[i + static_cast<int>(x_max / step_size)]);
					dis_array[i][5] = 0.0; //no row ahead
				}
				//since it has been filtered to be at the last column; this means being precise baout (i == (x_max - 1)) and (i == (DATA_SIZE - 1))
				//shouldn't be any problem at all
			}
		}
		
	}

	
	cout << "\n\nDone with disorientation array claculation\n\n";
}


double mod_(double num)
{
	if (num < 0)
		return -1 * num;
	else
		return num;
}


double misorientation(const double ci1, const double ci2, const vector <double> &euler1, const vector <double> &euler2)
{

	if (ci1 < min_ci || ci2 < min_ci) //incorporating low ci pixels!
		return 99.99;

	if (euler1 == euler2)
		return 0.0;
	
	
	//mind the change here
	vector <double>quat1(4), quat2(4), quatF(4);
	eulerToQuat(euler1, quat1);
	eulerToQuat(euler2, quat2);
	quatInverse(quat1);//got it inverted
	quatMultQuat(quat1, quat2, quatF); //effectively inverse(q1)*q2

	double e1 = quatF[0], e2 = quatF[1], e3 = quatF[2], e4 = quatF[3]; 
	//e4 is the real/angular part

	double eq[23] = { 0.0 };   //just storing the 4th quaterenian, instead of storing all 4; 
	eq[0] = -1 * e1;
	eq[1] = -1 * e2;
	eq[2] = -1 * e3;
	eq[3] = (e4 - e1 - e2 - e3) / 2;
	eq[4] = (e4 + e2 + e1 + e3) / 2;
	eq[5] = (e4 - e2 + e1 - e3) / 2;
	eq[6] = (e4 + e2 - e1 + e3) / 2;
	eq[7] = (e4 + e2 - e1 - e3) / 2;
	eq[8] = (e4 - e2 + e1 + e3) / 2;
	eq[9] = (e4 - e2 - e1 + e3) / 2;
	eq[10] = (e4 + e2 + e1 - e3) / 2;
	eq[11] = (-e2 - e3) / sqrt(2);
	eq[12] = (-e1 - e3) / sqrt(2);
	eq[13] = (-e2 - e1) / sqrt(2);
	eq[14] = (-e2 + e3) / sqrt(2);
	eq[15] = (-e1 + e3) / sqrt(2);
	eq[16] = (-e1 + e2) / sqrt(2);
	eq[17] = (e4 - e1) / sqrt(2);
	eq[18] = (e4 - e2) / sqrt(2);
	eq[19] = (e4 - e3) / sqrt(2);
	eq[20] = (e4 + e1) / sqrt(2);
	eq[21] = (e2 + e4) / sqrt(2);
	eq[22] = (e4 + e3) / sqrt(2);
	//eq[23] = e4;

	double largest = abs(e4);

	for (int i = 0; i < 23; i++)
	{
		if (largest < abs(eq[i]))
		{
			largest = abs(eq[i]);
		}
	}

	if (largest > 1) largest = 1;
	double ret = 2 * acos(largest) * 180 / pi;

	if (ret > 90)
		return (180 - ret);
	else
		return ret;
	
}


int find_root(int index)
{
	while (index != id[index])
		index = id[id[index]];     //path compression; bug free
	return index;
}

void union_pix(int index1, int index2)
{
	int l = find_root(index1);
	int m = find_root(index2);
	int size_l = size[l];
	int size_m = size[m];

	if (l != m)
	{
		if (size_l < size_m)   //weighted! efficient!
		{
			id[l] = m;
			size[m] += size_l;
		}
		else
		{
			id[m] = l;
			size[l] += size_m;
		}
	}


}

void labelling(const vector <double> &x_coord, vector <vector <double>> &dis_array, const int DATA_SIZE, \
	const double orient_angle, const vector <double> &ci)
{
	const double x_max = *max_element(x_coord.begin(), x_coord.end());
	const double step_size = x_coord[1] - x_coord[0];



	cout << "Now calculating labelling array....";

	for (int i = 0; i < DATA_SIZE; ++i)
	{
		//if (ci[i] >= 0.1) ..not needed
		{
			double t = dis_array[i][2];
			double u = dis_array[i][1];
			double v = dis_array[i][0];


			if ((x_coord[i] < (x_max - (step_size - 0.05) / 2)) && (x_coord[i] > (step_size - 0.05)))// every column except 1st, 2nd and last column
			{
				if (i > static_cast <int> (x_max / step_size)) //every row except 1st row
				{


					if ((t <= orient_angle) && (v <= orient_angle) && (u <= orient_angle))
					{
						union_pix(i, (i - 1));
						union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
						union_pix(i, (i - static_cast<int>(x_max / step_size)));
					}

					else if ((t <= orient_angle) && (u <= orient_angle))
					{
						union_pix(i, (i - 1));
						union_pix(i, (i - static_cast<int>(x_max / step_size)));
					}

					else if ((t <= orient_angle) && (v <= orient_angle))
					{
						union_pix(i, (i - 1));
						union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
					}

					else if ((v <= orient_angle) && (u <= orient_angle))
					{
						union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
						union_pix(i, (i - static_cast<int>(x_max / step_size)));
					}

					else if (u <= orient_angle)
						union_pix(i, (i - static_cast<int>(x_max / step_size)));
					else if (v <= orient_angle)
						union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
					else if (t <= orient_angle)
						union_pix(i, (i - 1));

				}
				else if (i < static_cast <int> (x_max / step_size))  //1st row of all columns except that of 1st, 2nd and last column
				{
					if (t <= orient_angle)
						union_pix(i, (i - 1));
				}

			}
			else if (x_coord[i] < ((step_size + 0.05) / 2) && x_coord[i] > ((step_size - 0.05) / 2)) //Now dealing with only 2nd column of hexagonal grid
			{
				//if (i > static_cast<int> (x_max / step_size)) // every row except first row; in 1st row there is no 2nd column; so condition is useless
				//{
				if ((v <= orient_angle) && (u <= orient_angle))
				{
					union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
					union_pix(i, (i - static_cast<int>(x_max / step_size)));
				}
				else if (u <= orient_angle)
					union_pix(i, (i - static_cast<int>(x_max / step_size)));
				else if (v <= orient_angle)
					union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
				//}
			}

			else if (x_coord[i] < ((step_size - 0.05) / 2)) //Now dealing with the 1st column
			{
				if (i > 0) //every row except 1st row ====
				{
					if (u <= orient_angle)
						union_pix(i, (i - static_cast<int>(x_max / step_size)));
				}
				//Seems like there is no arrangement for 1st column 1st row....yes there is: this step is avoided to avoid redundancy!
			}
			else if (x_coord[i] >(x_max - (step_size - 0.05) / 2)) //Now dealing with the last column
			{
				if (i >  static_cast<int> (x_max / step_size)) //every row except first row ====
				{
					if ((t <= orient_angle) && (v <= orient_angle))
					{
						union_pix(i, (i - 1));
						union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
					}
					else if (v <= orient_angle)
						union_pix(i, (i - static_cast<int>(x_max / step_size + 1)));
					else if (t <= orient_angle)
						union_pix(i, (i - 1));
				}
				else //this is supposed to handle last column first row
				{
					if (t <= orient_angle)
						union_pix(i, (i - 1));
				}
			}
		}
		
	}


	for (int i = 0; i < DATA_SIZE; ++i)
		if (ci[i] < min_ci)
			id[i] = i;
	//making sure that indeed they are treated as single grains!

	int count = 0; //counting no of grains!
	//taking fresh values of id
	for (int i = 0; i < DATA_SIZE; ++i) // Reusing the find function to get the final values of all the labels.
		id[i] = find_root(i); //shouldn't it be just find(i) instead of find(id[i])-> done

	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ re-working on sizes! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int i = 0; i < DATA_SIZE; ++i)
		size[i] = 0; //there is definitely one root there => that incorporates for size = 1

	for (int i = 0; i < DATA_SIZE; ++i)
		size[id[i]] += 1; //this also incorporates 
	
	//counting no of different grains and sizes here.
	for (int i = 0; i < DATA_SIZE; ++i)  
		if (ci[i] >= min_ci)
		{
			if (id[i] == i)
				++count;
			size[i] = size[id[i]]; // modified size!
		}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ done with sizes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	cout << "Done with labelling array calculation\n\n";

}




void grain_mapping(const vector <vector<double>> &euler, const vector<double> &x_coord, const vector<double> &y_coord, 
	const vector<double> &ci, const double &min_ci, const int &DATA_SIZE, const double &y_max, const double &step_size)
{

	vector<double> v = { 0, 0, 1 }; //this one is just for 001 and nothing else
	vector <vector <int>> color_(DATA_SIZE, vector<int>(3));
	newMapping(euler, DATA_SIZE, color_, v);

	int x_count, y_count;

	for (int i = 1; i < DATA_SIZE; ++i) //notice., i starts from 1 here
	{
		if (x_coord[i] > x_coord[i - 1])
			continue;
		else
		{
			x_count = i; // no of x points on the plot
			break;
		}
	}

	
	y_count = 2 * DATA_SIZE / (x_count + (x_count - 1)); //no of y points on the plot

	BMP_plot(x_count, y_count, DATA_SIZE, color_, "Grain Map.bmp");

	cout << "Done with grain map plotting using gnuplot\n";

}
