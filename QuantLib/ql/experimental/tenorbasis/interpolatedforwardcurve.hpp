/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
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

#ifndef quantlib_interpolated_forward_curve_hpp
#define quantlib_interpolated_forward_curve_hpp

//#include <ql/math/interpolation.hpp>
//#include <vector>
#include <ql/experimental/tenorbasis/forwardratecurve.hpp>
//#include <ql/termstructures/interpolatedcurve.hpp>
//#include <ql/math/interpolations/loginterpolation.hpp>
#include <ql/math/comparison.hpp>
#include <utility>

namespace QuantLib {

    //! TermStructure based on interpolation of forward rate

    template <class Interpolator>
    class InterpolatedForwardCurve : public ForwardRateCurve,
                                     public InterpolatedCurve<Interpolator> {
    public:
        InterpolatedForwardCurve(
            const std::string& fwdFamilyName,
            const Period& fwdTenor,
            Natural fwdSettlementDays,
            const Currency& fwdCurrency,
            const Calendar& fwdFixingCalendar,
            BusinessDayConvention fwdConvention,
            bool fwdEndOfMonth,
            const DayCounter& fwdDayCounter,
            const std::vector<boost::shared_ptr<typename Traits::helper> >&
            instruments,
            const DayCounter& dayCounter,
            Real accuracy,
            const Interpolator& i = Interpolator()
            //const Bootstrap<this_curve>& bootstrap = Bootstrap<this_curve>()
            )
        : ForwardRateCurve(fwdFamilyName, fwdTenor, fwdSettlementDays,
                           fwdCurrency, fwdFixingCalendar, fwdConvention, 
                           fwdEndOfMonth, fwdDayCounter), 
          instruments_(instruments), accuracy_(accuracy), 
          bootstrap_(IterativeBootstrap<this_curve>) {
            bootstrap_.setup(this);

            ForwardRateCurve(
                
                
                BusinessDayConvention fwdConvention,
                bool fwdEndOfMonth,
                const DayCounter& fwdDayCounter,
                const DayCounter& dc = DayCounter());

        }


        //! \name TermStructure interface
        //@{
        Date maxDate() const;
        //@}
        //! \name other inspectors
        //@{
        const std::vector<Time>& times() const;
        const std::vector<Date>& dates() const;
        const std::vector<Real>& data() const;
        const std::vector<Rate>& forwards() const;
        std::vector<std::pair<Date, Real> > nodes() const;
        //@}
    protected:
        InterpolatedForwardCurve(
            const DayCounter&,
            const std::vector<Handle<Quote> >& jumps = std::vector<Handle<Quote> >(),
            const std::vector<Date>& jumpDates = std::vector<Date>(),
            const Interpolator& interpolator = Interpolator());
        InterpolatedForwardCurve(
            const Date& referenceDate,
            const DayCounter&,
            const std::vector<Handle<Quote> >& jumps = std::vector<Handle<Quote> >(),
            const std::vector<Date>& jumpDates = std::vector<Date>(),
            const Interpolator& interpolator = Interpolator());
        InterpolatedForwardCurve(
            Natural settlementDays,
            const Calendar&,
            const DayCounter&,
            const std::vector<Handle<Quote> >& jumps = std::vector<Handle<Quote> >(),
            const std::vector<Date>& jumpDates = std::vector<Date>(),
            const Interpolator& interpolator = Interpolator());
        //! \name ForwardRateStructure implementation
        //@{
        Rate forwardImpl(Time t) const;
        Rate zeroYieldImpl(Time t) const;
        //@}
        mutable std::vector<Date> dates_;
    private:
        void initialize();
    };

        Rate forwardRate(Time t, bool extrapolate = false) const {
            return interpolation_(t, extrapolate);
        }
}

#endif