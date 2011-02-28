find_path(GLOG_INCLUDE_DIR glog/logging.h
    HINTS
        $ENV{GLog_ROOT}
    PATH_SUFFIXES include src/windows
    PATHS
        ${GLog_ROOT}
        ${GLog_INCLUDEDIR}
)

find_library(GLOG_LIBRARY 
    NAMES glog
    HINTS
        $ENV{GLog_ROOT}
        ${GLog_ROOT}
        ${GLog_LIBRARYDIR}
)

# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLog DEFAULT_MSG GLOG_LIBRARY GLOG_INCLUDE_DIR)

mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_DIR)
