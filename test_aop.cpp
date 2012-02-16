/*
    Copyright (C) 2011 Hugo Arregui

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

#include <iostream>
#include <cmath>
#include "aop.h"

template <template <class> class A = aop::NullAspect>
class Number
{
    public:
    typedef typename aop::InnerType<Number, A>::Type Type;

    Number(float n)
        : n(n)
    {}

    Type operator+(const Type& other)
    {
        return Type(n + other.n);
    }
    float n;
};

template <class A>
class RoundAspect : public A
{
    public:
        typedef typename A::Type Type;

    RoundAspect(float n)
        : A(n)
    {}

    RoundAspect(const A& a)
        : A(a)
    {}

    Type operator+(const Type& other)
    {
        return Type(round(A::operator+(other).n));
    }

    private:
    static float round(float f) 
    {
        return std::ceil(f);
    }
};

template <class N>
void example()
{
    N a(1);
    N b(1.3);
    N c = a + b;
    std::cout << c.n << std::endl;
}

int main()
{
    example<Number<>>();

    typedef aop::Decorate<Number>::with<RoundAspect>::Type RoundNumber;
    example<RoundNumber>();
}
