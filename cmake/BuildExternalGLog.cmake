# Build the GLog vendor library
set(GLog_ROOT "${VENDOR_PREFIX}/src/GLog")

if(WIN32)
    set(glog_configure "")
	set(glog_build msbuild "${GLog_ROOT}/google-glog_vc10.sln")
else()
    set(glog_configure ${GLog_ROOT}/configure)
	set(glog_build make)
endif()

ExternalProject_Add(GLog
	PREFIX ${VENDOR_PREFIX}
	GIT_REPOSITORY https://github.com/anhstudios/glog.git
	GIT_TAG 92b92d0e
	UPDATE_COMMAND ""
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND "${glog_configure}"
	BUILD_COMMAND ${glog_build}
	INSTALL_COMMAND ""
)
