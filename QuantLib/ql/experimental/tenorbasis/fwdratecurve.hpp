/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005, 2006, 2007, 2008 StatPro Italia srl
 Copyright (C) 2007, 2008, 2009 Ferdinando Ametrano
 Copyright (C) 2007 Chris Kenyon
 Copyright (C) 2015 Paolo Mazzocchi

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file piecewiseyieldcurve.hpp
    \brief piecewise-interpolated term structure
*/

#ifndef quantlib_fwd_rate_curve_hpp
#define quantlib_fwd_rate_curve_hpp

#include <ql/termstructures/iterativebootstrap.hpp>
#include <ql/termstructures/localbootstrap.hpp>
#include <ql/termstructures/bootstraphelper.hpp>
#include <ql/experimental/tenorbasis/interpolatedforwardratecurve.hpp>
#include <ql/patterns/lazyobject.hpp>

namespace QuantLib {

    
    //! Forward-Rate-curve traits
    struct ForwardRateTraits {
        // interpolated curve type
        template <class Interpolator>
        struct curve {
            typedef InterpolatedForwardRateCurve<Interpolator> type;
        };
        // helper class
        typedef BootstrapHelper<ForwardRateCurve> helper;

        // start of curve data
        static Date initialDate(const ForwardRateCurve* c) {
            return c->referenceDate();
        }
        // value at reference date
        static Real initialValue(const ForwardRateCurve*) {
            return 0.000242;
        }

        // guesses
        template <class C>
        static Real guess(Size i,
                          const C* c,
                          bool validData,
                          Size) // firstAliveHelper
        {
            if (validData) // previous iteration value
                return c->data()[i];

            if (i==1) // first pillar
                return 0.000242;

            // extrapolate
            Date d = c->dates()[i];
            return c->forwardInterestRate(d, true); //why true?
        }

        // possible constraints based on previous values
        template <class C>
        static Real minValueAfter(Size i,
                                  const C* c,
                                  bool validData,
                                  Size) // firstAliveHelper
        {
            if (validData) {
                Real r = *(std::min_element(c->data().begin(), c->data().end()));
                #if defined(QL_NEGATIVE_RATES)
                return r<0.0 ? r*2.0 : r / 2.0;
                #else
                return r/2.0;
                #endif
            }
            #if defined(QL_NEGATIVE_RATES)
            // no constraints.
            // We choose as min a value very unlikely to be exceeded.
            return -0.2;
            #else
            return QL_EPSILON;
            #endif
        }
        template <class C>
        static Real maxValueAfter(Size i,
                                  const C* c,
                                  bool validData,
                                  Size) // firstAliveHelper
        {
            if (validData) {
                Real r = *(std::max_element(c->data().begin(), c->data().end()));
                #if defined(QL_NEGATIVE_RATES)
                return r<0.0 ? r/2.0 : r*2.0;
                #else
                return r*2.0;
                #endif
            }
            // no constraints.
            // We choose as max a value very unlikely to be exceeded.
            return 0.2;
        }

        // root-finding update
        static void updateGuess(std::vector<Real>& data,
                                Real rate,
                                Size i) {
            data[i] = rate;
        }
        // upper bound for convergence loop
        static Size maxIterations() { return 100; }
    };

    //! fwd rate term structure
    /*! This term structure is bootstrapped on a number of interest
        rate instruments which are passed as a vector of handles to
        RateHelper instances. Their maturities mark the boundaries of
        the interpolated segments.

        Each segment is determined sequentially starting from the
        earliest period to the latest and is chosen so that the
        instrument whose maturity marks the end of such segment is
        correctly repriced on the curve.

        \warning The bootstrapping algorithm will raise an exception if
                 any two instruments have the same maturity date.

    */
    template <class Interpolator,
              template <class> class Bootstrap = IterativeBootstrap>
    class FwdRateCurve
        : public InterpolatedForwardRateCurve<Interpolator>,
          public LazyObject {
      private:
          typedef InterpolatedForwardRateCurve<Interpolator> base_curve;
          typedef FwdRateCurve<Interpolator, Bootstrap> this_curve;
      public:
        typedef ForwardRateTraits traits_type;
        typedef Interpolator interpolator_type;
        //! \name Constructors
        //@{
        FwdRateCurve(
              const std::string& fwdFamilyName,
              const Period& fwdTenor,
              Natural fwdSettlementDays,
              const Currency& fwdCurrency,
              const Calendar& fwdFixingCalendar,
              BusinessDayConvention fwdConvention,
              bool fwdEndOfMonth,
              const DayCounter& fwdDayCounter,
              const std::vector<boost::shared_ptr<typename traits_type::helper> >&
                                                                   instruments,
              Real accuracy,
              const Interpolator& i = Interpolator(),
              const Bootstrap<this_curve>& bootstrap = Bootstrap<this_curve>())
        : base_curve(fwdFamilyName, fwdTenor, fwdSettlementDays, fwdCurrency, 
                     fwdFixingCalendar, fwdConvention, fwdEndOfMonth, 
                     fwdDayCounter, i),
          instruments_(instruments),
          accuracy_(accuracy), bootstrap_(bootstrap) {
            bootstrap_.setup(this);
        }
        //@}
        //! \name TermStructure interface
        //@{
        Date maxDate() const;
        //@}
        //! \name base_curve interface
        //@{
        const std::vector<Time>& times() const;
        const std::vector<Date>& dates() const;
        const std::vector<Real>& data() const;
        std::vector<std::pair<Date, Real> > nodes() const;
		Rate forwardRate(Time t, bool extrapolate = false) const;
		//@}
        //! \name Observer interface
        //@{
        void update();
        //@}
      private:
        //! \name LazyObject interface
        //@{
        void performCalculations() const;
        //@}
        // data members
        std::vector<boost::shared_ptr<typename traits_type::helper> > instruments_;
        Real accuracy_;

        // bootstrapper classes are declared as friend to manipulate
        // the curve data. They might be passed the data instead, but
        // it would increase the complexity---which is high enough
        // already.
        friend class Bootstrap<this_curve>;
        friend class BootstrapError<this_curve> ;
        friend class PenaltyFunction<this_curve>;
        Bootstrap<this_curve> bootstrap_;
    };


    // inline definitions

    template <class I, template <class> class B>
    inline Date FwdRateCurve<I, B>::maxDate() const {
        calculate();
        return base_curve::maxDate();
    }

    template <class I, template <class> class B>
    inline const std::vector<Time>& FwdRateCurve<I, B>::times() const {
        calculate();
        return base_curve::times();
    }

    template <class I, template <class> class B>
    inline const std::vector<Date>& FwdRateCurve<I, B>::dates() const {
        calculate();
        return base_curve::dates();
    }

    template <class I, template <class> class B>
    inline const std::vector<Real>& FwdRateCurve<I, B>::data() const {
        calculate();
        return base_curve::data();
    }

    template <class I, template <class> class B>
    inline std::vector<std::pair<Date, Real> >
        FwdRateCurve<I, B>::nodes() const {
        calculate();
        return base_curve::nodes();
    }

	template <class I, template <class> class B>
	inline Rate FwdRateCurve<I, B>::forwardRate(Time t,
		                                        bool extrapolate) const {
		calculate();
		return base_curve::forwardRate(t, extrapolate);
	}

    template <class I, template <class> class B>
    inline void FwdRateCurve<I, B>::update() {

        // it dispatches notifications only if (!calculated_ && !frozen_)
        LazyObject::update();

        // do not use base_curve::update() as it would always notify observers

        // TermStructure::update() update part
        if (this->moving_)
            this->updated_ = false;

    }

    template <class I, template <class> class B>
    inline void FwdRateCurve<I, B>::performCalculations() const {
        // just delegate to the bootstrapper
        bootstrap_.calculate();
    }

}

#endif
