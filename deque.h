#pragma once
#include"deque_algorithm.h"

namespace cq
{
template<typename __T,typename __Allocator = std::allocator<__T>>
class deque
{
public:
	using value_type = __T;
	using allocator_type = __Allocator;
private:
	using __alloc_traits = std::allocator_traits<allocator_type>;
public:
	using size_type = typename __alloc_traits::size_type;
	using difference_type = typename __alloc_traits::difference_type;
	using reference = value_type&;
	using const_reference = value_type const&;
	using pointer = typename __alloc_traits::pointer;
	using const_pointer = typename __alloc_traits::const_pointer;
	using local_iterator = pointer;
	using const_local_iterator = const_pointer;
	using reverse_local_iterator = std::reverse_iterator<local_iterator>;
	using const_reverse_local_iterator = std::reverse_iterator<const_local_iterator>;

private:
	using __rballoc_type = typename __alloc_traits::template rebind_alloc<pointer>;
	using __rballoc_traits = typename __alloc_traits::template rebind_traits<pointer>;
	using __map_pointer = typename __rballoc_traits::pointer;
	pointer __b,__e;
	__map_pointer __map_of_b,__map_of_e;
	pointer __begin_of_map_of_b,__begin_of_map_of_e,__end_of_map_of_b,__end_of_map_of_e;
	__map_pointer __map_begin,__map_end,__map_reserved_begin,__map_reserved_end;
	allocator_type __a;
	__rballoc_type __rt;
	
public:
	using iterator = cq::__deque_details::__deque_iterator<false,value_type,difference_type,pointer,reference,__map_pointer>;
	using const_iterator = cq::__deque_details::__deque_iterator<true,value_type,difference_type,const_pointer,const_reference,__map_pointer>;

/*	auto __max_bucket_count() const
	{
		return __rballoc_traits::max_size(__rt);
	}*/
private:
	size_type __calculate_growth(size_type __required_count) const
	{
		__required_count<<=1;
		auto constexpr __max_bucket_count(__rballoc_traits::max_size(__rt));
		if(__required_count<__max_bucket_count)
			return __required_count;
		return __max_bucket_count;
	}
	void __more_buckets_before_begin(size_type __count)
	{
	}

	void __more_buckets_at_end(size_type __count)
	{
		if(__map_end-__map_reserved_end<__count)
		{
			size_type const __required_count(__map_reserved_end-__map_reserved_begin+__count);
			auto const __new_bucket_size(__calculate_growth(__required_count));
			auto const __new_map(__rballoc_traits::allocate(__rt,__new_bucket_size));
			auto __new_reserved_size(__new_bucket_size-__count);
			if((__required_count>>2) <__new_reserved_size)
				__new_reserved_size=__required_count>>2;
			auto const __new_reserved_begin(__new_map+__new_reserved_size);
			auto const __new_reserved_end=std::uninitialized_move(__map_reserved_begin,__map_reserved_end,__new_reserved_begin);
			auto __i(__new_reserved_end);
			try
			{
				for(auto __i,__ei(__i+__count);i!=__ei;++__i)
					__rballoc_traits::construct(__rt,__i,__alloc_traits::allocate(__a,__deque_details::__deque_size<value_type>::__n));
			}
			catch(...)
			{
				for(;__i!=__new_reserved_end;__alloc_traits::destroy(__a,--__i,__deque_details::__deque_size<value_type>::__n));
				throw;
			}
			__map_of_b = __new_reserved_begin+(__map_of_b-__map_reserved_begin);
			__map_of_e = __new_reserved_begin+(__map_of_e-__map_reserved_begin);
			__map_reserved_begin = __new_reserved_begin;
			__map_reserved_end = __new_reserved_end;
			__map_begin = __new_map;
			__map_end = __new_map + __new_bucket_size;
		}
		else
		{
			auto __i(__map_reserved_end);
			try
			{
				for(auto __i,__ei(__i+__count);i!=__ei;++__i)
					__rballoc_traits::construct(__rt,__i,__alloc_traits::allocate(__a,__deque_details::__deque_size<value_type>::__n));
			}
			catch(...)
			{
				for(;__i!=__map_reserved_end;__alloc_traits::destroy(__a,--__i,__deque_details::__deque_size<value_type>::__n));
				throw;
			}
		}
	}

	void __clean() noexcept
	{
		if(!__map_reserved_begin)
			return;
		for(auto __i(__map_begin);__i<__map_of_b;++__i)
		{
			__alloc_traits::deallocate(__a,*__i,__deque_details::__deque_size<value_type>::__n);
			__rballoc_traits::destroy(__rt,__i);
		}
		if(__map_of_b==__map_of_e)
		{
			for(auto __i(__b);__i!=__e;++__i)
				__alloc_traits::destroy(__a,__i);
		}
		else
		{
			for(auto __i(__b);__i!=__end_of_map_of_b;++__i)
				__alloc_traits::destroy(__a,__i);
			__alloc_traits::deallocate(__a,__begin_of_map_of_b,__deque_details::__deque_size<value_type>::__n);
			__rballoc_traits::destroy(__rt,__map_of_b);
			for(auto __i(__map_of_b+1);__i<__map_of_e;++__i)
			{
				auto __start(*__i);
				for(auto __j(__start),__je(__j+__deque_details::__deque_size<value_type>::__n);__j!=__je;++__j)
					__alloc_traits::destroy(__a,__j);
				__alloc_traits::deallocate(__a,__start,__deque_details::__deque_size<value_type>::__n);
				__rballoc_traits::destroy(__rt,__i);
			}
			for(auto __i(__begin_of_map_of_e);__i!=__e;++__i)
				__alloc_traits::destroy(__a,__i);
		}
		__alloc_traits::deallocate(__a,__begin_of_map_of_e,__deque_details::__deque_size<value_type>::__n);
		__rballoc_traits::destroy(__rt,__map_of_e);
		for(auto __i(__map_of_e+1);__i<__map_end;++__i)
		{
			__alloc_traits::deallocate(__a,*__i,__deque_details::__deque_size<value_type>::__n);
			__rballoc_traits::destroy(__rt,__i);
		}
		__rballoc_traits::deallocate(__rt,__map_reserved_begin,__map_reserved_end-__map_reserved_begin);
	}
public:
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	explicit deque() noexcept(noexcept(allocator_type())):deque(allocator_type()){}
	explicit deque(allocator_type const& __alloc) noexcept:__b(nullptr),__e(nullptr),__map_of_b(nullptr),__map_of_e(nullptr),
								__begin_of_map_of_b(nullptr),__begin_of_map_of_e(nullptr),__end_of_map_of_b(nullptr),__end_of_map_of_e(nullptr),
								__map_begin(nullptr),__map_end(nullptr),__map_reserved_begin(nullptr),__map_reserved_end(nullptr),
								__a(__alloc),__rt(__a){}
	size_type max_size() const noexcept
	{
		return __alloc_traits::max_size(__a);
	}
	bool empty() const noexcept
	{
		return __b==__e;
	}
	void clear() noexcept
	{
		if(__map_of_b==__map_of_e)
		{
			for(auto __i(__b);__i!=__e;++__i)
				__alloc_traits::destroy(__a,__i);
			__b=__e=__begin_of_map_of_b;
		}
		else
		{
			for(auto __i(__b);__i!=__end_of_map_of_b;++__i)
				__alloc_traits::destroy(__a,__i);
			for(auto __i(__map_of_b+1);__i<__map_of_e;++__i)
				for(auto __j(*__i),__je(__j+__deque_details::__deque_size<value_type>::__n);__j!=__je;++__j)
					__alloc_traits::destroy(__a,__j);
			for(auto __i(__begin_of_map_of_e);__i!=__e;++__i)
				__alloc_traits::destroy(__a,__i);
			__begin_of_map_of_e=__b=__e=__begin_of_map_of_b;
			__map_of_e=__map_of_b;
			__end_of_map_of_e=__end_of_map_of_b;
		}
	}
	size_type size() const noexcept
	{
		return (__map_of_e-__map_of_b)*__deque_details::__deque_size<value_type>::__n+(__begin_of_map_of_b-__b)+(__e-__begin_of_map_of_e);
	}
	const_reference front() const noexcept
	{
		return *__b;
	}
	const_reference back() const noexcept
	{
		if(__e==__begin_of_map_of_e)
			return __map_of_e[-1][__deque_details::__deque_size<value_type>::__mmn];
		return __e[-1];
	}
	reference front() noexcept
	{
		return *__b;
	}
	reference back() noexcept
	{
		if(__e==__begin_of_map_of_e)
			return __map_of_e[-1][__deque_details::__deque_size<value_type>::__mmn];
		return __e[-1];
	}
	allocator_type get_allocator() const
	{
		return __a;
	}
	const_reference operator[](size_type __pos) const noexcept
	{
		__pos+=__b-__begin_of_map_of_b;
		return __map_of_b[__pos/__deque_details::__deque_size<value_type>::__n][__pos%__deque_details::__deque_size<value_type>::__n];
	}
	reference operator[](size_type __pos) noexcept
	{
		__pos+=__b-__begin_of_map_of_b;
		return __map_of_b[__pos/__deque_details::__deque_size<value_type>::__n][__pos%__deque_details::__deque_size<value_type>::__n];
	}
	const_reference at(size_type __pos) const
	{
		if(__pos<size())
			return operator[](__pos);
		throw std::out_of_range("deque<__T>::at out of range");	
	}
	reference at(size_type __pos)
	{
		if(__pos<size())
			return operator[](__pos);
		throw std::out_of_range("deque<__T>::at out of range");
	}
	iterator begin() noexcept
	{
		return iterator{__b,__begin_of_map_of_b,__end_of_map_of_b ,__map_of_b};
	}
	const_iterator cbegin() const noexcept
	{
		return const_iterator{__b,__begin_of_map_of_b,__end_of_map_of_b,__map_of_b};
	}
	const_iterator begin() const noexcept
	{
		return cbegin();
	}
	iterator end() noexcept
	{
		return iterator{__e,__begin_of_map_of_e,__begin_of_map_of_e+__deque_details::__deque_size<value_type>::__n,__begin_of_map_of_e};
	}
	const_iterator cend() const noexcept
	{
		return const_iterator{__e,__begin_of_map_of_e,__begin_of_map_of_e+__deque_details::__deque_size<value_type>::__n,__map_of_e};
	}
	const_iterator end() const noexcept
	{
		return cend();
	}
	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}
	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}
	iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}
	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	template<typename ...__Args>
	reference emplace_back(__Args&& ...__args)
	{
		__alloc_traits::construct(__a,__e,std::forward<__Args>(__args)...);
		if(__e==__end_of_map_of_e)
		{
			__more_buckets_at_end(1);
			reference __temp(*__e);
			__end_of_map_of_e=(__e=__begin_of_map_of_e=*++__map_of_e)+__deque_details::__deque_size<value_type>::__mmn;
			return __temp;
		}
		return *__e++;
	}
	void pop_back()
	{
		if(__e==__begin_of_map_of_e)
			__alloc_traits::destroy(__a,__end_of_map_of_e=__e=(__begin_of_map_of_e=*--__map_of_e)+__deque_details::__deque_size<value_type>::__mmn);
		else
			__alloc_traits::destroy(__a,--__e);
	}
	void push_back(const_reference __r)
	{
		emplace_back(__r);
	}
	void shrink_to_fit()
	{
		if(1<__map_of_b-__map_reserved_begin||1<__map_reserved_end-__map_of_e)
		{
			auto const __cap(__map_of_e-__map_of_b+1);
			auto __newp(__rballoc_traits::allocate(__rt,__cap));
			auto __i(__newp);
			try
			{
				for(auto __j(__map_of_b);__j!=__map_of_e;++__j,++__i)
					__rballoc_traits::construct(__rt,__i,std::move_if_noexcept(*__j));
			}
			catch(...)
			{
				for(;__i!=__newp;--__i)
					__rballoc_traits::destroy(__rt,__i-1);
				__rballoc_traits::deallocate(__rt,__newp,__cap);
				throw;
			}
			for(auto __i(__map_begin);__i<__map_of_b;++__i)
			{
				__alloc_traits::deallocate(__a,*__i,__deque_details::__deque_size<value_type>::__n);
				__rballoc_traits::destroy(__rt,__i);
			}
			
			for(auto __i(__map_of_b);__i<=__map_of_e;++__i)
				__rballoc_traits::destroy(__rt,__i);
			for(auto __i(__map_of_e+1);__i<__map_end;++__i)
			{
				__alloc_traits::deallocate(__a,*__i,__deque_details::__deque_size<value_type>::__n);
				__rballoc_traits::destroy(__rt,__i);
			}
			__rballoc_traits::deallocate(__rt,__map_reserved_begin,__map_reserved_end-__map_reserved_begin);
			__map_reserved_begin=__map_begin=__map_of_b=__newp;
			__map_of_e=__newp+__cap-1;
			__map_end=__map_reserved_end=__newp+__cap;
		}
	}
	template<typename ...__Args>
	reference emplace_front(__Args&& ...__args)
	{
		if(__b==__begin_of_map_of_b)
		{
			__more_buckets_before_begin(1);
			reference __temp(*__e);
			__end_of_map_of_e=(__e=__begin_of_map_of_e=*++__map_of_e)+__deque_details::__deque_size<value_type>::__mmn;
			return __temp;
		}
		__alloc_traits::construct(__a,__b-1,std::forward<__Args>(__args)...);
		return *--__b;
	}
	void pop_front()
	{
		__alloc_traits::destroy(__a,__b);
		if(++__b==__end_of_map_of_b)
			__end_of_map_of_b=(__b=(__begin_of_map_of_b=*++__map_of_b))+__deque_details::__deque_size<value_type>::__n;
	}
	void push_front(const_reference __r)
	{
		emplace_front(__r);
	}
	deque(deque&& __o) noexcept : __b(__o.__b),__e(__o.__e),__map_of_b(__o.__map_of_b),__map_of_e(__o.__map_of_e),__begin_of_map_of_b(__o.__begin_of_map_of_b),__begin_of_map_of_e(__o.__begin_of_map_of_e),__end_of_map_of_b(__o.__end_of_map_of_b),__end_of_map_of_e(__o.__end_of_map_of_e),__map_begin(__o.__map_begin),__map_end(__o.__map_end),__map_reserved_begin(__o.__map_reserved_begin),__map_reserved_end(__o.__map_reserved_end),__a(std::move(__a))
	{
		__o.__b=__o.__e=nullptr;
		__o.__map_of_b=__o.__map_of_e=nullptr;
		__o.__begin_of_map_of_b=__o.__begin_of_map_of_e=__o.__end_of_map_of_b=__o.__end_of_map_of_e=nullptr;
		__o.__map_begin=__o.__map_end=__o.__map_reserved_begin=__o.__map_reserved_end=nullptr;
	}
	deque(deque&& __o, __Allocator const& __alloc):__b(__o.__b),__e(__o.__e),__map_of_b(__o.__map_of_b),__map_of_e(__o.__map_of_e),__begin_of_map_of_b(__o.__begin_of_map_of_b),__begin_of_map_of_e(__o.__begin_of_map_of_e),__end_of_map_of_b(__o.__end_of_map_of_b),__end_of_map_of_e(__o.__end_of_map_of_e),__map_begin(__o.__map_begin),__map_end(__o.__map_end),__map_reserved_begin(__o.__map_reserved_begin),__map_reserved_end(__o.__map_reserved_end),__a(__alloc)
	{
		__o.__b=__o.__e=nullptr;
		__o.__map_of_b=__o.__map_of_e=nullptr;
		__o.__begin_of_map_of_b=__o.__begin_of_map_of_e=__o.__end_of_map_of_b=__o.__end_of_map_of_e=nullptr;
		__o.__map_begin=__o.__map_end=__o.__map_reserved_begin=__o.__map_reserved_end=nullptr;
	}
	deque& operator=(deque&& __o) noexcept(__alloc_traits::propagate_on_container_move_assignment::__value)
	{
		if(this!=&__o)
		{
			__clean();
			__b=__o.__b;
			__e=__o.__e;
			__map_of_b=__o.__map_of_b;
			__map_of_e=__o.__map_of_e;
			__begin_of_map_of_b=__o.__begin_of_map_of_b;
			__end_of_map_of_b=__o.__end_of_map_of_b;
			__begin_of_map_of_e=__o.__begin_of_map_of_e;
			__end_of_map_of_e=__o.__end_of_map_of_e;
			__map_begin=__o.__map_begin;
			__map_end=__o.__map_end;
			__map_reserved_begin=__o.__map_reserved_begin;
			__map_reserved_end=__o.__map_reserved_end;
			__a=std::move(__a);
			__rt=std::move(__rt);
			__o.__b=__o.__e=nullptr;
			__o.__map_of_b=__o.__map_of_e=nullptr;
			__o.__begin_of_map_of_b=__o.__begin_of_map_of_e=__o.__end_of_map_of_b=__o.__end_of_map_of_e=nullptr;
			__o.__map_begin=__o.__map_end=__o.__map_reserved_begin=__o.__map_reserved_end=nullptr;
		}
		return *this;
	}
	deque(deque const& __dq):deque()
	{
		for(auto const &__e : __dq)
			emplace_back(__e);
	}
	deque& operator=(deque const& __dq)
	{
		clear();
		for(auto const &__e : __dq)
			emplace_back(__e);
	}
	template<typename __Iter>
	deque(__Iter __c_begin,__Iter __c_end,__Allocator const& __alloc = __Allocator()):deque(__alloc)
	{
		std::for_each(__c_begin,__c_end,[&](auto &ele)
		{
			emplace_back(ele);
		});
	}
	deque(std::initializer_list<__T> __ilist,__Allocator const& __alloc = __Allocator()):deque(__ilist.begin(),__ilist.end(),__alloc){}
	deque& operator=(std::initializer_list<__T> __ilist)
	{
		clear();
		for(auto const& __e : __ilist)
			emplace_back(__e);	
	}
	deque(size_type __count, const_reference __value = __T(),__Allocator const& __alloc = __Allocator()):deque(__alloc)
	{
		for(size_type __i(0);__i!=__count;++__i)
			emplace_back(__value);
	}
	explicit deque(size_type __count,__Allocator const& __alloc = __Allocator()):deque(__alloc)
	{
		for(size_type __i(0);__i!=__count;++__i)
			emplace_back();
	}
	void assign(size_type __count,const_reference __value)
	{
		clear();
		for(size_type __i(0);__i!=__count;++__i)
			emplace_back(__value);
	}
	template<typename __Iter>
	void assign(__Iter __c_begin,__Iter __c_end)
	{
		clear();
		for(;__c_begin!=__c_end;++__c_begin)
			emplace_back(*__c_begin);
	}
	void assign(std::initializer_list<__T> __ilist)
	{
		assign(__ilist.begin(),__ilist.end());
	}
	~deque()
	{
		__clean();
	}
	template<typename ...__Args>
	auto emplace(const_iterator __cit,__Args&& ...__args)
	{
		auto& __it(static_cast<iterator&>(__cit));
		size_type __pos(__it-begin());
		if((__pos<<1)<size())
		{
			emplace_front(std::move_if_noexcept(front()));
			auto __front1(begin());
			++__front1;
			auto __front2(__front1);
			++__front2;
			auto __pos1(__it=begin()+__pos);
			std::move(__front2,++__pos1,__front1); 
		}
		else
		{
			emplace_back(std::move_if_noexcept(back()));
			auto __back1(end());
			--__back1;
			auto __back2(__back1);
			--__back2;
			std::move_backward((__it=begin()+__pos),__back2,__back1);
		}
		__alloc_traits::construct(__a,__it.__i,std::forward<__Args>(__args)...);
		return __it;
	}
	iterator insert(const_iterator __cit,const_reference __value)
	{
		return emplace(__cit,__value);
	}
	iterator insert(const_iterator __cit,value_type &&__value)
	{
		return emplace(__cit,std::move(__value));
	}
/*	iterator insert(const_iterator __cit,size_type __count,const_reference __value)
	{
		size_type __pos(__cit-cbegin());
		if((__pos<<1)<size())
		{
			for(size_type __i(0);__i!=__count;++__i)
				emplace_front(__value);
			auto const __new_after_pos(begin()+(__pos+__count));
			std::rotate(begin(),begin()+__count,__new_after_pos);
			return __new_after_pos;
		}
		else
		{
			for(size_type __i(0);__i!=__count;++__i)
				emplace_back(__value);
			auto const new_it(begin()+__pos);
			std::rotate(new_it,end()-__count,end());
			return new_it;
		}
	}
	template<typename __Iter>
	iterator insert(const_iterator __cit,__Iter __c_begin,__Iter __c_end)
	{
	}
	iterator insert(const_iterator __cit,std::initializer_list<__T> __ilist)
	{
		return insert(__cit,__ilist.begin(),__ilist.end());
	}*/
	void __append(__map_pointer __required)
	{
/*		if(__map_reserved_end<=__required)
		{}
		for(size_type __i(__map_end);__i!=__map_required_size;++__i)*/
	}
	void __erase_since(iterator __hi)
	{
		std::for_each(__hi,end(),[&](auto& __e)
		{
			__alloc_traits::destroy(__a,std::addressof(__e));
		});
		__e=__hi.__i;
		__begin_of_map_of_e=__hi.__b;
		__end_of_map_of_e=__hi.__e;
		__map_of_e=__hi.__m;
	}
	void __erase_before(iterator __he)
	{
		std::for_each(begin(),__he,[&](auto& __e)
		{
			__alloc_traits::destroy(__a,std::addressof(__e));
		});
		__b=__he.__i;
		__begin_of_map_of_b=__he.__b;
		__end_of_map_of_b=__he.__e;
		__map_of_b=__he.__m;
	}

	iterator erase(const_iterator __c_begin,const_iterator __c_end)
	{
		auto const& __begin(static_cast<iterator const&>(__c_begin));
		auto const& __end(static_cast<iterator const&>(__c_end));
		if(__begin-begin()<end()-__end)
		{
			__erase_before(std::move_backward(begin(),__begin,__end));
			return __end;
		}
		else
		{
			__erase_since(std::move(__end,end(),__begin));
			return __begin;
		}
	}
	iterator erase(const_iterator __cit)
	{
		auto& __it(static_cast<iterator&>(__cit));
		size_type const __pos(__it-begin());
		auto __next(__it);
		++__next;
		if((__pos<<1)<size())
		{
			std::move_backward(begin(),__it,__next);
			pop_front();
			return __next;
		}
		else
		{
			std::move(__next,end(),__it);
			pop_back();
			return __it;
		}
	}

	void resize(size_type __count)
	{
		auto __sz(size());
		for(;__sz<__count;++__sz)
			emplace_back();
		for(;__count<__sz;++__count)
			pop_back();
	}
	void resize(size_type __count,const_reference __value)
	{
		auto __sz(size());
		for(;__sz<__count;++__sz)
			emplace_back(__value);
		for(;__count<__sz;++__count)
			pop_back();
	}
	void swap(deque& __o) noexcept(__alloc_traits::is_always_equal::__value)
	{
		using std::swap;
		swap(__b,__o.__b);
		swap(__e,__o.__e);
		swap(__map_of_b,__o.__map_of_b);
		swap(__map_of_e,__o.__map_of_e);
		swap(__begin_of_map_of_b,__o.__begin_of_map_of_b);
		swap(__end_of_map_of_b,__o.__end_of_map_of_b);
		swap(__begin_of_map_of_e,__o.__begin_of_map_of_e);
		swap(__end_of_map_of_e,__o.__end_of_map_of_e);
		swap(__map_begin,__o.__map_begin);
		swap(__map_end,__o.__map_end);
		swap(__map_reserved_begin,__o.__map_reserved_begin);
		swap(__a,__o.__a);
		swap(__rt,__o.__rt);
	}
};

template<typename __T,typename __Allocator>
inline void swap(deque<__T,__Allocator>& __lhs,deque<__T,__Allocator>& __rhs) noexcept(noexcept(__lhs.swap(__rhs)))
{
	__lhs.swap(__rhs);
}

template<typename __T,typename __Allocator>
inline bool operator==(deque<__T,__Allocator> const& __a,deque<__T,__Allocator> const& __b){return std::equal(__a.cbegin(),__a.cend(),__b.cbegin(),__b.cend());}
template<typename __T,typename __Allocator>
inline bool operator!=(deque<__T,__Allocator> const& __a,deque<__T,__Allocator> const& __b){return !(__a==__b);}
template<typename __T,typename __Allocator>
inline bool operator<(deque<__T,__Allocator> const& __a,deque<__T,__Allocator> const& __b){return std::lexicographical_compare(__a.cbegin(),__a.cend(),__b.cbegin(),__b.cend());}
template<typename __T,typename __Allocator>
inline bool operator>(deque<__T,__Allocator> const& __a,deque<__T,__Allocator> const& __b){return __b<__a;}
template<typename __T,typename __Allocator>
inline bool operator<=(deque<__T,__Allocator> const& __a,deque<__T,__Allocator> const& __b){return !(__b<__a);}
template<typename __T,typename __Allocator>
inline bool operator>=(deque<__T,__Allocator> const& __a,deque<__T,__Allocator> const& __b){return !(__a<__b);}
}
