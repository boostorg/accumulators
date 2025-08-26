//  (C) Copyright Quentin Chateau 2018.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/container/set.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/sorted_rolling_window.hpp>

using namespace boost;
using namespace unit_test;
using namespace accumulators;
using namespace container;

typedef accumulator_set<int, stats<tag::sorted_rolling_window> > SortedAccumulator;

///////////////////////////////////////////////////////////////////////////////
// check_equal
//
void check_equal(const multiset<int>& expected, const SortedAccumulator& acc)
{
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), sorted_rolling_window(acc).begin(), sorted_rolling_window(acc).end());
}

///////////////////////////////////////////////////////////////////////////////
// test_stat
//
void test_stat()
{
    SortedAccumulator acc(tag::rolling_window::window_size = 3);

    check_equal({}, acc);

    acc(1);
    check_equal({1}, acc);

    acc(3);
    check_equal({1, 3}, acc);

    acc(5);
    check_equal({1, 3, 5}, acc);

    acc(7);
    check_equal({3, 5, 7}, acc);

    acc(6);
    check_equal({5, 6, 7}, acc);

    acc(7);
    check_equal({6, 7, 7}, acc);

    acc(1);
    check_equal({1, 6, 7}, acc);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("sorted rolling window test");

    test->add(BOOST_TEST_CASE(&test_stat));

    return test;
}
