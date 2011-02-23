# Build the Boost vendor library
set(boost_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/boost")

if(MSVC)
	set(boost_lib_args
		-DENABLE_SHARED:BOOL=OFF
		-DENABLE_STATIC:BOOL=ON
	)
else()	
	set(boost_lib_args
		-DENABLE_SHARED:BOOL=ON
		-DENABLE_STATIC:BOOL=OFF
	)
endif()

ExternalProject_Add(boost
	PREFIX ${VENDOR_PREFIX}
	URL ${boost_source}
	UPDATE_COMMAND ""
	CMAKE_ARGS
		${boost_lib_args}
    	-DBUILD_SHARED_LIBS:BOOL=ON
    	-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${VENDOR_PREFIX}/src/boost-build/lib
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${VENDOR_PREFIX}/src/boost-build/lib
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${VENDOR_PREFIX}/src/boost-build/lib
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${VENDOR_PREFIX}/src/boost-build/lib
    	-DBUILD_EXAMPLES:BOOL=OFF
    	-DBUILD_TESTING:BOOL=OFF
    	-DBUILD_VERSIONED:BOOL=OFF
    	-DINSTALL_VERSIONED:BOOL=OFF
    	-DWITH_MPI:BOOL=OFF
    	-DBUILD_PROJECTS:LIST=date_time^^thread^^regex^^system
    LIST_SEPARATOR ^^
    INSTALL_COMMAND ""
)

# Set the path to the built Boost for use by other projects
set(Boost_INCLUDE_DIR "${boost_source}" CACHE PATH "" FORCE)
set(Boost_LIBRARY_DIRS "${VENDOR_PREFIX}/src/boost-build/lib" CACHE PATH "" FORCE)