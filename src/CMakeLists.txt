cmake_minimum_required(VERSION 2.8.2)

project(swganh CXX)

# current version
set(swganh_VERSION_MAJOR 1)
set(swganh_VERSION_MINOR 0)
set(swganh_VERSION_PATCH 0)
set(swganh_VERSION 
	${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})

# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/Boost")
set(Glm_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/Glm")
set(GLog_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/GLog")
set(GTest_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/GTest")
set(GMock_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/GMock")
set(MYSQL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/MysqlConnectorC-install/lib/mysql")
set(MYSQL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/MysqlConnectorC-install/include/mysql")
set(MysqlConnectorCpp_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/MysqlConnectorCpp")
set(TBB_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/TBB")
set(ZLib_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../build/vendor/src/ZLib")

# Use the static/multithreaded libraries.
set(Boost_NO_SYSTEM_PATHS ON)
SET(Boost_USE_MULTITHREADED ON)
set(Boost_COMPILER "-vc")

FIND_PACKAGE(Boost 1.45.0 COMPONENTS date_time regex thread system REQUIRED)
FIND_PACKAGE(Glm REQUIRED)
FIND_PACKAGE(GLog REQUIRED)
FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(GMock REQUIRED)
FIND_PACKAGE(MysqlConnectorC REQUIRED)
FIND_PACKAGE(MysqlConnectorCpp REQUIRED)
FIND_PACKAGE(TBB REQUIRED)
FIND_PACKAGE(ZLib REQUIRED)

# Add boost and tbb to the include and link directories as everything needs it.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${Boost_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR} ${GLM_INCLUDE_DIR}  ${TBB_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${TBB_LIBRARY_DIRS})

# Enable testing and add gtest to the include directories.
ENABLE_TESTING()

# libraries
add_subdirectory(anh)
# executables
#add_subdirectory(login)