# Build the GTest vendor library
set(GTest_SOURCEDIR "${VENDOR_PREFIX}/src/GTest")
set(GTest_INCLUDEDIR "${GTest_SOURCEDIR}/include")
set(GTest_LIBRARYDIR "${GTest_SOURCEDIR}")

list(APPEND vendor_args
	"-DGTest_INCLUDEDIR:PATH=${GTest_INCLUDEDIR}")
list(APPEND vendor_args
	"-DGTest_LIBRARYDIR:PATH=${GTest_LIBRARYDIR}")
	
if(MSVC)
	set(gtest_lib_args
		-Dgtest_force_shared_crt=ON
	)
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

