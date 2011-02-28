# Build the Boost vendor library
set(Boost_SOURCEDIR "${VENDOR_PREFIX}/src/Boost" CACHE PATH "" FORCE)
set(Boost_INCLUDEDIR "${Boost_SOURCEDIR}" CACHE PATH "" FORCE)
set(Boost_LIBRARYDIR "${Boost_SOURCEDIR}/../Boost-build/lib" CACHE PATH "" FORCE)

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
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${Boost_LIBRARYDIR}
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${Boost_LIBRARYDIR}
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${Boost_LIBRARYDIR}
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${Boost_LIBRARYDIR}
    	-DBUILD_EXAMPLES:BOOL=OFF
    	-DBUILD_TESTING:BOOL=OFF
    	-DBUILD_VERSIONED:BOOL=OFF
    	-DINSTALL_VERSIONED:BOOL=OFF
    	-DWITH_MPI:BOOL=OFF
    	-DBUILD_PROJECTS:LIST=date_time^^thread^^regex^^system^^serialization^^program_options^^filesystem
    LIST_SEPARATOR ^^
    INSTALL_COMMAND ""
)

