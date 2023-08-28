///////////////////////////////////////////////////////////////////////////////
// rolling_max.hpp
//
//  Copyright 2018 Quentin Chateau. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_ROLLING_MAX_HPP_QC_20_12_2018
#define BOOST_ACCUMULATORS_STATISTICS_ROLLING_MAX_HPP_QC_20_12_2018

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/rolling_window.hpp>
#include <boost/accumulators/statistics/sorted_rolling_window.hpp>

namespace boost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // immediate_rolling_max_impl
    //      max is calculated on insersion using a sorted_rolling_window
    template<typename Sample>
    struct immediate_rolling_max_impl
      : accumulator_base
    {
        // for boost::result_of
        typedef Sample result_type;

        immediate_rolling_max_impl(dont_care)
        {
        }

        template<typename Args>
        result_type result(Args const &args) const
        {
            if (sorted_rolling_window(args).empty())
            {
                return numeric::as_min(Sample());
            }
            return sorted_rolling_window(args).back();
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // lazy_rolling_max_impl
    //      max is calculated on extraction by iterating the rolling window
    template<typename Sample>
    struct lazy_rolling_max_impl
      : accumulator_base
    {
        // for boost::result_of
        typedef Sample result_type;

        lazy_rolling_max_impl(dont_care)
        {
        }

        template<typename Args>
        result_type result(Args const &args) const
        {
            if (rolling_window(args).empty())
            {
                return numeric::as_min(Sample());
            }
            return *max_element(rolling_window(args).begin(), rolling_window(args).end());
        }
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::immediate_rolling_max
// tag::lazy_rolling_max
// tag::rolling_max
//
namespace tag
{
    struct immediate_rolling_max
      : depends_on< sorted_rolling_window >
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::immediate_rolling_max_impl<mpl::_1> impl;

#ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
         /// tag::rolling_window::window_size named parameter
         static boost::parameter::keyword<tag::rolling_window_size> const window_size;
#endif
    };

    struct lazy_rolling_max
      : depends_on< rolling_window >
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::lazy_rolling_max_impl<mpl::_1> impl;

#ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
         /// tag::rolling_window::window_size named parameter
         static boost::parameter::keyword<tag::rolling_window_size> const window_size;
#endif
    };

      // make lazy_rolling_max the default implementation
      struct rolling_max : lazy_rolling_max {};
}

///////////////////////////////////////////////////////////////////////////////
// extract::rolling_max
//
namespace extract
{
    extractor<tag::immediate_rolling_max> const immediate_rolling_max = {};
    extractor<tag::lazy_rolling_max> const lazy_rolling_max = {};
    extractor<tag::rolling_max> const rolling_max = {};

    BOOST_ACCUMULATORS_IGNORE_GLOBAL(immediate_rolling_max)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(lazy_rolling_max)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_max)
}

using extract::immediate_rolling_max;
using extract::lazy_rolling_max;
using extract::rolling_max;

// rolling_max(lazy) -> lazy_rolling_max
template<>
struct as_feature<tag::rolling_max(lazy)>
{
    typedef tag::lazy_rolling_max type;
};

// rolling_max(immediate) -> immediate_rolling_max
template<>
struct as_feature<tag::rolling_max(immediate)>
{
    typedef tag::immediate_rolling_max type;
};

// for the purposes of feature-based dependency resolution,
// immediate_rolling_max provides the same feature as rolling_max
template<>
struct feature_of<tag::immediate_rolling_max>
    : feature_of<tag::rolling_max>
{
};

// for the purposes of feature-based dependency resolution,
// lazy_rolling_max provides the same feature as rolling_max
template<>
struct feature_of<tag::lazy_rolling_max>
    : feature_of<tag::rolling_max>
{
};

}} // namespace boost::accumulators

#endif
