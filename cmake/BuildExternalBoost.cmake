# Build the Boost vendor library
set(Boost_ROOT "${VENDOR_PREFIX}/src/Boost" CACHE PATH "" FORCE)

if(MSVC)
	set(boost_lib_args
		-DENABLE_SHARED:BOOL=OFF
		-DENABLE_STATIC:BOOL=ON
		-DLIBPREFIX=lib
	)
else()	
	set(boost_lib_args
		-DENABLE_SHARED:BOOL=OFF
		-DENABLE_STATIC:BOOL=ON
	)
endif()

ExternalProject_Add(Boost
	PREFIX ${VENDOR_PREFIX}
	GIT_REPOSITORY https://github.com/anhstudios/boost.git
	GIT_TAG 7b35086
	UPDATE_COMMAND ""
	CMAKE_ARGS
		${boost_lib_args}
		-DENABLE_MULTITHREADED:BOOL=ON
    	-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    	-DBUILD_EXAMPLES:BOOL=OFF
    	-DBUILD_TESTING:BOOL=OFF
    	-DBUILD_VERSIONED:BOOL=OFF
    	-DINSTALL_VERSIONED:BOOL=OFF
    	-DWITH_MPI:BOOL=OFF
    	-DBUILD_PROJECTS:LIST=date_time^^program_options^^thread^^regex^^system
    LIST_SEPARATOR ^^
    INSTALL_COMMAND ""
)

# Set the path to the built Boost for use by other projects
set(Boost_INCLUDEDIR "${Boost_ROOT}" CACHE PATH "" FORCE)
set(Boost_LIBRARYDIR "${Boost_ROOT}/../Boost-build/lib" CACHE PATH "" FORCE)
