// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "authentication_manager.h"

#include "swganh/login/account.h"
#include "swganh/login/login_client_interface.h"

using namespace swganh::login;
using namespace swganh::login;
using namespace std;

AuthenticationManager::AuthenticationManager(std::shared_ptr<encoders::EncoderInterface> encoder) 
    : encoder_(encoder) {}

std::shared_ptr<encoders::EncoderInterface> AuthenticationManager::encoder() {
    return encoder_;
}

bool AuthenticationManager::Authenticate(std::shared_ptr<LoginClientInterface> client, std::shared_ptr<Account> account) {

    auto current_account = client->GetAccount();

    if (current_account) {
        // make sure the password hasn't changed
        if (current_account->password() != account->password()) {
            // consider throwing here perhaps?
            return false;
        }
    } else {
        string presented_password = client->GetPassword();
        if (presented_password.empty()) {
            return false;
        }

        if (!encoder_->IsPasswordValid(account->password(), presented_password, account->salt())) {
            return false;
        }
    }

    return true;
}
