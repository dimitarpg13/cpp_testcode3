#include <iostream>
#include <type_traits>
#include <boost/static_assert.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
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

int main(int argc, char **argv) {


    return 0;
}
