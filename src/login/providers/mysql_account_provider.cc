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

#include "login/providers/mysql_account_provider.h"


using namespace login;
using namespace providers;
using namespace std;

MysqlAccountProvider::MysqlAccountProvider() {}

MysqlAccountProvider::~MysqlAccountProvider() {}

shared_ptr<Account> MysqlAccountProvider::FindByUsername(string username) {
    auto account = make_shared<Account>(true);

    account->account_id(1234);
    account->username("test");
    account->password("f78036ebcb098b728278dcb05f957d5d5568b3e8bf9973a5de3df3716b3825b372603409d179f053955853ed07b9f53c326cc98582e323937d172903e4665e03");
    account->salt("f78036ebcb098b728278dcb05f957d5d5568b3e8bf9973a5de3df3716b3825b372603409d179f053955853ed07b9f53c326cc98582e323937d172903e4665e03");
    account->algorithm("sha512");

    return account;
}
