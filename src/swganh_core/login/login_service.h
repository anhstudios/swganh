// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/login/login_service_interface.h"

namespace swganh {
namespace login {

class LoginService 
    : public swganh::login::LoginServiceInterface
{
public:
    LoginService(
        std::string listen_address, 
        uint16_t listen_port, 
        swganh::app::SwganhKernel* kernel);
    ~LoginService();
    
    swganh::service::ServiceDescription GetServiceDescription();
    
    bool RemoveSession(std::shared_ptr<swganh::network::soe::Session> session);

    std::shared_ptr<swganh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint);

    uint32_t GetAccountBySessionKey(const std::string& session_key);
        
    int galaxy_status_check_duration_secs() const;
    void galaxy_status_check_duration_secs(int new_duration);

    bool login_auto_registration() const;
    void login_auto_registration(bool auto_registeration);
    
    int login_error_timeout_secs() const;
    void login_error_timeout_secs(int new_timeout);
    
    void Startup();

    void Shutdown();

private:
    LoginService();
    
    std::shared_ptr<swganh::network::soe::Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint);
        
    void HandleLoginClientId_(const std::shared_ptr<swganh::login::LoginClientInterface>& login_client, swganh::messages::LoginClientId* message);

    std::vector<swganh::login::GalaxyStatus> GetGalaxyStatus_();
    void UpdateGalaxyStatus_();
    
    typedef std::map<
        boost::asio::ip::udp::endpoint,
        std::shared_ptr<swganh::login::LoginClientInterface>
    > SessionMap;
    
    boost::mutex session_map_mutex_;
    SessionMap session_map_;

    swganh::app::SwganhKernel* kernel_;
    swganh::character::CharacterServiceInterface* character_service_;
    std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;

	swganh::galaxy::GalaxyServiceInterface* galaxy_service_;
    std::shared_ptr<swganh::login::AuthenticationManagerInterface> authentication_manager_;
    std::shared_ptr<swganh::login::providers::AccountProviderInterface> account_provider_;
    
    std::vector<swganh::login::GalaxyStatus> galaxy_status_;
    
    bool login_auto_registration_;
    int galaxy_status_check_duration_secs_;
    int login_error_timeout_secs_;
    boost::asio::deadline_timer galaxy_status_timer_;
    std::shared_ptr<boost::asio::deadline_timer> session_timer_;
    
    std::string listen_address_;
    uint16_t listen_port_;
    swganh::ActiveObject active_;
};

}} // namespace swganh::login

