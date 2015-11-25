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

#include <ql/experimental/tenorbasis/forwardratecurve.hpp>

namespace QuantLib {

    ForwardRateCurve::ForwardRateCurve(const std::string& fwdFamilyName,
                                       const Period& fwdTenor,
                                       Natural fwdSettlementDays,
                                       const Currency& fwdCurrency,
                                       const Calendar& fwdFixingCalendar,
                                       BusinessDayConvention fwdConvention,
                                       bool fwdEndOfMonth,
                                       const DayCounter& fwdDayCounter,
                                       const DayCounter& dc)
    : TermStructure(fwdSettlementDays, fwdFixingCalendar, dc),
      fwdFamilyName_(fwdFamilyName), fwdTenor_(fwdTenor),
      fwdSettlementDays_(fwdSettlementDays), fwdCurrency_(fwdCurrency),
      fwdFixingCalendar_(fwdFixingCalendar), fwdConvention_(fwdConvention),
      fwdEndOfMonth_(fwdEndOfMonth), fwdDayCounter_(fwdDayCounter) {}

    ForwardRateCurve::ForwardRateCurve(const std::string& fwdFamilyName,
                                       const Period& fwdTenor,
                                       Natural fwdSettlementDays,
                                       const Currency& fwdCurrency,
                                       const Calendar& fwdFixingCalendar,
                                       BusinessDayConvention fwdConvention,
                                       bool fwdEndOfMonth,
                                       const DayCounter& fwdDayCounter,
                                       const Date& referenceDate,
                                       const Calendar& cal,
                                       const DayCounter& dc)
    : TermStructure(referenceDate, cal, dc),
      fwdFamilyName_(fwdFamilyName), fwdTenor_(fwdTenor),
      fwdSettlementDays_(fwdSettlementDays), fwdCurrency_(fwdCurrency),
      fwdFixingCalendar_(fwdFixingCalendar), fwdConvention_(fwdConvention),
      fwdEndOfMonth_(fwdEndOfMonth), fwdDayCounter_(fwdDayCounter) {}

    ForwardRateCurve::ForwardRateCurve(const std::string& fwdFamilyName,
                                       const Period& fwdTenor,
                                       Natural fwdSettlementDays,
                                       const Currency& fwdCurrency,
                                       const Calendar& fwdFixingCalendar,
                                       BusinessDayConvention fwdConvention,
                                       bool fwdEndOfMonth,
                                       const DayCounter& fwdDayCounter,
                                       Natural settlementDays,
                                       const Calendar& cal,
                                       const DayCounter& dc)
    : TermStructure(settlementDays, cal, dc),
      fwdFamilyName_(fwdFamilyName), fwdTenor_(fwdTenor),
      fwdSettlementDays_(fwdSettlementDays), fwdCurrency_(fwdCurrency),
      fwdFixingCalendar_(fwdFixingCalendar), fwdConvention_(fwdConvention),
      fwdEndOfMonth_(fwdEndOfMonth), fwdDayCounter_(fwdDayCounter) {}

    InterestRate ForwardRateCurve::forwardInterestRate(const Date& d,
                                                       bool extrap) const {
        Time t = timeFromReference(d);
        return forwardInterestRate(t, extrap);
    }

    Rate ForwardRateCurve::forwardRate(const Date& d,
                                       bool extrap) const {
        Time t = timeFromReference(d);
        return forwardRate(t, extrap);
    }

    InterestRate ForwardRateCurve::forwardInterestRate(Time t,
                                                       bool extrap) const {
        Rate fwd = forwardRate(t, extrap);
        return InterestRate(fwd, fwdDayCounter_, Simple, Annual);
    }

}
