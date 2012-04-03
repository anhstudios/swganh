#include "logger.h"

using namespace anh;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::init(const std::string& app_name) 
{
    // Console Logs info and above
    logging::init_log_to_console(
        std::clog,
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) >= info,
        keywords::format = "%TimeStamp%: %_%"
        );
// If we're in debug mode compile debug and above
#ifdef _DEBUG
    // Log Everything info and above to swganh.log
    logging::init_log_to_file
    (
        "logs/" + app_name + "_debug.log",
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) >= info,
        keywords::format = fmt::format("%1% [%2%] <%3%> %4%")
            % fmt::date_time("TimeStamp", std::nothrow)
            % fmt::attr<severity_level>("Severity", std::nothrow)
            % fmt::attr<attrs::current_thread_id::held_type>("ThreadID")
            % fmt::message()
    );

#else
    // Log Everything info and above to swganh.log
    logging::init_log_to_file
    (
        "logs/" + app_name + ".log",
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) >= info,
        keywords::format = fmt::format("%1% [%2%] <%3%> %4%")
            % fmt::date_time("TimeStamp", std::nothrow)
            % fmt::attr<severity_level>("Severity", std::nothrow)
            % fmt::attr<attrs::current_thread_id::held_type>("ThreadID")
            % fmt::message()
    );
#endif
    // Log Everything warning and above to swganh.log
    logging::init_log_to_file
    (
        "logs/"+ app_name + "_warning.log",
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) >= warning,
        keywords::format = fmt::format("%1% [%2%] <%3%> %4%")
            % fmt::date_time("TimeStamp", std::nothrow)
            % fmt::attr<severity_level>("Severity", std::nothrow)
            % fmt::attr<attrs::current_thread_id::held_type>("ThreadID")
            % fmt::message()
    );
    
    // Log Client/Server messages
    logging::init_log_to_file
    (
        "logs/"+ app_name + "_events.log",
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) == event,
        keywords::format = fmt::format("%1% <%2%> %3%")
            % fmt::date_time("TimeStamp", std::nothrow)
            % fmt::attr<attrs::current_thread_id::held_type>("ThreadID")
            % fmt::message()
    );
    
    logging::add_common_attributes();
}