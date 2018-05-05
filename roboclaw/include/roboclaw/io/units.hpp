#pragma once

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/current.hpp>
#include <boost/units/systems/si/electric_potential.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/units/systems/temperature/celsius.hpp>

template<class Units>
using quantity = boost::units::quantity<Units, float>;

using boost::units::si::electric_potential;
using boost::units::si::volt;
using boost::units::si::volts;

using boost::units::si::ampere;
using boost::units::si::amperes;
using boost::units::si::current;

using boost::units::celsius::temperature;
namespace celsius = boost::units::celsius;
