# Build the GLog vendor library

set(glog_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glog")

if(WIN32)
    set(glog_configure "")
	set(glog_build msbuild "${VENDOR_PREFIX}/src/glog/google-glog_vc10.sln")
	set(glog_build_insource BUILD_IN_SOURCE 1)
else()
    set(glog_configure ${glog_source}/configure)
	set(glog_build make)
	set(glog_build_insource )
    set(GLog_INCLUDE_DIR "${glog_binary}/src" CACHE PATH "" FORCE)
    set(GLog_LIBRARY_DIRS "${glog_binary}/src" CACHE PATH "" FORCE)
endif()

ExternalProject_Add(glog
	PREFIX ${VENDOR_PREFIX}
	URL ${glog_source}
	UPDATE_COMMAND ""
	${glog_build_insource}
	CONFIGURE_COMMAND "${glog_configure}"
	BUILD_COMMAND ${glog_build}
	INSTALL_COMMAND ""
)
