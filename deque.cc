
#include<iostream>
#include"deque.h"
#include<stdexcept>
#include<exception>
#include<algorithm>

int main()
{
	try
	{
		cq::deque<int> d;
/*		for(auto it(d.crbegin());it!=d.crend();++it)
			std::cout<<*it<<'\n';
		std::sort(d.begin(),d.end());
		for(std::size_t i(0);i!=10000;++i)
			d.emplace_front(i);
		for(std::size_t i(0);i!=10000;++i)
			d.emplace_back(i);
		for(std::size_t i(0);i!=10000;++i)
			d.emplace_back(i);
		for(std::size_t i(0);i!=10000;++i)
			d.emplace_back(i);

		for(std::size_t i(0);i!=14000;++i)
			d.pop_back();
		for(std::size_t i(0);i!=14000;++i)
			d.pop_back();*/

		
		for(std::size_t i(0);i!=4096;++i)
			d.emplace_back(i);
		for(auto const &e : d);
		std::cout<<"not crash\n";
//			std::cout<<e<<'\n';
/*		d.erase(d.cbegin()+2000,d.cend());
		for(auto const & e : d)
			std::cout<<e<<'\n';*/
/*		d.__erase_since(d.cbegin()+2000);
		for(auto const & e : d)
			std::cout<<e<<'\n';*/
//		std::cout<<e1.m-d.rb<<'\n';
//		std::cout<<f<<'\n';
//		cq::deque<int> d1(std::move_if_noexcept(d));

	}
	catch(const std::exception &ex)
	{
		std::cerr<<ex.what()<<'\n';
	}
}