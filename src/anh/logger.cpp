// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "logger.h"

using namespace swganh;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::init(const std::string& app_name) 
{
    // Console Logs info and above
    console_sink_ = logging::init_log_to_console(
        std::clog,
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) >= info,
        keywords::format = "%TimeStamp%: %_%",
        keywords::auto_flush = true
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
            % fmt::attr<attrs::current_thread_id::value_type>("ThreadID")
            % fmt::message(),
        keywords::auto_flush = true
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
            % fmt::attr<attrs::current_thread_id::value_type>("ThreadID")
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
            % fmt::attr<attrs::current_thread_id::value_type>("ThreadID")
            % fmt::message(),
        keywords::auto_flush = true
    );
    
    // Log Client/Server messages
    logging::init_log_to_file
    (
        "logs/"+ app_name + "_events.log",
        keywords::filter = flt::attr<severity_level>("Severity", std::nothrow) == event,
        keywords::format = fmt::format("%1% <%2%> %3%")
            % fmt::date_time("TimeStamp", std::nothrow)
            % fmt::attr<attrs::current_thread_id::value_type>("ThreadID")
            % fmt::message(),
        keywords::auto_flush = true
    );
    
    logging::add_common_attributes();
}

void Logger::EnableConsoleLogging()
{
    logging::core::get()->add_sink(console_sink_);
}

void Logger::DisableConsoleLogging()
{
    logging::core::get()->remove_sink(console_sink_);
}
