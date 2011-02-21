# Build the GLog vendor library

set(glog_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/glog")
set(glog_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/glog-build")

if(WIN32)
    set(glog_configure "")
	set(glog_build msbuild "${CMAKE_CURRENT_BINARY_DIR}/vendor/glog/google-glog_vc10.sln")
else()
    set(glog_configure ${glog_source}/configure)
	set(glog_build make)

    set(GLog_INCLUDE_DIR "${glog_binary}/src" CACHE PATH "" FORCE)
    set(GLog_LIBRARY_DIRS "${glog_binary}/src" CACHE PATH "" FORCE)
endif()

ExternalProject_Add(glog
	GIT_REPOSITORY https://github.com/anhstudios/glog.git
	GIT_TAG v0.3.1-custom
	UPDATE_COMMAND ""
	SOURCE_DIR ${glog_source}
	BINARY_DIR ${glog_binary}
	CONFIGURE_COMMAND "${glog_configure}"
	BUILD_COMMAND ${glog_build}
	INSTALL_COMMAND ""
)
