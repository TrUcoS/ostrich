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
/** \file HDF5FeatureCheckpoint.h
 *  Declaration of the parameter class HDF5FeatureCheckpoint.
 *  \author <a href="mailto:p.gueth@stud.uni-heidelberg.de">
 *      Philipp Güth</a>
 *  \date 12.04.2013
 */

#ifndef _HDF5FEATURECHECKPOINT_H_
#define _HDF5FEATURECHECKPOINT_H_

#ifdef _MSC_VER
#ifdef hdf5featurecheckpoint_EXPORTS
/// Visual C++ specific code
#define hdf5featurecheckpoint_DECLDIR __declspec(dllexport)
#else
#define hdf5featurecheckpoint_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define hdf5featurecheckpoint_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>

typedef vigra::MultiArray<3, float> VigraArray3float;

/// Loads features from HDF5 file
/** If there are features in HDF5 file they should be loaded rather than the
 *  ones from the image.
 */
template <typename T>
class hdf5featurecheckpoint_DECLDIR HDF5FeatureCheckpoint :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	HDF5FeatureCheckpoint(const std::string& name = "");

	/// The feature matrix
	InputSlot< VigraArray3float > inputMatrix;

	/// The actual features that are supposed to be used
	OutputSlot< VigraArray3float > featureMatrix;

	/// Location of the HDF5 file
	Parameter< std::string > location;

	/// Name of dataset within HDF5 file
	Parameter< std::string > fileName;

	/// Features shall be recalculated?
	Parameter< bool > recalculate;

	/// Update object.
	virtual void execute();
};

#endif // _HDF5FEATURECHECKPOINT_H_
