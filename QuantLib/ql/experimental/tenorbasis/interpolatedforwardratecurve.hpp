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

#include <ql/experimental/tenorbasis/forwardratecurve.hpp>
#include <ql/termstructures/interpolatedcurve.hpp>
#include <ql/math/comparison.hpp>
#include <utility>

namespace QuantLib {

    //! TermStructure based on interpolation of forward rate
    template <class Interpolator>
    class InterpolatedForwardRateCurve : public ForwardRateCurve,
                                       public InterpolatedCurve<Interpolator> {
    public:
        // constructor
        InterpolatedForwardRateCurve(const std::string& fwdFamilyName,
                                     const Period& fwdTenor,
                                     Natural fwdSettlementDays,
                                     const Currency& fwdCurrency,
                                     const Calendar& fwdFixingCalendar,
                                     BusinessDayConvention fwdConvention,
                                     bool fwdEndOfMonth,
                                     const DayCounter& fwdDayCounter,
                                     const std::vector<Date>& dates,
                                     const std::vector<Rate>& forwards,
                                     const Interpolator& i = Interpolator());
        //! \name TermStructure interface
        //@{
        Date maxDate() const;
        //@}
        //! \name other inspectors
        //@{
        const std::vector<Time>& times() const;
        const std::vector<Date>& dates() const;
        const std::vector<Real>& data() const;
        const std::vector<Rate>& forwardRates() const;
        std::vector<std::pair<Date, Real> > nodes() const;
        //@}
        //! \name ForwardRateCurve implementation
        //@{
        Rate forwardRate(Time t, bool extrapolate = false) const {
                                         return interpolation_(t, extrapolate);
        }
        // we need it ? 
        // Rate forwardRate(Time t) const { return forwardRate(t, false); }
        //@}
    protected:
        InterpolatedForwardRateCurve(
            const std::string& fwdFamilyName,
            const Period& fwdTenor,
            Natural fwdSettlementDays,
            const Currency& fwdCurrency,
            const Calendar& fwdFixingCalendar,
            BusinessDayConvention fwdConvention,
            bool fwdEndOfMonth,
            const DayCounter& fwdDayCounter,
            const Interpolator& interpolator = Interpolator());

        mutable std::vector<Date> dates_;
    private:
        void initialize();
    };

    // inline definitions

    template <class T>
    inline Date InterpolatedForwardRateCurve<T>::maxDate() const {
        if (this->maxDate_ != Date())
            return this->maxDate_;
        return dates_.back();
    }

    template <class T>
    inline const std::vector<Time>&
        InterpolatedForwardRateCurve<T>::times() const {
        return this->times_;
    }

    template <class T>
    inline const std::vector<Date>&
        InterpolatedForwardRateCurve<T>::dates() const {
        return dates_;
    }

    template <class T>
    inline const std::vector<Real>&
        InterpolatedForwardRateCurve<T>::data() const {
        return this->data_;
    }

    template <class T>
    inline const std::vector<Rate>&
        InterpolatedForwardRateCurve<T>::forwardRates() const {
        return this->data_;
    }

    template <class T>
    inline std::vector<std::pair<Date, Real> >
        InterpolatedForwardRateCurve<T>::nodes() const {
        std::vector<std::pair<Date, Real> > results(dates_.size());
        for (Size i = 0; i<dates_.size(); ++i)
            results[i] = std::make_pair(dates_[i], this->data_[i]);
        return results;
    }

#ifndef __DOXYGEN__

    // template definitions

    template <class T>
    InterpolatedForwardRateCurve<T>::InterpolatedForwardRateCurve(
        const std::string& fwdFamilyName,
        const Period& fwdTenor,
        Natural fwdSettlementDays,
        const Currency& fwdCurrency,
        const Calendar& fwdFixingCalendar,
        BusinessDayConvention fwdConvention,
        bool fwdEndOfMonth,
        const DayCounter& fwdDayCounter,
        const T& interpolator)
    : ForwardRateCurve(fwdFamilyName, fwdTenor, fwdSettlementDays, fwdCurrency,
                       fwdFixingCalendar, fwdConvention, fwdEndOfMonth, 
                       fwdDayCounter), 
      InterpolatedCurve<T>(interpolator) {}

    template <class T>
    InterpolatedForwardRateCurve<T>::InterpolatedForwardRateCurve(
                                        const std::string& fwdFamilyName,
                                        const Period& fwdTenor,
                                        Natural fwdSettlementDays,
                                        const Currency& fwdCurrency,
                                        const Calendar& fwdFixingCalendar,
                                        BusinessDayConvention fwdConvention,
                                        bool fwdEndOfMonth,
                                        const DayCounter& fwdDayCounter,
                                        const std::vector<Date>& dates,
                                        const std::vector<Rate>& forwards,
                                        const T& i)
    : ForwardRateCurve(fwdFamilyName, fwdTenor, fwdSettlementDays, fwdCurrency,
                       fwdFixingCalendar, fwdConvention, fwdEndOfMonth,
                       fwdDayCounter),
      InterpolatedCurve<T>(std::vector<Time>(), forwards, i), dates_(dates)
        {
            initialize();
        }

    #endif

    template <class T>
    void InterpolatedForwardRateCurve<T>::initialize()
    {
        QL_REQUIRE(dates_.size() >= T::requiredPoints,
            "not enough input dates given");
        QL_REQUIRE(this->data_.size() == dates_.size(),
            "dates/data count mismatch");

        this->times_.resize(dates_.size());
        this->times_[0] = 0.0;
        for (Size i = 1; i<dates_.size(); ++i) {
            QL_REQUIRE(dates_[i] > dates_[i - 1],
                "invalid date (" << dates_[i] << ", vs "
                << dates_[i - 1] << ")");
            this->times_[i] = dayCounter().yearFraction(dates_[0], dates_[i]);
            QL_REQUIRE(!close(this->times_[i], this->times_[i - 1]),
                "two dates correspond to the same time "
                "under this curve's day count convention");
            #if !defined(QL_NEGATIVE_RATES)
            QL_REQUIRE(this->data_[i] >= 0.0, "negative forward");
            #endif
        }

        this->interpolation_ =
            this->interpolator_.interpolate(this->times_.begin(),
                                            this->times_.end(),
                                            this->data_.begin());
        this->interpolation_.update();
    }

}

#endif