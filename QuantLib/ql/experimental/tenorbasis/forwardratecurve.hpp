/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2015 Ferdinando Ametrano

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

/*! \file forwardratecurve.hpp
    \brief Interest-rate forward rate curve
*/

#ifndef quantlib_forward_rate_curve_hpp
#define quantlib_forward_rate_curve_hpp

#include <ql/termstructure.hpp>
#include <ql/currency.hpp>
#include <ql/interestrate.hpp>

namespace QuantLib {

    //! Term structure able to calculate forward interest rates (no discounts)
    /*! blah blah
    */
    class ForwardRateCurve : public TermStructure {
      public:
        /*! \name Constructors
            See the TermStructure documentation for issues regarding
            constructors.
        */
        //@{
        ForwardRateCurve(const std::string& fwdFamilyName,
                         const Period& fwdTenor,
                         Natural fwdSettlementDays,
                         const Currency& fwdCurrency,
                         const Calendar& fwdFixingCalendar,
                         BusinessDayConvention fwdConvention,
                         bool fwdEndOfMonth,
                         const DayCounter& fwdDayCounter,
                         const DayCounter& dc = DayCounter());
        ForwardRateCurve(const std::string& fwdFamilyName,
                         const Period& fwdTenor,
                         Natural fwdSettlementDays,
                         const Currency& fwdCurrency,
                         const Calendar& fwdFixingCalendar,
                         BusinessDayConvention fwdConvention,
                         bool fwdEndOfMonth,
                         const DayCounter& fwdDayCounter,
                         const Date& referenceDate,
                         const Calendar& cal = Calendar(),
                         const DayCounter& dc = DayCounter());
        ForwardRateCurve(const std::string& fwdFamilyName,
                         const Period& fwdTenor,
                         Natural fwdSettlementDays,
                         const Currency& fwdCurrency,
                         const Calendar& fwdFixingCalendar,
                         BusinessDayConvention fwdConvention,
                         bool fwdEndOfMonth,
                         const DayCounter& fwdDayCounter,
                         Natural settlementDays,
                         const Calendar& cal,
                         const DayCounter& dc = DayCounter());
        //@}
        /*! \name Forward rates

            These methods returns the forward interest rate for the fixed
            given tenor at a given date or time.  In the former case, time
            is calculated as fractions of year from the reference date,
            using the term structure daycounter.
        */
        //@{
        /*! The resulting interest rate has the required day-counting rule.
        */
        InterestRate forwardInterestRate(const Date& d,
                                         bool extrapolate = false) const;
        Rate forwardRate(const Date& d,
                         bool extrapolate = false) const;
        /*! The resulting interest rate has the required day-counting rule.
            The term structure daycounter has to be used for calculating the
            passed time t.
        */
        InterestRate forwardInterestRate(Time t,
                                         bool extrapolate = false) const;
        virtual Rate forwardRate(Time t,
                                 bool extrapolate = false) const = 0;
        //! the date at which discount = 1.0 and/or variance = 0.0
        const Date& referenceDate() const;
        //@}
      protected:
        std::string fwdFamilyName_;
        Period fwdTenor_;
        Natural fwdSettlementDays_;
        Currency fwdCurrency_;
        Calendar fwdFixingCalendar_;
        BusinessDayConvention fwdConvention_;
        bool fwdEndOfMonth_;
        DayCounter fwdDayCounter_;
    };

    inline const Date& ForwardRateCurve::referenceDate() const {
        Date today = Settings::instance().evaluationDate();
        if (!updated_) {
            fwdFixingCalendar_.advance(today, fwdSettlementDays_, Days);
            updated_ = true;
        }
        return fwdFixingCalendar_.advance(today, fwdSettlementDays_, Days);
    }

}

#endif
