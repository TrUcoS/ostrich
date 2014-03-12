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
/** \file Images2Classifier.cpp
 *  Implementation of parameter class Images2Classifier.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 28.03.2013
 */
#include "vigra\rgbvalue.hxx"
#include "vigra\linear_algebra.hxx"
#include <charon-core/ParameteredObject.hxx>
#include "Images2Classifier.h"


using namespace vigra;
Images2Classifier::Images2Classifier(const std::string& name) :
		ParameteredObject(
			"Images2Classifier", name,
			"<h2>Filters input image for labelled feature points</h2><br>"
			"Input:<br>"
			"- Training image<br>"
			"- Label Image<br>"
			"<br>"
			"Output:<br>"
			"- unfiltered training image<br>"
			"- filtered training data as vector<br>- labels as vector"
		),
		featureImages(false,true),
		noLabelValue("0")
{

	ParameteredObject::_addInputSlot(
		featureImages, "featureImages",
		"Feature Images as 3-dimensional vigra array",
		"VigraArray3float");
	ParameteredObject::_addInputSlot(
		labelImage, "labelImage",
		"Label Image as 3-dimensional vigra array",
		"VigraArray3float");

	ParameteredObject::_addOutputSlot(
		unfilteredFeatureMatrix, "unfilteredFeatureMatrix",
		"All feature images put together in feature matrix",
		"VigraArray2float");
	ParameteredObject::_addOutputSlot(
		filteredFeatureMatrix, "filteredFeatureMatrix",
		"All pixels known to have a label",
		"VigraArray2float");
	ParameteredObject::_addOutputSlot(
		filteredLabelMatrix, "filteredLabelMatrix",
		"All labels",
		"VigraArray2float");

	ParameteredObject::_addParameter(
		noLabelValue, "noLabelValue"
		"Value that a pixel without label has. If labelImage is RGB, then"
		"either 3 vector should be specified or value is replicated 3 times",
		"float");

}

void Images2Classifier::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// define stuff for later use
	bool grayscale = labelImage().shape(2)==1;
	int feature_sample_size = featureImages[0].size();
	int count_ = 0;

	// determine the number of feature channels
	int no_feature_channels=0;
	for(int i=0;i<featureImages.size();i++){
		no_feature_channels+=featureImages[i].shape(2);
	}



//	std::set<MultiArray<1, float>> label_set;

	// Determine number of different label types
	// since there is a problem with using MultiArrays in Maps ( "<" operator not defined for MultiArrays)
	// the MultiArrays will be stored in a vector which in turn is stored in the map
	std::set<std::vector<float>> label_set;
	for(int i = 0; i<labelImage().shape(0); i++){
		for(int j = 0; j<labelImage().shape(1);j++){
			vigra::MultiArray<1,float> tmp(labelImage().bindAt(0,i).bindAt(1,j));
			std::vector<float> label(tmp.begin(), tmp.end());//++++++++++++++++++++++++++++++++++++++++++++++++++++
			label_set.insert(label);
			//label_set.insert(MultiArray<1, float>(labelImage().bindAt(0,i).bindAt(1,j)));******************************************************
		}
	}
	
	// transform label set into label vector
	//std::set<MultiArray<1, float>>::iterator it;*********************************************************************************
	std::set<std::vector<float>>::iterator it;

	//std::vector<MultiArray<1,float>> labels;*********************************************************************************
	std::vector<std::vector<float>> labels;
	for(it = label_set.begin(); it != label_set.end(); ++it){
		labels.push_back(*it);
	}

	// check number of channels of labelImage 
	//labelImage().shape(2) > 1 

	// Reshape noLabel input value so it is suitable for further use 
	// simultaneously the noLabel-label is set to be the first element of the label vector
	MultiArray<2, float> noLabel;
	noLabel.reshape(MultiArrayShape<2>::type(1,labelImage().shape(2)));
	for(int i = 0; i < labelImage().shape(2);i++){
		noLabel(0,i) = 0;//noLabelValue[i];
	} // end - reshape labels
	
	// set label "unlabelled",  (0,0,0), as first element of vector
	//std::vector<MultiArray<1, float>>::iterator it2;******************************************************
	std::vector<std::vector<float>>::iterator it2;
	for(it2 = labels.begin(); it2 != labels.end(); ++it2){
		if(labelImage().shape(2) > 1){
			if(noLabel(0) == (*it2).at(0) && noLabel(1) == (*it2).at(1) && noLabel(2) == (*it2).at(2)){
				labels.insert(labels.begin(), *it2);
				labels.erase(it2);
				break;
			}
		}else{
			if(noLabel(0) == (*it2).at(0)){
				labels.insert(labels.begin(), *it2);
				labels.erase(it2);
				break;
			}
		}
	} //end - set label as first element of vector

	// if labelImage is RGB - convert RGB image to 1-channel image
	MultiArray<3, float> final_label_image;
	final_label_image.reshape(MultiArrayShape<3>::type(1,labelImage().shape(0),labelImage().shape(1)));
	if(grayscale == false){
		// hier vektor nehmen
		//*it - .begin gibt index
		int label_type = 0;
		for(int i = 0; i<labelImage().shape(0); i++){
			for(int j = 0; j<labelImage().shape(1);j++){
				label_type = 0;
				for(it = label_set.begin(); it != label_set.end(); ++it){
					// The next two lines use the same workaround as above
					vigra::MultiArray<1,float> tmp(labelImage().bindAt(0,i).bindAt(1,j));
					std::vector<float> tmp_v(tmp.begin(), tmp.end());
					// vorher in if statt tmp_v: labelImage().bindAt(0,i).bindAt(1,j)***************************
					if(labels.at(label_type) == tmp_v){
						final_label_image(i,j,1)=label_type;
						break;
					}
					label_type++;
				}
			}
		}
	} // end - convert RGB image to 1-channel image 

	// Determine number of labelled pixels
	int no_labelled_pixels = 0;
	for(int i = 0; i<labelImage().shape(0); i++){
		for(int j = 0; j<labelImage().shape(1);j++){
			// iterator starts at begin+1 since 1st element in vector means "unlabelled" and does not count
			for(it2 = labels.begin()+1; it2 != labels.end(); ++it2){
				// The next two lines use the same workaround as above
				vigra::MultiArray<1,float> tmp(labelImage().bindAt(0,i).bindAt(1,j));
				std::vector<float> tmp_v(tmp.begin(), tmp.end());
				if(tmp_v == *it2){
					no_labelled_pixels++;
				}
			}
		}
	} // end - count no of labelled pixels
	
	// create unfilteredFeatureMatrix
	MultiArray<2, float> unfilteredFeatures;
	unfilteredFeatures.reshape(MultiArrayShape<2>::type(feature_sample_size, no_feature_channels));
	
	for(int i=0;i<featureImages.size();i++){
		MultiArrayView<2,float> currentFeatureView(MultiArrayShape<2>::type(feature_sample_size, featureImages[i].shape(2)),featureImages[i].data());
		MultiArrayShape<2>::type offset(0,count_);
		count_+=featureImages[i].shape(2);
			unfilteredFeatures.subarray(offset,offset+currentFeatureView.shape()) = currentFeatureView;
	}
	// set output slot for unfiltered features
	unfilteredFeatureMatrix = unfilteredFeatures;

	// labelMatrixView including unlabelled pixels
	MultiArrayView<2,float> labelMatrixView(MultiArrayShape<2>::type(feature_sample_size, labelImage().shape(2)),labelImage().data());
	
	// find number of different labels and each respective label count
	MultiArray<2,float> labelArray(MultiArrayShape<2>::type(feature_sample_size,1));
	//std::map<MultiArray<2, float>, int> label_count, label_value;
	std::map<std::vector<float>, int> label_count, label_value;
	std::vector<float> noLabel2(noLabel.begin(), noLabel.end());
	

	label_count[noLabel2] = 0;
	label_value[noLabel2] = 0;
	int current_label_value = 1;
	for(int i=0; i<feature_sample_size;i++){
		// The next two lines use the same workaround as above
		vigra::MultiArray<2,float> currentRow = vigra::rowVector(labelMatrixView,i);
		std::vector<float> currentRow_(currentRow.begin(), currentRow.end());
		if(label_count.find(currentRow_)!=label_count.end()){
			label_count[currentRow_]+=1;
			labelArray(i,0)= label_value[currentRow_];
		}else{
			label_count[currentRow_]=0;
			label_value[currentRow_]=current_label_value+1;
			current_label_value+=1;
		}
	} // end - find number of different labels

	// put labels in label vector and corresponding features in feature vector
	
	MultiArray<2,float> featureArray;
	featureArray.reshape(MultiArrayShape<2>::type(3,no_labelled_pixels));

	// create filtered featureMatrix - only labelled pixels
	MultiArray<2, float> filteredFeatures;
	MultiArray<2, float> filteredLabels;
	filteredFeatures.reshape(MultiArrayShape<2>::type(no_labelled_pixels, no_feature_channels));
	filteredLabels.reshape(MultiArrayShape<2>::type(no_labelled_pixels, 1));

	count_ = 0;
	for(int i=0;i<labelArray.size();i++){
		if(labelArray(i,0)!= 0){
			rowVector(filteredFeatures, count_) = rowVector(filteredFeatures,i);
			filteredLabels(count_,0) = labelArray(i,0);
		}
	} // end - create filtered featureMatrix

	// set output slots for filtered features and filtered labels
	filteredFeatureMatrix = filteredFeatures;
	filteredLabelMatrix = filteredLabels;

} // end - Images2Classifier::execute() 

// the following functions are needed
// for class Images2Classifier to work as a charon plugin.
extern "C" images2classifier_DECLDIR ParameteredObject*
		create(const std::string& name, template_type) {
	return new Images2Classifier(name);
}

extern "C" images2classifier_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" images2classifier_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
