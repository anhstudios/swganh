# This file is part of SWGANH which is released under the MIT license.
# See file LICENSE or go to http://swganh.com/LICENSE

macro(ANH_CHECK_C_COMPILER_FLAG
	cflags__
	cache_var__
	flag__)

	include(CheckCCompilerFlag)

	CHECK_C_COMPILER_FLAG("${flag__}" "${cache_var__}")
	if(${cache_var__})
		message(STATUS "Using C flag: ${flag__}")
		set(${cflags__} "${${cflags__}} ${flag__}")
	else()
		message(STATUS "Unsupported C flag: ${flag__}")
	endif()
endmacro()

macro(ANH_CHECK_CXX_COMPILER_FLAG
	cxxflags__
	cache_var__
	flag__)

	include(CheckCXXCompilerFlag)

	CHECK_CXX_COMPILER_FLAG("${flag__}" "${cache_var__}")
	if(${cache_var__})
		message(STATUS "Using CXX flag: ${flag__}")
		set(${cxxflags__} "${${cxxflags__}} ${flag__}")
	else()
		message(STATUS "Unsupported CXX flag: ${flag__}")
	endif()
endmacro()
