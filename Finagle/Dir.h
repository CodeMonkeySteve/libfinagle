/*!
** \file Dir.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Dec 16 2003
** Copyright (C) 2004 by Steve Sloan
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

#ifndef FINAGLE_DIR_H
#define FINAGLE_DIR_H

#include <unistd.h>
#include <iterator>
#include <Finagle/File.h>

namespace Finagle {

class Dir : public File {
public:
  class Iterator : public std::iterator<std::forward_iterator_tag, Finagle::File> {
  public:
    Iterator( void );
    Iterator( Finagle::Dir const &dir, const char *ext = 0 );
    Iterator( Iterator const &that );
   ~Iterator( void );

    File &operator *( void );
    File *operator ->( void );
    bool operator ==( Iterator const &that ) const;
    bool operator !=( Iterator const &that ) const;

    Iterator &operator ++( void );
    Iterator &operator ++( int );

  protected:
    void findNextFile( void );

  protected:
    File _curFile;
    FilePath _dir;
    const char *_ext;
    void *_search;

  private:
    Iterator &operator =( Iterator const & );
  };

public:
  static const Dir root;
  static Dir current( void );
  static bool current( Dir const &dir );
  static Dir home( void );

public:
  Dir( String const &dirPath = String() );
  Dir( const char *dirPath, const char *dirName );

  long long spaceFree( void ) const;
  long long spaceUsed( void ) const;

  bool create( unsigned mode = 0700 ) const;
  bool erase( bool recursive = false ) const;
  bool sync( Dir &dest ) const;

  unsigned count( const char *Ext = 0 ) const;

  template <typename CType>
  void list( CType &Files, const char *Ext = 0 ) const;

  Iterator begin( const char *Ext = 0 ) const;
  Iterator const &end( void ) const;
};


class TempDir : public Dir {
public:
  TempDir( unsigned mode = 0700 );
 ~TempDir( void );
};

// INLINE IMPLEMENTATION ******************************************************

//! Initializes the class point to the directory with the path \a DirPath.
inline Dir::Dir( String const &dirPath )
: File( dirPath )
{}

//! Initializes the class point to the directory with the name \a "DirPath/DirName".
inline Dir::Dir( const char *dirPath, const char *dirName )
: File( dirPath, dirName )
{}

template <typename CType>
void Dir::list( CType &files, const char *ext ) const
{
  for ( Iterator f = begin( ext ); f != end() ; ++f )
    files.push_back( *f );
}


inline Dir::Iterator::Iterator( void )
: _ext( 0 ), _search( 0 )
{}

inline Dir::Iterator Dir::begin( const char *ext ) const
{
  return Iterator( *this, ext );
}

inline Dir::Iterator const &Dir::end( void ) const
{
  static Iterator end;
  return end;
}

inline File &Dir::Iterator::operator *( void )
{
  return _curFile;
}

inline File *Dir::Iterator::operator ->( void )
{
  return &_curFile;
}

inline Dir::Iterator &Dir::Iterator::operator ++( void )
{
  _curFile.clear();
  findNextFile();
  return *this;
}

inline bool Dir::Iterator::operator ==( Iterator const &that ) const
{
  return _curFile.path() == that._curFile.path();
}

inline bool Dir::Iterator::operator !=( Iterator const &that ) const
{
  return !operator==( that );
}


}

#endif
