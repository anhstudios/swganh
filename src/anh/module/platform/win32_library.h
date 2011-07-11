
#ifndef ANH_MODULE_PLATFORM_WIN32_LIBRARY_H_
#define ANH_MODULE_PLATFORM_WIN32_LIBRARY_H_

#include <memory>
#include <string>

#include <Windows.h>

#include <boost/noncopyable.hpp>

#include "anh/module/bindings.h"
#include "anh/module/platform/library_interface.h"

namespace anh {
namespace module {
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
    ExitFunc exit_func_;
};

}}}  // namespace anh::module::platform

#endif  // ANH_MODULE_PLATFORM_WIN32_LIBRARY_H_
