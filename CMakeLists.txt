# CMake project file for ostrichftw.
# This file has been set up by the charon TemplateGenerator.
# It may be modified to be adapted to your needs.
#
# The last line (add additional modules here) serves as
# mark where the TemplateGenerator inserts new module
# code if requested, so please do not edit this line.
# new module code is inserted just above this mark.
PROJECT(ostrichftw)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Template generator options.
# This is not changed later, i.e. if new modules are
# added. If the new modules need CImg/Vigra and this
# flag was disabled on first setup, please activate
# it by hand.
SET(USE_CIMG  0)
SET(USE_VIGRA 1)
SET(CMAKE_MODULE_PATH CMAKE_MODULE_PATH 
${CMAKE_CURRENT_SOURCE_DIR}/config)



# required packages
FIND_PACKAGE(charon-core REQUIRED HINTS $ENV{CHARON_DIR})
INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
IF(USE_CIMG)
	FIND_PACKAGE(charon-utils REQUIRED HINTS $ENV{CHARON_DIR})
	INCLUDE(CImgConfig)
	INCLUDE_DIRECTORIES(${CIMG_INCLUDE_DIRS})
ENDIF(USE_CIMG)
IF(USE_VIGRA)
	FIND_PACKAGE(Vigra 1.7.1 REQUIRED)
	INCLUDE_DIRECTORIES(${Vigra_INCLUDE_DIRS})
	SET(Vigra_LIBRARIES vigraimpex)
ENDIF(USE_VIGRA)

INCLUDE(InitFlags)

# add additional modules here
#---- HDF5 -------
INCLUDE(VigraFindPackage)
VIGRA_FIND_PACKAGE( HDF5)
INCLUDE_DIRECTORIES( ${HDF5_INCLUDE_DIR} )




# module generation
ADD_CHARON_PLUGIN(	hdf5featurecheckpoint 
					HDF5FeatureCheckpoint.cpp 
					HDF5FeatureCheckpoint.h 
					HDF5FeatureCheckpoint.hxx
					LINK_LIBRARIES
					${CIMG_LINK_LIBRARIES}
					${HDF5_CORE_LIBRARY} 
					${HDF5_HL_LIBRARY} 
					${HDF5_Z_LIBRARY}
					${HDF5_SZ_LIBRARY}
					)					


#---- classifieroutput2image -------
ADD_CHARON_PLUGIN(	classifieroutput2image
					ClassifierOutput2Image.cpp
	ClassifierOutput2Image.h
	ClassifierOutput2Image.hxx
					LINK_LIBRARIES
					${CIMG_LINK_LIBRARIES}
					${HDF5_CORE_LIBRARY} 
					${HDF5_HL_LIBRARY} 
					${HDF5_Z_LIBRARY}
					${HDF5_SZ_LIBRARY}
					)	



#---- images2classifier -------
ADD_LIBRARY(images2classifier SHARED
	Images2Classifier.cpp
	Images2Classifier.h
)
TARGET_LINK_LIBRARIES(images2classifier
	charon-core
	${CIMG_LINK_LIBRARIES}
)


#---- learnRF -------
ADD_LIBRARY(learnrf SHARED
	LearnRF.cpp
	LearnRF.h
	LearnRF.hxx
)
TARGET_LINK_LIBRARIES(learnrf
	charon-core
#	${CIMG_LINK_LIBRARIES}
	${HDF5_CORE_LIBRARY} 
	${HDF5_HL_LIBRARY} 
	${HDF5_Z_LIBRARY}
	${HDF5_SZ_LIBRARY}
)

#---- loadRF -------
#ADD_LIBRARY(loadrf SHARED
	#LoadRF.cpp
	#LoadRF.h
#)
#TARGET_LINK_LIBRARIES(loadrf
	#charon-core
#	${CIMG_LINK_LIBRARIES}
	#${HDF5_CORE_LIBRARY} 
	#${HDF5_HL_LIBRARY} 
	#${HDF5_Z_LIBRARY}
	#${HDF5_SZ_LIBRARY}
	#${Vigra_LIBRARIES}
#)

#---- predictRF -------
ADD_LIBRARY(predictrf SHARED
	PredictRF.cpp
	PredictRF.h
	PredictRF.hxx
)
TARGET_LINK_LIBRARIES(predictrf
	charon-core
	${CIMG_LINK_LIBRARIES}
)

#---- saveRF -------
#ADD_LIBRARY(saverf SHARED
#	SaveRF.cpp
#	SaveRF.h
#)
#TARGET_LINK_LIBRARIES(saverf
#	charon-core
#	${CIMG_LINK_LIBRARIES}
#	${HDF5_CORE_LIBRARY} 
#	${HDF5_HL_LIBRARY} 
#	${HDF5_Z_LIBRARY}
#	${HDF5_SZ_LIBRARY}
#	${Vigra_LIBRARIES}
#)

