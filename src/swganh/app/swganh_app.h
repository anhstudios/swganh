// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_APP_SWGANH_APP_H_
#define SWGANH_APP_SWGANH_APP_H_

#include <atomic>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "anh/app/app_interface.h"
#include "anh/service/service_directory.h"
#include "swganh/app/swganh_kernel.h"

namespace swganh {
namespace app {
    
/*!
 * @Brief Base SWGANH Application that starts up serverices, initializes resources and handles the lifetime of objects"
 * @see anh::app::AppInterface
 */
class SwganhApp : public anh::app::AppInterface, private boost::noncopyable {
public:    
    /*!
     * @Brief Default constructor
     */
    SwganhApp(int argc, char* argv[]);
    ~SwganhApp();

    /*!
     * @Brief Initializes the application taking in arguments
     * @param argc command line options for initializing the server
     * @param argv command line values for initializing
     */
    void Initialize(int argc, char* argv[]);

    /*!
     * @Brief Starts the application, ServiceManager and begins the processing
     * @see anh::service::ServiceManager
     */
    void Start();

    /*!
     * @Brief Stops the application, ServiceManager, and joins threads.
     */
    void Stop();

    /*!
     * @Brief Check to see if the server is up and running.
     * @return true if the server is running, false else
     */
    bool IsRunning();

    /*!
     * @Brief Gets the Application's Kernel
     * @return std::shared_ptr of the anh::app::KernelInterface
     * @see anh::app::KernelInterface
     */
    SwganhKernel* GetAppKernel() const;

private:
    SwganhApp();

    void LoadAppConfig_(int argc, char* argv[]);
    void LoadPlugins_(std::vector<std::string> plugins);
    void LoadCoreServices_();

    void CleanupServices_();

    void GalaxyStatusTimerHandler_(const boost::system::error_code& e, int delay_in_secs);

    void SetupLogging_();
    
    boost::asio::io_service io_service_;
    boost::asio::io_service::work io_work_;
    std::shared_ptr<boost::asio::deadline_timer> galaxy_timer_;
    std::vector<boost::thread> io_threads_;
    std::shared_ptr<SwganhKernel> kernel_;
    std::atomic<bool> running_;
    bool initialized_;
};

}}  // namespace swganh::app

#endif  // SWGANH_APP_SWGANH_APP_H_
