/*!
** \file OptionParser.cpp
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
** \class Finagle::Option::Parser
** \brief Provides command-line argument parsing, using \c getopt_long(3).
**
** Arguments are specified using the Argument and Flag class, and loaded with the parse() function.  Options may then
** be read from the cmdLine singleton.
**
** Example: \code
** Option::Argument<unsigned short> Port( "port", 'p', 4242 );  // i.e. "--port=X" or "-p X"
** Option::Flag Verbose( "verbose", 'v' );                      // i.e. "--verbose" or "--no-verbose"
**
** if ( !cmdLine().parse( argc, argv ) ) {
**   // print usage
**   exit(0);
** }
** unsigned short port = Port();
** bool verbose = Verbose();
** ...
** \endcode
*/

/*! \class Finagle::Option::Argument
** \brief Represents a named, typed command-line option.
*/

/*! \class Finagle::Option::Flag
** \brief Represents a named Boolean flag command-line option.
**
** For example, "--name" = \c true and "--no-name" = \c false.
*/

/*!
** Parses command line options (where \a numArgs is \c argc and \a args is \c argv).
** Returns \c true if all arguments were parsed and recognized, or \c false otherwise.
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
