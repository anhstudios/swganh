# Build the GLog vendor library

set(glog_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/glog")
set(glog_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/glog-build")

if(WIN32)
	set(glog_build msbuild "${CMAKE_CURRENT_BINARY_DIR}/vendor/glog/google-glog_vc10.sln")
else()
	set(glog_build make)
endif()

ExternalProject_Add(glog
	GIT_REPOSITORY https://github.com/anhstudios/glog.git
	GIT_TAG v0.3.1-custom
	UPDATE_COMMAND ""
	SOURCE_DIR ${glog_source}
	BINARY_DIR ${glog_binary}
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${glog_build}
	INSTALL_COMMAND ""
)

# Set the path to the built Boost for use by other projects
set(GLog_INCLUDE_DIR "${glog_source}" CACHE PATH "" FORCE)
set(GLog_LIBRARY_DIRS "${glog_binary}/lib" CACHE PATH "" FORCE)