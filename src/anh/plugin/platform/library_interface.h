
#ifndef ANH_PLUGIN_PLATFORM_LIBRARY_INTERFACE_H_
#define ANH_PLUGIN_PLATFORM_LIBRARY_INTERFACE_H_

namespace anh {
namespace plugin {
namespace platform {

class LibraryInterface {
public:
    virtual ~LibraryInterface() {}
    
    template<typename T>
    T GetSymbol(const std::string& symbol) {
        return *static_cast<T*>(GetSymbol(symbol));
    }

    virtual void * GetSymbol(const std::string& symbol) = 0;
};

}}}  // namespace anh::plugin::platform;

#endif  // ANH_PLUGIN_PLATFORM_LIBRARY_INTERFACE_H_
