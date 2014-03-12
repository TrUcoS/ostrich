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
/** \file HDF5FeatureCheckpoint.hxx
 *  Implementation of the parameter class HDF5FeatureCheckpoint.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 12.04.2013
 */

#ifndef _HDF5FEATURECHECKPOINT_HXX_
#define _HDF5FEATURECHECKPOINT_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "HDF5FeatureCheckpoint.h"

#include "vigra\hdf5impex.hxx"

template <typename T>
HDF5FeatureCheckpoint<T>::HDF5FeatureCheckpoint(const std::string& name) :
		TemplatedParameteredObject<T>(
			"HDF5FeatureCheckpoint", name,
			"<h2>Loads features from HDF5 file</h2><br>"
			"If there are features in HDF5 file they should be loaded rather "
			"than the ones from the image."
		), inputMatrix(true, false)
{
	ParameteredObject::_addInputSlot(
		inputMatrix, "inputMatrix",
		"The feature matrix",
		"VigraArray3float"
		);

	ParameteredObject::_addOutputSlot(
		featureMatrix, "featureMatrix",
		"The actual features that are supposed to be used",
		"vigraArray3float");

	// may have to set default (empty) strings for location and fileName
	ParameteredObject::_addParameter(
		location, "location",
		"Location of the HDF5 file",
		"string");
	
	ParameteredObject::_addParameter(
		fileName, "fileName",
		"Name of dataset within HDF5 file",
		"string");

		ParameteredObject::_addParameter(
		recalculate, "recalculate",
		"State whether features should be calculated or taken from HDF5 file",
		false,
		"bool");

}

template <typename T>
void HDF5FeatureCheckpoint<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();


	if(recalculate() == true){
		if(location().empty() || fileName().empty()){
			throw std::runtime_error("Need location and file name to save input matrix in HDF5 file.");
		}else{
			// save input matrix in HDF5 file
			vigra::HDF5File file(location, vigra::HDF5File::OpenMode::Open);
			file.write("featureMatrix", inputMatrix());
			//writeHDF5(location(), fileName(), inputMatrix());
		}
		// connect input slot with output slot
		featureMatrix() = inputMatrix();
	}else if(location().empty()){
		throw std::runtime_error("There is neither an input matrix nor a HDF5 file available. Need input.");
	}else if(fileName().empty()){
		throw std::runtime_error("Can't recalculate, no file name is set.");
	}else{

		// open HDF5 file and read feature matrix
		vigra::HDF5File file(location(), vigra::HDF5File::OpenMode::Open);
		vigra::MultiArray<3,float> readArray;
		file.readAndResize(fileName(), readArray);

		// set output slot
		featureMatrix() = readArray;
	} 
}

#endif /* _HDF5FEATURECHECKPOINT_HXX_ */
