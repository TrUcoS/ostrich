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
/** \file LoadRF.cpp
 *  Implementation of parameter class LoadRF.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 09.04.2013
 */

#include <charon-core/ParameteredObject.hxx>
#include "LoadRF.h"

#include "vigra\hdf5impex.hxx"
#include "vigra\random_forest_hdf5_impex.hxx"

LoadRF::LoadRF(const std::string& name) :
		ParameteredObject(
			"LoadRF", name,
			"<h2>Loads random forest from location</h2><br>"
			"Loads random forest from location"
		)
{

	ParameteredObject::_addOutputSlot(
		randomForest, "randomForest",
		"Random forest that was loaded",
		"randomForest");

	ParameteredObject::_addParameter(
		location, "location",
		"Location from which random forest should be loaded",
		"string");
	ParameteredObject::_addParameter(
		fileName, "fileName",
		"Name of random forest stored in HDF5 file",
		"string");
}

void LoadRF::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// declare RandomForest object
	vigra::RandomForest<double> rf;
	// import random forest from HDF5 file
	vigra::rf_import_HDF5(rf, fileName(), location());
	// set output slot
	randomForest = rf;
	
}

// the following functions are needed
// for class LoadRF to work as a charon plugin.
extern "C" loadrf_DECLDIR ParameteredObject*
		create(const std::string& name, template_type) {
	return new LoadRF(name);
}

extern "C" loadrf_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" loadrf_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
