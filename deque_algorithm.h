#pragma once
#include"deque_iterator.h"

namespace std
{

template<bool __C,typename __V,typename __D,typename __P,typename __R,typename __M, typename __Pred >
inline auto for_each(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end, __Pred __fn )
{
	for(;__begin.__m!=__end.__m;__begin.__to_next_block())
		for(auto __i(__begin.__i);__i!=__begin.__e;++__i)
			__fn(*__i);
	for(auto __i(__begin.__i);__i!=__end.__i;++__i)
		__fn(*__i);
	return __fn;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto move(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end,__RAIter __to)
{
	for(;__begin.__i!=__end.__i;__begin.__to_next_block())
	{
		auto const __same_block(__begin.__b==__end.__b);
		auto const __end_block(__same_block?__end.__i:__begin.__e);
		__to=move(__begin.__i,__end_block,__to);
		if(__same_block)
			break;
	}
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto move(__RAIter __begin,__RAIter __end,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __to)
{
	while(__begin!=__end)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __diff1(__end-__begin);
			if(__diff1<__diff2)
			{
				__to.__i=move(__begin,__end,__to.__i);
				break;
			}
			else
			{
				auto const __temp(__begin);
				__to.__i=move(__temp,__begin+=__diff2,__to.__i);
			}
		}
		else
			__to.__to_next_block();
	return __to;
}

template<bool __C0,typename __V,typename __D0,typename __P0,typename __R0,typename __M0,
		bool __C1,typename __D1,typename __P1,typename __R1,typename __M1>
inline auto move(cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __begin,cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __end,cq::__deque_details::__deque_iterator<__C1,__V,__D1,__P1,__R1,__M1> __to)
{
	while(__begin.__i!=__end.__i)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __same_block(__begin.__b==__end.__b);
			auto const __end_block(__same_block?__end.__i:__begin.__e);
			auto const __diff1(__end_block-__begin.__i);
			if(__diff1<=__diff2)
			{
				__to.__i=move(__begin.__i,__end_block,__to.__i);
				if(__same_block)
					break;
				__begin.__to_next_block();
			}
			else
			{
				auto const __temp(__begin.__i);
				__to.__i=move(__temp,__begin.__i+=__diff2,__to.__i);
			}			
		}
		else
			__to.__to_next_block();
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto move_backward(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end,__RAIter __to)
{
	for(;__begin.__i!=__end.__i;__end.__to_previous_block())
	{
		auto const __same_block(__begin.__b==__end.__b);
		auto const __begin_block(__same_block?__begin.__i:__end.__b);
		__to=move_backward(__begin_block,__end.__i,__to);
		if(__same_block)
			break;
	}
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto move_backward(__RAIter __begin,__RAIter __end,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __to)
{
	while(__begin!=__end)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __diff1(__end-__begin);
			if(__diff1<__diff2)
			{
				__to.__i=move(__begin,__end,__to.__i);
				break;
			}
			else
			{
				auto const __temp(__end);
				__to.__i=move_backward(__end-=__diff2,__temp,__to.__i);
			}
		}
		else
			__to.__to_previous_block();
	return __to;
}


template<bool __C0,typename __V,typename __D0,typename __P0,typename __R0,typename __M0,
		bool __C1,typename __D1,typename __P1,typename __R1,typename __M1>
inline auto move_backward(cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __begin,cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __end,cq::__deque_details::__deque_iterator<__C1,__V,__D1,__P1,__R1,__M1> __to)
{
	while(__begin.__i!=__end.__i)
		if(auto const __diff2=__to.__i-__to.__b)
		{
			auto const __same_block(__begin.__b==__end.__b);
			auto const __begin_block(__same_block?__begin.__i:__end.__b);
			auto const __diff1(__end.__i-__begin_block);
			if(__diff1<=__diff2)
			{
				__to.__i=move_backward(__begin_block,__end.__i,__to.__i);
				if(__same_block)
					break;
				__end.__to_previous_block();
			}
			else
			{
				auto const __temp(__end.__i);
				__to.__i=move_backward(__end.__i-=__diff2,__temp,__to.__i);
			}
		}
		else
			__to.__to_previous_block();
	return __to;
}



template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto copy(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end,__RAIter __to)
{
	for(;__begin.__i!=__end.__i;__begin.__to_next_block())
	{
		auto const __same_block(__begin.__b==__end.__b);
		auto const __end_block(__same_block?__end.__i:__begin.__e);
		__to=copy(__begin.__i,__end_block,__to);
		if(__same_block)
			break;
	}
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto copy(__RAIter __begin,__RAIter __end,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __to)
{
	while(__begin!=__end)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __diff1(__end-__begin);
			if(__diff1<__diff2)
			{
				__to.__i=copy(__begin,__end,__to.__i);
				break;
			}
			else
			{
				auto const __temp(__begin);
				__to.__i=copy(__temp,__begin+=__diff2,__to.__i);
			}
		}
		else
			__to.__to_next_block();
	return __to;
}

template<bool __C0,typename __V,typename __D0,typename __P0,typename __R0,typename __M0,
		bool __C1,typename __D1,typename __P1,typename __R1,typename __M1>
inline auto copy(cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __begin,cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __end,cq::__deque_details::__deque_iterator<__C1,__V,__D1,__P1,__R1,__M1> __to)
{
	while(__begin.__i!=__end.__i)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __same_block(__begin.__b==__end.__b);
			auto const __end_block(__same_block?__end.__i:__begin.__e);
			auto const __diff1(__end_block-__begin.__i);
			if(__diff1<=__diff2)
			{
				__to.__i=copy(__begin.__i,__end_block,__to.__i);
				if(__same_block)
					break;
				__begin.__to_next_block();
			}
			else
			{
				auto const __temp(__begin.__i);
				__to.__i=copy(__temp,__begin.__i+=__diff2,__to.__i);
			}			
		}
		else
			__to.__to_next_block();
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto copy_backward(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end,__RAIter __to)
{
	for(;__begin.__i!=__end.__i;__end.__to_previous_block())
	{
		auto const __same_block(__begin.__b==__end.__b);
		auto const __begin_block(__same_block?__begin.__i:__end.__b);
		__to=copy_backward(__begin_block,__end.__i,__to);
		if(__same_block)
			break;
	}
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto copy_backward(__RAIter __begin,__RAIter __end,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __to)
{
	while(__begin!=__end)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __diff1(__end-__begin);
			if(__diff1<__diff2)
			{
				__to.__i=copy(__begin,__end,__to.__i);
				break;
			}
			else
			{
				auto const __temp(__end);
				__to.__i=copy_backward(__end-=__diff2,__temp,__to.__i);
			}
		}
		else
			__to.__to_previous_block();
	return __to;
}


template<bool __C0,typename __V,typename __D0,typename __P0,typename __R0,typename __M0,
		bool __C1,typename __D1,typename __P1,typename __R1,typename __M1>
inline auto copy_backward(cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __begin,cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __end,cq::__deque_details::__deque_iterator<__C1,__V,__D1,__P1,__R1,__M1> __to)
{
	while(__begin.__i!=__end.__i)
		if(auto const __diff2=__to.__i-__to.__b)
		{
			auto const __same_block(__begin.__b==__end.__b);
			auto const __begin_block(__same_block?__begin.__i:__end.__b);
			auto const __diff1(__end.__i-__begin_block);
			if(__diff1<=__diff2)
			{
				__to.__i=copy_backward(__begin_block,__end.__i,__to.__i);
				if(__same_block)
					break;
				__end.__to_previous_block();
			}
			else
			{
				auto const __temp(__end.__i);
				__to.__i=copy_backward(__end.__i-=__diff2,__temp,__to.__i);
			}
		}
		else
			__to.__to_previous_block();
	return __to;
}



template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto uninitialized_move(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end,__RAIter __to)
{
	for(;__begin.__i!=__end.__i;__begin.__to_next_block())
	{
		auto const __same_block(__begin.__b==__end.__b);
		auto const __end_block(__same_block?__end.__i:__begin.__e);
		__to=uninitialized_move(__begin.__i,__end_block,__to);
		if(__same_block)
			break;
	}
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto uninitialized_move(__RAIter __begin,__RAIter __end,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __to)
{
	while(__begin!=__end)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __diff1(__end-__begin);
			if(__diff1<__diff2)
			{
				__to.__i=uninitialized_move(__begin,__end,__to.__i);
				break;
			}
			else
			{
				auto const __temp(__begin);
				__to.__i=uninitialized_move(__temp,__begin+=__diff2,__to.__i);
			}
		}
		else
			__to.__to_next_block();
	return __to;
}

template<bool __C0,typename __V,typename __D0,typename __P0,typename __R0,typename __M0,
		bool __C1,typename __D1,typename __P1,typename __R1,typename __M1>
inline auto uninitialized_move(cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __begin,cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __end,cq::__deque_details::__deque_iterator<__C1,__V,__D1,__P1,__R1,__M1> __to)
{
	while(__begin.__i!=__end.__i)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __same_block(__begin.__b==__end.__b);
			auto const __end_block(__same_block?__end.__i:__begin.__e);
			auto const __diff1(__end_block-__begin.__i);
			if(__diff1<=__diff2)
			{
				__to.__i=uninitialized_move(__begin.__i,__end_block,__to.__i);
				if(__same_block)
					break;
				__begin.__to_next_block();
			}
			else
			{
				auto const __temp(__begin.__i);
				__to.__i=uninitialized_move(__temp,__begin.__i+=__diff2,__to.__i);
			}			
		}
		else
			__to.__to_next_block();
	return __to;
}


template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto uninitialized_copy(cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __begin,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __end,__RAIter __to)
{
	for(;__begin.__i!=__end.__i;__begin.__to_next_block())
	{
		auto const __same_block(__begin.__b==__end.__b);
		auto const __end_block(__same_block?__end.__i:__begin.__e);
		__to=uninitialized_copy(__begin.__i,__end_block,__to);
		if(__same_block)
			break;
	}
	return __to;
}

template<typename __RAIter,bool __C,typename __V,typename __D,typename __P,typename __R,typename __M,
		enable_if_t<is_same_v<random_access_iterator_tag,typename iterator_traits<__RAIter>::iterator_category>>*>
inline auto uninitialized_copy(__RAIter __begin,__RAIter __end,cq::__deque_details::__deque_iterator<__C,__V,__D,__P,__R,__M> __to)
{
	while(__begin!=__end)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __diff1(__end-__begin);
			if(__diff1<__diff2)
			{
				__to.__i=uninitialized_copy(__begin,__end,__to.__i);
				break;
			}
			else
			{
				auto const __temp(__begin);
				__to.__i=uninitialized_copy(__temp,__begin+=__diff2,__to.__i);
			}
		}
		else
			__to.__to_next_block();
	return __to;
}

template<bool __C0,typename __V,typename __D0,typename __P0,typename __R0,typename __M0,
		bool __C1,typename __D1,typename __P1,typename __R1,typename __M1>
inline auto uninitialized_copy(cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __begin,cq::__deque_details::__deque_iterator<__C0,__V,__D0,__P0,__R0,__M0> __end,cq::__deque_details::__deque_iterator<__C1,__V,__D1,__P1,__R1,__M1> __to)
{
	while(__begin.__i!=__end.__i)
		if(auto const __diff2=__to.__e-__to.__i)
		{
			auto const __same_block(__begin.__b==__end.__b);
			auto const __end_block(__same_block?__end.__i:__begin.__e);
			auto const __diff1(__end_block-__begin.__i);
			if(__diff1<=__diff2)
			{
				__to.__i=uninitialized_copy(__begin.__i,__end_block,__to.__i);
				if(__same_block)
					break;
				__begin.__to_next_block();
			}
			else
			{
				auto const __temp(__begin.__i);
				__to.__i=uninitialized_copy(__temp,__begin.__i+=__diff2,__to.__i);
			}			
		}
		else
			__to.__to_next_block();
	return __to;
}

}
