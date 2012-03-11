#ifndef SWGANH_PYTHON_SHAREDPTR_H_
#define SWGANH_PYTHON_SHAREDPTR_H_

#include <memory>

namespace boost {
    template<class T> const T* get_pointer(const std::shared_ptr<T>& ptr)
    {
        return ptr.get();
    }

    template<class T> T* get_pointer(std::shared_ptr<T>& ptr)
    {
        return ptr.get();
    }
}

//#include <boost/python.hpp>
//namespace boost{ namespace python{
//    template <class T>
//    struct pointee< std::shared_ptr<T> >
//    {
//        typedef T type;
//    };
//
//}}


#endif//SWGANH_PYTHON_SHAREDPTR_H_
