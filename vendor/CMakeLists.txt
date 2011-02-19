## Vendor Configuration Options

# Some automated systems need an xml report which requires a second run of the
# tests. To prevent these unnecessary runnings during normal builds the
# following option is set with a default of false.
option(swganh_USE_SYSTEM_BOOST
    "If false, swganh will download and build it's own copy of Boost" FALSE)
option(swganh_USE_SYSTEM_GLM
    "If false, swganh will download and build it's own copy of GLM" FALSE)
option(swganh_USE_SYSTEM_GLOG
    "If false, swganh will download and build it's own copy of GLog" FALSE)
option(swganh_USE_SYSTEM_GMOCK
    "If false, swganh will download and build it's own copy of GMock" FALSE)
option(swganh_USE_SYSTEM_GTEST
    "If false, swganh will download and build it's own copy of GTest" FALSE)
option(swganh_USE_SYSTEM_MYSQL_CONNECTOR_C
    "If false, swganh will download and build it's own copy of Mysql 
        Connector/C" FALSE)
option(swganh_USE_SYSTEM_MYSQL_CONNECTOR_CPP
    "If false, swganh will download and build it's own copy of Mysql 
        Connector/C++" FALSE)
option(swganh_USE_SYSTEM_TBB
    "If false, swganh will download and build it's own copy of TBB" FALSE)
option(swganh_USE_SYSTEM_ZLIB
    "If false, swganh will download and build it's own copy of ZLib" FALSE)
        
## Build and/or configure Boost
if(NOT swganh_USE_SYSTEM_BOOST)
    include(BuildExternalBoost)
else()
    
endif()