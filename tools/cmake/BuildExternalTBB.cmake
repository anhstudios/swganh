# Build the TBB vendor library
set(TBB_ROOT "${VENDOR_PREFIX}/src/TBB")

list(APPEND vendor_args
	"-DTBB_INSTALL_DIR:PATH=${TBB_ROOT}")
	
if(WIN32)
	set(tbb_build msbuild "${TBB_ROOT}/build/vc10/makefile.sln")
else()
	set(tbb_build make tbb_root=${TBB_ROOT} tbb_build_dir=${TBB_ROOT}/lib tbb_build_prefix=gcc)
endif()

ExternalProject_Add(TBB
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/tbb.git
	GIT_TAG 6656acc
	BUILD_IN_SOURCE 1
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${tbb_build}
	INSTALL_COMMAND ""
)
