find_path(Glm_INCLUDE_DIR glm/glm.hpp
    HINTS
        $ENV{Glm_ROOT}
    PATH_SUFFIXES include
    PATHS
        ${Glm_ROOT}
        ${Glm_INCLUDEDIR}
)

# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Glm DEFAULT_MSG Glm_INCLUDE_DIR)

mark_as_advanced(Glm_INCLUDE_DIR)
