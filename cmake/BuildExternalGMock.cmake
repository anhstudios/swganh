# Build the GMock vendor library
set(gmock_source "${VENDOR_PREFIX}/src/gmock")
set(gmock_binary "${VENDOR_PREFIX}/src/gmock-build")

set(GMock_INCLUDE_DIR "${gmock_source}/include" CACHE PATH "" FORCE)

if(MSVC)
	set(gmock_lib_args
		-Dgtest_force_shared_crt=ON
	)
	set(GMock_LIBRARY_DIRS "${gmock_binary}/$(Configuration)" CACHE PATH "" FORCE)
else()	
	set(gmock_lib_args)
	set(GMock_LIBRARY_DIRS "${gmock_binary}/lib" CACHE PATH "" FORCE)
endif()

ExternalProject_Add(GMock
	PREFIX ${VENDOR_PREFIX}
	GIT_REPOSITORY https://github.com/anhstudios/gmock.git
	GIT_TAG 6f315ddc
	UPDATE_COMMAND ""
	CMAKE_ARGS
		${gmock_lib_args}
	INSTALL_COMMAND ""
)
