/**
 * @file   call_trace.hpp
 * @date   19.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_FUNCTIONAL_HPP_
#define MW_TEST_PARSER_FUNCTIONAL_HPP_

#include <mw/test/parser/code.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <mw/test/ast/functional.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::call_trace,
	(mw::test::ast::code, name)
	(boost::optional<std::vector<mw::test::ast::call_trace>>, content)
	(boost::optional<unsigned int>,  count)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::call_trace_decl,
	(std::string, id)
	(mw::test::ast::code, name)
	(boost::optional<std::vector<mw::test::ast::call_trace>>, content)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::stub,
	(mw::test::ast::code, name)
	(boost::optional<std::string>, return_name)
	(std::vector<std::string>, arg_names)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::fake,
	(mw::test::ast::code, name)
	(mw::test::ast::code, func)
);

namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class function_name, ast::code> const function_name;
auto const function_name_def = eps[code::set_beg] >>
		omit[lexeme[+(!(lit(',') | "=>" ) >> code_chunk_step)]]
		  >> eps[code::set_end];;

x3::rule<class call_trace, ast::call_trace> const call_trace;

x3::rule<class call_trace_list, std::vector<ast::call_trace>> const call_trace_list;


auto const call_trace_list_def =
		(call_trace % ",") |
		eps ;


auto const call_trace_def =
		function_name
		>> -("=>" >> lit("{")
		>> call_trace_list
		>> "}")
		>> -('*' >> x3::uint_);



x3::rule<class call_trace_decl, ast::call_trace_decl> const call_trace_decl;
auto const call_trace_decl_def =
		lexeme["call" 	>> skipper] >>
		lexeme["trace" 	>> skipper] >>
		lexeme[id  	 >> skipper] >>
		lexeme["for" >> skipper] >>
		function_name >> "=>"  >>lit("{")
		>> call_trace_list
		>> "}" >> ";";

BOOST_SPIRIT_DEFINE(function_name);
BOOST_SPIRIT_DEFINE(call_trace);
BOOST_SPIRIT_DEFINE(call_trace_decl);
BOOST_SPIRIT_DEFINE(call_trace_list);


x3::rule<class id_list, std::vector<std::string>> id_list;

auto const id_list_def = (id % ',') | eps;

x3::rule<class stub, ast::stub> const stub;
auto const stub_def =
		lexeme["stub" >> skipper]
		>> 	function_name
		>> "=>"
		>>	("void" | id)
		>> '(' >> ((id % ',') | eps)
		>> ')' >> ';';



BOOST_SPIRIT_DEFINE(stub);
BOOST_SPIRIT_DEFINE(id_list);

x3::rule<class fake, ast::fake> const fake;
auto const fake_def =
			lexeme["fake" >> skipper]
		>>  function_name
		>>  "=>"
		>>  code_section;

BOOST_SPIRIT_DEFINE(fake);


x3::rule<class call_trace_decl_doc, ast::call_trace_decl> const call_trace_decl_doc;
x3::rule<class stub_doc, ast::stub> const stub_doc;
x3::rule<class fake_doc, ast::fake> const fake_doc;

auto const call_trace_decl_doc_def = doc(call_trace_decl);
auto const stub_doc_def = doc(stub);
auto const fake_doc_def = doc(fake);

BOOST_SPIRIT_DEFINE(call_trace_decl_doc);
BOOST_SPIRIT_DEFINE(stub_doc);
BOOST_SPIRIT_DEFINE(fake_doc);

x3::rule<class functional, ast::functional> const functional;
auto const functional_def =
			call_trace_decl_doc | stub_doc | fake_doc;


BOOST_SPIRIT_DEFINE(functional);

}
}
}



#endif /* MW_TEST_PARSER_FUNCTIONAL_HPP_ */
