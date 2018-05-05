#pragma once

#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

using boost::log::trivial::debug;
using boost::log::trivial::error;
using boost::log::trivial::fatal;
using boost::log::trivial::info;
using boost::log::trivial::trace;
using boost::log::trivial::warning;

typedef boost::log::sources::severity_logger<boost::log::trivial::severity_level>
        logger_t;
BOOST_LOG_GLOBAL_LOGGER(logger, logger_t)
