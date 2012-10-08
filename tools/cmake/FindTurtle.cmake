find_path(TURTLE_INCLUDE_DIR turtle/mock.hpp
    HINTS
        $ENV{TURTLE_ROOT}
    PATH_SUFFIXES include
    PATHS
        ${TURTLE_ROOT}
        ${TURTLE_INCLUDE_DIR}
)

# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Turtle DEFAULT_MSG TURTLE_INCLUDE_DIR)

mark_as_advanced(TURTLE_INCLUDE_DIR)
