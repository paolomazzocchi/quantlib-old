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

/*! \file ForwardHelpers.hpp
    \brief deposit, FRA, futures, and various swap rate helpers
*/

#ifndef quantlib_ForwardHelpers_hpp
#define quantlib_ForwardHelpers_hpp

#include <ql/termstructures/bootstraphelper.hpp>
#include <ql/experimental/tenorbasis/forwardratecurve.hpp>
#include <ql/instruments/vanillaswap.hpp>
#include <ql/instruments/bmaswap.hpp>
#include <ql/instruments/futures.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/daycounter.hpp>

namespace QuantLib {

    class SwapIndex;
    class ForwardIborIndex;
    class Quote;

    typedef BootstrapHelper<ForwardRateCurve> ForwardHelper;
    typedef RelativeDateBootstrapHelper<ForwardRateCurve>
                                                        RelativeDateForwardHelper;

    //! Rate helper for bootstrapping over ForwardIborIndex futures prices
    class FuturesForwardHelper : public ForwardHelper {
      public:
        FuturesForwardHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          Natural lengthInMonths,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(Real price,
                          const Date& iborStartDate,
                          Natural lengthInMonths,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          const Date& iborEndDate,
                          const DayCounter& dayCounter,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(Real price,
                          const Date& iborStartDate,
                          const Date& endDate,
                          const DayCounter& dayCounter,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(Real price,
                          const Date& iborStartDate,
                          const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        //@}
        //! \name FuturesForwardHelper inspectors
        //@{
        Real convexityAdjustment() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        Time yearFraction_;
        Handle<Quote> convAdj_;
    };


    //! Rate helper for bootstrapping over deposit rates
    class DepositForwardHelper : public RelativeDateForwardHelper {
      public:
        DepositForwardHelper(const Handle<Quote>& rate,
                          const Period& tenor,
                          Natural fixingDays,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter);
        DepositForwardHelper(Rate rate,
                          const Period& tenor,
                          Natural fixingDays,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter);
        DepositForwardHelper(const Handle<Quote>& rate,
                          const boost::shared_ptr<ForwardIborIndex>& iborIndex);
        DepositForwardHelper(Rate rate,
                          const boost::shared_ptr<ForwardIborIndex>& iborIndex);
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        void initializeDates();
        Date fixingDate_;
        boost::shared_ptr<ForwardIborIndex> iborIndex_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over %FRA rates
    class FraForwardHelper : public RelativeDateForwardHelper {
      public:
        FraForwardHelper(const Handle<Quote>& rate,
                      Natural monthsToStart,
                      Natural monthsToEnd,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Natural monthsToStart,
                      Natural monthsToEnd,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(const Handle<Quote>& rate,
                      Natural monthsToStart,
                      const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Natural monthsToStart,
                      const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(const Handle<Quote>& rate,
                      Period periodToStart,
                      Natural lengthInMonths,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Period periodToStart,
                      Natural lengthInMonths,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(const Handle<Quote>& rate,
                      Period periodToStart,
                      const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Period periodToStart,
                      const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        void initializeDates();
        Date fixingDate_;
        Period periodToStart_;
        Pillar::Choice pillarChoice_;
        boost::shared_ptr<ForwardIborIndex> iborIndex_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over swap rates
    /*! \todo use input SwapIndex to create the swap */
    class SwapForwardHelper : public RelativeDateForwardHelper {
      public:
        SwapForwardHelper(const Handle<Quote>& rate,
                       const Period& tenor,
                       const Calendar& calendar,
                       // fixed leg
                       Frequency fixedFrequency,
                       BusinessDayConvention fixedConvention,
                       const DayCounter& fixedDayCount,
                       // floating leg
                       const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<YieldTermStructure>& discountingCurve
                                                = Handle<YieldTermStructure>(),
                       Natural settlementDays = Null<Natural>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapForwardHelper(Rate rate,
                       const Period& tenor,
                       const Calendar& calendar,
                       // fixed leg
                       Frequency fixedFrequency,
                       BusinessDayConvention fixedConvention,
                       const DayCounter& fixedDayCount,
                       // floating leg
                       const boost::shared_ptr<ForwardIborIndex>& iborIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<YieldTermStructure>& discountingCurve
                                                = Handle<YieldTermStructure>(),
                       Natural settlementDays = Null<Natural>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name SwapForwardHelper inspectors
        //@{
        Spread spread() const;
        boost::shared_ptr<VanillaSwap> swap() const;
        const Period& forwardStart() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      protected:
        void initializeDates();
        Natural settlementDays_;
        Period tenor_;
        Pillar::Choice pillarChoice_;
        Calendar calendar_;
        BusinessDayConvention fixedConvention_;
        Frequency fixedFrequency_;
        DayCounter fixedDayCount_;
        boost::shared_ptr<ForwardIborIndex> iborIndex_;
        boost::shared_ptr<VanillaSwap> swap_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
        Handle<Quote> spread_;
        Period fwdStart_;
        Handle<YieldTermStructure> discountHandle_;
        RelinkableHandle<YieldTermStructure> discountRelinkableHandle_;
    };

    // inline

    inline Spread SwapForwardHelper::spread() const {
        return spread_.empty() ? 0.0 : spread_->value();
    }

    inline boost::shared_ptr<VanillaSwap> SwapForwardHelper::swap() const {
        return swap_;
    }

    inline const Period& SwapForwardHelper::forwardStart() const {
        return fwdStart_;
    }

}

#endif
