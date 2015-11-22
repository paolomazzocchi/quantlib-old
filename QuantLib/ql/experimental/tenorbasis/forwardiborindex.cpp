/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2006, 2008, 2009 StatPro Italia srl
 Copyright (C) 2009 Ferdinando Ametrano

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

#include <ql/experimental/tenorbasis/forwardiborindex.hpp>
//#include <ql/termstructures/ForwardRateCurve.hpp>

namespace QuantLib {

    ForwardIborIndex::ForwardIborIndex(const std::string& familyName,
                         const Period& tenor,
                         Natural settlementDays,
                         const Currency& currency,
                         const Calendar& fixingCalendar,
                         BusinessDayConvention convention,
                         bool endOfMonth,
                         const DayCounter& dayCounter,
                         const Handle<ForwardRateCurve>& h)
    : IborIndex(familyName, tenor, settlementDays, currency,
                fixingCalendar, convention, endOfMonth, dayCounter),
      termStructure_(h){
        registerWith(termStructure_);
      }

    Rate ForwardIborIndex::forecastFixing(const Date& fixingDate) const {
        Date d1 = valueDate(fixingDate);
        Date d2 = maturityDate(d1);
        Time t = dayCounter_.yearFraction(d1, d2);
        QL_REQUIRE(t>0.0,
            "\n cannot calculate forward rate between " <<
            d1 << " and " << d2 <<
            ":\n non positive time (" << t <<
            ") using " << dayCounter_.name() << " daycounter");
        return forecastFixing(d1, d2, t);
    }

    boost::shared_ptr<ForwardIborIndex> ForwardIborIndex::clone(
                               const Handle<ForwardRateCurve>& h) const {
        return boost::shared_ptr<ForwardIborIndex>(
                                        new ForwardIborIndex(familyName(),
                                                      tenor(),
                                                      fixingDays(),
                                                      currency(),
                                                      fixingCalendar(),
                                                      businessDayConvention(),
                                                      endOfMonth(),
                                                      dayCounter(),
                                                      h));
    }

}
