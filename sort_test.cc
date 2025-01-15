#include<algorithm>
#include"deque.h"
#include<iostream>
#include"timer.h"
#include<cstddef>
#include<vector>
#include<random>
#include<stdexcept>
#include<cstddef>
#include<deque>

std::random_device rdv;
std::mt19937_64 eng(rdv());

template<std::size_t t,std::size_t N,std::size_t loops,typename Cont>
std::size_t unit_test(const std::string &s)
{
	
	cqw::timer t1(std::to_string(t)+"\t"+s);
	Cont cont;
	{
		cqw::timer emt("emplace_back time");
		for(std::size_t i(0);i!=N;++i)
			cont.emplace_back(i);
	}
	{
		cqw::timer sttm("sort timer");
		for(std::size_t i(0);i!=loops;++i)
		{
			std::shuffle(cont.begin(),cont.end(),eng);
			std::sort(cont.begin(),cont.end());
			if(!std::is_sorted(cont.cbegin(),cont.cend()))
				throw std::runtime_error("not sorted");
		}
	}
	{
		cqw::timer amt("accumulate time");
		return std::accumulate(cont.cbegin(),cont.cend(),std::size_t(0));
	}
}


int main()
{
	try
	{
		constexpr std::size_t N(100000),loops(1);
		std::cout<<unit_test<0,N,loops,std::vector<std::size_t>>("std::vector")<<'\n';
		std::cout<<unit_test<0,N,loops,cq::deque<std::size_t>>("cq::deque")<<'\n';
		std::cout<<unit_test<0,N,loops,std::deque<std::size_t>>("std::deque")<<'\n';

	}
	catch(const std::exception &ex)
	{
		std::cerr<<ex.what()<<'\n';
	}
}