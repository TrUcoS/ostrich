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
/** \file LearnRF.h
 *  Declaration of the parameter class LearnRF.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 08.04.2013
 */

#ifndef _LEARNRF_H_
#define _LEARNRF_H_

#ifdef _MSC_VER
#ifdef learnrf_EXPORTS
/// Visual C++ specific code
#define learnrf_DECLDIR __declspec(dllexport)
#else
#define learnrf_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define learnrf_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>

#include "vigra\random_forest.hxx"
#include "vigra\random_forest\rf_common.hxx"

typedef vigra::RandomForest<double> randomForest;
typedef vigra::MultiArray<2,float> VigraArray2float;
typedef vigra::MultiArrayShape<2>::type shape_t;



/// Performs learning of RandomForest
/** Uses the feature matrix and label matrix to train a random forest
 */
template <typename T>
class learnrf_DECLDIR LearnRF :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	LearnRF(const std::string& name = "");

	/// contains features
	InputSlot< VigraArray2float > featureMatrix;
	/// contains labels
	InputSlot< VigraArray2float > labelMatrix;

	/// The trained random forest
	OutputSlot< randomForest > randomForest;
	/// Out of Bag error
	OutputSlot< float > oob;
	OutputSlot<VigraArray2float >varImp;

	/// Features per Node
	Parameter< int > features_per_node;
	/// Number of examples required for a node to be split.
	Parameter< int > min_split_node_size;
	/// weight each tree with number of samples in that node
	Parameter< bool > predict_weighted;
	/// sample from training population with or without replacement?
	Parameter< bool > sample_with_replacement;
	// States whether the samples_per_tree parameter is supposed to be in absolute or relative terms
	Parameter< bool > samples_per_tree_absolute;
	/// specify the fraction of the total number of samples used per tree for
	/// learning.
	Parameter< double > samples_per_tree;
	/// Number of trees
	Parameter< int > tree_count;
	/// specify stratification strategy
	Parameter< std::string > use_stratification;

	/// Update object.
	virtual void execute();
};

#endif // _LEARNRF_H_
