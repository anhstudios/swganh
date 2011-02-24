# Build the GLog vendor library
set(glog_source "${VENDOR_PREFIX}/src/glog")

if(WIN32)
    set(glog_configure "")
	set(glog_build msbuild "${VENDOR_PREFIX}/src/glog/google-glog_vc10.sln")
	set(glog_build_insource BUILD_IN_SOURCE 1)
    set(GLog_INCLUDE_DIR "${glog_source}/src/windows" CACHE PATH "" FORCE)
    set(GLog_LIBRARY_DIRS "${glog_source}/src/$(Configuration)" CACHE PATH "" FORCE)
else()
    set(glog_configure ${glog_source}/configure)
	set(glog_build make)
	set(glog_build_insource )
    set(GLog_INCLUDE_DIR "${glog_source}/src" CACHE PATH "" FORCE)
    set(GLog_LIBRARY_DIRS "${glog_source}/src" CACHE PATH "" FORCE)
endif()

ExternalProject_Add(GLog
	PREFIX ${VENDOR_PREFIX}
	GIT_REPOSITORY https://github.com/anhstudios/glog.git
	GIT_TAG 92b92d0e
	UPDATE_COMMAND ""
	${glog_build_insource}
	CONFIGURE_COMMAND "${glog_configure}"
	BUILD_COMMAND ${glog_build}
	INSTALL_COMMAND ""
)
