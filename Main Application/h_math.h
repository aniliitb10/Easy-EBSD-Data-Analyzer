/**************************************************************************************
*	Easy EBSD Data Analyzer

*	Author: Anil Kumar
*	email: aniliitb10@gmail.com
*	file: h_math.h
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
#include <cmath>
using namespace std;

extern vector <int> id, size;
#define pi 3.142857
#define tol -0.01
#define p_tol 0.000001
void quatMult(const double &t1, const vector <double> &v1, const double &t2, const vector <double> &v2, double &t3, vector <double> &v3);
void mapping_(const vector <vector<double>> &euler, vector <int> &color_, int DATA_SIZE, const vector <double> &v);
void quatRot(const vector<double> &q, const vector <double> &v, vector <double> &ans_);
void makeItUnitQuatOrVector(vector <double> &col);

/*void quatMult(const double &t1, const vector <double> &v1, const double &t2, const vector <double> &v2, double &t3, vector <double> &v3)
void time_spent(SYSTEMTIME t1)
{
SYSTEMTIME t;
GetLocalTime(&t);
double time_sec = 0;
int time_h = 0;
int time_m = 0;
int time_s = 0;
int time_ms = 0;


time_sec += (static_cast<double>(t.wMilliseconds - t1.wMilliseconds)) / 1000 + (t.wSecond - t1.wSecond);
time_sec += (t.wMinute - t1.wMinute) * 60 + (t.wHour - t1.wHour) * 3600;

time_h = static_cast<int>(time_sec / 3600);
time_sec = time_sec - time_h * 3600;

time_m = static_cast<int>(time_sec / 60);
time_sec = time_sec - time_m * 60;

time_s = static_cast<int>(time_sec / 1);
time_sec = time_sec - time_s;

time_ms = static_cast<int>(time_sec / 0.001);

printf("%02dhr: %02dm: %02ds: %03dms\n", time_h, time_m, time_s, time_ms);
}
*/


//this matrix multiplicaion, takes two 3x3 matrices, multiplies first two and it's result is stored in the third one!
//void matrix_mult(double matrix1[][3], double matrix2[][3], double result[][3]): checked and verified : working
void matrix_mult(double matrix1[][3], double matrix2[][3], double result[][3])
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result[i][j] = 0;
			for (int k = 0; k < 3; ++k) {
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
}

void matrix_mult(const vector<vector<double>> &matrix1, const vector<vector<double>> &matrix2, vector<vector<double>> &result) //for vectors
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result[i][j] = 0;
			for (int k = 0; k < 3; ++k) {
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
}



//for vectors
//first overloading in this code
//declared in h_twin_bound.h =======================modified for 3x3 * 3x1 multiplication
void matrix_mult(const vector<vector<double>> &matrix2, const vector<double> &matrix1, vector<double> &result)
{
	//notice here, i < 1, because the matrices are expexted to be 1x3 and 3x3
	for (int j = 0; j < 3; ++j) {
		result[j] = 0;
		for (int k = 0; k < 3; ++k) {
			result[j] += matrix2[j][k] * matrix1[k];
		}
	}
}


//void matrix_transpose(double matrix[][3]): checked and verified: working
void matrix_transpose(double matrix[][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 3; j++)            //tricky intialization for j
		{
			swap(matrix[i][j], matrix[j][i]);
		}
	}
}

void squareMatrixTranspose(vector <vector <double>> &matrix) //it is assumed that matrix is a square matrix (vector)
{

	int l = matrix.size();
	double temp = 0;
	for (int i = 0; i < l; i++)
	{
		for (int j = i + 1; j < l; j++)            //tricky intialization for j
		{
			temp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp;
			//swap(matrix[i][j], matrix[j][i]);
		}
	}
}
bool isSame(double num1, double num2)
{
	if ((num1 < num2 + p_tol) && (num1 > num2 - p_tol))
		return true;
	else
		return false;
}


double quat_dot(const vector <double> &v1, const vector <double> &v2)
{

	double ret = 0;
	for (int i = 0; i < 4; ++i)
	{
		ret += v1[i] * v2[i];
	}

	//double ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];

	if (ret < 0)
	{
		ret = 0;
		for (int i = 0; i < 3; ++i)
		{
			ret += (-1) * v1[i] * v2[i]; //axis of one of them is reversed
		}
		ret += v1[3] * v2[3]; //only angular part is still the same
	}

	return ret;
}

double vectorMod_(const vector <double> &v)
{
	unsigned int size_ = v.size();
	double ret = 0;

	for (unsigned int i = 0; i < size_; ++i)
	{
		ret += pow(v[i], 2);
	}

	ret = sqrt(ret);
	return ret;
}




void eulerToQuat(const vector <double> &euler, vector <double> &q)
{
	//from http://mathworld.wolfram.com/EulerParameters.html follows x -convention as by Goldstein (1980, p. 176)
	q[0] = cos(0.5 * (euler[0] - euler[2]))*sin(0.5 * euler[1]);
	q[1] = sin(0.5 * (euler[0] - euler[2]))*sin(0.5 * euler[1]);
	q[2] = sin(0.5 * (euler[0] + euler[2]))*cos(0.5 * euler[1]);
	q[3] = cos(0.5 * (euler[0] + euler[2]))*cos(0.5 * euler[1]); //actually q[3] represents the real/angular part

	makeItUnitQuatOrVector(q);
	//finding the minima of real/angular part makes no sense here, so not going for the symmetry opertaions
}




//Now starts coding related to colors, works for both vector and quaternions


double dist2D_(const vector<double> &v1, const vector<double> &v2)
{
	double ret = sqrt((v1[0] - v2[0])*(v1[0] - v2[0]) + (v1[1] - v2[1])*(v1[1] - v2[1]));
	return ret;
}


vector <double> coordinate(const vector <double> &A, const vector <double> &B, const vector <double> &C, const vector <double> &D)
{
	vector<double> ret(2);
	double K1 = (A[0] - B[0])*(C[1] - D[1]) - (A[1] - B[1])*(C[0] - D[0]);
	ret[0] = (A[0] * B[1] - A[1] * B[0]) * (C[0] - D[0]) - (A[0] - B[0]) * (C[0] * D[1] - C[1] * D[0]) / K1;
	ret[1] = (A[0] * B[1] - A[1] * B[0]) * (C[1] - D[1]) - (A[1] - B[1]) * (C[0] * D[1] - C[1] * D[0]) / K1;
	return ret;
}

vector <double> coordinateR_(const vector <double> &C)
{
	vector <double> ret(2);
	double K1 = (C[1] / C[0])*(C[1] / C[0]);
	ret[0] = (sqrt(K1 + 2) - 1) / (K1 + 1);
	ret[1] = (C[1] / C[0]) * ret[0];

	return ret;
}

double magnitude(double num)
{
	double ret = ((num > 0) ? num : -1 * num);
	return ret;
}

bool areSame_(const vector <double> &v1, const vector <double> &v2)
{
	if ((v1[0] == v2[0]) && (v1[1] == v2[1]))
		return true;
	else
		return false;
}



//dot product of any 2d, 3d vector or -------"quaternion" ----- Of course of the same dimension
double dot(const vector <double> &v1, const vector <double> &v2)
{
	int size_ = v1.size();
	double ret = 0;
	for (int i = 0; i < size_; ++i)
	{
		ret += v1[i] * v2[i];
	}
	//double ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	return ret;
}


vector <double> cross(const vector<double> &v1, const vector<double> &v2)
{
	vector <double> ret = { v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] };
	return ret;
}


//this one is direct quat and quat multiplication giving another quat, both q1 and q2 have 4 elements
void quatMultQuat(const vector <double> &q1, const vector<double> &q2, vector <double> &q)
{
	double t1, t2, t3;
	vector <double> v1(3), v2(3), v3(3);
	t1 = q1[3]; t2 = q2[3]; //And it should not be t1 = v1[0]; t1 = v2[0]; angular/real part is the last one

	for (int i = 0; i < 3; ++i)
	{
		v1[i] = q1[i];
		v2[i] = q2[i];
	}

	quatMult(t1, v1, t2, v2, t3, v3);
	for (int i = 0; i < 3; ++i)
	{
		q[i] = v3[i];
	}
	q[3] = t3; //angular/real part is the last one
	makeItUnitQuatOrVector(q);
}

void quatMult(const double &t1, const vector <double> &v1, const double &t2, const vector <double> &v2, double &t3, vector <double> &v3)
{
	//if (isSame())
	
	t3 = t1*t2 - dot(v1, v2);
	v3 = cross(v1, v2);

	for (int i = 0; i < 3; ++i)
	{
		v3[i] += t1*v2[i] + t2*v1[i];
	}

	
	//done here
}


void quatRot(const vector<double> &q, const vector <double> &v, vector <double> &ans_)
{
	
	/*
	$	q is quaternion having 4 components, v contains just indices of an axis like [0 0 1] to be rotated,
	$	and ans_ is indices of an axis like [1 0 1] after rotation
	*/
	
	
	double t1 = q[3]; //assuming that the last one is angle
	vector <double> v1 = { q[0], q[1], q[2] };
	double t2 = 0;
	vector <double> v2 = { v[0], v[1], v[2] }; //assuming v to contain just indices like [0 0 1]
	double tempT;
	vector <double> tempV; //isn't required to mention dimension, fills up during cross product
	quatMult(t1, v1, t2, v2, tempT, tempV);

	v1[0] *= -1;
	v1[1] *= -1;
	v1[2] *= -1;
	// v1 has now become conjugate of v1, bit misleading!!

	double ansT;
	vector<double> ansV;
	quatMult(tempT, tempV, t1, v1, ansT, ansV);

	//ansT should be zero!!
	if (!isSame(ansT, 0))
	{
		cout << "Please stop, something is wrong in quatRot\n";
		system("pause");
	}
	

	//need to make sure that size of ans_ is 3
	ans_[0] = ansV[0];
	ans_[1] = ansV[1];
	ans_[2] = ansV[2];
}


void cubicSymOps(vector<vector <vector<double>>> &syms) // assumed to be 24by3by3 matrix!
{
	//source Introduction to TEXTURE ANALYSIS Macrotexture, Microtexture, and Orientation Mapping OLAF ENGLER VALERIE RANDL 2nd Ed
	//-page 381, Appendix II
	//vector <vector <vector <int>>> syms(24, vector <vector <int>>(3, vector <int>(3)));
	syms[0] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
	syms[1] = { { 0, 0, -1 }, { 0, -1, 0 }, { -1, 0, 0 } };
	syms[2] = { { 0, 0, -1 }, { 0, 1, 0 }, { 1, 0, 0 } };
	syms[3] = { { -1, 0, 0 }, { 0, 1, 0 }, { 0, 0, -1 } };
	syms[4] = { { 0, 0, 1 }, { 0, 1, 0 }, { -1, 0, 0 } };
	syms[5] = { { 1, 0, 0 }, { 0, 0, -1 }, { 0, 1, 0 } };
	syms[6] = { { 1, 0, 0 }, { 0, -1, 0 }, { 0, 0, -1 } };
	syms[7] = { { 1, 0, 0 }, { 0, 0, 1 }, { 0, -1, 0 } };
	syms[8] = { { 0, -1, 0 }, { 1, 0, 0 }, { 0, 0, 1 } };
	syms[9] = { { -1, 0, 0 }, { 0, -1, 0 }, { 0, 0, 1 } };
	syms[10] = { { 0, 1, 0 }, { -1, 0, 0 }, { 0, 0, 1 } };
	syms[11] = { { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 } };
	syms[12] = { { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 } };
	syms[13] = { { 0, 0, -1 }, { -1, 0, 0 }, { 0, 1, 0 } };
	syms[14] = { { 0, -1, 0 }, { 0, 0, 1 }, { -1, 0, 0 } };
	syms[15] = { { 0, 1, 0 }, { 0, 0, -1 }, { -1, 0, 0 } };
	syms[16] = { { 0, 0, -1 }, { 1, 0, 0 }, { 0, -1, 0 } };
	syms[17] = { { 0, 0, 1 }, { -1, 0, 0 }, { 0, -1, 0 } };
	syms[18] = { { 0, -1, 0 }, { 0, 0, -1 }, { 1, 0, 0 } };
	syms[19] = { { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, -1 } };
	syms[20] = { { -1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 } };
	syms[21] = { { 0, 0, 1 }, { 0, -1, 0 }, { 1, 0, 0 } };
	syms[22] = { { 0, -1, 0 }, { -1, 0, 0 }, { 0, 0, -1 } };
	syms[23] = { { -1, 0, 0 }, { 0, 0, -1 }, { 0, -1, 0 } };

}


void eulerToMatrix(const vector <double> &euler, vector <vector<double>> &matrix)
{

	double p1 = euler[0];
	double p = euler[1];
	double p2 = euler[2];

	matrix = { { (cos(p1)*cos(p2) - sin(p1)*sin(p2)*cos(p)), (sin(p1)*cos(p2) + cos(p1)*sin(p2)*cos(p)), \
		(sin(p2)*sin(p)) }, { (-cos(p1)*sin(p2) - sin(p1)*cos(p2)*cos(p)), (-sin(p1)*sin(p2) + cos(p1)*cos(p2)*cos(p)), \
		(cos(p2)*sin(p)) }, { (sin(p1)*sin(p)), (-cos(p1)*sin(p)), (cos(p)) } };
}

//3by3 to 3by1 multiplication
void matrix_mult_3331(const vector<vector<double>> &matrix1, const vector<double> &matrix2, vector<double> &result)
{
	for (int j = 0; j < 3; ++j) {
		result[j] = 0;
		for (int k = 0; k < 3; ++k) {
			result[j] += matrix1[j][k] * matrix2[k];
		}
	}
}





void newMapping(const vector <vector<double>> &euler, const int &DATA_SIZE, vector <vector<int>> &color_, const vector <double> &vect)
{

	vector <vector <double>> matrix(3, vector <double>(3)), tempMatrix(3, vector <double>(3));
	vector <vector <vector <double>>> syms(24, vector <vector <double>>(3, vector <double>(3)));
	cubicSymOps(syms);
	vector<double> result_(3), ans_(3);
	double u, v, w, vec_mod, uvw_max = 0;
	vector <double> check_(DATA_SIZE); //mind the initialization here
	vector <vector<int>> colorTemp(DATA_SIZE, vector <int>(3));

	for (int i = 0; i < DATA_SIZE; ++i)
	{

		/*
		* in IPF, the vector pointing along a given sample direction, 001 here, is projected onto planes determined
		* by the local crystallographic orientation; another equivalent idea is to get this vector is following:
		* 1) Assume both reference orientation axes are coinciding at origin
		* 2) Calculate a rotation matrix = transpose of (Euler to matrix transformation)
		* 3) Use this rotation matrix to rotate 001 of crystal; that would be 001 of sample, bingo!
		* 4) We have just rotated, no translation; so just normalize it to find its intersection with unit sphere
		* 5) Take its projection on the plane determined by local crystallographic orientation,
		*    infact that's the plane we are dealing with
		* done
		*/


		eulerToMatrix(euler[i], matrix); //creating rotation matrix;
		squareMatrixTranspose(matrix); //transposed to get the IPF here

		matrix_mult_3331(matrix, vect, result_); //rotating the vector vect to get result_
		for (int j = 0; j < 24; ++j)
		{
			tempMatrix = syms[j];
			matrix_mult_3331(tempMatrix, result_, ans_); //ans_ will keep changing during symmetry operations

			vec_mod = vectorMod_(ans_);//its mod is always one!

			ans_[0] /= vec_mod;
			ans_[1] /= vec_mod;
			ans_[2] /= vec_mod;

			u = ans_[2] - ans_[0];
			v = ans_[0] - ans_[1];
			w = ans_[1];


			if ((u >= tol) && (v >= tol) && (w >= tol))
			{
				uvw_max = 0; //important step!
				uvw_max = ((uvw_max < u) ? u : uvw_max);
				uvw_max = ((uvw_max < v) ? v : uvw_max);
				uvw_max = ((uvw_max < w) ? w : uvw_max);

				/*color_[i][0] = static_cast<int>(u * 255 / uvw_max);
				color_[i][1] = static_cast<int>(v * 255 / uvw_max);
				color_[i][2] = static_cast<int>(w * 255 / uvw_max);*/

				colorTemp[i][0] = static_cast<int>(u * 255 / uvw_max);
				colorTemp[i][1] = static_cast<int>(v * 255 / uvw_max);
				colorTemp[i][2] = static_cast<int>(w * 255 / uvw_max);

				/*colorTemp[i][0] = static_cast<int>(u * 255 / (u + v + w));
				colorTemp[i][1] = static_cast<int>(v * 255 / (u + v + w));
				colorTemp[i][2] = static_cast<int>(w * 255 / (u + v + w));*/


				break;
			}
		}
	}

	vector<vector<long long>> sumAtRoot(DATA_SIZE, vector<long long>(3));
	vector<int>count(DATA_SIZE);
	for (int i = 0; i < DATA_SIZE; ++i)
	{
		if ((colorTemp[i][0] > 250) || (colorTemp[i][1] > 250) || (colorTemp[i][2] > 250))
		{
			sumAtRoot[id[i]][0] += colorTemp[i][0];
			sumAtRoot[id[i]][1] += colorTemp[i][1];
			sumAtRoot[id[i]][2] += colorTemp[i][2];
			count[id[i]] += 1;
		}
	}

	double colorMAX, vb0, vb1, vb2;

	for (int i = 0; i < DATA_SIZE; ++i)
		{
		if (count[id[i]] > 0) //mind the change here
			{
				colorMAX = 0;

				vb0 = sumAtRoot[id[i]][0] / static_cast<double>(count[id[i]]);
				vb1 = sumAtRoot[id[i]][1] / static_cast<double>(count[id[i]]);
				vb2 = sumAtRoot[id[i]][2] / static_cast<double>(count[id[i]]);

				colorMAX = ((colorMAX < vb0) ? vb0 : colorMAX);
				colorMAX = ((colorMAX < vb1) ? vb1 : colorMAX);
				colorMAX = ((colorMAX < vb2) ? vb2 : colorMAX);

				color_[i][0] = static_cast<int>(vb0 / colorMAX * 255);
				color_[i][1] = static_cast<int>(vb1 / colorMAX * 255);
				color_[i][2] = static_cast<int>(vb2 / colorMAX * 255);
			}
	}
	colorTemp.clear();
	sumAtRoot.clear();
	count.clear();
}


void quatToMatrix(const vector <double> &q, vector <vector<double>> &m) //assummed their sizes to be 1by4 and 3by3
{
	double e0 = q[3], e1 = q[0], e2 = q[1], e3 = q[2]; //mind the change in indices, in q, last component is the real/angular part

	//source: http://mathworld.wolfram.com/EulerAngles.html (Goldstein 1980, p. 153) x-convention for euler angles

	m = { { (pow(e0, 2) + pow(e1, 2) - pow(e2, 2) - pow(e3, 2)), 2 * (e1*e2 + e0*e3), 2 * (e1*e3 - e0*e2) }, \
	{2 * (e1*e2 - e0*e3), (pow(e0, 2) - pow(e1, 2) + pow(e2, 2) - pow(e3, 2)), 2 * (e2*e3 + e0*e1)}, \
	{2 * (e1*e3 + e0*e2), 2 * (e2*e3 - e0*e1), (pow(e0, 2) - pow(e1, 2) - pow(e2, 2) + pow(e3, 2))} };
}


vector <double> avgAndDev(const vector <double> &col)
{
	unsigned int size_ = col.size();
	double avg, dev, sum_dev = 0, sum = 0; //double uses 8 bytes (15 digits)
	vector <double> ret(2);

	for (unsigned int i = 0; i < size_; ++i)
	{
		sum += col[i];
	}
	avg = sum / size_;

	for (unsigned int i = 0; i < size_; ++i)
	{
		sum_dev += pow((col[i] - avg), 2); //mind taking the abs here
	}

	//sum_dev = abs(sum_dev); //taking the absolute values
	dev = sqrt(sum_dev / (size_ - 1));
	ret = { avg, dev };
	return ret;
}

void quatInverse(vector <double> &quat)
{
	quat[0] *= -1;
	quat[1] *= -1;
	quat[2] *= -1;
	//quat[3] *= -1; 4th one is the angular part, so not this one!
}



vector <double> temp(4);
double newMisorientation(const vector <double> &euler, const vector <double> &quat2)
{
	/*
	*Imputs to this function are 1by3 vector and 3by3 vector
	*(supposed to be euler angles and quaternions respectively) 
	*/

	vector <double>quat1(4), quatF(4);
	eulerToQuat(euler, quat1);
	quatInverse(quat1);//got it inverted
	if (dot(quat1, quat2) > 0 )
		quatMultQuat(quat1, quat2, quatF); //effectively inverse(q1)*q2
	else
	{
		quatInverse(quat1);
		quatMultQuat(quat1, quat2, quatF); //effectively inverse(q1)*q2
	}

	double e1 = quatF[0], e2 = quatF[1], e3 = quatF[2], e4 = quatF[3];


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

void makeItUnitQuatOrVector(vector <double> &col)
{
	double temp_mod = vectorMod_(col);
	if (isSame(temp_mod, 0))
	{
		for (unsigned int i = 0; i < col.size(); ++i)
			col[i] = 0;
	}
	else
	{
		for (unsigned int i = 0; i < col.size(); ++i)
			col[i] /= temp_mod;
	}

}
