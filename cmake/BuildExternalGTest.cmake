# Build the GTest vendor library
set(gtest_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gtest")

if(MSVC)
	set(gtest_lib_args
		-Dgtest_force_shared_crt=ON
	)
else()	
	set(gtest_lib_args)
endif()

ExternalProject_Add(gtest
	PREFIX ${VENDOR_PREFIX}
	URL ${gtest_source}
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	CMAKE_ARGS
		${gtest_lib_args}
	INSTALL_COMMAND ""
)

# Set the path to the built GTest for use by other projects
set(GTest_INCLUDE_DIR "${gtest_source}" CACHE PATH "" FORCE)
set(GTest_LIBRARY_DIRS "${gtest_binary}/lib" CACHE PATH "" FORCE)