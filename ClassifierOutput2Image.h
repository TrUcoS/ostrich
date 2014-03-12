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
/** \file ClassifierOutput2Image.h
 *  Declaration of the parameter class ClassifierOutput2Image.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 11.04.2013
 */

#ifndef _CLASSIFIEROUTPUT2IMAGE_H_
#define _CLASSIFIEROUTPUT2IMAGE_H_

#ifdef _MSC_VER
#ifdef classifieroutput2image_EXPORTS
/// Visual C++ specific code
#define classifieroutput2image_DECLDIR __declspec(dllexport)
#else
#define classifieroutput2image_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define classifieroutput2image_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>

/// Reshapes Images2ClassifierInput so it can be visualized
/** Reshapes a 1 x n label matrix to an m x k matrix.
 *  Reshapes a n x f (n is # of pixel, f # of features) to a m x k x f (m+k =
 *  n) matrix.
 */



typedef vigra::MultiArray<2, float> VigraArray2float;
typedef vigra::MultiArray<3, float> VigraArray3float;

template <typename T>
class classifieroutput2image_DECLDIR ClassifierOutput2Image :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	ClassifierOutput2Image(const std::string& name = "");

	/// 2-dimensional vigra array
	InputSlot< VigraArray2float > inputMatrix;

	/// 3-dimensional vigra array
	OutputSlot< VigraArray3float > outputMatrix;

	// int parameter for width and height of output image
	Parameter< int > width;
	Parameter< int > height;

	/// Update object.
	virtual void execute();
};

#endif // _CLASSIFIEROUTPUT2IMAGE_H_
