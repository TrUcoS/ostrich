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
/** \file SaveRF.h
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 09.04.2013
 *  Declaraion of parameter class SaveRF.
 */
#ifndef _SAVERF_H_
#define _SAVERF_H_

#ifdef _MSC_VER
#ifdef saverf_EXPORTS
/// Visual C++ specific code
#define saverf_DECLDIR __declspec(dllexport)
#else
#define saverf_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define saverf_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include "vigra/multi_array.hxx"
#include "charon-utils/CImg.h"
#include "vigra\random_forest.hxx"
#include "vigra\hdf5impex.hxx"

typedef vigra::RandomForest<double> randomForest;
typedef vigra::MultiArray<2,float> vigraArray2;

/// Saves random forest in hdf5 file format
/** Saves random forest in hdf5 file format
 */
class saverf_DECLDIR SaveRF : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	SaveRF(const std::string& name = "");

	/// random forest
	InputSlot< randomForest > randomForest;

	/// Destination of file
	Parameter< std::string > location;

	/// Name of file
	Parameter< std::string > fileName;

	/// Update object.
	virtual void execute();
};

#endif /* _SAVERF_H_ */
