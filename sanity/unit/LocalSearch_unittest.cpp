/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Soumyajit De
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <tesseract/algorithm/LocalSearch.hpp>
#include <tesseract/regularizer/SmoothedDifferentialEntropy.hpp>
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace tesseract;
using namespace Eigen;

void test1()
{
	int dim = 5;
	int N = 10;
	MatrixXd m(N, dim + 1);

	m <<
	0.3192164253901439,0.1830189038046517,0.2216994825373632,0.442620783465354,0.2066900041715171,0.4009646555961595,
	0.01928553046639133,0.2176736154053891,0.4937059505042571,0.2200592461757551,0.3665433362877432,0.1767118075993788,
	0.2648456755672952,0.0164205996240577,0.1456804246646856,0.2826117422209888,0.03640865710016588,0.2904715581889157,
	0.2481205530826417,0.2179855202093644,0.189426306658292,0.2654317143725495,0.2092954354042388,0.1079768326044813,
	0.0278019765722265,0.4053432918214635,0.5043292222497134,0.4144890235951803,0.2942863750017691,0.05678335348131101,
	0.4759718485028774,0.2858921057689419,0.3512555279131513,0.05970802754798522,0.4064719361424702,0.3904339815907013,
	0.445153342326724,0.3742765073699061,0.3391483339336612,0.4081925971864195,0.1526819519085699,0.6110410982730742,
	0.4599745669691397,0.4359156545941428,0.02447723554476081,0.253319518103433,0.3361592574725628,0.3847223971764809,
	0.2008646379028169,0.3947951023464055,0.1025837484563378,0.05358540921542497,0.589870804959913,0.07759394063289483,
	0.2976827970769158,0.3732679195991875,0.3822644357939626,0.4430663702153999,0.2042272226111877,0.1700033823703085;

	MatrixXd X = m.block(0,0,N,dim);
	VectorXd y = m.block(0,dim,N,1);

	LocalSearch<SmoothedDifferentialEntropy, float64_t> ls(X,y);
	typedef LocalSearch<SmoothedDifferentialEntropy, float64_t>::param_type param_type;
	ls.set_params(param_type());
	std::vector<index_t> inds = ls.run();

	// all the features are selected
	assert(inds.size() == dim);
}

int main(int argc, char** argv)
{
	test1();
	return 0;
}
