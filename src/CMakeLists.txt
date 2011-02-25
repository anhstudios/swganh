cmake_minimum_required(VERSION 2.8.2)

project(swganh CXX)

set(swganh_VERSION_MAJOR 1)
set(swganh_VERSION_MINOR 0)
set(swganh_VERSION_PATCH 0)
set(swganh_VERSION 
	${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
## compiler checks

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG(-Wno-invalid-offsetof HAS_NO_INVALID_OFFSETOF)
if(HAS_NO_INVALID_OFFSETOF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
endif()

CHECK_CXX_COMPILER_FLAG(-Wno-write-strings HAS_NO_WRITE_STRINGS)
if(HAS_NO_WRITE_STRINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
endif()

if(NOT MSVC EQUAL "" AND MSVC_VERSION LESS 1600)
	message(FATAL_ERROR "Visual Studio 2010 or newer is required ")
else()	
	# any custom flags that need set for VS compilers only should be set here
endif()

## find/configure vendor libraries
set(ENV{MYSQL_DIR} ${MysqlConnectorC_ROOT})

FIND_PACKAGE(PythonLibs REQUIRED)
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.45.0 COMPONENTS date_time regex thread system REQUIRED)
SET(Boost_USE_STATIC_LIBS OFF)
FIND_PACKAGE(Boost 1.45.0 COMPONENTS python REQUIRED)

find_package(Glm REQUIRED)
find_package(GLog REQUIRED)
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
find_package(MysqlConnectorC REQUIRED)
find_package(MysqlConnectorCpp REQUIRED)
find_package(TBB REQUIRED)
find_package(ZLib REQUIRED)


enable_testing()

## project libraries

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(anh)

## project executables
