#include"timer.h"
#include<deque>
#include<string>
#include<iostream>
#include<vector>
#include"deque.h"
#include<boost\circular_buffer.hpp>

std::size_t K(50);

template<std::size_t N,typename T,typename ...Args>
std::size_t test(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	{
		cqw::timer t1("push_back"s);
		for(std::size_t i(0);i!=N;++i)
			cont.push_back(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(const auto &ele : cont)
			v^=ele;
	}
	return v;
}

template<std::size_t N,typename T,typename ...Args>
std::size_t test0(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	cont.reserve(N);
	{
		cqw::timer t1("push_back"s);
		for(std::size_t i(0);i!=N;++i)
			cont.push_back(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(const auto &ele : cont)
			v^=ele;
	}
	return v;
}

template<std::size_t N,typename T,typename ...Args>
std::size_t test1(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	cont.reserve(N);
	{
		cqw::timer t1("nonreallocated_push_back"s);
		for(std::size_t i(0);i!=N;++i)
			cont.nonreallocated_push_back(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(const auto &ele : cont)
			v^=ele;
	}
	return v;
}


template<std::size_t N,typename T,typename ...Args>
std::size_t test4(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	cont.reserve(N);
	cont.nonrc_resize(N);
	{
		cqw::timer t1("nonrcresize"s);
		for(std::size_t i(0);i!=N;++i)
			cont[i]=i;
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(const auto &ele : cont)
			v^=ele;
	}
	return v;
}


template<std::size_t N,typename T,typename ...Args>
std::size_t test6(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	{
		cqw::timer t1("emplace_back"s);
		for(std::size_t i(0);i!=N;++i)
			cont.emplace_back(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(std::size_t i(0);i!=cont.bucket_count();++i)
		{
			for(auto k(cont.cbegin(i)),e(cont.cend(i));k!=e;++k)
				v^=*k;
		}
	}
	return v;
}

template<std::size_t N,typename T,typename ...Args>
std::size_t test7(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	{
		cqw::timer t1("emplace_front"s);
		for(std::size_t i(0);i!=N;++i)
			cont.emplace_front(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(std::size_t i(0);i!=cont.bucket_count();++i)
		{
			for(auto k(cont.cbegin(i)),e(cont.cend(i));k!=e;++k)
				v^=*k;
		}
	}
	return v;
}
template<std::size_t N,typename T,typename ...Args>
std::size_t test8(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	{
		cqw::timer t1("emplace_front"s);
		for(std::size_t i(0);i!=N;++i)
			cont.emplace_front(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(auto it(cont.cbegin());it!=cont.cend();++it)
			v^=*it;
	}
	return v;
}

template<std::size_t N,typename T,typename ...Args>
std::size_t test9(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	{
		cqw::timer t1("emplace_front"s);
		for(std::size_t i(0);i!=N;++i)
			cont.emplace_front(i);
	}
	std::size_t v(1);
	{
		cqw::timer t2("loop"s);
		for(std::size_t k(0);k!=K;++k)
		for(auto it(cont.cbegin());it!=cont.cend();++it)
			v^=*it;
	}
	{
		cqw::timer t1("pop_back()"s);
//		for(std::size_t i(0);i!=N;++i)
//			cont.emplace_front(i);
		for(;!cont.empty();cont.pop_back());
	}
	return v;
}

template<std::size_t N,typename T,typename ...Args>
auto test10(Args&& ...args)
{
	using namespace std::string_literals;
	std::string s(std::forward<Args>(args)...);
	std::cerr<<"Testing: "s<<s<<'\n';
	cqw::timer t0("total: "s);
	T cont;
	{
		cqw::timer t1("emplace_back"s);
		for(std::size_t i(0);i!=N;++i)
			cont.emplace_back(i);
	}
	{
		auto it(cont.cbegin()+(cont.size()>>1));
		{
		cqw::timer t1("emplace"s);
		for(std::size_t i(0);i!=N;++i)
			it=cont.emplace(it,i);
		}
		{
		cqw::timer t1("erase"s);
		for(std::size_t i(0);i!=N;++i)
			it=cont.erase(it);
		}
	}
	return cont;
}


int main()
{
	std::size_t constexpr N(100000000),M(200000);
/*	test<N,std::vector<std::uint32_t>>("vector init");
	std::cerr<<test6<N,cq::deque<std::size_t>>("6 cq::deque6")<<"\n\n";
	std::cerr<<test6<N,cq::deque<std::size_t>>("6 cq::deque6")<<"\n\n";

	std::cerr<<test7<N,cq::deque<std::size_t>>("7 cq::deque")<<"\n\n";

	std::cerr<<test0<N,std::vector<std::size_t>>("0 reserve std::vector")<<"\n\n";
	std::cerr<<test<N,std::vector<std::size_t>>("normal std::vector")<<"\n\n";
	std::cerr<<test<N,boost::circular_buffer<std::size_t>>("boost circular_buffer")<<"\n\n";

	std::cerr<<test<N,boost::circular_buffer<std::size_t>>("0 reserve boost circular_buffer")<<"\n\n";*/

	
//	std::cerr<<test7<N,cq::deque<std::size_t>>("7 cq::deque")<<"\n\n";

	auto v1(test10<M,cq::deque<std::size_t>>("10 cq::deque9"));
	auto v2(test10<M,std::deque<std::size_t>>("10 std::deque10"));
	std::cerr<<std::equal(v1.cbegin(),v1.cend(),v2.cbegin(),v2.cend())<<'\n';

}