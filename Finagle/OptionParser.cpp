/*!
** \file Parser.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Dec 28 2006
** Copyright (C) 2006 by Steve Sloan
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 2.1 of the License, or
** (at your option) any later version.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, you may access it via the web
** at http://www.gnu.org/copyleft/lesser.html .
*/

#include <iostream>
#include "Util.h"
#include "OptionParser.h"

using namespace std;
using namespace Finagle;
using namespace Option;

/*!
** \class Parser
**
*/

bool Parser::parse( unsigned numArgs, char * const args[] )
{
  if ( _parsed )
    return true;

  Array<option> longOpts( _opts.size() + 1 );
  String shortOpts;

  Array<option>::Iterator longOpt = longOpts.begin();
  for ( OptionArray::Iterator o = _opts.begin(); o != _opts.end(); ++o, ++longOpt ) {
    OptionBase &opt( **o );

    longOpt->name = opt.name();
    longOpt->has_arg = opt.valRequired() ? required_argument : optional_argument;
    longOpt->flag = 0;

    if ( opt.ch() ) {
      shortOpts += String::format( "%c:", opt.ch() );
      if ( !opt.valRequired() )
        shortOpts += ':';
    }
  }
  memset( &*longOpt, 0, sizeof(option) );  // array terminator

  int longOptIndex;
  int res;
  while ( (res = getopt_long( numArgs, args, shortOpts, &longOpts[0], &longOptIndex )) != -1 ) {
    if ( res != 0 )
      return false;

    if ( !_opts[longOptIndex]->set(optarg) ) {
      cerr << execTitle() << ": unrecognized argument '" << optarg << "' to option '" << _opts[longOptIndex]->name() << "'" << endl;
      return false;
    }
  }

  while ( unsigned(optind) < numArgs )
    _params.push_back( args[optind++] );

  _parsed = true;
  return true;
}


