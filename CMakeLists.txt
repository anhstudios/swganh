cmake_minimum_required(VERSION 2.8.2)

project(bootstrap CXX)

# current version
set(swganh_VERSION_MAJOR 1)
set(swganh_VERSION_MINOR 0)
set(swganh_VERSION_PATCH 4)
set(swganh_VERSION 
	${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})
	
# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")

# include commonly used cmake modules
include(ExternalProject)

## Global Configuration Options

# Some automated systems need an xml report which requires a second run of the
# tests. To prevent these unnecessary runnings during normal builds the
# following option is set with a default of false.
option(ENABLE_TEST_REPORT
    "Enables the generation of a test report when running tests" false)

option(USE_SYSTEM_LIBRARIES
    "Uses system libraries instead of downloading them" false)
    
## Vendor libraries
set(vendor_libraries "")
set(vendor_args "")

if(ENABLE_TEST_REPORT)
enable_testing()
endif()

if(MSVC10 AND NOT DEFINED VENDOR_PREFIX AND NOT USE_SYSTEM_LIBRARIES)
    set(VENDOR_MD5 1d36278c9494a7bcf5f1da73fb4c73f0)
    
	include(FetchPreBuiltDependencies)
	
    # Configure and build the SWGANH project
    ExternalProject_Add(swganh
    	DEPENDS ${vendor_libraries}
    	DOWNLOAD_COMMAND ""
    	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
    	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/swganh
    	CMAKE_ARGS 
            ${vendor_args}
            -DVENDOR_PREFIX:PATH=${VENDOR_PREFIX}
            -Dswganh_VERSION_MAJOR=${swganh_VERSION_MAJOR}
            -Dswganh_VERSION_MINOR=${swganh_VERSION_MINOR}
            -Dswganh_VERSION_PATCH=${swganh_VERSION_PATCH}
            -DCMAKE_MODULE_PATH:PATH=${CMAKE_MODULE_PATH}
            -DUSE_SYSTEM_LIBRARIES:BOOL=${USE_SYSTEM_LIBRARIES}
            -DENABLE_TEST_REPORT:BOOL=${ENABLE_TEST_REPORT}
    	INSTALL_COMMAND ""
    )
else()
	add_subdirectory(src)
endif()
