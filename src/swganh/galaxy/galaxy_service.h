
#ifndef SWGANH_GALAXY_GALAXY_SERVICE_H_
#define SWGANH_GALAXY_GALAXY_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>

namespace swganh {
namespace object {
    class ObjectController;
}}  // namespace swganh::object

namespace swganh {
namespace galaxy {

    class GalaxyService
    {
    public:

    private:

        typedef std::map<
            uint64_t, 
            std::shared_ptr<swganh::object::ObjectController>
        > ObjectControllerMap;


    };

}}  // namespace swganh::galaxy

#endif  // SWGANH_GALAXY_GALAXY_SERVICE_H_
