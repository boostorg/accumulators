///////////////////////////////////////////////////////////////////////////////
// rolling_min.hpp
//
//  Copyright 2018 Quentin Chateau. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_ROLLING_MIN_HPP_QC_20_12_2018
#define BOOST_ACCUMULATORS_STATISTICS_ROLLING_MIN_HPP_QC_20_12_2018

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/sorted_rolling_window.hpp>

namespace boost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // rolling_min_impl
    template<typename Sample>
    struct rolling_min_impl
      : accumulator_base
    {
        // for boost::result_of
        typedef Sample result_type;

        rolling_min_impl(dont_care)
        {
        }

        template<typename Args>
        result_type result(Args const &args) const
        {
            if (sorted_rolling_window(args).empty())
            {
                return numeric::as_max(Sample());
            }
            return sorted_rolling_window(args).front();
        }
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::rolling_min
//
namespace tag
{
    struct rolling_min
      : depends_on< sorted_rolling_window >
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::rolling_min_impl<mpl::_1> impl;
    };
}

///////////////////////////////////////////////////////////////////////////////
// extract::rolling_min
//
namespace extract
{
    extractor<tag::rolling_min> const rolling_min = {};

    BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_min)
}

using extract::rolling_min;

}} // namespace boost::accumulators

#endif
