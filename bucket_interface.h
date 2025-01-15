	size_type bucket_size(size_type __pos) const noexcept
	{
		if(!__pos)
			return __end_of_map_of_b-__b;
		else if(__map_of_e-__map_of_b==__pos)
			return __e-__begin_of_map_of_e;
		else
			return __deque_details::__deque_size<value_type>::__n;
	}

	local_iterator begin(size_type __bucket) noexcept
	{
		if(__bucket)
			return __map_of_b[__bucket];
		else
			return __b;
	}
	const_local_iterator cbegin(size_type __bucket) const noexcept
	{
		if(__bucket)
			return __map_of_b[__bucket];
		else
			return __b;
	}
	pointer data(size_type __bucket) noexcept
	{
		if(__bucket)
			return __map_of_b[__bucket];
		else
			return __b;
	}
	const_pointer data(size_type __bucket) const noexcept
	{
		if(__bucket)
			return __map_of_b[__bucket];
		else
			return __b;
	}
	const_local_iterator begin(size_type __bucket) const noexcept
	{
		return cbegin(__bucket);
	}
	local_iterator end(size_type __bucket) noexcept
	{
		if(__map_of_e-__map_of_b==__bucket)
			return __e;
		else
			return __map_of_b[__bucket]+__deque_details::__deque_size<value_type>::__n;
	}
	const_local_iterator cend(size_type __bucket) const noexcept
	{
		if(__map_of_e-__map_of_b==__bucket)
			return __e;
		else
			return __map_of_b[__bucket]+__deque_details::__deque_size<value_type>::__n;
	}
	const_local_iterator end(size_type __bucket) const noexcept
	{
		return cend(__bucket);
	}
	reverse_local_iterator rbegin(size_type __bucket) noexcept
	{
		return reverse_iterator(end(__bucket));
	}
	const_reverse_local_iterator rbegin(size_type __bucket) const noexcept
	{
		return const_reverse_iterator(cend(__bucket));
	}
	const_reverse_local_iterator crbegin(size_type __bucket) const noexcept
	{
		return const_reverse_iterator(cend(__bucket));
	}
	reverse_local_iterator rend(size_type __bucket) noexcept
	{
		return reverse_local_iterator(begin(__bucket));
	}
	const_reverse_local_iterator rend(size_type __bucket) const noexcept
	{
		return const_reverse_local_iterator(cbegin(__bucket));
	}
	const_reverse_local_iterator crend(size_type __bucket) const noexcept
	{
		return const_reverse_local_iterator(cbegin(__bucket));
	}
	size_type bucket_count() const noexcept
	{
		if(__b==__e)
			return 0;
		return __map_of_e-__map_of_b+1;
	}