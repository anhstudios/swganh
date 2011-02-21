# Build the TBB vendor library

set(tbb_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/tbb")
set(tbb_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/tbb-build")

set(TBB_INCLUDE_DIR "${tbb_source}/include" CACHE PATH "" FORCE)

if(WIN32)
	set(tbb_build msbuild "${CMAKE_CURRENT_BINARY_DIR}/vendor/tbb/build/vc10/makefile.sln")
else()
	set(tbb_build make tbb_root=${tbb_source} tbb_build_dir=${tbb_binary}/lib tbb_build_prefix=gcc)

    set(TBB_LIBRARY_DIRS "${tbb_binary}/lib" CACHE PATH "" FORCE)
endif()

ExternalProject_Add(tbb
	GIT_REPOSITORY https://github.com/anhstudios/tbb.git
	GIT_TAG v3.0.5-custom
	UPDATE_COMMAND ""
	SOURCE_DIR ${tbb_source}
	BINARY_DIR ${tbb_source}
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${tbb_build}
	INSTALL_COMMAND ""
)
