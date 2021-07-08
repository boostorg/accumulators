///////////////////////////////////////////////////////////////////////////////
// sorted_rolling_window.hpp
//
// Copyright 2018 Quentin Chateau. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_SORTED_ROLLING_WINDOW_HPP_QC_20_12_2018
#define BOOST_ACCUMULATORS_STATISTICS_SORTED_ROLLING_WINDOW_HPP_QC_20_12_2018

#include <boost/container/set.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/rolling_window.hpp>

namespace boost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // sorted_rolling_window
    //    stores the latest N samples, where N is specified at construction time
    //    with the rolling_window_size named parameter. samples are sorted
    //    on insersion
    template<typename Sample>
    struct sorted_rolling_window_impl
      : accumulator_base
    {
        typedef typename container::multiset<Sample>::const_iterator const_iterator;
        typedef iterator_range<const_iterator> result_type;

        template<typename Args>
        sorted_rolling_window_impl(Args const &)
        {}

        template<typename Args>
        void operator ()(Args const &args)
        {
            if(is_rolling_window_plus1_full(args))
            {
                const_iterator it = this->sorted_buffer_.find(rolling_window_plus1(args).front());
                this->sorted_buffer_.erase(it);
            }
            this->sorted_buffer_.insert(args[sample]);
        }

        result_type result(dont_care) const
        {
            return result_type(this->sorted_buffer_.begin(), this->sorted_buffer_.end());
        }

    private:
        container::multiset<Sample> sorted_buffer_;
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::sorted_rolling_window
//
namespace tag
{
    struct sorted_rolling_window
      : depends_on< rolling_window_plus1 >
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::sorted_rolling_window_impl< mpl::_1 > impl;

        #ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
        /// tag::sorted_rolling_window::size named parameter
        static boost::parameter::keyword<tag::rolling_window_size> const window_size;
        #endif
    };

} // namespace tag

///////////////////////////////////////////////////////////////////////////////
// extract::sorted_rolling_window
//
namespace extract
{
    extractor<tag::sorted_rolling_window> const sorted_rolling_window = {};

    BOOST_ACCUMULATORS_IGNORE_GLOBAL(sorted_rolling_window)
}

using extract::sorted_rolling_window;

}} // namespace boost::accumulators

#endif
