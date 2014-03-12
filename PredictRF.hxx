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
/** \file PredictRF.hxx
 *  Implementation of the parameter class PredictRF.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 10.04.2013
 */

#ifndef _PREDICTRF_HXX_
#define _PREDICTRF_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "PredictRF.h"

#include "vigra\random_forest.hxx"


template <typename T>
PredictRF<T>::PredictRF(const std::string& name) :
		TemplatedParameteredObject<T>(
			"PredictRF", name,
			"<h2>Predicts labels for features given an existing random "
			"Forest</h2><br>"
			"Predicts labels for features given an existing random Forest"
		)
{
	ParameteredObject::_addInputSlot(
		randomForest, "randomForest",
		"pre-existing random forest",
		"randomForest");
	ParameteredObject::_addInputSlot(
		featureMatrix, "featureMatrix",
		"matrix containing features",
		"VigraArray2float");

		ParameteredObject::_addOutputSlot(
		predictedLabelMatrix, "predictedLabelMatrix",
		"The prediction of labels",
		"VigraArray2float");

}

template <typename T>
void PredictRF<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// was 2, float, multiarrayshape<2>
	vigra::MultiArray<2,float> featureArray(vigra::MultiArrayShape<2>::type(featureMatrix().size(),1));

	vigra::MultiArray<2,float> rf_predict(shape_t(featureMatrix.size(),1),1);
}

#endif /* _PREDICTRF_HXX_ */
