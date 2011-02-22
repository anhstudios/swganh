# Build the GTest vendor library

set(gtest_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/gtest")
set(gtest_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/gtest-build")

if(MSVC)
	set(gtest_lib_args
		-Dgtest_force_shared_crt=ON
	)
else()	
	set(gtest_lib_args)
endif()

ExternalProject_Add(gtest
	GIT_REPOSITORY https://github.com/anhstudios/gtest.git
	GIT_TAG master
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	SOURCE_DIR ${gtest_source}
	BINARY_DIR ${gtest_binary}
	CMAKE_ARGS
		${gtest_lib_args}
	INSTALL_COMMAND ""
)

# Set the path to the built GTest for use by other projects
set(GTest_INCLUDE_DIR "${gtest_source}" CACHE PATH "" FORCE)
set(GTest_LIBRARY_DIRS "${gtest_binary}/lib" CACHE PATH "" FORCE)