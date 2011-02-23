# Build the GTest vendor library
set(gtest_source "${VENDOR_PREFIX}/src/gtest")

# Set the path to the built GTest for use by other projects
set(GTest_INCLUDE_DIR "${gtest_source}/include" CACHE PATH "" FORCE)

if(MSVC)
	set(gtest_lib_args
		-Dgtest_force_shared_crt=ON
	)
	set(GTest_LIBRARY_DIRS "${VENDOR_PREFIX}/src/gtest-build/\$(Configuration)" CACHE PATH "" FORCE)
else()	
	set(gtest_lib_args)
endif()

ExternalProject_Add(gtest
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/gtest.git
	GIT_TAG 9b0cb547
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	CMAKE_ARGS
		${gtest_lib_args}
	INSTALL_COMMAND ""
)


## GTEST functions
function(GTEST_ADD_TESTS executable extra_args)
    if(NOT ARGN)
        message(FATAL_ERROR "Missing ARGN: Read the documentation for GTEST_ADD_TESTS")
    endif()
    foreach(source ${ARGN})
        file(READ "${source}" contents)
        string(REGEX MATCHALL "TEST_?F?\\(([A-Za-z_0-9 ,]+)\\)" found_tests ${contents})
        foreach(hit ${found_tests})
            string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+), *([A-Za-z_0-9]+) *\\).*" "\\1.\\2" test_name ${hit})
            add_test(NAME ${test_name} COMMAND ${executable} --gtest_filter=${test_name} ${extra_args})
            list(APPEND _test_names ${test_name})
        endforeach()
    endforeach()    
    set(GTEST_ADD_TEST_NAMES ${_test_names} PARENT_SCOPE)
endfunction()

function(_gtest_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()