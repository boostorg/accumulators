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

size_t window_size = 3;

///////////////////////////////////////////////////////////////////////////////
// test_impl
//
template<typename Accumulator>
void test_impl(Accumulator& acc)
{
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

void test_rolling_max()
{
    accumulator_set<int, stats<tag::rolling_max> > acc_default(tag::rolling_window::window_size = window_size);
    test_impl(acc_default);

    accumulator_set<int, stats<tag::rolling_max(lazy)> > acc_lazy(tag::rolling_window::window_size = window_size);
    test_impl(acc_lazy);

    accumulator_set<int, stats<tag::lazy_rolling_max> > acc_lazy2(tag::rolling_window::window_size = window_size);
    test_impl(acc_lazy2);

    accumulator_set<int, stats<tag::rolling_max(immediate)> > acc_immediate(tag::rolling_window::window_size = window_size);
    test_impl(acc_immediate);

    accumulator_set<int, stats<tag::immediate_rolling_max> > acc_immediate2(tag::rolling_window::window_size = window_size);
    test_impl(acc_immediate2);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("rolling max test");

    test->add(BOOST_TEST_CASE(&test_rolling_max));

    return test;
}
