# Build the GMock vendor library

set(gmock_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/gmock")
set(gmock_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/gmock-build")

if(MSVC)
	set(gmock_lib_args
		-Dgtest_force_shared_crt=ON
	)
else()	
	set(gmock_lib_args)
endif()

ExternalProject_Add(gmock
	GIT_REPOSITORY https://github.com/anhstudios/gmock.git
	GIT_TAG master
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	SOURCE_DIR ${gmock_source}
	BINARY_DIR ${gmock_binary}
	CMAKE_ARGS
		${gmock_lib_args}
	INSTALL_COMMAND ""
)

# Set the path to the built GMock for use by other projects
set(GMock_INCLUDE_DIR "${gmock_source}" CACHE PATH "" FORCE)
set(GMock_LIBRARY_DIRS "${gmock_binary}/lib" CACHE PATH "" FORCE)