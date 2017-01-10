///////////////////////////////////////////////////////////////////////////////
// quartile.hpp
//

#ifndef BOOST_ACCUMULATORS_STATISTICS_QUARTILE
#define BOOST_ACCUMULATORS_STATISTICS_QUARTILE

#include <boost/mpl/placeholders.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/p_square_quantile.hpp>
#include <boost/accumulators/statistics/density.hpp>
#include <boost/accumulators/statistics/p_square_cumul_dist.hpp>

namespace boost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // first_quartile_impl
    //
    /**
        @brief Median estimation based on the \f$P^2\f$ quantile estimator

        The \f$P^2\f$ algorithm is invoked with a quantile probability of 0.5.
    */
    template<typename Sample>
    struct first_quartile_impl
      : accumulator_base
    {
        // for boost::result_of
        typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type result_type;

        first_quartile_impl(dont_care) {}

        template<typename Args>
        result_type result(Args const &args) const
        {
            return p_square_quantile_for_first_quartile(args);
        }
    };
    ///////////////////////////////////////////////////////////////////////////////
    // with_density_first_quartile_impl
    //
    /**
        @brief Median estimation based on the density estimator

        The algorithm determines the bin in which the \f$0.5*cnt\f$-th sample lies, \f$cnt\f$ being
        the total number of samples. It returns the approximate horizontal position of this sample,
        based on a linear interpolation inside the bin.
    */
    template<typename Sample>
    struct with_density_first_quartile_impl
      : accumulator_base
    {
        typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type float_type;
        typedef std::vector<std::pair<float_type, float_type> > histogram_type;
        typedef iterator_range<typename histogram_type::iterator> range_type;
        // for boost::result_of
        typedef float_type result_type;

        template<typename Args>
        with_density_first_quartile_impl(Args const &args)
          : sum(numeric::fdiv(args[sample | Sample()], (std::size_t)1))
          , is_dirty(true)
        {
        }

        void operator ()(dont_care)
        {
            this->is_dirty = true;
        }


        template<typename Args>
        result_type result(Args const &args) const
        {
            if (this->is_dirty)
            {
                this->is_dirty = false;

                std::size_t cnt = count(args);
                range_type histogram = density(args);
                typename range_type::iterator it = histogram.begin();
                while (this->sum < 0.5 * cnt)
                {
                    this->sum += it->second * cnt;
                    ++it;
                }
                --it;
                float_type over = numeric::fdiv(this->sum - 0.5 * cnt, it->second * cnt);
                this->first_quartile = it->first * over + (it + 1)->first * (1. - over);
            }

            return this->first_quartile;
        }

    private:
        mutable float_type sum;
        mutable bool is_dirty;
        mutable float_type first_quartile;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // with_p_square_cumulative_distribution_first_quartile_impl
    //
    /**
        @brief Median estimation based on the \f$P^2\f$ cumulative distribution estimator

        The algorithm determines the first (leftmost) bin with a height exceeding 0.5. It
        returns the approximate horizontal position of where the cumulative distribution
        equals 0.5, based on a linear interpolation inside the bin.
    */
    template<typename Sample>
    struct with_p_square_cumulative_distribution_first_quartile_impl
      : accumulator_base
    {
        typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type float_type;
        typedef std::vector<std::pair<float_type, float_type> > histogram_type;
        typedef iterator_range<typename histogram_type::iterator> range_type;
        // for boost::result_of
        typedef float_type result_type;

        with_p_square_cumulative_distribution_first_quartile_impl(dont_care)
          : is_dirty(true)
        {
        }

        void operator ()(dont_care)
        {
            this->is_dirty = true;
        }

        template<typename Args>
        result_type result(Args const &args) const
        {
            if (this->is_dirty)
            {
                this->is_dirty = false;

                range_type histogram = p_square_cumulative_distribution(args);
                typename range_type::iterator it = histogram.begin();
                while (it->second < 0.5)
                {
                    ++it;
                }
                float_type over = numeric::fdiv(it->second - 0.5, it->second - (it - 1)->second);
                this->first_quartile = it->first * over + (it + 1)->first * ( 1. - over );
            }

            return this->first_quartile;
        }
    private:

        mutable bool is_dirty;
        mutable float_type first_quartile;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // third_quartile_impl
    //
    /**
        @brief Median estimation based on the \f$P^2\f$ quantile estimator

        The \f$P^2\f$ algorithm is invoked with a quantile probability of 0.5.
    */
    template<typename Sample>
    struct third_quartile_impl
      : accumulator_base
    {
        // for boost::result_of
        typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type result_type;

        third_quartile_impl(dont_care) {}

        template<typename Args>
        result_type result(Args const &args) const
        {
            return p_square_quantile_for_third_quartile(args);
        }
    };
    ///////////////////////////////////////////////////////////////////////////////
    // with_density_third_quartile_impl
    //
    /**
        @brief Median estimation based on the density estimator

        The algorithm determines the bin in which the \f$0.5*cnt\f$-th sample lies, \f$cnt\f$ being
        the total number of samples. It returns the approximate horizontal position of this sample,
        based on a linear interpolation inside the bin.
    */
    template<typename Sample>
    struct with_density_third_quartile_impl
      : accumulator_base
    {
        typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type float_type;
        typedef std::vector<std::pair<float_type, float_type> > histogram_type;
        typedef iterator_range<typename histogram_type::iterator> range_type;
        // for boost::result_of
        typedef float_type result_type;

        template<typename Args>
        with_density_third_quartile_impl(Args const &args)
          : sum(numeric::fdiv(args[sample | Sample()], (std::size_t)1))
          , is_dirty(true)
        {
        }

        void operator ()(dont_care)
        {
            this->is_dirty = true;
        }


        template<typename Args>
        result_type result(Args const &args) const
        {
            if (this->is_dirty)
            {
                this->is_dirty = false;

                std::size_t cnt = count(args);
                range_type histogram = density(args);
                typename range_type::iterator it = histogram.begin();
                while (this->sum < 0.5 * cnt)
                {
                    this->sum += it->second * cnt;
                    ++it;
                }
                --it;
                float_type over = numeric::fdiv(this->sum - 0.5 * cnt, it->second * cnt);
                this->third_quartile = it->third * over + (it + 1)->third * (1. - over);
            }

            return this->third_quartile;
        }

    private:
        mutable float_type sum;
        mutable bool is_dirty;
        mutable float_type third_quartile;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // with_p_square_cumulative_distribution_third_quartile_impl
    //
    /**
        @brief Median estimation based on the \f$P^2\f$ cumulative distribution estimator

        The algorithm determines the third (leftmost) bin with a height exceeding 0.5. It
        returns the approximate horizontal position of where the cumulative distribution
        equals 0.5, based on a linear interpolation inside the bin.
    */
    template<typename Sample>
    struct with_p_square_cumulative_distribution_third_quartile_impl
      : accumulator_base
    {
        typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type float_type;
        typedef std::vector<std::pair<float_type, float_type> > histogram_type;
        typedef iterator_range<typename histogram_type::iterator> range_type;
        // for boost::result_of
        typedef float_type result_type;

        with_p_square_cumulative_distribution_third_quartile_impl(dont_care)
          : is_dirty(true)
        {
        }

        void operator ()(dont_care)
        {
            this->is_dirty = true;
        }

        template<typename Args>
        result_type result(Args const &args) const
        {
            if (this->is_dirty)
            {
                this->is_dirty = false;

                range_type histogram = p_square_cumulative_distribution(args);
                typename range_type::iterator it = histogram.begin();
                while (it->second < 0.5)
                {
                    ++it;
                }
                float_type over = numeric::fdiv(it->second - 0.5, it->second - (it - 1)->second);
                this->third_quartile = it->third * over + (it + 1)->third * ( 1. - over );
            }

            return this->third_quartile;
        }
    private:

        mutable bool is_dirty;
        mutable float_type third_quartile;
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::first_quartile
// tag::with_densisty_first_quartile
// tag::with_p_square_cumulative_distribution_first_quartile
// tag::third_quartile
// tag::with_densisty_third_quartile
// tag::with_p_square_cumulative_distribution_third_quartile
//
namespace tag
{
    struct first_quartile
      : depends_on<p_square_quantile_for_first_quartile>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::first_quartile_impl<mpl::_1> impl;
    };
    struct with_density_first_quartile
      : depends_on<count, density>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::with_density_first_quartile_impl<mpl::_1> impl;
    };
    struct with_p_square_cumulative_distribution_first_quartile
      : depends_on<p_square_cumulative_distribution>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::with_p_square_cumulative_distribution_first_quartile_impl<mpl::_1> impl;
    };
    struct third_quartile
      : depends_on<p_square_quantile_for_third_quartile>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::third_quartile_impl<mpl::_1> impl;
    };
    struct with_density_third_quartile
      : depends_on<count, density>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::with_density_third_quartile_impl<mpl::_1> impl;
    };
    struct with_p_square_cumulative_distribution_third_quartile
      : depends_on<p_square_cumulative_distribution>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::with_p_square_cumulative_distribution_third_quartile_impl<mpl::_1> impl;
    };
}

///////////////////////////////////////////////////////////////////////////////
// extract::first_quartile
// extract::with_density_first_quartile
// extract::with_p_square_cumulative_distribution_first_quartile
// extract::third_quartile
// extract::with_density_third_quartile
// extract::with_p_square_cumulative_distribution_third_quartile
//
namespace extract
{
    extractor<tag::first_quartile> const first_quartile = {};
    extractor<tag::with_density_first_quartile> const with_density_first_quartile = {};
    extractor<tag::with_p_square_cumulative_distribution_first_quartile> const with_p_square_cumulative_distribution_first_quartile = {};
    extractor<tag::third_quartile> const third_quartile = {};
    extractor<tag::with_density_third_quartile> const with_density_third_quartile = {};
    extractor<tag::with_p_square_cumulative_distribution_third_quartile> const with_p_square_cumulative_distribution_third_quartile = {};

    BOOST_ACCUMULATORS_IGNORE_GLOBAL(first_quartile)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(with_density_first_quartile)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(with_p_square_cumulative_distribution_first_quartile)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(third_quartile)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(with_density_third_quartile)
    BOOST_ACCUMULATORS_IGNORE_GLOBAL(with_p_square_cumulative_distribution_third_quartile)
}

using extract::first_quartile;
using extract::with_density_first_quartile;
using extract::with_p_square_cumulative_distribution_first_quartile;
using extract::third_quartile;
using extract::with_density_third_quartile;
using extract::with_p_square_cumulative_distribution_third_quartile;

// first_quartile(with_p_square_quantile) -> first_quartile
template<>
struct as_feature<tag::first_quartile(with_p_square_quantile)>
{
    typedef tag::first_quartile type;
};

// first_quartile(with_density) -> with_density_first_quartile
template<>
struct as_feature<tag::first_quartile(with_density)>
{
    typedef tag::with_density_first_quartile type;
};

// first_quartile(with_p_square_cumulative_distribution) -> with_p_square_cumulative_distribution_first_quartile
template<>
struct as_feature<tag::first_quartile(with_p_square_cumulative_distribution)>
{
    typedef tag::with_p_square_cumulative_distribution_first_quartile type;
};

// for the purposes of feature-based dependency resolution,
// with_density_first_quartile and with_p_square_cumulative_distribution_first_quartile
// provide the same feature as first_quartile
template<>
struct feature_of<tag::with_density_first_quartile>
  : feature_of<tag::first_quartile>
{
};

template<>
struct feature_of<tag::with_p_square_cumulative_distribution_first_quartile>
  : feature_of<tag::first_quartile>
{
};

// So that first_quartile can be automatically substituted with
// weighted_first_quartile when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::first_quartile>
{
    typedef tag::weighted_first_quartile type;
};

template<>
struct feature_of<tag::weighted_first_quartile>
  : feature_of<tag::first_quartile>
{
};

// So that with_density_first_quartile can be automatically substituted with
// with_density_weighted_first_quartile when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::with_density_first_quartile>
{
    typedef tag::with_density_weighted_first_quartile type;
};

template<>
struct feature_of<tag::with_density_weighted_first_quartile>
  : feature_of<tag::with_density_first_quartile>
{
};

// So that with_p_square_cumulative_distribution_first_quartile can be automatically substituted with
// with_p_square_cumulative_distribution_weighted_first_quartile when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::with_p_square_cumulative_distribution_first_quartile>
{
    typedef tag::with_p_square_cumulative_distribution_weighted_first_quartile type;
};

template<>
struct feature_of<tag::with_p_square_cumulative_distribution_weighted_first_quartile>
  : feature_of<tag::with_p_square_cumulative_distribution_first_quartile>
{
};

// third_quartile(with_p_square_quantile) -> third_quartile
template<>
struct as_feature<tag::third_quartile(with_p_square_quantile)>
{
    typedef tag::third_quartile type;
};

// third_quartile(with_density) -> with_density_third_quartile
template<>
struct as_feature<tag::third_quartile(with_density)>
{
    typedef tag::with_density_third_quartile type;
};

// third_quartile(with_p_square_cumulative_distribution) -> with_p_square_cumulative_distribution_third_quartile
template<>
struct as_feature<tag::third_quartile(with_p_square_cumulative_distribution)>
{
    typedef tag::with_p_square_cumulative_distribution_third_quartile type;
};

// for the purposes of feature-based dependency resolution,
// with_density_third_quartile and with_p_square_cumulative_distribution_third_quartile
// provide the same feature as third_quartile
template<>
struct feature_of<tag::with_density_third_quartile>
  : feature_of<tag::third_quartile>
{
};

template<>
struct feature_of<tag::with_p_square_cumulative_distribution_third_quartile>
  : feature_of<tag::third_quartile>
{
};

// So that third_quartile can be automatically substituted with
// weighted_third_quartile when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::third_quartile>
{
    typedef tag::weighted_third_quartile type;
};

template<>
struct feature_of<tag::weighted_third_quartile>
  : feature_of<tag::third_quartile>
{
};

// So that with_density_third_quartile can be automatically substituted with
// with_density_weighted_third_quartile when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::with_density_third_quartile>
{
    typedef tag::with_density_weighted_third_quartile type;
};

template<>
struct feature_of<tag::with_density_weighted_third_quartile>
  : feature_of<tag::with_density_third_quartile>
{
};

// So that with_p_square_cumulative_distribution_third_quartile can be automatically substituted with
// with_p_square_cumulative_distribution_weighted_third_quartile when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::with_p_square_cumulative_distribution_third_quartile>
{
    typedef tag::with_p_square_cumulative_distribution_weighted_third_quartile type;
};

template<>
struct feature_of<tag::with_p_square_cumulative_distribution_weighted_third_quartile>
  : feature_of<tag::with_p_square_cumulative_distribution_third_quartile>
{
};


}} // namespace boost::accumulators

#endif
