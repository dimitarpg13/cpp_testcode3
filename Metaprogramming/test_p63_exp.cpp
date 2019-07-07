#include <iostream>
#include <type_traits>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/map.hpp>
//#include <boost/mpl/map/mapn.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/deref.hpp> 
#include <boost/mpl/less.hpp>
#include <boost/mpl/iter_fold.hpp>

namespace mpl = boost::mpl;

using namespace mpl::placeholders;

typedef mpl::vector_c<int,1,0,0,0,0,0,0> d_mass; // m
typedef mpl::vector_c<int,0,1,0,0,0,0,0> d_length; // l
typedef mpl::vector_c<int,0,0,1,0,0,0,0> d_time; // t
typedef mpl::vector_c<int,0,0,0,1,0,0,0> d_charge; // q
typedef mpl::vector_c<int,0,0,0,0,1,0,0> d_temperature;  // T
typedef mpl::vector_c<int,0,0,0,0,0,0,1> d_angle; // rad

// base dimension: m l t ...
typedef mpl::vector_c<int,0,1,-1,0,0,0,0> d_velocity; // l/t
typedef mpl::vector_c<int,0,1,-2,0,0,0,0> d_acceleration; // l/t^2
typedef mpl::vector_c<int,1,1,-1,0,0,0,0> d_momentum; // ml/t
typedef mpl::vector_c<int,1,1,-2,0,0,0,0> d_force; // ml/t^2
typedef mpl::vector_c<int,0,0,0,0,0,0,0> d_scalar; // - 

template <class T, class Dimensions>
struct quantity
{
   explicit quantity(T x)
       : m_value(x)
   {}

   T value() const { return m_value; }
private:
   T m_value;
};

template <class T, class D>
quantity<T,D>
operator+(quantity<T,D> x, quantity<T,D> y)
{
    return quantity<T,D>(x.value() + y.value());
};

template <class T, class D>
quantity<T,D>
operator-(quantity<T,D> x, quantity<T,D> y)
{
    return quantity<T,D>(x.value() - y.value());
};

struct plus_f
{
    template <class T1, class T2>
    struct apply
    {
       typedef typename mpl::plus<T1,T2>::type type;
    };
};

template <class T, class D1, class D2>
quantity<T, typename mpl::transform<D1,D2,plus_f>::type>
operator* (quantity<T,D1> x, quantity<T,D2> y)
{
    typedef typename mpl::transform<D1,D2,plus_f>::type dim;
    return quantity<T,dim>( x.value() * y.value() );
}

template <class D1, class D2>
struct divide_dimensions
  : mpl::transform< D1,D2,mpl::minus<_1,_2> > // forwarding again
{};

template <class T, class D1, class D2>
quantity<T, typename divide_dimensions<D1,D2>::type>
operator/(quantity<T,D1> x, quantity<T,D2> y) 
{
   return quantity<T, typename divide_dimensions<D1,D2>::type> (
           x.value() / y.value());
};

template <class UnaryMetaFunctionClass, class Arg>
struct apply1 : UnaryMetaFunctionClass::template apply<Arg>
{};

/*template <class F, class X>
struct twice : apply1<F, typename apply1<F,X>::type>
{};*/

struct add_pointer_f
{
   template <class T>
   struct apply : boost::add_pointer<T> {};
};

/*template <class X>
struct two_pointers
   : twice<typename mpl::lambda<boost::add_pointer<_1> >::type, X>
{};*/

template <class F, class X>
struct twice : 
    apply1<
        typename mpl::lambda<F>::type, 
        typename apply1<
             typename mpl::lambda<F>::type, 
             X
        >::type
    >
{};

typedef mpl::transform<
      mpl::range_c<int,0,10>
    , mpl::plus<_1, mpl::int_<7> >
    , mpl::back_inserter< mpl::vector0<> >
    >::type r1;

typedef mpl::transform<r1, mpl::minus<_1,mpl::int_<2>>>::type r2;

typedef mpl::reverse_transform<r2,mpl::minus<_1,mpl::int_<5>>,mpl::front_inserter<mpl::vector0<>>>::type r3;

typedef mpl::transform<mpl::range_c<int,0,10>
                     , mpl::plus<_1,_1>
                     , mpl::back_inserter< mpl::vector0<> >
                     >::type r4;

template< typename T > struct f1 
{
    typedef T type;
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
struct f5
{
    // no 'type' member!
};

typedef mpl::quote1<f1>::apply<int>::type tp1;
typedef mpl::quote5<f5>::apply<char,short,int,long,float>::type tp5;

typedef mpl::map<
           mpl::pair<int,unsigned>
         , mpl::pair<char, unsigned char>
         , mpl::pair<mpl::long_<5>,char[17]>
         , mpl::pair<int[42],bool>
         > m4;

typedef mpl::vector<long, float, short, double, float, long, long double> types;
typedef mpl::fold<
      types
    , mpl::int_<0>
    , mpl::if_< boost::is_float<_2>,mpl::next<_1>,_1 >
    >::type number_of_floats;

struct f0
{
    template< typename T = int > struct apply
    {
        typedef char type;
    };
};

struct g0
{
    struct apply { typedef char type; };
};

struct f2
{
    template< typename T1, typename T2 > struct apply
    {
        typedef T2 type;
    };
};

typedef mpl::apply_wrap0< f0 >::type rr1;
typedef mpl::apply_wrap0< g0 >::type rr2;
typedef mpl::apply_wrap2< f2, int, char >::type rr3;

typedef mpl::vector_c<int, 5, -1, 0, 7, 2, 0, -5, 4> numbers;
typedef mpl::iter_fold<
      numbers
    , mpl::begin<numbers>::type
    , mpl::if_< mpl::less< mpl::deref<_1>, mpl::deref<_2> >, _2, _1 >
    >::type max_element_iter;

int main(int argc, char **argv) {
    quantity<float,d_length> l (1.0f);
    quantity<float,d_mass> m(2.0f);
    quantity<float,d_length> len1 (1.0f);
    quantity<float,d_length> len2(2.0f);
    len1 = len1 + len2;
    quantity<float,d_acceleration> a(9.8f);
    std::cout << "force = " << (m * a).value() << std::endl;

    BOOST_STATIC_ASSERT(
            boost::is_same<
                   twice<add_pointer_f, int>::type,
                   int**
                   >::value
            );

    /*BOOST_STATIC_ASSERT(
            boost::is_same<
                   typename two_pointers<int>::type,
                   int**
            >::value
           );
    */

    int * x;
    twice<add_pointer_f, int>::type p = &x;
    twice<boost::add_pointer<_1>, int>::type q = &x;
    BOOST_MPL_ASSERT((mpl::equal<r1, mpl::range_c<int, 7, 17> >));
    BOOST_MPL_ASSERT((mpl::equal<r2, mpl::range_c<int, 5, 15> >));
    BOOST_MPL_ASSERT((mpl::equal<r3, mpl::range_c<int, 0, 10> >));
    BOOST_MPL_ASSERT_RELATION(mpl::size<r4>::value, ==, 10 );
    BOOST_MPL_ASSERT((mpl::equal<r4, mpl::vector_c<int, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18> >));
    BOOST_MPL_ASSERT((boost::is_same<tp1,int>));
    BOOST_MPL_ASSERT((boost::is_same<tp5, f5<char, short, int, long, float> >));
    BOOST_MPL_ASSERT_RELATION( mpl::size<m4>::value, ==, 4 );
    BOOST_MPL_ASSERT_NOT(( mpl::empty<m4> ));

    BOOST_MPL_ASSERT(( boost::is_same< mpl::at<m4, int>::type, unsigned > ));
    BOOST_MPL_ASSERT(( boost::is_same< mpl::at<m4, mpl::long_<5> >::type, char[17] > ));  
    BOOST_MPL_ASSERT(( boost::is_same< mpl::at<m4, int[42]>::type, bool > ));
    BOOST_MPL_ASSERT(( boost::is_same< mpl::at<m4, long>::type, mpl::void_ > ));
    BOOST_MPL_ASSERT_RELATION( number_of_floats::value, ==, 4 );

    BOOST_MPL_ASSERT(( boost::is_same<rr1, char> ));
    BOOST_MPL_ASSERT(( boost::is_same<rr2, char> ));
    BOOST_MPL_ASSERT(( boost::is_same<rr3, char> ));
    return 0;
}
