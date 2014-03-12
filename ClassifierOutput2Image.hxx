/*  Copyright (C) 2013 Philipp Güth

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file ClassifierOutput2Image.hxx
 *  Implementation of the parameter class ClassifierOutput2Image.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 11.04.2013
 */

#ifndef _CLASSIFIEROUTPUT2IMAGE_HXX_
#define _CLASSIFIEROUTPUT2IMAGE_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ClassifierOutput2Image.h"
#include <math.h>
#include "vigra\matrix.hxx"
#include "vigra\linear_algebra.hxx"

template <typename T>
ClassifierOutput2Image<T>::ClassifierOutput2Image(const std::string& name) :
		TemplatedParameteredObject<T>(
			"ClassifierOutput2Image", name,
			"<h2>Reshapes Images2ClassifierInput so it can be "
			"visualized</h2><br>"
			"Reshapes a 1 x n label matrix to an m x k matrix.<br>"
			"Reshapes a n x f (n is # of pixel, f # of features) to a m x k "
			"x f (m+k = n) matrix."
		)
{
	ParameteredObject::_addInputSlot(
		inputMatrix, "inputMatrix",
		"2-dimensional vigra array",
		"VigraArray2float");

	ParameteredObject::_addOutputSlot(
		outputMatrix, "outputMatrix",
		"3-dimensional vigra array",
		"VigraArray3float");

	ParameteredObject::_addParameter(
		width, "width",
		"Desired pixel width of output image",
		0,
		"int");
	ParameteredObject::_addParameter(
		height, "height",
		"Desired pixel height of output image",
		0,
		"int");
}

template <typename T>
void ClassifierOutput2Image<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	int w,h;
	// check if parameters for height and width have been entered
	// it is possible to only set one parameter, the other one is calculated accordingly
	if(width() != 0 && height() !=0){
		w = width();
		h = height();
	}else if(width == 0 && height == 0){
		if(((int)sqrt((double)inputMatrix().shape(0)))  %1 !=0){	
			throw std::runtime_error("Input image was not square - parameters for width and heigt needed.");
		}else{
			w = sqrt((double)inputMatrix().shape(0));
			h = w;
		}
	}else if(width == 0 && height != 0){
		h = height();
		if((inputMatrix().shape(0)/h)%1 != 0){
			throw std::runtime_error("Height can not be the given parameter - image dimensions do not agree.");
		}else{
			w = (inputMatrix().shape(0))/h;
		}
	}else if(width != 0 && height == 0){
		w = width();
		if((inputMatrix().shape(0)/w)%1 != 0){
			throw std::runtime_error("Width can not be the given parameter - image dimensions do not agree.");
		}else{
			h = (inputMatrix().shape(0))/w;	
		}
		
	}
	int d = inputMatrix().shape(1);

	// go through columns of input matrix and reinterpret eas as 2-dimensional matrices
	// on the 3rd axis of a 3-dimensional MultiArray
	vigra::MultiArrayView<3,float> output(vigra::MultiArrayShape<3>::type(w,h,d), inputMatrix().data());
	
	// set output slot
	outputMatrix() = output;

}

#endif /* _CLASSIFIEROUTPUT2IMAGE_HXX_ */
