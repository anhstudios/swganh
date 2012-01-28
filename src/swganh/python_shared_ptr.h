#ifndef SWGANH_PYTHON_SHAREDPTR_H_
#define SWGANH_PYTHON_SHAREDPTR_H_

#include "boost/python.hpp"
#include <memory>

namespace std {
template<class T>
inline T * get_pointer(std::shared_ptr<T> const& p){
    return p.get();
}
}

namespace boost{ namespace python{
    template <class T>
    struct pointee< std::shared_ptr<T> >{
        typedef T type;
    };

} }


#endif//SWGANH_PYTHON_SHAREDPTR_H_
