include(ANHPlugin)

AddANHPlugin(quadtree_spatial
    DEPENDS
        anh_lib
        swganh_lib
	ADDITIONAL_INCLUDE_DIRS
	    ${Boost_INCLUDE_DIR}
            ${GLM_INCLUDE_DIR}
	ADDITIONAL_LIBRARY_DIRS
	    ${Boost_LIBRARY_DIRS}  
)