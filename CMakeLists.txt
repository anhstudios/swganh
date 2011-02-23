cmake_minimum_required(VERSION 2.8.2)

project(swganh CXX)

# current version
set(swganh_VERSION_MAJOR 1)
set(swganh_VERSION_MINOR 0)
set(swganh_VERSION_PATCH 0)
set(swganh_VERSION 
	${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})
	
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


## 3rd party libraries

set(VENDOR_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/vendor")

# Build and/or configure Boost
option(swganh_USE_SYSTEM_BOOST
    "If false, swganh will download and build it's own copy of Boost" FALSE)

if(NOT swganh_USE_SYSTEM_BOOST)
    include(BuildExternalBoost)
else()
    
endif()

# Build and/or configure TBB
option(swganh_USE_SYSTEM_TBB
    "If false, swganh will download and build it's own copy of TBB" FALSE)
    	
if(NOT swganh_USE_SYSTEM_TBB)
	include(BuildExternalTBB)
else()
	
endif()

# Build and/or configure GLog
option(swganh_USE_SYSTEM_GLog
    "If false, swganh will download and build it's own copy of GLog" FALSE)
    	
if(NOT swganh_USE_SYSTEM_GLog)
	include(BuildExternalGLog)
else()
	
endif()

# Build and/or configure ZLib
option(swganh_USE_SYSTEM_ZLib
    "If false, swganh will download and build it's own copy of ZLib" FALSE)
    	
if(NOT swganh_USE_SYSTEM_ZLib)
	include(BuildExternalZLib)
else()
	
endif()

# Build and/or configure GTest
option(swganh_USE_SYSTEM_GTest
    "If false, swganh will download and build it's own copy of GTest" FALSE)
    	
if(NOT swganh_USE_SYSTEM_GTest)
	include(BuildExternalGTest)
else()
	
endif()

# Build and/or configure GMock
option(swganh_USE_SYSTEM_GMock
    "If false, swganh will download and build it's own copy of GMock" FALSE)
    	
if(NOT swganh_USE_SYSTEM_GMock)
	include(BuildExternalGMock)
else()
	
endif()

# Build and/or configure Mysql Connector/C
option(swganh_USE_SYSTEM_MysqlConnectorC
    "If false, swganh will download and build it's own copy of MysqlConnectorC" FALSE)
    	
if(NOT swganh_USE_SYSTEM_MysqlConnectorC)
	include(BuildExternalMysqlConnectorC)
else()
	
endif()

# Build and/or configure Mysql Connector/Cpp
option(swganh_USE_SYSTEM_MysqlConnectorCpp
    "If false, swganh will download and build it's own copy of MysqlConnectorCpp" FALSE)
    	
if(NOT swganh_USE_SYSTEM_MysqlConnectorCpp)
	include(BuildExternalMysqlConnectorCpp)
else()
	
endif()
# Add boost and tbb to the include and link directories as everything needs it.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${Boost_INCLUDE_DIR} ${GLog_INCLUDE_DIR} ${GLM_INCLUDE_DIR}  ${TBB_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${TBB_LIBRARY_DIRS})
MESSAGE("GLOG," ${GLog_INCLUDE_DIR})
MESSAGE("GTEST," ${GTest_INCLUDE_DIR})
MESSAGE("GMOCK," ${GMock_INCLUDE_DIR})
MESSAGE("MSQL includes" ${MYSQL_INCLUDE_DIR},${MysqlConnectorCpp_INCLUDE_DIR})
MESSAGE("GLM include " ${GLM_INCLUDE_DIR})
MESSAGE("Boost includes " ${Boost_INCLUDE_DIR})
# Enable testing and add gtest to the include directories.
ENABLE_TESTING()

ADD_SUBDIRECTORY(src)

