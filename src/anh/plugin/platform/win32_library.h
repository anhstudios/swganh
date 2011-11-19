
#ifndef ANH_PLUGIN_PLATFORM_WIN32_LIBRARY_H_
#define ANH_PLUGIN_PLATFORM_WIN32_LIBRARY_H_

#ifdef WIN32

#include <memory>
#include <string>

#include <Windows.h>

#include <boost/noncopyable.hpp>

#include "anh/plugin/bindings.h"
#include "anh/plugin/platform/library_interface.h"

namespace anh {
namespace plugin {
namespace platform {

class Win32Library : public LibraryInterface, private boost::noncopyable {
public:
    static std::shared_ptr<Win32Library> Load(std::string library);

public:
    ~Win32Library();
    
    void * GetSymbol(const std::string& symbol);

private:
    Win32Library();
    explicit Win32Library(HMODULE handle);
    
private:
    HMODULE handle_;
    anh::plugin::ExitFunc exit_func_;
};

}}}  // namespace anh::plugin::platform

#endif  // WIN32

#endif  // ANH_PLUGIN_PLATFORM_WIN32_LIBRARY_H_
