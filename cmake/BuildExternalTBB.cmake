# Build the TBB vendor library

set(tbb_source "${VENDOR_PREFIX}/src/tbb")

set(TBB_INCLUDE_DIR "${tbb_source}/include" CACHE PATH "" FORCE)

if(WIN32)
	set(tbb_build msbuild "${VENDOR_PREFIX}/src/tbb/build/vc10/makefile.sln")
	set(TBB_LIBRARY_DIRS "${VENDOR_PREFIX}/src/tbb/build/vc10/ia32/\$(Configuration)" CACHE PATH "" FORCE)
else()
	set(tbb_build make -f ${VENDOR_PREFIX}/src/tbb/Makefile tbb_root=${VENDOR_PREFIX}/src tbb_build_dir=${tbb_prefix}/lib tbb_build_prefix=gcc)
endif()

ExternalProject_Add(tbb
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/tbb.git
	GIT_TAG 5909edfd
	BUILD_IN_SOURCE 1
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${tbb_build}
	INSTALL_COMMAND ""
)
