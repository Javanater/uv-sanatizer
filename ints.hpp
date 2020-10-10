#ifndef __INTS_HPP__
#define __INTS_HPP__

#ifdef TESTING

// #include <boost/safe_numerics/safe_integer.hpp>
// #include <boost/safe_numerics/exception_policies.hpp>
// #include <boost/safe_numerics/automatic.hpp>

// using duration_t = boost::safe_numerics::safe<long long,
//   boost::safe_numerics::native,
//   boost::safe_numerics::loose_trap_policy>;
// using timepoint_t = boost::safe_numerics::safe<unsigned long,
//   boost::safe_numerics::native,
//   boost::safe_numerics::loose_trap_policy>;

using duration_t = long long;
using timepoint_t = unsigned long;

#else

using duration_t = long long;
using timepoint_t = unsigned long;

#endif

#endif