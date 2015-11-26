/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 StatPro Italia srl
 Copyright (C) 2007, 2008, 2009, 2015 Ferdinando Ametrano
 Copyright (C) 2007, 2009 Roland Lichters
 Copyright (C) 2015 Maddalena Zanzi
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

#include <ql/experimental/tenorbasis/forwardhelpers.hpp>
#include <ql/experimental/tenorbasis/forwardiborindex.hpp>
#include <ql/time/imm.hpp>
#include <ql/time/asx.hpp>
#include <ql/time/calendars/jointcalendar.hpp>
#include <ql/instruments/makevanillaswap.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/quote.hpp>
#include <ql/currency.hpp>
#include <ql/indexes/swapindex.hpp>
#ifdef QL_USE_INDEXED_COUPON
    #include <ql/cashflows/floatingratecoupon.hpp>
#endif

using boost::shared_ptr;

namespace QuantLib {

    namespace {
        void no_deletion(ForwardRateCurve*) {}
    }

    FuturesForwardHelper::FuturesForwardHelper(const Handle<Quote>& price,
                                         const Date& iborStartDate,
                                         Natural lengthInMonths,
                                         const Calendar& calendar,
                                         BusinessDayConvention convention,
                                         bool endOfMonth,
                                         const DayCounter& dayCounter,
                                         const Handle<Quote>& convAdj,
                                         Futures::Type type)
    : ForwardHelper(price), convAdj_(convAdj) {
        switch (type) {
          case Futures::IMM:
            QL_REQUIRE(IMM::isIMMdate(iborStartDate, false),
                       iborStartDate << " is not a valid IMM date");
            break;
          case Futures::ASX:
            QL_REQUIRE(ASX::isASXdate(iborStartDate, false),
                       iborStartDate << " is not a valid ASX date");
            break;
          default:
            QL_FAIL("unknown futures type (" << Integer(type) << ")");
        }
        earliestDate_ = iborStartDate;
        maturityDate_ = calendar.advance(iborStartDate, lengthInMonths*Months,
                                         convention, endOfMonth);
        yearFraction_ = dayCounter.yearFraction(earliestDate_, maturityDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;

        registerWith(convAdj_);
    }

    FuturesForwardHelper::FuturesForwardHelper(Real price,
                                         const Date& iborStartDate,
                                         Natural lengthInMonths,
                                         const Calendar& calendar,
                                         BusinessDayConvention convention,
                                         bool endOfMonth,
                                         const DayCounter& dayCounter,
                                         Rate convAdj,
                                         Futures::Type type)
    : ForwardHelper(price),
      convAdj_(Handle<Quote>(shared_ptr<Quote>(new SimpleQuote(convAdj))))
    {
        switch (type) {
          case Futures::IMM:
            QL_REQUIRE(IMM::isIMMdate(iborStartDate, false),
                iborStartDate << " is not a valid IMM date");
            break;
          case Futures::ASX:
            QL_REQUIRE(ASX::isASXdate(iborStartDate, false),
                iborStartDate << " is not a valid ASX date");
            break;
          default:
            QL_FAIL("unknown futures type (" << Integer(type) << ")");
        }
        earliestDate_ = iborStartDate;
        maturityDate_ = calendar.advance(iborStartDate, lengthInMonths*Months,
            convention, endOfMonth);
        yearFraction_ = dayCounter.yearFraction(earliestDate_, maturityDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;
    }

    FuturesForwardHelper::FuturesForwardHelper(const Handle<Quote>& price,
                                         const Date& iborStartDate,
                                         const Date& iborEndDate,
                                         const DayCounter& dayCounter,
                                         const Handle<Quote>& convAdj,
                                         Futures::Type type)
    : ForwardHelper(price), convAdj_(convAdj) {
        switch (type) {
          case Futures::IMM:
            QL_REQUIRE(IMM::isIMMdate(iborStartDate, false),
                       iborStartDate << " is not a valid IMM date");
            if (iborEndDate == Date()) {
                // advance 3 months
                maturityDate_ = IMM::nextDate(iborStartDate, false);
                maturityDate_ = IMM::nextDate(maturityDate_, false);
                maturityDate_ = IMM::nextDate(maturityDate_, false);
            }
            else {
                QL_REQUIRE(iborEndDate>iborStartDate,
                           "end date (" << iborEndDate <<
                           ") must be greater than start date (" <<
                           iborStartDate << ")");
                maturityDate_ = iborEndDate;
            }
            break;
          case Futures::ASX:
            QL_REQUIRE(ASX::isASXdate(iborStartDate, false),
                       iborStartDate << " is not a valid ASX date");
            if (iborEndDate == Date()) {
                // advance 3 months
                maturityDate_ = ASX::nextDate(iborStartDate, false);
                maturityDate_ = ASX::nextDate(maturityDate_, false);
                maturityDate_ = ASX::nextDate(maturityDate_, false);
            }
            else {
                QL_REQUIRE(iborEndDate>iborStartDate,
                           "end date (" << iborEndDate <<
                           ") must be greater than start date (" <<
                          iborStartDate << ")");
                maturityDate_ = iborEndDate;
            }
            break;
          default:
            QL_FAIL("unknown futures type (" << Integer(type) << ")");
        }
        earliestDate_ = iborStartDate;
        yearFraction_ = dayCounter.yearFraction(earliestDate_, maturityDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;

        registerWith(convAdj_);
    }

    FuturesForwardHelper::FuturesForwardHelper(Real price,
                                         const Date& iborStartDate,
                                         const Date& iborEndDate,
                                         const DayCounter& dayCounter,
                                         Rate convAdj,
                                         Futures::Type type)
    : ForwardHelper(price),
      convAdj_(Handle<Quote>(shared_ptr<Quote>(new SimpleQuote(convAdj))))
    {
        switch (type) {
          case Futures::IMM:
            QL_REQUIRE(IMM::isIMMdate(iborStartDate, false),
                       iborStartDate << " is not a valid IMM date");
            if (iborEndDate == Date()) {
                // advance 3 months
                maturityDate_ = IMM::nextDate(iborStartDate, false);
                maturityDate_ = IMM::nextDate(maturityDate_, false);
                maturityDate_ = IMM::nextDate(maturityDate_, false);
            }
            else {
                QL_REQUIRE(iborEndDate>iborStartDate,
                           "end date (" << iborEndDate <<
                           ") must be greater than start date (" <<
                           iborStartDate << ")");
                maturityDate_ = iborEndDate;
            }
            break;
          case Futures::ASX:
            QL_REQUIRE(ASX::isASXdate(iborStartDate, false),
                iborStartDate << " is not a valid ASX date");
            if (iborEndDate == Date()) {
                // advance 3 months
                maturityDate_ = ASX::nextDate(iborStartDate, false);
                maturityDate_ = ASX::nextDate(maturityDate_, false);
                maturityDate_ = ASX::nextDate(maturityDate_, false);
            }
            else {
                QL_REQUIRE(iborEndDate>iborStartDate,
                           "end date (" << iborEndDate <<
                           ") must be greater than start date (" <<
                           iborStartDate << ")");
                latestRelevantDate_ = iborEndDate;
            }
            break;
          default:
            QL_FAIL("unknown futures type (" << Integer(type) << ")");
        }
        earliestDate_ = iborStartDate;
        yearFraction_ = dayCounter.yearFraction(earliestDate_, maturityDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;
    }

    FuturesForwardHelper::FuturesForwardHelper(const Handle<Quote>& price,
                                         const Date& iborStartDate,
                                         const shared_ptr<ForwardIborIndex>& i,
                                         const Handle<Quote>& convAdj,
                                         Futures::Type type)
    : ForwardHelper(price), convAdj_(convAdj) {
        switch (type) {
          case Futures::IMM:
            QL_REQUIRE(IMM::isIMMdate(iborStartDate, false),
                       iborStartDate << " is not a valid IMM date");
            break;
          case Futures::ASX:
            QL_REQUIRE(ASX::isASXdate(iborStartDate, false),
                       iborStartDate << " is not a valid ASX date");
            break;
          default:
            QL_FAIL("unknown futures type (" << Integer(type) << ")");
        }
        earliestDate_ = iborStartDate;
        const Calendar& cal = i->fixingCalendar();
        maturityDate_ = cal.advance(iborStartDate, i->tenor(),
                                    i->businessDayConvention());
        yearFraction_ = i->dayCounter().yearFraction(earliestDate_,
                                                     maturityDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;

        registerWith(convAdj);
    }

    FuturesForwardHelper::FuturesForwardHelper(Real price,
                                         const Date& iborStartDate,
                                         const shared_ptr<ForwardIborIndex>& i,
                                         Rate convAdj,
                                         Futures::Type type)
    : ForwardHelper(price),
      convAdj_(Handle<Quote>(shared_ptr<Quote>(new SimpleQuote(convAdj))))
    {
        switch (type) {
          case Futures::IMM:
            QL_REQUIRE(IMM::isIMMdate(iborStartDate, false),
                iborStartDate << " is not a valid IMM date");
            break;
          case Futures::ASX:
            QL_REQUIRE(ASX::isASXdate(iborStartDate, false),
                iborStartDate << " is not a valid ASX date");
            break;
          default:
            QL_FAIL("unknown futures type (" << Integer(type) << ")");
        }
        earliestDate_ = iborStartDate;
        const Calendar& cal = i->fixingCalendar();
        maturityDate_ = cal.advance(iborStartDate, i->tenor(),
                                    i->businessDayConvention());
        yearFraction_ = i->dayCounter().yearFraction(earliestDate_,
                                                     maturityDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;
    }

    Real FuturesForwardHelper::impliedQuote() const {
        QL_REQUIRE(termStructure_ != 0, "term structure not set");
        Rate forwardRate = (termStructure_->forwardRate(earliestDate_));
        Rate convAdj = convAdj_.empty() ? 0.0 : convAdj_->value();
        // Convexity, as FRA/futures adjustment, has been used in the
        // past to take into account futures margining vs FRA.
        // Therefore, there's no requirement for it to be non-negative.
        Rate futureRate = forwardRate + convAdj;
        return 100.0 * (1.0 - futureRate);
    }

    Real FuturesForwardHelper::convexityAdjustment() const {
        return convAdj_.empty() ? 0.0 : convAdj_->value();
    }

    void FuturesForwardHelper::accept(AcyclicVisitor& v) {
        Visitor<FuturesForwardHelper>* v1 =
            dynamic_cast<Visitor<FuturesForwardHelper>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            ForwardHelper::accept(v);
    }

    DepositForwardHelper::DepositForwardHelper(const Handle<Quote>& rate,
                                         const Period& tenor,
                                         Natural fixingDays,
                                         const Calendar& calendar,
                                         BusinessDayConvention convention,
                                         bool endOfMonth,
                                         const DayCounter& dayCounter)
    : RelativeDateForwardHelper(rate) {
        iborIndex_ = shared_ptr<ForwardIborIndex>(new
            ForwardIborIndex("no-fix", // never take fixing into account
                      tenor, fixingDays,
                      Currency(), calendar, convention,
                      endOfMonth, dayCounter, termStructureHandle_));
        initializeDates();
    }

    DepositForwardHelper::DepositForwardHelper(Rate rate,
                                         const Period& tenor,
                                         Natural fixingDays,
                                         const Calendar& calendar,
                                         BusinessDayConvention convention,
                                         bool endOfMonth,
                                         const DayCounter& dayCounter)
    : RelativeDateForwardHelper(rate) {
        iborIndex_ = shared_ptr<ForwardIborIndex>(new
            ForwardIborIndex("no-fix", // never take fixing into account
                      tenor, fixingDays,
                      Currency(), calendar, convention,
                      endOfMonth, dayCounter, termStructureHandle_));
        initializeDates();
    }

    DepositForwardHelper::DepositForwardHelper(const Handle<Quote>& rate,
                                         const shared_ptr<ForwardIborIndex>& i)
    : RelativeDateForwardHelper(rate) {
        iborIndex_ = i->clone(termStructureHandle_);
        initializeDates();
    }

    DepositForwardHelper::DepositForwardHelper(Rate rate,
                                         const shared_ptr<ForwardIborIndex>& i)
    : RelativeDateForwardHelper(rate) {
        iborIndex_ = i->clone(termStructureHandle_);
        initializeDates();
    }

    Real DepositForwardHelper::impliedQuote() const {
        QL_REQUIRE(termStructure_ != 0, "term structure not set");
        // the forecast fixing flag is set to true because
        // we do not want to take fixing into account
        return iborIndex_->fixing(fixingDate_, true);
    }

    void DepositForwardHelper::setTermStructure(ForwardRateCurve* t) {
        // do not set the relinkable handle as an observer -
        // force recalculation when needed---the index is not lazy
        bool observer = false;

        shared_ptr<ForwardRateCurve> temp(t, no_deletion);
        termStructureHandle_.linkTo(temp, observer);

        RelativeDateForwardHelper::setTermStructure(t);
    }

    void DepositForwardHelper::initializeDates() {
        // if the evaluation date is not a business day
        // then move to the next business day
        Date referenceDate =
            iborIndex_->fixingCalendar().adjust(evaluationDate_);
        earliestDate_ = iborIndex_->valueDate(referenceDate);
        fixingDate_ = iborIndex_->fixingDate(earliestDate_);
        maturityDate_ = iborIndex_->maturityDate(earliestDate_);
        latestDate_ = latestRelevantDate_ = maturityDate_;
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;
    }

    void DepositForwardHelper::accept(AcyclicVisitor& v) {
        Visitor<DepositForwardHelper>* v1 =
            dynamic_cast<Visitor<DepositForwardHelper>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            ForwardHelper::accept(v);
    }


    FraForwardHelper::FraForwardHelper(const Handle<Quote>& rate,
                                 Natural monthsToStart,
                                 Natural monthsToEnd,
                                 Natural fixingDays,
                                 const Calendar& calendar,
                                 BusinessDayConvention convention,
                                 bool endOfMonth,
                                 const DayCounter& dayCounter,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(monthsToStart*Months),
      pillarChoice_(pillarChoice) {
        QL_REQUIRE(monthsToEnd>monthsToStart,
                   "monthsToEnd (" << monthsToEnd <<
                   ") must be grater than monthsToStart (" << monthsToStart <<
                   ")");
        // no way to take fixing into account,
        // even if we would like to for FRA over today
        iborIndex_ = shared_ptr<ForwardIborIndex>(new
            ForwardIborIndex("no-fix", // correct family name would be needed
                      (monthsToEnd-monthsToStart)*Months,
                      fixingDays,
                      Currency(), calendar, convention,
                      endOfMonth, dayCounter, termStructureHandle_));
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(Rate rate,
                                 Natural monthsToStart,
                                 Natural monthsToEnd,
                                 Natural fixingDays,
                                 const Calendar& calendar,
                                 BusinessDayConvention convention,
                                 bool endOfMonth,
                                 const DayCounter& dayCounter,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(monthsToStart*Months),
      pillarChoice_(pillarChoice) {
        QL_REQUIRE(monthsToEnd>monthsToStart,
                   "monthsToEnd (" << monthsToEnd <<
                   ") must be grater than monthsToStart (" << monthsToStart <<
                   ")");
        // no way to take fixing into account,
        // even if we would like to for FRA over today
        iborIndex_ = shared_ptr<ForwardIborIndex>(new
            ForwardIborIndex("no-fix", // correct family name would be needed
                      (monthsToEnd-monthsToStart)*Months,
                      fixingDays,
                      Currency(), calendar, convention,
                      endOfMonth, dayCounter, termStructureHandle_));
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(const Handle<Quote>& rate,
                                 Natural monthsToStart,
                                 const shared_ptr<ForwardIborIndex>& i,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(monthsToStart*Months),
      pillarChoice_(pillarChoice) {
        // take fixing into account
        iborIndex_ = i->clone(termStructureHandle_);
        // We want to be notified of changes of fixings, but we don't
        // want notifications from termStructureHandle_ (they would
        // interfere with bootstrapping.)
        iborIndex_->unregisterWith(termStructureHandle_);
        registerWith(iborIndex_);
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(Rate rate,
                                 Natural monthsToStart,
                                 const shared_ptr<ForwardIborIndex>& i,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(monthsToStart*Months),
      pillarChoice_(pillarChoice) {
        // take fixing into account
        iborIndex_ = i->clone(termStructureHandle_);
        // see above
        iborIndex_->unregisterWith(termStructureHandle_);
        registerWith(iborIndex_);
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(const Handle<Quote>& rate,
                                 Period periodToStart,
                                 Natural lengthInMonths,
                                 Natural fixingDays,
                                 const Calendar& calendar,
                                 BusinessDayConvention convention,
                                 bool endOfMonth,
                                 const DayCounter& dayCounter,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(periodToStart),
      pillarChoice_(pillarChoice) {
        // no way to take fixing into account,
        // even if we would like to for FRA over today
        iborIndex_ = shared_ptr<ForwardIborIndex>(new
            ForwardIborIndex("no-fix", // correct family name would be needed
                      lengthInMonths*Months,
                      fixingDays,
                      Currency(), calendar, convention,
                      endOfMonth, dayCounter, termStructureHandle_));
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(Rate rate,
                                 Period periodToStart,
                                 Natural lengthInMonths,
                                 Natural fixingDays,
                                 const Calendar& calendar,
                                 BusinessDayConvention convention,
                                 bool endOfMonth,
                                 const DayCounter& dayCounter,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(periodToStart),
      pillarChoice_(pillarChoice) {
        // no way to take fixing into account,
        // even if we would like to for FRA over today
        iborIndex_ = shared_ptr<ForwardIborIndex>(new
            ForwardIborIndex("no-fix", // correct family name would be needed
                      lengthInMonths*Months,
                      fixingDays,
                      Currency(), calendar, convention,
                      endOfMonth, dayCounter, termStructureHandle_));
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(const Handle<Quote>& rate,
                                 Period periodToStart,
                                 const shared_ptr<ForwardIborIndex>& i,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(periodToStart),
      pillarChoice_(pillarChoice) {
        // take fixing into account
        iborIndex_ = i->clone(termStructureHandle_);
        // see above
        iborIndex_->unregisterWith(termStructureHandle_);
        registerWith(iborIndex_);
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    FraForwardHelper::FraForwardHelper(Rate rate,
                                 Period periodToStart,
                                 const shared_ptr<ForwardIborIndex>& i,
                                 Pillar::Choice pillarChoice,
                                 Date customPillarDate)
    : RelativeDateForwardHelper(rate), periodToStart_(periodToStart),
      pillarChoice_(pillarChoice) {
        // take fixing into account
        iborIndex_ = i->clone(termStructureHandle_);
        // see above
        iborIndex_->unregisterWith(termStructureHandle_);
        registerWith(iborIndex_);
        pillarDate_ = customPillarDate;
        initializeDates();
    }

    Real FraForwardHelper::impliedQuote() const {
        QL_REQUIRE(termStructure_ != 0, "term structure not set");
        return iborIndex_->fixing(fixingDate_, true);
    }

    void FraForwardHelper::setTermStructure(ForwardRateCurve* t) {
        // do not set the relinkable handle as an observer -
        // force recalculation when needed---the index is not lazy
        bool observer = false;

        shared_ptr<ForwardRateCurve> temp(t, no_deletion);
        termStructureHandle_.linkTo(temp, observer);

        RelativeDateForwardHelper::setTermStructure(t);
    }

    void FraForwardHelper::initializeDates() {
        // if the evaluation date is not a business day
        // then move to the next business day
        Date referenceDate =
            iborIndex_->fixingCalendar().adjust(evaluationDate_);
        Date spotDate = iborIndex_->fixingCalendar().advance(
            referenceDate, iborIndex_->fixingDays()*Days);
        earliestDate_ = iborIndex_->fixingCalendar().advance(
                               spotDate,
                               periodToStart_,
                               iborIndex_->businessDayConvention(),
                               iborIndex_->endOfMonth());
        // maturity date is calculated from spot date
        maturityDate_ = iborIndex_->fixingCalendar().advance(
                               spotDate,
                               periodToStart_ + iborIndex_->tenor(),
                               iborIndex_->businessDayConvention(),
                               iborIndex_->endOfMonth());
        // latest relevant date is calculated from earliestDate_ instead
        latestRelevantDate_ = iborIndex_->maturityDate(earliestDate_);
        // the pillar date is equal to the value date
        pillarDate_ = earliestDate_;

        latestDate_ = pillarDate_; // backward compatibility

        fixingDate_ = iborIndex_->fixingDate(earliestDate_);
    }

    void FraForwardHelper::accept(AcyclicVisitor& v) {
        Visitor<FraForwardHelper>* v1 =
            dynamic_cast<Visitor<FraForwardHelper>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            ForwardHelper::accept(v);
    }

    SwapForwardHelper::SwapForwardHelper(const Handle<Quote>& rate,
                                   const Period& tenor,
                                   const Calendar& calendar,
                                   Frequency fixedFrequency,
                                   BusinessDayConvention fixedConvention,
                                   const DayCounter& fixedDayCount,
                                   const shared_ptr<ForwardIborIndex>& iborIndex,
                                   const Handle<Quote>& spread,
                                   const Period& fwdStart,
                                   const Handle<YieldTermStructure>& discount,
                                   Natural settlementDays,
                                   Pillar::Choice pillarChoice,
                                   Date customPillarDate)
    : RelativeDateForwardHelper(rate),
      settlementDays_(settlementDays),
      tenor_(tenor), calendar_(calendar),
      fixedConvention_(fixedConvention),
      fixedFrequency_(fixedFrequency),
      fixedDayCount_(fixedDayCount),
      spread_(spread),
      fwdStart_(fwdStart), discountHandle_(discount), pillarChoice_(pillarChoice) {

        if (settlementDays_==Null<Natural>())
            settlementDays_ = iborIndex->fixingDays();

        // take fixing into account
        iborIndex_ = iborIndex->clone(termStructureHandle_);
        // We want to be notified of changes of fixings, but we don't
        // want notifications from termStructureHandle_ (they would
        // interfere with bootstrapping.)
        iborIndex_->unregisterWith(termStructureHandle_);

        registerWith(iborIndex_);
        registerWith(spread_);
        registerWith(discountHandle_);

        pillarDate_ = customPillarDate;
        initializeDates();
    }

    SwapForwardHelper::SwapForwardHelper(Rate rate,
                                   const Period& tenor,
                                   const Calendar& calendar,
                                   Frequency fixedFrequency,
                                   BusinessDayConvention fixedConvention,
                                   const DayCounter& fixedDayCount,
                                   const shared_ptr<ForwardIborIndex>& iborIndex,
                                   const Handle<Quote>& spread,
                                   const Period& fwdStart,
                                   const Handle<YieldTermStructure>& discount,
                                   Natural settlementDays,
                                   Pillar::Choice pillarChoice,
                                   Date customPillarDate)
    : RelativeDateForwardHelper(rate),
      settlementDays_(settlementDays),
      tenor_(tenor), calendar_(calendar),
      fixedConvention_(fixedConvention),
      fixedFrequency_(fixedFrequency),
      fixedDayCount_(fixedDayCount),
      spread_(spread),
      fwdStart_(fwdStart), discountHandle_(discount), pillarChoice_(pillarChoice) {

        if (settlementDays_==Null<Natural>())
            settlementDays_ = iborIndex->fixingDays();

        // take fixing into account
        iborIndex_ = iborIndex->clone(termStructureHandle_);
        // We want to be notified of changes of fixings, but we don't
        // want notifications from termStructureHandle_ (they would
        // interfere with bootstrapping.)
        iborIndex_->unregisterWith(termStructureHandle_);

        registerWith(iborIndex_);
        registerWith(spread_);
        registerWith(discountHandle_);

        pillarDate_ = customPillarDate;
        initializeDates();
    }

    void SwapForwardHelper::initializeDates() {

        // 1. do not pass the spread here, as it might be a Quote
        //    i.e. it can dinamically change
        // 2. input discount curve Handle might be empty now but it could
        //    be assigned a curve later; use a RelinkableHandle here
        swap_ = MakeVanillaSwap(tenor_, iborIndex_, 0.0, fwdStart_)
            .withSettlementDays(settlementDays_)
            .withDiscountingTermStructure(discountRelinkableHandle_)
            .withFixedLegDayCount(fixedDayCount_)
            .withFixedLegTenor(Period(fixedFrequency_))
            .withFixedLegConvention(fixedConvention_)
            .withFixedLegTerminationDateConvention(fixedConvention_)
            .withFixedLegCalendar(calendar_)
            .withFloatingLegCalendar(calendar_);

        earliestDate_ = swap_->startDate();

        // Usually...
        maturityDate_ = latestRelevantDate_ = swap_->maturityDate();

        // ...but due to adjustments, the last floating coupon might
        // need a later date for fixing
        #ifdef QL_USE_INDEXED_COUPON
        shared_ptr<FloatingRateCoupon> lastCoupon =
            boost::dynamic_pointer_cast<FloatingRateCoupon>(
                                                 swap_->floatingLeg().back());
        Date fixingValueDate = iborIndex_->valueDate(lastCoupon->fixingDate());
        Date endValueDate = iborIndex_->maturityDate(fixingValueDate);
        latestRelevantDate_ = std::max(latestRelevantDate_, endValueDate);
        #endif

        // the pillar date is equal to the instruments last fixing
        pillarDate_ = 
                calendar_.advance(swap_->maturityDate(),- iborIndex_->tenor());

        latestDate_ = pillarDate_; // backward compatibility

    }

    void SwapForwardHelper::setTermStructure(ForwardRateCurve* t) {
        // do not set the relinkable handle as an observer -
        // force recalculation when needed
        bool observer = false;

        shared_ptr<ForwardRateCurve> temp(t, no_deletion);
        termStructureHandle_.linkTo(temp, observer);
        QL_REQUIRE(!(discountHandle_.empty()), "discount term structure not set");
        discountRelinkableHandle_.linkTo(*discountHandle_, observer);

        RelativeDateForwardHelper::setTermStructure(t);
    }

    Real SwapForwardHelper::impliedQuote() const {
        QL_REQUIRE(termStructure_ != 0, "term structure not set");
        // we didn't register as observers - force calculation
        swap_->recalculate();
        // weak implementation... to be improved
        static const Spread basisPoint = 1.0e-4;
        Real floatingLegNPV = swap_->floatingLegNPV();
        Spread spread = spread_.empty() ? 0.0 : spread_->value();
        Real spreadNPV = swap_->floatingLegBPS()/basisPoint*spread;
        Real totNPV = - (floatingLegNPV+spreadNPV);
        Real result = totNPV/(swap_->fixedLegBPS()/basisPoint);
        return result;
    }

    void SwapForwardHelper::accept(AcyclicVisitor& v) {
        Visitor<SwapForwardHelper>* v1 =
            dynamic_cast<Visitor<SwapForwardHelper>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            ForwardHelper::accept(v);
    }

}
