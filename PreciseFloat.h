#ifndef PRECISE_FLOAT_H
#define PRECISE_FLOAT_H

// Define USE_BOOST_MULTIPRECISION externally to enable Boost
#ifdef USE_BOOST_MULTIPRECISION

  // Try to include Boost.Multiprecision
  #include <boost/version.hpp>
  #if !defined(BOOST_VERSION)
    #error "USE_BOOST_MULTIPRECISION defined, but Boost is not available or not found."
  #endif

  #include <boost/multiprecision/cpp_bin_float.hpp>

  namespace Precise {
      using pfloat = boost::multiprecision::cpp_bin_float_quad;
      constexpr const char* implementation = "Boost.Multiprecision: cpp_bin_float_quad";
  }

#else

  // Fallback to long double
  namespace Precise {
      using pfloat = long double;
      constexpr const char* implementation = "long double";
  }

#endif

#endif // PRECISE_FLOAT_H
