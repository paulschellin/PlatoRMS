#pragma once


#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>
#include <string>

#include <utility>

#include<infix_iterator.h>


/*
	User-defined std::string literal
 */
std::string operator "" _s(const char* str, size_t /*length*/)
{
	return std::string(str);
}



/*
	Advance a copy instead of the original iterator

	It's just like std::advance, just non-modifying to it's input iterator.
 */
template <class InputIterator, class Distance>
InputIterator
advance_copy (const InputIterator& it, Distance n)
{
	auto result = it;
	std::advance(result, n);
	return result;
}



template <typename ContainerT, typename Distance>
typename ContainerT::iterator
iter_at(ContainerT& container, Distance n)
{
	return advance_copy(container.begin(), n);
}


/*
	This is a somewhat awkward function which is used when two
	containers have indices which are linked. If you have an iterator
	to the right element in container A, this function gives you the
	iterator to the element associated with that in container B.
  */
template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator2
get_equivalent_iterator ( ForwardIterator1 first1		//	beginning of first range
						, ForwardIterator1 position1	//	position of first range
						, ForwardIterator2 first2		//	beginning of second range
						)
{
	return advance_copy(first2, std::distance(first1, position1));
}



template <typename ForwardIterator1, typename ForwardIterator2>
bool
iter_is_in_range (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 it)
{
	while (first1 != last1)
	{
		if (it == first1)
			return true;
		++first1;
	}

	return false;
}


template <typename PairT>
struct pair_first_equal_to {
	typedef typename PairT::first_type CompT;
	CompT value;

	pair_first_equal_to (const CompT& val)
		: value(val)
	{}
	
	bool operator() (const PairT& x) const {return x.first == value;}
	typedef PairT first_argument_type;
	
	typedef bool result_type;
};

template <typename PairT>
pair_first_equal_to <PairT>
make_pair_first_equal_to (const typename PairT::first_type& val)
{
	return pair_first_equal_to<PairT>(val);
}

template <typename PairT>
struct pair_second_equal_to {
	typedef typename PairT::second_type CompT;
	CompT value;

	pair_second_equal_to (const CompT& val)
		: value(val)
	{}
	
	bool operator() (const PairT& x) const {return x.second == value;}
	typedef PairT first_argument_type;
	
	typedef bool result_type;
};

template <typename PairT>
pair_second_equal_to <PairT>
make_pair_second_equal_to (const typename PairT::second_type& val)
{
	return pair_second_equal_to<PairT>(val);
}


/*
template <typename T, typename CharT>
//auto
std::ostream_iterator<T, CharT>
make_ostream_iterator (T& x, const CharT* d)
//	-> decltype( std::declval(std::ostream_iterator<T>(x, d)) )
{
	return std::ostream_iterator<T>(x, d);
}
*/

template <typename PairT>
struct pair_printer {
	
	const PairT& pairRef;

	const std::string firstStr;
	const std::string secondStr;


	pair_printer () = delete;

	pair_printer (const PairT& pair_in
				, const std::string& firstStr_in = "first"_s
				, const std::string& secondStr_in = "second"_s)
		: pairRef(pair_in)
		, firstStr(firstStr_in)
		, secondStr(secondStr_in)
	{
	}


	friend
	std::ostream&
	operator<< (std::ostream& os, const pair_printer& p)
	{
		os << "{" << p.firstStr << ": "
			<< p.pairRef.first << ", "
			<< p.secondStr << ": "
			<< p.pairRef.second << "}";
		return os;
	}


};

template <typename PairT>
pair_printer<PairT>
make_pair_printer(PairT pair_in
				, const std::string& firstStr_in = "first"_s
				, const std::string& secondStr_in = "second"_s)
{
			return pair_printer<PairT>(pair_in, firstStr_in, secondStr_in);
}


struct container_format_options {
	const std::string container_open;
	const std::string container_close;

	const bool prefix_element_with_index;
	const std::string prefix_separator;
	const std::string element_separator;

	container_format_options ()
		: container_open("{")
		, container_close("}")
		, prefix_element_with_index(false)
		, prefix_separator(": ")
		, element_separator(", ")
	{ }
};

template <typename ContainerT>
struct container_printer {

	typedef typename ContainerT::value_type value_type;
	const ContainerT& containerRef;

	//const std::string

	container_format_options formatOptions;


	container_printer (const ContainerT& container

						)
		: containerRef(container)
		, formatOptions()
	{

	}

	friend
	std::ostream&
	operator<< (std::ostream& os, const container_printer& p)
	{
		auto& opts = p.formatOptions;
		os << opts.container_open;
		
		/*
		std::copy(p.containerRef.begin()
					, p.containerRef.end()
					, infix_ostream_iterator<value_type>(os, opts.element_separator.c_str()));
		*/

		for (auto it = p.containerRef.begin(); it != p.containerRef.end() - 1; ++it)
		{
			os << *it << opts.element_separator;
		}

		os << p.containerRef.back();

		os << opts.container_close;
		return os;
	}
};

template <typename ContainerT>
container_printer<ContainerT>
make_container_printer (const ContainerT& container)
{
	return container_printer<ContainerT>(container);
}
