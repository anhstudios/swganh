// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_PROPERTIES_TRE_LOADER_H_
#define PUB14_CORE_COMMAND_COMMAND_PROPERTIES_TRE_LOADER_H_

#include <cstdint>
#include <vector>

#include "swganh/command/v2/command_properties_loader_interface.h"
#include "swganh/tre/readers/datatable_reader.h"

namespace swganh {
namespace tre {
    class TreArchive;
}}  // namespace swganh::tre

namespace pub14_core {
namespace command {

    class CommandPropertiesTreLoader : public swganh::command::v2::CommandPropertiesLoaderInterface
    {
    public:
        explicit CommandPropertiesTreLoader(swganh::tre::TreArchive* archive);

        swganh::command::v2::CommandPropertiesMap LoadCommandPropertiesMap();

    private:
        CommandPropertiesTreLoader();

        uint64_t BuildAllowInLocomotion(swganh::tre::readers::DatatableRow* row);
        uint64_t BuildAllowInState(swganh::tre::readers::DatatableRow* row);
        uint64_t BuildBitmask(const std::vector<int>& bits);

        swganh::tre::TreArchive* archive_;
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_PROPERTIES_TRE_LOADER_H_
