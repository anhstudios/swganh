# Build the Boost vendor library

set(__boost_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/boost")
set(__boost_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/boost-build")

if(MSVC)
	set(__boost_lib_args
		-DENABLE_SHARED:BOOL=OFF
		-DENABLE_STATIC:BOOL=ON
	)
else()	
	set(__boost_lib_args
		-DENABLE_SHARED:BOOL=ON
		-DENABLE_STATIC:BOOL=OFF
	)
endif()

ExternalProject_Add(boost
	GIT_REPOSITORY https://github.com/anhstudios/boost.git
	GIT_TAG 1.45.0
	UPDATE_COMMAND ""
	SOURCE_DIR ${__boost_source}
	BINARY_DIR ${__boost_binary}
	CMAKE_ARGS
		${__boost_lib_args}
    	-DBUILD_SHARED_LIBS:BOOL=ON
    	-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${__boost_binary}/lib
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${__boost_binary}/lib
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${__boost_binary}/lib
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${__boost_binary}/lib
    	-DBUILD_EXAMPLES:BOOL=OFF
    	-DBUILD_TESTING:BOOL=OFF
    	-DBUILD_VERSIONED:BOOL=OFF
    	-DINSTALL_VERSIONED:BOOL=OFF
    	-DWITH_MPI:BOOL=OFF
	INSTALL_COMMAND ""
)

# Set the path to the built Boost for use by other projects
set(Boost_INCLUDE_DIR "${__boost_source}" CACHE PATH "" FORCE)
set(Boost_LIBRARY_DIRS "${__boost_binary}/lib" CACHE PATH "" FORCE)