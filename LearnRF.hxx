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
/** \file LearnRF.hxx
 *  Implementation of the parameter class LearnRF.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 08.04.2013
 */

#ifndef _LEARNRF_HXX_
#define _LEARNRF_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "LearnRF.h"


#include "vigra/random_forest.hxx"
#include "vigra\random_forest_hdf5_impex.hxx"
#include <string>


template <typename T>
LearnRF<T>::LearnRF(const std::string& name) :
		TemplatedParameteredObject<T>(
			"LearnRF", name,
			"<h2>Performs learning of RandomForest</h2><br>"
			"Uses the feature matrix and label matrix to train a random "
			"forest"
		)
{
	ParameteredObject::_addInputSlot(
		featureMatrix, "featureMatrix",
		"contains features",
		"VigraArray2float");
	//********************
	ParameteredObject::_addInputSlot(
		labelMatrix, "labelMatrix",
		"contains labels",
		"VigraArray2float");
	//********************


	ParameteredObject::_addOutputSlot(
		randomForest, "randomForest",
		"The trained random forest",
		"randomForest");

	ParameteredObject::_addOutputSlot(
		oob, "oob",
		"Out of Bag error",
		"float");
	
	ParameteredObject::_addOutputSlot(
		varImp, "varImp",
		"Variance Importance",
		"VigraArray2float");


	ParameteredObject::_addParameter< int >(
		features_per_node, "features_per_node",
		"Features per Node",
		0, "int");
	ParameteredObject::_addParameter< int >(
		min_split_node_size, "min_split_node_size",
		"Number of examples required for a node to be split.",
		0, "int");
	ParameteredObject::_addParameter(
		predict_weighted, "predict_weighted",
		"weight each tree with number of samples in that node", false,
		"bool");
	ParameteredObject::_addParameter(
		sample_with_replacement, "sample_with_replacement",
		"sample from training population with or without replacement?", false,
		"bool");
	ParameteredObject::_addParameter(
		samples_per_tree_absolute, "samples_per_tree_absolute",
		"States whether the samples_per_tree parameter is supposed to"
		"be in absolute or relative terms", false,
		"bool");
	ParameteredObject::_addParameter(
		samples_per_tree, "samples_per_tree",
		"specify the fraction of the total number of samples used per tree"
		"for learning.", 1.0,
		"double");

	ParameteredObject::_addParameter(
		tree_count, "tree_count",
		"Number of trees", 32, 
		"int");
	ParameteredObject::_addParameter(
		use_stratification, "use_stratification",
		"specify stratification strategy",
		"RF_OptionTag");

}

		

template <typename T>
void LearnRF<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	
	// declare randomForest, out of bag error and variable importance
	vigra::RandomForest<double> rf;
	vigra::rf::visitors::OOB_Error oob_v;
	vigra::rf::visitors::VariableImportanceVisitor varimp;

	// Create RandomForestOptions-object and set options
	vigra::RandomForestOptions rfOptions = vigra::RandomForestOptions();
	rfOptions.features_per_node(features_per_node());
	rfOptions.min_split_node_size(min_split_node_size());
	if(predict_weighted == true){
		rfOptions.predict_weighted();
	}
	rfOptions.sample_with_replacement(sample_with_replacement());
		if(samples_per_tree_absolute() == true){
		rfOptions.samples_per_tree((int)samples_per_tree());
	}else{
		rfOptions.samples_per_tree((double)samples_per_tree());
	}
	rfOptions.tree_count(tree_count());

	if(use_stratification().compare("RF_EQUAL")){
		rfOptions.use_stratification(vigra::RF_EQUAL);
	}else if(use_stratification().compare("RF_PROPORTIONAL")){
		rfOptions.use_stratification(vigra::RF_PROPORTIONAL);
	}else if(use_stratification().compare("RF_EXTERNAL")){
		rfOptions.use_stratification(vigra::RF_EXTERNAL);
	}else if(use_stratification().compare("RF_NONE")){
		rfOptions.use_stratification(vigra::RF_NONE);
	}

	//if(!strcmp(str, "RF_EQUAL")){
	//	rfOptions.use_stratification("RF_EQUAL");
	//}
			
	// learn
	rf.learn(featureMatrix(), labelMatrix(), vigra::rf::visitors::create_visitor(oob_v, varimp));

	// set output parameters
	oob() = oob_v.oob_breiman;
	randomForest() = rf;
	varImp() = varimp.variable_importance_;
}

#endif /* _LEARNRF_HXX_ */
