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
/** \file LoadRF.h
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 09.04.2013
 *  Declaraion of parameter class LoadRF.
 */
#ifndef _LOADRF_H_
#define _LOADRF_H_

#ifdef _MSC_VER
#ifdef loadrf_EXPORTS
/// Visual C++ specific code
#define loadrf_DECLDIR __declspec(dllexport)
#else
#define loadrf_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define loadrf_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>

#include "vigra\random_forest.hxx"

typedef vigra::RandomForest<double> randomForest;

/// Loads random forest from location
/** Loads random forest from location
 */
class loadrf_DECLDIR LoadRF : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	LoadRF(const std::string& name = "");

	/// Random forest that was loaded
	OutputSlot< randomForest > randomForest;

	/// Location from which random forest should be loaded
	Parameter< std::string > location;

	Parameter< std::string > fileName;

	/// Update object.
	virtual void execute();
};

#endif /* _LOADRF_H_ */
