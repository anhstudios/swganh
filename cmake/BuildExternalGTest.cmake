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

ExternalProject_Add(GTest
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/gtest.git
	GIT_TAG 9b0cb547
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	BUILD_IN_SOURCE 1
	CMAKE_ARGS
		${gtest_lib_args}
	INSTALL_COMMAND ""
)

