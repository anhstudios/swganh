# Build the TBB vendor library

set(tbb_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tbb")

set(TBB_INCLUDE_DIR "${tbb_source}/include" CACHE PATH "" FORCE)

if(WIN32)
	set(tbb_build msbuild "${VENDOR_PREFIX}/src/tbb/build/vc10/makefile.sln")
else()
	set(tbb_build make -f ${VENDOR_PREFIX}/src/tbb/Makefile tbb_root=${VENDOR_PREFIX}/src tbb_build_dir=${tbb_prefix}/lib tbb_build_prefix=gcc)
endif()

ExternalProject_Add(tbb
	PREFIX ${VENDOR_PREFIX}
	URL ${tbb_source}
	BUILD_IN_SOURCE 1
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${tbb_build}
	INSTALL_COMMAND ""
)
