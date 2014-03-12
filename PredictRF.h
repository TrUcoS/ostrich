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
/** \file PredictRF.h
 *  Declaration of the parameter class PredictRF.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 10.04.2013
 */

#ifndef _PREDICTRF_H_
#define _PREDICTRF_H_

#ifdef _MSC_VER
#ifdef predictrf_EXPORTS
/// Visual C++ specific code
#define predictrf_DECLDIR __declspec(dllexport)
#else
#define predictrf_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define predictrf_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>

#include "vigra\random_forest.hxx"

typedef vigra::RandomForest<double> randomForest;
typedef vigra::MultiArray<2,float> VigraArray2float;
typedef vigra::MultiArray<2,float> VigraArray2float;
typedef vigra::MultiArrayShape<2>::type shape_t;

/// Predicts labels for features given an existing random Forest
/** Predicts labels for features given an existing random Forest
 */
template <typename T>
class predictrf_DECLDIR PredictRF :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	PredictRF(const std::string& name = "");

	/// pre-existing random forest
	InputSlot< randomForest > randomForest;
	/// matrix containing features
	InputSlot< VigraArray2float > featureMatrix;

	///Matrix containing predicted labels
	OutputSlot<VigraArray2float > predictedLabelMatrix;

	/// Update object.
	virtual void execute();
};

#endif // _PREDICTRF_H_
