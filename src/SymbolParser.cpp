// Copyright (c) 2004 E.J. Pronk and R.E.W. van Beusekom
//
// This file is part of Nmdepend.
// 
// Nmdepend is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// Nmdepend is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Nmdepend; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "SymbolParser.h"

//#define BOOST_SPIRIT_DEBUG

#include "boost/spirit/core.hpp"
#include "boost/spirit/iterator.hpp"
#include "boost/spirit/utility/loops.hpp"
#include "boost/spirit/tree/ast.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace boost::spirit;

typedef char const*         IteratorT;
typedef tree_match<IteratorT> parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator iter_t;

enum
{
	symbolID,
	identifierID,
	typeID,
	basetypeID,
	template_nameID,
	template_argumentID,
	type_parameterID,
	nameID,
	template_idID,
	template_argument_listID,
	template_parameterID,
	typelistID
};

struct SymbolGrammer : public grammar<SymbolGrammer>
{
    struct my_functor
    {
        my_functor(std::string& str_)
        : str(str_) {}

        void
        operator()(IteratorT first, IteratorT last) const
        {
            str.assign(first, last);
        }

        std::string& str;
    };

    template <typename ScannerT>
    struct definition
    {

		definition(SymbolGrammer const&) : thafunc(mystring)
        {
            symbol
				= root_node_d [name]
				>> inner_node_d['(' >> typelist >> ')']
				;

			typelist
				= type [my_functor(mystring)]
				>> !(',' >> typelist)
					;

			name
				= identifier
				>> !( "::" >> identifier)
				;

			identifier
				= leaf_node_d [ lexeme_d[ 
                    ((alpha_p | '_' | '$') >> *(alnum_p | '_' | '$'))
                    //- (keywords >> anychar_p - (alnum_p | '_' | '$'))
                ] ]
				;

			type
				= template_id
				|| template_id >> "const&"
				|| name >> '*'
				|| name >> "const&"
				|| name
				;

			template_id
				= template_name
				>> '<'
				>> template_argument_list
				>> '>'
				;

			template_argument_list
				= template_argument >> !(',' >> template_argument);

			template_argument
				= type
				;

			template_parameter
				= type_parameter
				;

			type_parameter =
				name
				;

			template_name =
				name
				;

			debug();
        }

        rule<ScannerT, parser_context<>, parser_tag<symbolID> > symbol;
		rule<ScannerT, parser_context<>, parser_tag<identifierID> > identifier;
		rule<ScannerT, parser_context<>, parser_tag<typeID> > type;
		rule<ScannerT, parser_context<>, parser_tag<basetypeID> > basetype;
		rule<ScannerT, parser_context<>, parser_tag<template_nameID> > template_name;
		rule<ScannerT, parser_context<>, parser_tag<template_argumentID> > template_argument;
		rule<ScannerT, parser_context<>, parser_tag<type_parameterID> > type_parameter;
		rule<ScannerT, parser_context<>, parser_tag<nameID> > name;
		rule<ScannerT, parser_context<>, parser_tag<template_idID> > template_id;
		rule<ScannerT, parser_context<>, parser_tag<template_argument_listID> > template_argument_list;
		rule<ScannerT, parser_context<>, parser_tag<template_parameterID> > template_parameter;
		rule<ScannerT, parser_context<>, parser_tag<typelistID> > typelist;
		;

		my_functor thafunc;

		std::string mystring;

		void debug()
        {
			BOOST_SPIRIT_DEBUG_RULE(identifier);
			BOOST_SPIRIT_DEBUG_RULE(type);
			BOOST_SPIRIT_DEBUG_RULE(basetype);
			BOOST_SPIRIT_DEBUG_RULE(template_name);
			BOOST_SPIRIT_DEBUG_RULE(template_argument);
			BOOST_SPIRIT_DEBUG_RULE(type_parameter);
			BOOST_SPIRIT_DEBUG_RULE(name);
			BOOST_SPIRIT_DEBUG_RULE(template_id);
			BOOST_SPIRIT_DEBUG_RULE(template_argument_list);
			BOOST_SPIRIT_DEBUG_RULE(template_parameter);
        }


        rule<ScannerT, parser_context<>, parser_tag<symbolID> > const&
        start() const { return symbol; }
    };
};

// todo : make this a static member
static SymbolGrammer s_Grammar;

 SymbolParser::SymbolParser() : mGrammar(s_Grammar)
{
}

void SymbolParser::Parse(const std::string& symbol)
{
	tree_parse_info<> info = ast_parse(symbol.c_str(), mGrammar, space_p);
        const bool success = info.full;
}
