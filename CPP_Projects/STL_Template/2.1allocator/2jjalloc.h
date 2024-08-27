#ifndef _JJALLOCATOR_H
#define _JJALLOCATOR_H

#include <new> //for placement new
#include <cstddef> //for ptrdiff_t,size_t
#include <cstdlib> //for exit(1)
#include <climits> //for UINT_MAX
#include <iostream> //for cerr

namespace JJ
{
    template<class T>
    inline T* _allocate(ptrdiff_t size,T* t)
    {
        set_new_handle(0);
        //卸载目前的内存分配处理异常函数，这样导致一旦分配失败，C++就会强制性的抛出std::bad_alloc异常，而不是去处理异常函数
        T* tmp=(T*)(::operator new((size_t)(size*sizeof(T))));
        if(tmp==0)
        {
            cerr<<"out of memory\n";
            exit(1);
        }
        return tmp;
    }

    template <class T>
    inline void _deallocate(T* buffer)
    {
        ::operator delete(buffer);
    }

    template <class T1,class T2>
    inline void _construct(T1* p,const T2& value)
    {
        new(p) T1(value);//placement new. invoke ctor of T1
    }

    template <class T>
    inline void _destory(T* ptr)
    {
        ptr->~T();
    }

    template <class T>
    class allocator
    {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_Type;

        template <class U>
        
        struct rebind
        {
            typedef allocator<U> other;
        }

        pointer allocate(size_type n,const void* hint=0)
        {
            return _allocate((difference_Type)n,(pointer)0);
        }

        void deallocate(pointer p,size_type n)
        {
            _deallocate(p);
        }

        void construct(pointer p,const T& value)
        {
            _construct(p,value);
        }

        void destroy(pointer p)
        {
            _destory(p);
        }

        point address(reference x)
        {
            return (pointer)&x;
        }

        const_pointer const_address(const_reference x)
        {
            return (const_pointer)&x;
        }

        size_type max_size() const
        {
            return size_type(UINT_MAX/sizeof(T));
        }
    };

}


#endif