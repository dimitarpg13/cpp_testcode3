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

template <class F, class X>
struct twice : apply1<F, typename apply1<F,X>::type>
{};

struct add_pointer_f
{
   template <class T>
   struct apply : boost::add_pointer<T> {};
};

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

    return 0;
}
