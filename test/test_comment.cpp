/**
 * @file   test_comment.cpp
 * @date   16.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_comment
#include <boost/test/minimal.hpp>
#include <mw/test/parser/comment.hpp>


int test_main (int, char**)
{
	std::string s;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.begin();

	bool res;
	using namespace mw::test::parser;

	using c = mw::test::listener::comment;

	std::string parsed;

	auto p = [&](auto rule)
		{
			parsed.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			res = x3::parse(itr, end, rule, parsed);
		};

	// line comment
	s = "// some comment �12893\n";

	p(comment);

	BOOST_CHECK(c::get().empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "// /some comment �12893\n";

	p(comment);

	BOOST_CHECK(c::get().empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/// some comment �12893\n";

	p(comment);


	BOOST_CHECK(!c::get().empty());
	BOOST_CHECK( c::get().before == " some comment �12893");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	c::get().clear();

	s = "///< some comment �12893\n";

	p(comment);

	BOOST_CHECK(!c::get().empty());
	BOOST_CHECK( c::get().behind == " some comment �12893");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	c::get().clear();


	// block comment
	s = "/*/ some comment �12893\n */";

	p(comment);


	BOOST_CHECK(c::get().empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/* *some comment �12893\n1*23\n asc#+ */";

	p(comment);

	BOOST_CHECK(c::get().empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/** some comment �12893\n */";

	p(comment);

	std::cerr << c::get().before << std::endl;
	std::cerr << (itr-beg) << std::endl;


	BOOST_CHECK(!c::get().empty());
	BOOST_CHECK( c::get().before == " some comment �12893\n ");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	c::get().clear();
	s = "/**< some comment �12893\n */";

	p(comment);

	std::cerr << c::get().before << std::endl;

	BOOST_CHECK(!c::get().empty());
	BOOST_CHECK( c::get().behind == " some comment �12893\n ");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	return 0;
}
