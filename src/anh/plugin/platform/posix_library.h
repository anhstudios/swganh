// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32

#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include "anh/plugin/bindings.h"
#include "anh/plugin/platform/library_interface.h"

namespace anh {
namespace plugin {
namespace platform {

class PosixLibrary : public LibraryInterface, private boost::noncopyable {
public:
    static std::shared_ptr<PosixLibrary> Load(std::string library);

public:
    ~PosixLibrary();

    void * GetSymbol(const std::string& symbol);

private:
    PosixLibrary();
    explicit PosixLibrary(void* handle);

private:
    void* handle_;
    anh::plugin::ExitFunc exit_func_;
};

}}}  // namespace anh::module::platform

#endif  // WIN32

