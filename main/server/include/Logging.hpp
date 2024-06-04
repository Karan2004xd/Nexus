#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

class Logging {
public:
  static void init();
};
#endif // LOGGING_HPP
