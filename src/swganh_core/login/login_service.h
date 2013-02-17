// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/login/login_service_interface.h"

namespace swganh {
namespace login {

/**
* Provides the functionality of the login service.
*/
class LoginService 
    : public swganh::login::LoginServiceInterface
{
public:

	/**
	* Creates a new instance
	*/
    LoginService(
        std::string listen_address, 
        uint16_t listen_port, 
        swganh::app::SwganhKernel* kernel);
    
	/**
	* Custom destructor
	*/
	~LoginService();
    
	/**
	* @return the service description for this service
	*/
    swganh::service::ServiceDescription GetServiceDescription();
    
	/**
	* Removes a session from this service.
	* @param session the session to remove
	* @return true if success
	*/
    bool RemoveSession(std::shared_ptr<swganh::network::soe::Session> session);

	/**
	* @param endpoint the ip of the session to fetch
	* @return the session for a given ip
	*/
    std::shared_ptr<swganh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint);

	/**
	* @param session_key the key of the session to fetch the account id for
	* @return the account_id of the session found
	*/
    uint32_t GetAccountBySessionKey(const std::string& session_key);
       
	/**
	* @return the number of seconds between galaxy status updates
	*/
    int galaxy_status_check_duration_secs() const;

	/**
	* Sets the number of seconds between galaxy status updates
	* @param the new duration
	*/
    void galaxy_status_check_duration_secs(int new_duration);

	/**
	* @return true if auto account creation is enabled
	*/
    bool login_auto_registration() const;

	/**
	* Sets the status of account auto creation
	* @param auto_registration true if the enabled
	*/
    void login_auto_registration(bool auto_registeration);
    
	/**
	* @return the timeout of the login server in seconds
	*/
    int login_error_timeout_secs() const;

	/**
	* Sets the login timeout
	* @param new_timeout the new login timeout in seconds
	*/
    void login_error_timeout_secs(int new_timeout);
    
	/**
	* Called on startup
	*/
    void Startup();

	/**
	* Called on shutdown
	*/
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

