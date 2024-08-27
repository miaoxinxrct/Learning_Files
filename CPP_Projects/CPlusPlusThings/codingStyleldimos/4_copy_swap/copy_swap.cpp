#include <iostream>


namespace A
{
    template<typename T>
    class smart_ptr{
    public:
        smart_ptr() noexcept:_ptr(new T())
        {

        }

        smart_ptr(const T& ptr):_ptr(new T(ptr))
        {

        }

        smart_ptr(smart_ptr& rhs) noexcept
        {
            _ptr=rhs.release();
        }

        void swap(smart_ptr &rhs) noexcept
        {
            using std::swap;
            swap(_ptr,rhs.ptr);
        }

        T* release() noexcept
        {
            T* ptr=_ptr;
            _ptr=nullptr;
            return ptr;
        }

        T* get() const noexcept
        {
            return _ptr;
        }
    private:
        T* _ptr;
    };
}