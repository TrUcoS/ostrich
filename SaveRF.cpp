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
/** \file SaveRF.cpp
 *  Implementation of parameter class SaveRF.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 09.04.2013
 */

#include <charon-core/ParameteredObject.hxx>
#include "SaveRF.h"

#include "vigra\hdf5impex.hxx"

#include "vigra\random_forest.hxx"
#include "vigra\random_forest_hdf5_impex.hxx"

SaveRF::SaveRF(const std::string& name) :
		ParameteredObject(
			"SaveRF", name,
			"<h2>Saves random forest in hdf5 file format</h2><br>"
			"Saves random forest in hdf5 file format"
		),
		randomForest(true,false)
{

	ParameteredObject::_addInputSlot(
		randomForest, "randomForest",
		"random forest",
		"randomForest");
		
	ParameteredObject::_addParameter(
		fileName, "fileName",
		"Name of file",
		"string");
	
	ParameteredObject::_addParameter(
		location, "location",
		"Destination of file",
		"string");

}

void SaveRF::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// Open HDF5-File, will be created if necessary
	
	vigra::RandomForest<double> rafo = randomForest();
	vigra::rf_export_HDF5(rafo, "RandomForest", location());
	//vigra::RandomForest<float> rf = randomForest();

	//vigra::HDF5File data(location, vigra::HDF5File::New);

	//data.write("RandomForest", randomForest());


		
}

// the following functions are needed
// for class SaveRF to work as a charon plugin.
extern "C" saverf_DECLDIR ParameteredObject*
		create(const std::string& name, template_type) {
	return new SaveRF(name);
}

extern "C" saverf_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" saverf_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
