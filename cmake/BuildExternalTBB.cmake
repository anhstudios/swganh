# Build the TBB vendor library
set(TBB_ROOT "${VENDOR_PREFIX}/src/tbb")

if(WIN32)
	set(tbb_build msbuild "${VENDOR_PREFIX}/src/tbb/build/vc10/makefile.sln")
else()
	set(tbb_build make -f ${VENDOR_PREFIX}/src/tbb/Makefile tbb_root=${VENDOR_PREFIX}/src tbb_build_dir=${tbb_prefix}/lib tbb_build_prefix=gcc)
endif()

ExternalProject_Add(TBB
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/tbb.git
	GIT_TAG 5909edfd
	BUILD_IN_SOURCE 1
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${tbb_build}
	INSTALL_COMMAND ""
)
