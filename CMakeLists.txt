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
		-DBoost_INCLUDE_DIR:PATH=${Boost_INCLUDE_DIR}
		-DBoost_LIBRARY_DIRS:PATH=${Boost_LIBRARY_DIRS}
		-DGLM_INCLUDE_DIR:PATH=${GLM_INCLUDE_DIR}
		-DGLog_INCLUDE_DIR:PATH=${GLog_INCLUDE_DIR}
		-DGLog_LIBRARY_DIRS:PATH=${GLog_LIBRARY_DIRS}
		-DGMock_INCLUDE_DIR:PATH=${GMock_INCLUDE_DIR}
		-DGMock_LIBRARY_DIRS:PATH=${GMock_INCLUDE_DIR}
		-DGTest_INCLUDE_DIR:PATH=${GTest_INCLUDE_DIR}
		-DGTest_LIBRARY_DIRS:PATH=${GTest_INCLUDE_DIR}
		-DMysqlConnectorC_INCLUDE_DIR:PATH=${MysqlConnectorC_INCLUDE_DIR}
		-DMysqlConnectorC_LIBRARY_DIRS:PATH=${MysqlConnectorC_INCLUDE_DIR}
		-DMysqlConnectorCpp_INCLUDE_DIR:PATH=${MysqlConnectorCpp_INCLUDE_DIR}
		-DMysqlConnectorCpp_LIBRARY_DIRS:PATH=${MysqlConnectorCpp_INCLUDE_DIR}		
		-DMysqlConnectorC_INCLUDE_DIR:PATH=${MysqlConnectorC_INCLUDE_DIR}
		-DMysqlConnectorC_LIBRARY_DIRS:PATH=${MysqlConnectorC_INCLUDE_DIR}
		-DTBB_INCLUDE_DIR:PATH=${TBB_INCLUDE_DIR}
		-DTBB_LIBRARY_DIRS:PATH=${TBB_LIBRARY_DIRS}
		-DZLib_INCLUDE_DIR:PATH=${ZLib_INCLUDE_DIR}
		-DZLib_LIBRARY_DIRS:PATH=${ZLib_LIBRARY_DIRS}
	INSTALL_COMMAND ""
)
