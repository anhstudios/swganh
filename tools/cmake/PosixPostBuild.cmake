
if(NOT EXISTS "${PROJECT_BINARY_DIR}/bin/config/swganh.cfg")
configure_file(
        "${PROJECT_SOURCE_DIR}/data/config/swganh.cfg.in"
        "${PROJECT_BINARY_DIR}/bin/config/swganh.cfg"
        @ONLY)
endif()

add_custom_target(DEPS)

set_target_properties(DEPS
	PROPERTIES
	FOLDER "cmake_targets"
)

add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND mkdir -p "${PROJECT_BINARY_DIR}/bin/config/plugins"
    COMMAND find "${PROJECT_SOURCE_DIR}/plugins" -name "*.cfg" -exec cp {} "${PROJECT_BINARY_DIR}/bin/config/plugins" \;
    VERBATIM
)

set_target_properties(DEPS
	PROPERTIES
	FOLDER "cmake_targets"
)
