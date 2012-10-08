// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

namespace swganh {
namespace login {

class Account {
public:
    explicit Account(bool enabled = false);
    ~Account();

    uint32_t account_id() const;
    void account_id(uint32_t account_id);

    std::string username() const;
    void username(std::string username);

    std::string salt() const;
    void salt(std::string salt);

    std::string password() const;
    void password(std::string password);

    std::string algorithm() const;
    void algorithm(std::string algorithm);

    bool IsEnabled() const;
    void Enable();
    void Disable();   

protected:
    std::string username_;
    std::string password_;
    std::string salt_;
    std::string algorithm_;

    uint32_t account_id_;

    bool enabled_;
};

}}  // namespace swganh::login
