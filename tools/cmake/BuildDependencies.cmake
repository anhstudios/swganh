# macro for locating or building vendor libraries
macro(bootstrap_add_vendor name)
	if(NOT bootstrap_USE_SYSTEM_${name})
		include(BuildExternal${name})
		list(APPEND vendor_libraries ${name})
	endif()	
endmacro()

# Build and/or configure Boost
option(bootstrap_USE_SYSTEM_Boost
    "If false, swganh will download and build it's own copy of Boost" false)
bootstrap_add_vendor(Boost)

# Build and/or configure GLM
option(bootstrap_USE_SYSTEM_Glm
    "If false, swganh will download and build it's own copy of Glm" false)
bootstrap_add_vendor(Glm)
 	
# Build and/or configure GLog
option(bootstrap_USE_SYSTEM_GLog
    "If false, swganh will download and build it's own copy of GLog" false)
bootstrap_add_vendor(GLog)

# Build and/or configure GTest
option(bootstrap_USE_SYSTEM_GTest
    "If false, swganh will download and build it's own copy of GTest" false)
bootstrap_add_vendor(GTest)

# Build and/or configure GMock
option(bootstrap_USE_SYSTEM_GMock
    "If false, swganh will download and build it's own copy of GMock" false)
bootstrap_add_vendor(GMock)

# Build and/or configure Mysql Connector/C
option(bootstrap_USE_SYSTEM_MysqlConnectorC
    "If false, swganh will download and build it's own copy of MysqlConnectorC" false)
bootstrap_add_vendor(MysqlConnectorC)

# Build and/or configure Mysql Connector/Cpp
option(bootstrap_USE_SYSTEM_MysqlConnectorCpp
    "If false, swganh will download and build it's own copy of MysqlConnectorCpp" false)
bootstrap_add_vendor(MysqlConnectorCpp)

# Build and/or configure TBB
option(bootstrap_USE_SYSTEM_TBB
    "If false, swganh will download and build it's own copy of TBB" false)
bootstrap_add_vendor(TBB)

# Build and/or configure ZLib
option(bootstrap_USE_SYSTEM_ZLib
    "If false, swganh will download and build it's own copy of ZLib" false)
bootstrap_add_vendor(ZLib)