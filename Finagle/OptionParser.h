/*!
** \file Parser.h
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

#ifndef FINAGLE_OPTIONPARSER_H
#define FINAGLE_OPTIONPARSER_H

#include <Finagle/Array.h>
#include <Finagle/Singleton.h>
#include <Finagle/TextString.h>

namespace Finagle {
namespace Option {

#include <getopt.h>

class OptionBase {
public:
  virtual ~OptionBase( void ) {}

  String const &name( void ) const {  return _name;  }
  char ch( void ) const            {  return _ch;  }
  bool valRequired( void ) const   {  return _required;  }
  bool isSet( void ) const         {  return _set;  }

protected:
  OptionBase( const char *name, char ch, bool valRequired = false );
  virtual bool set( const char * );

protected:
  String _name;
  char _ch;
  bool _required, _set;
  friend class Parser;
};

template <typename ArgType>
class Argument : public OptionBase {
public:
  Argument( const char *name, char ch = 0, ArgType const &defaultVal = ArgType(), bool valRequired = true );
  ArgType const &operator()( void ) const;
  operator ArgType const &( void ) const;

  ArgType const &operator()( ArgType const &defaultVal );

protected:
  bool set( const char *str );
  ArgType _val;
};

class Flag : public Argument<bool> {
public:
  Flag( const char *name, char ch = 0, bool defaultVal = false );
  operator bool const &( void ) const;

protected:
  Argument<bool> _negate;
};


class Parser {
public:
  typedef Array<OptionBase *> OptionArray;

public:
  Parser( void );

  Parser &operator <<( OptionBase *opt );
  bool parse( unsigned numArgs, char * const args[] );

  Parser::OptionArray const &options( void ) const;
  String::Array const &params( void ) const;

protected:
  bool _parsed;
  OptionArray _opts;
  String::Array _params;
};

}

static Singleton<Option::Parser> cmdLine;

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

namespace Option {

inline OptionBase::OptionBase( const char *name, char ch, bool valRequired )
: _name(name), _ch(ch), _required(valRequired), _set(false)
{
  cmdLine() << this;
}

inline bool OptionBase::set( const char * )
{
  return _set = true;
}


template <typename ArgType>
inline Argument<ArgType>::Argument( const char *name, char ch, ArgType const &defaultVal, bool valRequired )
: OptionBase( name, ch, valRequired ),
  _val( defaultVal )
{}

template <typename ArgType>
inline ArgType const &Argument<ArgType>::operator()( void ) const
{
  return _val;
}

template <typename ArgType>
inline Argument<ArgType>::operator ArgType const &( void ) const
{
  return _val;
}

template <typename ArgType>
inline ArgType const &Argument<ArgType>::operator()( ArgType const &defaultVal )
{
  return _val = defaultVal;
}

template <typename ArgType>
inline bool Argument<ArgType>::set( const char *str )
{
  return OptionBase::set(str) && String(str).to( _val );
}

// Partial template instantiation to handle bools properly
template <>
inline bool Argument<bool>::set( const char *str )
{
  return (str ? String(str).to( _val ) : (_val = true)) && OptionBase::set(str);
}


inline Flag::Flag( const char *name, char ch, bool defaultVal )
: Argument<bool>( name, ch, defaultVal, false ), _negate( "no-" + String(name), 0, !defaultVal, false )
{}

inline Flag::operator bool const &( void ) const
{
  if ( _negate.isSet() )
    const_cast<Flag *>(this)->_val = !_negate;

  return _val;
}


inline Parser::Parser( void )
: _parsed(false)
{}


inline Parser &Parser::operator <<( OptionBase *opt )
{
  _opts.push_back(opt);
  return *this;
}

inline Parser::OptionArray const &Parser::options( void ) const
{
  return _opts;
}

inline String::Array const &Parser::params( void ) const
{
  return _params;
}

} }

#endif
