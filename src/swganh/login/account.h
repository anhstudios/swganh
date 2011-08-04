/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_LOGIN_ACCOUNT_H_
#define SWGANH_LOGIN_ACCOUNT_H_

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

#endif  // SWGANH_LOGIN_ACCOUNT_H_
