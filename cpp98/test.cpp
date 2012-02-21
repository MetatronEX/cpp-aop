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
    typedef aop::BaseAopData< ::Number, A> AopData; //Needed by lib
    typedef typename AopData::Type FullType;

    Number(float n)
        : n(n)
    {}

    FullType operator+(const FullType& other) const
    {
        return FullType(n + other.n);
    }

    friend std::ostream& operator<<(std::ostream& out, const Number& number)
    {
        return out << number.n;
    }
protected:
    float n;
};

/*
* Configurable Aspect sumExample
*/
template <unsigned int PRECISION>
struct RoundAspect
{
    template <class A>
    class Type : public A
    {
    public:
        typedef aop::AspectAopData< RoundAspect::Type, A> AopData;
        typedef typename AopData::Type FullType;

        Type(float n)
            : A(n)
        {}

        Type(const A& a)
            : A(a)
        {}

        FullType operator+(const FullType& other) const
        {
            return FullType(round(A::operator+(other).n));
        }

    private:
        static float round(float f)
        {
            const unsigned int e = std::pow(10, PRECISION);
            return float(int(f * e)) / e;
        }
    };
};

template <class A>
class LogicalAspect: public A
{
public:
    typedef aop::AspectAopData< ::LogicalAspect, A> AopData;
    typedef typename AopData::Type FullType;

    LogicalAspect(float n)
        : A(n)
    {}

    LogicalAspect(const A& a)
        : A(a)
    {}

    bool operator!() const
    {
        return !A::n;
    }
    
    bool operator&&(const LogicalAspect& o) const
    {
        return A::n && o.n;
    }

    bool operator||(const LogicalAspect& o) const
    {
        return A::n || o.n;
    }
};

template <class N>
void sumExample(float n1, float n2)
{
    N a(n1);
    N b(n2);
    N c = a + b;
    std::cout << c << std::endl;
}

template <class N>
void orExample(float n1, float n2)
{
    N a(n1);
    N b(n2);
    std::cout << (a || b) << std::endl;
}

int main()
{
    sumExample<Number<> >(1, 2);

    typedef aop::Decorate<Number>::with<TYPELIST_1(RoundAspect<2>::Type)>::Type RoundNumber;
    sumExample<RoundNumber>(1.339, 1.1233);

    typedef aop::Decorate<Number>::with<TYPELIST_2(RoundAspect<2>::Type, LogicalAspect)>::Type RoundLogicalNumber;
    orExample<RoundLogicalNumber>(1, 0);
    return 0;
}
