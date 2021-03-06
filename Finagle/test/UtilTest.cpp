/*!
** \file UtilTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Mon May 8 2007
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

#include <iostream>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class UtilTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( UtilTest );
  CPPUNIT_TEST_SUITE_END();

public:
};


CPPUNIT_TEST_SUITE_REGISTRATION( UtilTest );
