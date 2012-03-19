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

template <typename _UnderlyingType>
struct Number
{
    template <template <class> class A = aop::NullAspect>
    class Type
    {
    public:
        typedef _UnderlyingType UnderlyingType;
        typedef aop::BaseAopData< Number::Type, A> AopData; //Needed by lib
        typedef typename AopData::Type FullType;

        Type(UnderlyingType n)
            : n(n)
        {}

        FullType operator+(const FullType& other) const
        {
            return FullType(n + other.n);
        }

        friend std::ostream& operator<<(std::ostream& out, const Type& number)
        {
            return out << number.n;
        }
    protected:
        UnderlyingType n;
    };
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

        Type(typename A::UnderlyingType n)
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

    LogicalAspect(typename A::UnderlyingType n)
        : A(n)
    {}

    LogicalAspect(const A& a)
        : A(a)
    {}

    bool operator!() const
    {
        return !A::n;
    }
    
    bool operator&&(const FullType& o) const
    {
        return A::n && o.n;
    }

    bool operator||(const FullType& o) const
    {
        return A::n || o.n;
    }
};

template <class A>
class BitwiseAspect: public A
{
public:
    typedef aop::AspectAopData< ::BitwiseAspect, A> AopData;
    typedef typename AopData::Type FullType;

    template<class T>
    BitwiseAspect(T n)
        : A(n)
    {}

    BitwiseAspect(const A& a)
        : A(a)
    {}

    FullType operator~() const
    {
        return ~A::n;
    }
    
    FullType operator&(const FullType& o) const
    {
        return A::n & o.n;
    }

    FullType operator|(const FullType& o) const
    {
        return A::n | o.n;
    }

    FullType operator<<(const FullType& bitcount) const
    {
        return A::n << bitcount.n;
    }

    FullType operator>>(const FullType& bitcount) const
    {
        return A::n >> bitcount.n;
    }
};

template <class N>
void sumExample(typename N::UnderlyingType n1, typename N::UnderlyingType n2)
{
    N a(n1);
    N b(n2);
    N c = a + b;
    std::cout << c << std::endl;
}

template <class N>
void orExample(typename N::UnderlyingType n1, typename N::UnderlyingType n2)
{
    N a(n1);
    N b(n2);
    std::cout << (a || b) << std::endl;
}

template <class N>
void bitwiseExample(typename N::UnderlyingType n1, typename N::UnderlyingType n2)
{
    N a(n1);
    N b(n2);
    std::cout << (a + (b << 3)) << std::endl;
}

int main()
{
    sumExample<Number<int>::Type<> >(1, 2);

    typedef aop::Decorate<Number<float>::Type>::with<TYPELIST_1(RoundAspect<2>::Type)>::Type FloatRoundNumber;
    sumExample<FloatRoundNumber>(1.339, 1.1233);

    typedef aop::Decorate<Number<unsigned int>::Type>::with<TYPELIST_1(BitwiseAspect)>::Type IntegralNumber;
    bitwiseExample<IntegralNumber>(1, 2);

    typedef TYPELIST_2(RoundAspect<2>::Type, LogicalAspect) RoundLogicalList;
    typedef aop::Decorate<Number<float>::Type>::with<RoundLogicalList>::Type FloatRoundLogicalNumber;
    orExample<FloatRoundLogicalNumber>(1, 0);

    typedef aop::Decorate<Number<int>::Type>::with<TYPELIST_1(LogicalAspect)>::Type IntLogicalNumber;
    orExample<IntLogicalNumber>(1, 0);
    return 0;
}
