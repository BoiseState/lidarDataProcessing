/*
 * File name: GaussianFitting.hpp
 * Created on: 13-October-2017
 * Author: ravi
 */

#ifndef GAUSIANFITTING_HPP_
#define GAUSIANFITTING_HPP_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

int getNumPeaks();

void calculateFirstDifference();
void calculateSecondDifference();
int medianOfFive(int a, int b, int c, int d, int e);
void calculateSmoothSecondDifference();

/*
 * nLevel = user defined noise level
 * maxCount = total data points/pulse
 */
void findPeaks(std::vector<int> data, int nLevel, int maxCount); 

struct peak{
  double time;
  double amp;
  double width;
}



#endif /* GAUSIANFITTING_HPP_ */