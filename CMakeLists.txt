cmake_minimum_required(VERSION 2.8.2)

project(bootstrap CXX)

# current version
set(bootstrap_VERSION_MAJOR 1)
set(bootstrap_VERSION_MINOR 0)
set(bootstrap_VERSION_PATCH 0)
set(bootstrap_VERSION 
	${bootstrap_VERSION_MAJOR}.${bootstrap_VERSION_MINOR}.${bootstrap_VERSION_PATCH})
	
# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/modules")

# include commonly used cmake modules
include(ExternalProject)

## Global Configuration Options

# Some automated systems need an xml report which requires a second run of the
# tests. To prevent these unnecessary runnings during normal builds the
# following option is set with a default of false.
option(ENABLE_TEST_REPORT
    "Enables the generation of a test report when running tests" false)

## Vendor libraries
set(VENDOR_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
set(vendor_libraries "")
set(vendor_args "")

option(BUILD_DEPENDENCIES
    "Forces the downloading and building of the dependencies from source" false
)

if(MSVC10 AND NOT BUILD_DEPENDENCIES)
	include(FetchPreBuiltDependencies)
else()
	include(BuildDependencies)
endif()

# Configure and build the SWGANH project
ExternalProject_Add(swganh
	DEPENDS ${vendor_libraries}
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/swganh
	CMAKE_ARGS 
		${vendor_args}
		-DENABLE_TEST_REPORT:BOOL=${ENABLE_TEST_REPORT}
	INSTALL_COMMAND ""
)
