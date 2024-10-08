#ifndef MYTINYSTL_LIST_H_
#define MYTINYSTL_LIST_H_

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "execptdef.h"

namespace mystl
{
    template<class T> struct list_node_base;
    template<class T> struct list_node;

    template<class T>
    struct node_traits
    {
        typedef list_node_base<T>* base_ptr;
        typedef list_node<T>* node_ptr;
    };

    template<class T>
    struct list_node_base
    {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        base_ptr prev;
        base_ptr next;

        list_node_base()=default;

        node_ptr as_node()
        {
            return static_cast<node_ptr>(self());
        }

        void unlink()
        {
            prev=next=self();
        }

        base_ptr self()
        {
            return static_cast<base_ptr>(&*this);
        }

    };

    template <class T>
    struct list_node:public list_node_base<T>
    {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value;
        
        list_node()=default;
        list_node(const T& v):value(v)
        {
        }

        list_node(T&& v):value(mystl::move(v))
        {

        }

        base_ptr as_base()
        {
            return static_cast<base_ptr>(&*this);
        }

        node_ptr self()
        {
            return static_cast<node_ptr>(&*this);
        }
    };

    template<class T>
    struct lsit_iterator:public mystl::iterator<mystl::bidirectional_iterator_tag,T>
    {
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator<T> self;

        base_ptr node_;

        list_iterator()=default;
        list_iterator(base_ptr x):node_(x)
        {

        }
        list_iterator(node_ptr x):node_(x->as_base())
        {

        }
        list_iterator(const list_iterator& rhs):node_(rhs.node_)
        {

        }

        reference operator*() const
        {
            return node_->as_node()->value;
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        self& operator++()
        {
            MYSTL_DEBUG(node_!=nullptr);
            node_=node_->next;
            return *this;
        }

        self operator++(int)
        {
            self temp= *this
            ++*this;
            return tmp;
        }
        self& operator--()
        {
            node_=node_->prev;
            return *this;
        }
        self operator--(int)
        {
            self tmp=*this;
            --*this;
            return tmp;
        }

        bool operator==(const self& rhs) const
        {
            return node_==rhs.node_;
        }
        bool operator!=(const self& rhs) const
        {
            return node_!=rhs.node_;
        }

    };

    template<class T>
    struct list_const_iterator:public iterator<bidirectional_iterator_tag,T>
    {
        typedef T value_type;
        typedef const T* pointer;
        typdedef const T& reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_const_iterator<T> self;
    
        base_ptr node_;

        list_const_iterator()=default;
        list_const_iterator(base_ptr x):node_(x)
        {}
        list_const_iterator(node_ptr x):node(x->as_base())
        {}
        list_const_iterator(const list_iterator<T>& rhs):node_(rhs.node_)
        {}
        list_const_iterator(const list_const_iterator& rhs):node_(rhs.node_)
        {}

        reference operator*() const
        {
            return node_->as_node()->value();
        }
        pointer operator->() const
        {
            return &(operator*());
        }

        self& operator++()
        {
            node_=node_.next;
            return *this;
        }

        self operator++(int)
        {
            self tmp=*this;
            ++*this;
            return tmp;
        }

        self& operator--()
        {
            node_=node_->prev;
            return *this;
        }

        self operator--(int)
        {
            self tmp=*this;
            node_=node_->prev;
            return tmp;
        }

        bool operator==(const self& rhs) const
        {
            return node_==rhs.node_;
        }

        bool operator!=(const self& rhs) const
        {
            return node_!=rhs.node_;
        }
    };

    template<class  T>
    class list
    {
        public:
            typedef mystl::allocator<T> allocator_type;
            typedef mystl::allocator<T> data_allocator;
            typedef mystl::allocator<list_node_base<T>> base_allocator;
            typedef mystl::allocator<list_node<T>> node_allocator;

            typedef typename allocator_type::value_type value_type;
            typedef typename allocator_type::pointer pointer;
            typedef typename allocator_type::const_pointer const_pointer;
            typedef typename allocator_type::reference reference;
            typedef typename allocator_type::const_reference const_reference;
            typedef typename allocator_type::size_type size_type;
            typedef typename allocator_type::difference_type difference_type;

            typedef list_iterator<T> iterator;
            typedef list_const_iterator<T> const_iterator;
            typedef mystl::reverse_iterator<iterator> reverse_iterator;
            typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

            typedef typename node_traits<T>::base_ptr base_ptr;
            typedef typename node_traits<T>::node_ptr node_ptr;

            allocator_type get_allocator()
            {
                return node_allocator();
            }

        private:
            base_ptr node_;
            size_type size_;

        public:
            list()
            {
                fill_init(0,value_type());
            }

            explicit list(size_type n)
            {
                fill_init(n,value_type());
            }

            list(size_type n,const T& value)
            {
                fill_init(n,value);
            }

            template<class Iter,typename std::enable_if<
                    mystl::is_input_iterator<Iter>::value,int>::type=0> list(Iter first,Iter last)
            {
                copy_init(first,last);
            }

            list(std::initializer_list<T> ilist)
            {
                copy_init(ilist.begin(),ilist.end());
            }

            list(const list& rhs)
            {
                copy_init(rhs.cbegin(),rhs.cend());
            }

            list(list&& rhs) noexcept:node_(rhs.node_),size_(rhs.size_)
            {
                rhs.node_=nullptr;
                rhs.size_=0;
            }

            list& operator+(const list& rhs)
            {
                if(this!=&rhs)
                    assign(rhs.begin(),rhs.end());
                return *this;
            }

            list& operator=(list&& rhs) noexcept
            {
                clear();
                splice(end(),rhs);
                return *this;
            }

            list& operator=(std::initializer_list<T> ilist)
            {
                list tmp(ilist.begin(),ilist.end());
                swap(tmp);
                return *this;
            }

            ~list()
            {
                if(node_)
                {
                    clear();
                    base_allocator::deallocate(node_);
                    node_=nullptr;
                    size_=0;
                }
            }
        public:
            iterator begin() noexcept
            {
                return node_->next;
            }
            const_iterator begin() const noexcept
            {
                return node_->next;
            }
            iterator end() noexcept
            {
                return node_;
            }
            const_iterator end() const noexcept
            {
                return node_;
            }
            reverse_iterator rbegin() noexcept
            {
                return reverse_iterator(end());
            }
            const_reverse_iterator rbegin() const noexcept
            {
                return reverse_iterator(end());
            }
            reverse_iterator rend() noexcept
            {
                return reverse_iterator(begin());
            }
            const_reverse_iterator rend() const noexcept
            {
                return reverse_iterator(begin());
            }

            const_iterator cbeigin() const noexcept
            {
                return begin();
            }
            const_iterator cend() const noexcept
            {
                return end();
            }
            const_reverse_iterator crbegin() const noexcept
            {
                return rbegin();
            }
            const_reverse_iterator crend() const noexcept
            {
                return rend();
            }

            bool empty() const noexcept
            {
                return node_->next ==node_;
            }
            size_type size() const noexcept
            {
                return size_;
            }
            size_type max_size() const noexcept
            {
                return static_cast<size_type>(-1);
            }
            reference front()
            {
                assert(!empty());
                return *begin();
            }
            const_reference front() const
            {
                assert(!empty());
                return *begin()
            }

            reference back()
            {
                assert(!empty())
                return *(--end());
            }
            const_reference back() const
            {
                assert(!empty())
                return *(--end());
            }

            void assign(size_type n,const value_type& value)
            {
                fill_assign(n,value);
            }
            template<class Iter,typename std::enable_if<
                    mystl::is_input_iterator<Iter>::value,int>::type=0>
            void assign(Iter first,Iter last)
            {
                copy_assign(first,last);
            }
            void assign(std::initializer_list<T> ilist)
            {
                copy_assign(ilist.begin(),ilist.end());
            }

            template <class ...Args>
            void emplace_front(Args&& ...args)
            {
                THROW_LENGTH_ERROR_IF(size_>max_size()-1,"list<T>'s size too big");
                auto link_node=create_node(mystl::forward<Args>(args)...);
                link_nodes_at_front(link_node->as_base(),link_node->as_base());
                ++size();
            }
            template <class ...Args>
            void emplace_back(Args&& ...args)
            {
                THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
                auto link_node = create_node(mystl::forward<Args>(args)...);
                link_nodes_at_back(link_node->as_base(), link_node->as_base());
                ++size_;
            }

            template <class ...Args>
            iterator emplace(const_iterator pos, Args&& ...args)
            {
                THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
                auto link_node = create_node(mystl::forward<Args>(args)...);
                link_nodes(pos.node_, link_node->as_base(), link_node->as_base());
                ++size_;
                return iterator(link_node);
            }

            iterator insert(const_iterator pos,const value_type& value)
            {
                THROW_LENGTH_ERROR_IF(size_>max_size()-1,"list<T>'s size too big");
            }
    };


}


#endif