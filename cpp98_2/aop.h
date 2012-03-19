/*
    Copyright (C) 2012 Daniel Gutson

    This file is part of the cpp-aop Library.

    cpp-aop is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-aop is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-aop.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AOP_H
#define AOP_H

namespace aop
{

struct Tail
{
    template <class Prev>
    struct Inner
    {
        typedef Prev TheType;

        TheType* getThis()
        {
          return static_cast<TheType*>(this);
        }
    };
};

template <class AspectsList>
struct Head
{
    typedef typename AspectsList::template Inner<Head> TheType;
};
}
#endif
