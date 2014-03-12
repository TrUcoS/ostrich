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
/** \file Images2Classifier.h
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 28.03.2013
 *  Declaration of parameter class Images2Classifier.
 */
#ifndef _IMAGES2CLASSIFIER_H_
#define _IMAGES2CLASSIFIER_H_

#ifdef _MSC_VER
#ifdef images2classifier_EXPORTS
/// Visual C++ specific code
#define images2classifier_DECLDIR __declspec(dllexport)
#else
#define images2classifier_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define images2classifier_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>

/// Filters input image for labelled feature points
/** Input:
 *  - Training image
 *  - Label Image
 *  
 *  Output:
 *  - unfiltered training image
 *  - filtered training data as vector
 *	- labels as vector
 */

typedef vigra::MultiArray<3, float> VigraArray3float;
typedef vigra::MultiArray<2, float> VigraArray2float;

class images2classifier_DECLDIR Images2Classifier : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	Images2Classifier(const std::string& name = "");

	/// Feature Images as 3-dimensional vigra array
	InputSlot< VigraArray3float > featureImages;
	/// Label Image as 3-dimensional vigra array
	InputSlot< VigraArray3float > labelImage;

	/// All feature images put together in feature matrix
	OutputSlot< VigraArray2float > unfilteredFeatureMatrix;
	/// All pixels known to have a label
	OutputSlot< VigraArray2float > filteredFeatureMatrix;
	/// All labels
	OutputSlot< VigraArray2float > filteredLabelMatrix;

	/// Value that a pixel without label has. If labelImage is RGB, then
	/// either 3 vector should be specified or value is replicated 3 times
	ParameterList< float > noLabelValue;

	/// Update object.
	virtual void execute();
};

#endif /* _IMAGES2CLASSIFIER_H_ */
