// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_VALIDATOR_H_
#define PUB14_CORE_COMMAND_COMMAND_VALIDATOR_H_

#include "swganh/command/command_validator_interface.h"

namespace pub14_core {
namespace command {

    class CommandValidator : public swganh::command::CommandValidatorInterface
    {
    public:
        virtual ~CommandValidator();
    };

}}

#endif  // PUB14_CORE_COMMAND_COMMAND_VALIDATOR_H_
