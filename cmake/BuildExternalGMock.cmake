# Build the GMock vendor library
set(gmock_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gmock")

if(MSVC)
	set(gmock_lib_args
		-Dgtest_force_shared_crt=ON
	)
else()	
	set(gmock_lib_args)
endif()

ExternalProject_Add(gmock
	PREFIX ${VENDOR_PREFIX}
	URL ${gmock_source}
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	CMAKE_ARGS
		${gmock_lib_args}
	INSTALL_COMMAND ""
)

# Set the path to the built GMock for use by other projects
set(GMock_INCLUDE_DIR "${gmock_source}" CACHE PATH "" FORCE)
set(GMock_LIBRARY_DIRS "${gmock_binary}/lib" CACHE PATH "" FORCE)