cmake_minimum_required(VERSION 2.8.2)

project(bootstrap CXX)

# current version
set(bootstrap_VERSION_MAJOR 1)
set(bootstrap_VERSION_MINOR 0)
set(bootstrap_VERSION_PATCH 0)
set(bootstrap_VERSION 
	${bootstrap_VERSION_MAJOR}.${bootstrap_VERSION_MINOR}.${bootstrap_VERSION_PATCH})
	
# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# include commonly used cmake modules
include(ExternalProject)

## Global Configuration Options

# Some automated systems need an xml report which requires a second run of the
# tests. To prevent these unnecessary runnings during normal builds the
# following option is set with a default of false.
OPTION(ENABLE_TEST_REPORT
    "Enables the generation of a test report when running tests" FALSE)


## Vendor libraries
set(VENDOR_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/vendor")
set(vendor_libraries "")

# macro for locating or building vendor libraries
macro(bootstrap_add_vendor name)
	if(NOT bootstrap_USE_SYSTEM_${name})
		include(BuildExternal${name})
		list(APPEND vendor_libraries ${name})
	endif()	
endmacro()

# Build and/or configure Boost
option(bootstrap_USE_SYSTEM_Boost
    "If false, swganh will download and build it's own copy of Boost" FALSE)
bootstrap_add_vendor(Boost)
bootstrap_add_vendor(BoostShared)

# Build and/or configure GLM
option(bootstrap_USE_SYSTEM_Glm
    "If false, swganh will download and build it's own copy of Glm" FALSE)
bootstrap_add_vendor(Glm)
 	
# Build and/or configure GLog
option(bootstrap_USE_SYSTEM_GLog
    "If false, swganh will download and build it's own copy of GLog" FALSE)
bootstrap_add_vendor(GLog)

# Build and/or configure GTest
option(bootstrap_USE_SYSTEM_GTest
    "If false, swganh will download and build it's own copy of GTest" FALSE)
bootstrap_add_vendor(GTest)

# Build and/or configure GMock
option(bootstrap_USE_SYSTEM_GMock
    "If false, swganh will download and build it's own copy of GMock" FALSE)
bootstrap_add_vendor(GMock)

# Build and/or configure Mysql Connector/C
option(bootstrap_USE_SYSTEM_MysqlConnectorC
    "If false, swganh will download and build it's own copy of MysqlConnectorC" FALSE)
bootstrap_add_vendor(MysqlConnectorC)

# Build and/or configure Mysql Connector/Cpp
option(bootstrap_USE_SYSTEM_MysqlConnectorCpp
    "If false, swganh will download and build it's own copy of MysqlConnectorCpp" FALSE)

bootstrap_add_vendor(MysqlConnectorCpp)

# Build and/or configure TBB
option(bootstrap_USE_SYSTEM_TBB
    "If false, swganh will download and build it's own copy of TBB" FALSE)
bootstrap_add_vendor(TBB)

# Build and/or configure ZLib
option(bootstrap_USE_SYSTEM_ZLib
    "If false, swganh will download and build it's own copy of ZLib" FALSE)
bootstrap_add_vendor(ZLib)

# Configure and build the SWGANH project
ExternalProject_Add(swganh
	DEPENDS ${vendor_libraries}
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/swganh
	CMAKE_ARGS 
		-DBOOST_INCLUDEDIR:PATH=${Boost_INCLUDEDIR}
        -DBOOST_LIBRARYDIR:PATH=${Boost_LIBRARYDIR}
        -DBoost_USE_STATIC_LIBS:BOOL=${Boost_USE_STATIC_LIBS}
        -DBoost_USE_MULTITHREADED:BOOL=${Boost_USE_MULTITHREADED}
		-DGlm_INCLUDEDIR:PATH=${Glm_INCLUDEDIR}
		-DGLog_INCLUDEDIR:PATH=${GLog_INCLUDEDIR}
		-DGLog_LIBRARYDIR:PATH=${GLog_LIBRARYDIR}
		-DGTest_INCLUDEDIR:PATH=${GTest_INCLUDEDIR}
		-DGTest_LIBRARYDIR:PATH=${GTest_LIBRARYDIR}
		-DGMock_INCLUDEDIR:PATH=${GMock_INCLUDEDIR}
		-DGMock_LIBRARYDIR:PATH=${GMock_LIBRARYDIR}
		-DPYTHON_INCLUDE_PATH=${PYTHON_INCLUDE_DIR}
		-DPYTHON_LIBRARIES=${PYTHON_LIBRARY}
		-DPYTHON_DEBUG_LIBRARIES=${PYTHON_DEBUG_LIBRARY}
		-DMysqlConnectorC_ROOT:PATH=${MysqlConnectorC_ROOT}
		-DMysqlConnectorCpp_ROOT:PATH=${MysqlConnectorCpp_ROOT}
		-DTBB_INSTALL_DIR:PATH=${TBB_ROOT}
		-DZLib_INCLUDEDIR:PATH=${ZLib_INCLUDEDIR}
		-DZLib_LIBRARYDIR:PATH=${ZLib_LIBRARYDIR}
	INSTALL_COMMAND ""
)
