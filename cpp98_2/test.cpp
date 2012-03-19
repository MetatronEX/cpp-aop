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

#include <iostream>
#include <cmath>
#include "aop.h"

template <class _UnderlyingType, class Next>
struct BaseNumber
{
    template <class Prev>
    class Inner : public Next::template Inner<Inner <Prev> >
    {
    public:
        typedef _UnderlyingType UnderlyingType;
        typedef typename Next::template Inner<Inner<Prev> >::TheType TheType;

        Inner(UnderlyingType n = UnderlyingType(0))
            : n(n)
        {}

        Inner(const Inner& other)
            : n(other.n)
        {}

        TheType& operator=(const TheType& other)
        {
            n= other.n;
            return *static_cast<TheType*>(this);
        }

        TheType operator+ (const TheType& other) const
        {
            return n + other.n;
        }

        friend std::ostream& operator<<(std::ostream& out, const TheType& number)
        {
            return out << number.n;
        }
//    protected:    g++ bug?
        UnderlyingType n;
    };
};


/*
* Configurable Aspect sumExample
*/
template <unsigned int PRECISION, class Next>
struct RoundAspect
{
    template <class Prev>
    struct Inner : public Next::template Inner<Inner <Prev> >
    {
        typedef typename Next::template Inner<Inner <Prev> > Base;
        typedef typename Next::template Inner<Inner<Prev> >::TheType TheType;

        Inner(typename Base::UnderlyingType n)
            : Base(n)
        {}

        Inner(const Base& b)
            : Base(b)
        {}

        TheType operator+(const TheType& other) const
        {
            return TheType(round(Base::operator+(other).n));
        }

    private:
        static float round(float f)
        {
            const unsigned int e = std::pow(10, PRECISION);
            return float(int(f * e)) / e;
        }
    };
};

template <class Next>
class LogicalAspect
{
public:
    template <class Prev>
    struct Inner : public Next::template Inner<Inner <Prev> >
    {
        typedef typename Next::template Inner<Inner <Prev> > Base;
        typedef typename Next::template Inner<Inner<Prev> >::TheType TheType;

        Inner(typename TheType::UnderlyingType n)
            : Base(n)
        {}

        Inner(const Base& b)
            : Base(b)
        {}

        bool operator!() const
        {
            return !Base::n;
        }
        
        bool operator&&(const TheType& o) const
        {
            return Base::n && o.n;
        }

        bool operator||(const TheType& o) const
        {
            return Base::n || o.n;
        }
    };
};

template <class Next>
class BitwiseAspect
{
public:
    template <class Prev>
    struct Inner : public Next::template Inner<Inner <Prev> >
    {
        typedef typename Next::template Inner<Inner <Prev> > Base;
        typedef typename Next::template Inner<Inner<Prev> >::TheType TheType;

        Inner(typename Base::UnderlyingType n)
            : Base(n)
        {}

        Inner(const Base& b)
            : Base(b)
        {}

        TheType operator~() const
        {
            return ~Base::n;
        }
        
        TheType operator&(const TheType& o) const
        {
            return Base::n & o.n;
        }

        TheType operator|(const TheType& o) const
        {
            return Base::n | o.n;
        }

        TheType operator<<(const TheType& bitcount) const
        {
            return Base::n << bitcount.n;
        }

        TheType operator<<(int c) const
        {
            return Base::n << c;
        }

        TheType operator>>(const TheType& bitcount) const
        {
            return Base::n >> bitcount.n;
        }

        TheType& operator>>=(const TheType& bitcount)
        {
            Base::n >>= bitcount.n;
            //return *Base::getThis();
            return *static_cast<TheType*>(this);
        }
    };
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
    N c;
    c = a + b;
    //std::cout << (a + ((b>>=1) << N(3))) << std::endl;
}

int main()
{
    using namespace aop;

    typedef Head<RoundAspect<2, BaseNumber<float, Tail> > >::TheType FloatRoundNumber;
    sumExample<FloatRoundNumber>(1.339, 1.1233);

    //typedef aop::Decorate<Number<unsigned int>::Type>::with<TYPELIST_1(BitwiseAspect)>::Type IntegralNumber;
    //typedef Head<LogicalAspect<BitwiseAspect<BaseNumber<int, Tail> > > >::TheType IntegralNumber;
    typedef Head<BaseNumber<int, BitwiseAspect<LogicalAspect<Tail> > > >::TheType IntegralNumber;
    bitwiseExample<IntegralNumber>(1, 2);
/*
    typedef TYPELIST_2(RoundAspect<2>::Type, LogicalAspect) RoundLogicalList;
    typedef aop::Decorate<Number<float>::Type>::with<RoundLogicalList>::Type FloatRoundLogicalNumber;
    orExample<FloatRoundLogicalNumber>(1, 0);
*/
    typedef Head<LogicalAspect<BaseNumber<int, Tail> > >::TheType IntLogicalNumber;
    orExample<IntLogicalNumber>(1, 0);

    return 0;
}

