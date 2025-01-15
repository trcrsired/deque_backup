#pragma once
#include<memory>
#include<iterator>
#include<type_traits>
#include<stdexcept>
#include<algorithm>


namespace cq::__deque_details
{

template<typename __T,std::size_t __bytes = 1<<12>	//current memory is large enough and there is no reason this __value still remains 4096 like in 1990s
struct __deque_size
{
	static inline std::size_t constexpr __n=sizeof(__T)<__bytes?__bytes/sizeof(__T):1;
	static inline auto constexpr __mmn=__n-1;
};

template<bool __C,typename __V,typename __D,typename __P,typename __R,typename __map_pointer>
struct __deque_iterator
{
	using value_type = __V;
	using difference_type = __D;
	using pointer = __P;
	using reference = __R;
	using iterator_category = std::random_access_iterator_tag;
	using size_type = typename std::make_unsigned<difference_type>::type;
	pointer __i,__b,__e;
	__map_pointer __m;

	void __to_next_block()
	{
		__e=(__i=__b=*++__m)+__deque_size<value_type>::__n;
	}

	void __to_previous_block()
	{
		__e=__i=((__b=*--__m)+__deque_size<value_type>::__n);
	}

	__deque_iterator& operator++()
	{
		if(++__i!=__e)
			__e=(__i=__b=*++__m)+__deque_size<value_type>::__n;
		return *this;
	}
	__deque_iterator operator++(int)
	{
		auto __temp(*this);
		++*this;
		return __temp;
	}
	__deque_iterator& operator--()
	{
		if(__i==__b)
			__i=(__e=(__b=*--__m)+__deque_size<value_type>::__n)-1;
		else
			--__i;
		return *this;
	}
	__deque_iterator operator--(int) const
	{
		auto __temp(*this);
		--*this;
		return __temp;
	}
		
	reference operator*() const
	{
		return *__i;
	}
	
	pointer operator->() const
	{
		return __i;
	}

	__deque_iterator& operator+=(difference_type __d)
	{
		size_type const __r(__i-__b+__d);
		if(__r<__deque_size<value_type>::__n)
			__i+=__d;
		else
		{
			if(difference_type(__r)<0)
			{
				size_type const __mr(__deque_size<value_type>::__n-__r);
				__e=(__b=*(__m-=__mr/__deque_size<value_type>::__n))+__deque_size<value_type>::__n;
				__i=__e-__mr%__deque_size<value_type>::__n;
			}
			else
			{
				__e=(__b=*(__m+=__r/__deque_size<value_type>::__n))+__deque_size<value_type>::__n;
				__i=__b+__r%__deque_size<value_type>::__n;
			}
		}
		return *this;
	}
	__deque_iterator& operator-=(difference_type __d)
	{
		return *this+=-__d;
	}
	reference operator[](difference_type __d) const
	{
		size_type const __r(__i-__b+__d);
		if(__r<__deque_size<value_type>::__n)
			return __b[__r];
		else
		{
			if(difference_type(__r)<0)
			{
				size_type const __mr(__deque_size<value_type>::__n-__r);
				return __m[-__mr/__deque_size<value_type>::__n][__deque_size<value_type>::__n-__mr%__deque_size<value_type>::__n];
			}
			else
				return __m[__r/__deque_size<value_type>::__n][__r%__deque_size<value_type>::__n];
		}
	}
	template<typename __P1,typename __R1>
	operator __deque_iterator<!__C,__V,__D,__P1,__R1,__map_pointer> const&() const
	{
		return reinterpret_cast<__deque_iterator<!__C,__V,__D,__P1,__R1,__map_pointer>&>(*this);
	}

	template<typename __P1,typename __R1>
	operator __deque_iterator<!__C,__V,__D,__P1,__R1,__map_pointer>&()
	{
		return reinterpret_cast<__deque_iterator<!__C,__V,__D,__P1,__R1,__map_pointer>&>(*this);
	}
};

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline bool operator==(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	return __a.__i==__b.__i;
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline bool operator!=(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	return __a.__i!=__b.__i;
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline __D operator-(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	return static_cast<__D>(__deque_size<__V>::__n)*(__a.__m-__b.__m)+(__a.__i-__a.__b)-(__b.__i-__b.__b);		
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,typename __Diff>
inline auto operator+(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__Diff __d)
{
	auto __temp(__a);
	return __temp+=__d;
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,typename __Diff>
inline auto operator+(__Diff __d,__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a)
{
	auto __temp(__a);
	return __temp+=__d;
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,typename __Diff>
inline auto operator-(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__Diff __d)
{
	auto __temp(__a);
	return __temp-=__d;
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline bool operator<(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	if(__a.__m==__b.__m)
		return __a.__i<__b.__i;
	else
		return __a.__m<__b.__m;

}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline bool operator>(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	return __b<__a;
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline bool operator<=(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	return !(__b<__a);
}

template<bool __C1,typename __V,typename __D,typename __P0,typename __R0,typename __M,
		bool __C2,typename __P1,typename __R1>
inline bool operator>=(__deque_iterator<__C1,__V,__D,__P0,__R0,__M> const&__a,__deque_iterator<__C2,__V,__D,__P1,__R1,__M> const&__b)
{
	return !(__a<__b);
}

}
