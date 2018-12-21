//  (C) Copyright Quentin Chateau 2018.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/container/set.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_max.hpp>

using namespace boost;
using namespace unit_test;
using namespace accumulators;
using namespace container;

///////////////////////////////////////////////////////////////////////////////
// test_stat
//
void test_stat()
{
    accumulator_set<int, stats<tag::rolling_max> > acc(tag::rolling_window::window_size = 3);

    BOOST_CHECK_EQUAL(numeric::as_min(int()), rolling_max(acc));

    acc(1);
    BOOST_CHECK_EQUAL(1, rolling_max(acc));

    acc(3);
    BOOST_CHECK_EQUAL(3, rolling_max(acc));

    acc(5);
    BOOST_CHECK_EQUAL(5, rolling_max(acc));

    acc(7);
    BOOST_CHECK_EQUAL(7, rolling_max(acc));

    acc(6);
    BOOST_CHECK_EQUAL(7, rolling_max(acc));

    acc(1);
    BOOST_CHECK_EQUAL(7, rolling_max(acc));

    acc(3);
    BOOST_CHECK_EQUAL(6, rolling_max(acc));

    acc(2);
    BOOST_CHECK_EQUAL(3, rolling_max(acc));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("rolling max test");

    test->add(BOOST_TEST_CASE(&test_stat));

    return test;
}
