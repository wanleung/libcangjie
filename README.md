libcangjie
==========

CangJie Input Method Library

Installing
==========

This is pretty much a standard Autotools project. You can install it with the
usual sequence:

    $ ./configure
    $ make
    # make install

Of course, if you got the sources from Git, you will need to run the
`autogen.sh` script first.

Dependencies
============

You will need the following to build this library:

  - the C++ libdb bindings:
      -> http://www.oracle.com/database/berkeley-db/

  - a C++ compiler
      We strongly recomment g++
      -> http://gcc.gnu.org/

Legal
=====

Copyright (C) 2012  Wan Leung Wong <wanleung at linkomnia dot com>

libcangjie is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libcangjie is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libcangjie.  If not, see <http://www.gnu.org/licenses/>.
