/*!
** \file TestXML.cpp
** \date Tue Nov 13 2007
** \author Steve Sloan <steve@finagle.org>
** Copyright (C) 2007 by Steve Sloan
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

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;

int main( int argc, char *argv[] )
{
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  if ( (argc > 2) || ((argc > 1) && (string(argv[1]) == "--help")) ) {
    string app( argv[0] );
    cout << "Usage: " << app.substr( app.find_last_of('/') + 1 ) << " [test_case]" << endl
         << "Test cases:" << endl;

    for ( int i = 0; i < suite->getChildTestCount(); ++i )
      cout << "  " << suite->getChildTestAt(i)->getName() << endl;

    return 0;
  }

  CppUnit::TextTestRunner runner;
  runner.addTest( suite );

  return runner.run( (argc > 1) ? argv[1] : "" ) ? 0 : 1;
}
