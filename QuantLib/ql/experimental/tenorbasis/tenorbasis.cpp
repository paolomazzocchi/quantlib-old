/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2015 Ferdinando Ametrano
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

#include <ql/experimental/tenorbasis/tenorbasis.hpp>
#include <ql/indexes/iborindex.hpp>
#include <ql/math/abcdmathfunction.hpp>


using boost::shared_ptr;
using std::vector;

namespace QuantLib {

    namespace {
        void no_deletion(TenorBasis*) {}
    }


    TenorBasis::TenorBasis(Size nArguments,
                           shared_ptr<IborIndex> iborIndex,
                           const Handle<YieldTermStructure>& baseCurve,
                           Date referenceDate)
    : CalibratedModel(nArguments),
      index_(iborIndex), baseCurve_(baseCurve), referenceDate_(referenceDate) {
        
        QL_REQUIRE(iborIndex!=0, "empty iborIndex");
        if (referenceDate_ == Date()) {
            Date today = Settings::instance().evaluationDate();
            referenceDate_ = iborIndex->valueDate(today);
            // avoid the following as the Hande could be empty
            //referenceDate_ = baseCurve_->referenceDate();
        }
        dc_ = index_->dayCounter();
        bdc_ = index_->businessDayConvention();
        eom_ = index_->endOfMonth();
        cal_ = index_->fixingCalendar();
        tenor_ = index_->tenor();
        Date endDate = cal_.advance(referenceDate_, tenor_, bdc_, eom_);
        tau_ = dc_.yearFraction(referenceDate_, endDate);
    }

    Spread TenorBasis::value(Date d) const {
        Time t = timeFromReference(d);
        return value(t);
    }

    Rate TenorBasis::tenorForwardRate(Date d1) const {
        Date d2 = cal_.advance(d1, tenor_, bdc_, eom_);
        // baseCurve must be a discounting curve...
        // otherwise it could not provide fwd(d1, d2) with d2-d1!=tau
        Real accrFactor = baseCurve_->discount(d1) / baseCurve_->discount(d2);
        Time dt = dc_.yearFraction(d1, d2);
        Rate baseFwd = (accrFactor - 1.0) / dt;

        Rate basis = value(d1);
        return baseFwd + basis;
    }

    Rate TenorBasis::tenorForwardRate(Time t1) const {
        // we need Date algebra to calculate d2
        Date d1 = dateFromTime(t1);
        return tenorForwardRate(d1);
    }

    Rate TenorBasis::forwardRate(Date d1) const {
        Date d2 = cal_.advance(d1, tenor_, bdc_, eom_);
        return forwardRate(d1, d2);
    }

    Rate TenorBasis::forwardRate(Date d1,
                                 Date d2) const {
        QL_REQUIRE(d1 < d2,
                   "d2 (" << d2 << ") <= d1 (" << d1 << ")");
        // baseCurve must be a discounting curve...
        // otherwise it could not provide fwd(t1, t2) with t2-t1!=tau_
        Real accrFactor = baseCurve_->discount(d1) / baseCurve_->discount(d2);
        Real instContBasisIntegral = integrate_(d1, d2);
        accrFactor *= std::exp(instContBasisIntegral);

        Time dt = dc_.yearFraction(d1, d2);
        Rate fwd = (accrFactor - 1.0) / dt;
        return fwd;
    }

    Rate TenorBasis::forwardRate(Time t1,
                                 Time t2) const {
        Date d1 = dateFromTime(t1);
        Date d2 = dateFromTime(t2);
        return forwardRate(d1, d2);
    }

    Time TenorBasis::timeFromReference(Date d) const {
        // Actual365Fixed is hardcoded. It must be an invertible DayCounter
        // see also TenorBasis::dateFromTime(Time t)
        return Actual365Fixed().yearFraction(referenceDate_, d);
    }

    Date TenorBasis::dateFromTime(Time t) const {
        // Actual365Fixed is hardcoded. It must be an invertible DayCounter
        // see also TenorBasis::timeFromReference(Date d)
        BigInteger result =
            referenceDate_.serialNumber() + BigInteger(t*365.0);
        if (result >= Date::maxDate().serialNumber())
            return Date::maxDate();
        return Date(result);
    }

    const shared_ptr<IborIndex>& TenorBasis::iborIndex() const {
        return index_;
    }

    const Handle<YieldTermStructure>& TenorBasis::baseCurve() const {
        return baseCurve_;
    }

    Constraint TenorBasis::constraint() const {
        return NoConstraint();
    }

    Real TenorBasis::integrate_(Date d1) const {
        Date d2 = cal_.advance(d1, tenor_, bdc_, eom_);
        return integrate_(d1, d2);
    }

    Real TenorBasis::integrate_(Date d1,
                                Date d2) const {
        Time t1 = timeFromReference(d1);
        Time t2 = timeFromReference(d2);
        return integrate_(t1, t2);
    }

     void TenorBasis::calibrate(
                const std::vector<boost::shared_ptr<RateHelper> >& helpers,
                OptimizationMethod& method,
                const EndCriteria& endCriteria,
                const std::vector<Real>& weights,
                const std::vector<bool>& fixParameters) {
        TenorBasisYieldTermStructure yts(boost::shared_ptr<TenorBasis>(this, no_deletion));
        std::vector<boost::shared_ptr<CalibrationHelperBase> > cHelpers(helpers.size());
        for (Size i = 0; i<helpers.size(); ++i) {
            helpers[i]->setTermStructure(&yts);
            cHelpers[i] = helpers[i];
        }
        CalibratedModel::calibrate(cHelpers, method, endCriteria,
                                   constraint(), weights, fixParameters);
    }

     void TenorBasis::forwardCalibrate(
         const std::vector<boost::shared_ptr<ForwardHelper> >& helpers,
         OptimizationMethod& method,
         const EndCriteria& endCriteria,
         const std::vector<Real>& weights,
         const std::vector<bool>& fixParameters) {
         TenorBasisForwardRateCurve yts(boost::shared_ptr<TenorBasis>(this, no_deletion));
         std::vector<boost::shared_ptr<CalibrationHelperBase> > cHelpers(helpers.size());
         for (Size i = 0; i<helpers.size(); ++i) {
             helpers[i]->setTermStructure(&yts);
             cHelpers[i] = helpers[i];
         }
         CalibratedModel::calibrate(cHelpers, method, endCriteria,
             constraint(), weights, fixParameters);
     }

    namespace {

        // to constrained <- from unconstrained
        std::vector<Real> direct(const std::vector<Real>& x) {
            std::vector<Real> y(4);
            y[2] = std::exp(x[2]);
            y[3] = std::exp(-x[3] * x[3]);
            y[0] = x[0] * x[0] - y[3];
            y[1] = x[1] * x[1];
            return y;
        }

        // to unconstrained <- from constrained
        std::vector<Real> inverse(const std::vector<Real>& x) {
            std::vector<Real> y(4);
            y[2] = std::log(x[2]);
            y[3] = std::sqrt((x[3] == 0 ? -std::log(QL_EPSILON) : -std::log(x[3])));
            y[0] = std::sqrt(x[0] + x[3]);
            y[1] = std::sqrt(x[1]);
            return y;
        }

        class AbcdConstraint : public Constraint {
          private:
            class Impl : public Constraint::Impl {
                Real tau_;
                bool isSimple_;
              public:
                Impl(Real tau, bool isSimple) : tau_(tau), isSimple_(isSimple) {}
                bool test(const Array& params) const {
                    Real a = params[0];
                    Real b = params[1];
                    Real c = params[2];
                    Real d = params[3];

                    try {
                        AbcdMathFunction::validate(a, b, c, d);
                        AbcdMathFunction f(a,b,c,d);
                        vector<Real> v;
                        if (isSimple_) {
                            v = f.definiteDerivativeCoefficients(0.0, tau_);
                            AbcdMathFunction::validate(v[0] * tau_, v[1] * tau_, v[2], v[3] * tau_);
                        } else {
                            v = f.definiteIntegralCoefficients(0.0, tau_);
                            AbcdMathFunction::validate(a / tau_, b / tau_, c, d / tau_);
                        }
                        return true;
                    } catch (...) {
                        return false;
                    }
                }
            };
          public:
            AbcdConstraint(Real tau, bool isSimple)
            : Constraint(boost::shared_ptr<Constraint::Impl>(
                                  new AbcdConstraint::Impl(tau, isSimple))) {}
        };

    }

    AbcdTenorBasis::AbcdTenorBasis(shared_ptr<IborIndex> iborIndex,
                                   const Handle<YieldTermStructure>& baseCurve,
                                   Date referenceDate,
                                   bool isSimple,
                                   const std::vector<Real>& coeff)
    : TenorBasis(4, iborIndex, baseCurve, referenceDate) {
        //std::vector<Real> y = inverse(coeff);
        std::vector<Real> y = coeff;
        arguments_[0] = ConstantParameter(y[0], NoConstraint());
        arguments_[1] = ConstantParameter(y[1], NoConstraint());
        arguments_[2] = ConstantParameter(y[2], NoConstraint());
        arguments_[3] = ConstantParameter(y[3], NoConstraint());
        isSimple_ = isSimple;
        generateArguments();
    }

    Constraint AbcdTenorBasis::constraint() const {
        return AbcdConstraint(tau_, isSimple_);
    }

    void AbcdTenorBasis::generateArguments() {
        std::vector<Real> x(4);
        x[0] = arguments_[0](0.0);
        x[1] = arguments_[1](0.0);
        x[2] = arguments_[2](0.0);
        x[3] = arguments_[3](0.0);
        //std::vector<Real> y = direct(x);
        std::vector<Real> y = x;
        if (isSimple_) {
            basis_ = shared_ptr<AbcdMathFunction>(
                new AbcdMathFunction(y[0], y[1], y[2], y[3]));
            vector<Real> c = basis_->definiteDerivativeCoefficients(0.0, tau_);
            c[0] *= tau_;
            c[1] *= tau_;
            // unaltered c[2] (the c in abcd)
            c[3] *= tau_;
            instBasis_ = shared_ptr<AbcdMathFunction>(new AbcdMathFunction(c));
        } else {
            instBasis_ = shared_ptr<AbcdMathFunction>(
                new AbcdMathFunction(y[0], y[1], y[2], y[3]));
            vector<Real> c = 
                           instBasis_->definiteIntegralCoefficients(0.0, tau_);
            c[0] /= tau_;
            c[1] /= tau_;
            // unaltered c[2] (the c in abcd)
            c[3] /= tau_;
            basis_ = shared_ptr<AbcdMathFunction>(new AbcdMathFunction(c));
        }
    }

    const vector<Real>& AbcdTenorBasis::coefficients() const {
        return basis_->coefficients();
    }

    const vector<Real>& AbcdTenorBasis::instCoefficients() const {
        return instBasis_->coefficients();
    }

    Date AbcdTenorBasis::maximumLocation() const {
        Time maximumLocation = basis_->maximumLocation();
        return dateFromTime(maximumLocation);
    }

    Spread AbcdTenorBasis::maximumValue() const {
        Date d = maximumLocation();
        return TenorBasis::value(d);
    }

    Real AbcdTenorBasis::integrate_(Time t1,
                                    Time t2) const {
        return instBasis_->definiteIntegral(t1, t2);
    }


    PolynomialTenorBasis::PolynomialTenorBasis(
                                shared_ptr<IborIndex> iborIndex,
                                const Handle<YieldTermStructure>& baseCurve,
                                Date referenceDate,
                                bool isSimple,
                                const std::vector<Real>& coeff)
    : TenorBasis(coeff.size(), iborIndex, baseCurve, referenceDate),
      isSimple_(isSimple) {
        for (Size i = 0; i<coeff.size(); ++i)
            arguments_[i] = ConstantParameter(coeff[i], NoConstraint());
        generateArguments();
    }

    void PolynomialTenorBasis::generateArguments() {
        std::vector<Real> coeffs(arguments_.size());
        for (Size i = 0; i<coeffs.size(); ++i)
            coeffs[i] = arguments_[i](0.0);
        if (isSimple_) {
            basis_ =
                shared_ptr<PolynomialFunction>(new PolynomialFunction(coeffs));
            std::vector<Real> c =
                basis_->definiteDerivativeCoefficients(0.0, tau_);
            for (Size i=0; i<c.size(); ++i)
                c[i] *= tau_;
            instBasis_ =
                shared_ptr<PolynomialFunction>(new PolynomialFunction(c));
        } else {
            instBasis_ =
                shared_ptr<PolynomialFunction>(new PolynomialFunction(coeffs));
            std::vector<Real> c =
                instBasis_->definiteIntegralCoefficients(0.0, tau_);
            for (Size i=0; i<c.size(); ++i)
                c[i] /= tau_;
            basis_ =
                shared_ptr<PolynomialFunction>(new PolynomialFunction(c));
        }
    }
   
    const vector<Real>& PolynomialTenorBasis::coefficients() const {
        return basis_->coefficients();
    }

    const vector<Real>& PolynomialTenorBasis::instCoefficients() const {
        return instBasis_->coefficients();
    }

    Real PolynomialTenorBasis::integrate_(Time t1,
                                          Time t2) const {
        return instBasis_->definiteIntegral(t1, t2);
    }


    TenorBasisYieldTermStructure::TenorBasisYieldTermStructure(const boost::shared_ptr<TenorBasis>& basis)
    : YieldTermStructure(Actual365Fixed()), basis_(basis) {}

    const Date& TenorBasisYieldTermStructure::referenceDate() const {
        return basis_->referenceDate();
    }

    Calendar TenorBasisYieldTermStructure::calendar() const {
        return basis_->iborIndex()->fixingCalendar();
    }

    Natural TenorBasisYieldTermStructure::settlementDays() const {
        return basis_->iborIndex()->fixingDays();
    }

    Date TenorBasisYieldTermStructure::maxDate() const {
        return basis_->baseCurve()->maxDate();
    }

    DiscountFactor TenorBasisYieldTermStructure::discountImpl(Time t) const {
        if (t == 0)
            return 1.0;
        else{
           Date d1 = referenceDate();
           Date d2 = basis_->dateFromTime(t);
           Rate fwd = basis_->forwardRate(d1, d2);
           Time tau = basis_->iborIndex()->dayCounter().yearFraction(d1, d2);
           return 1.0 / (1.0 + fwd*tau);
        }
    }

    TenorBasisForwardRateCurve::TenorBasisForwardRateCurve(const boost::shared_ptr<TenorBasis>& basis)
        : ForwardRateCurve(basis_->iborIndex()->familyName(),
                           basis_->iborIndex()->tenor(),
                           basis_->iborIndex()->fixingDays(),
                           basis_->iborIndex()->currency(),
                           basis_->iborIndex()->fixingCalendar(),
                           basis_->iborIndex()->businessDayConvention(),
                           basis_->iborIndex()->endOfMonth(),
                           basis_->iborIndex()->dayCounter()), 
                           basis_(basis) {}

    const Date& TenorBasisForwardRateCurve::referenceDate() const {
        return basis_->referenceDate();
    }

    Calendar TenorBasisForwardRateCurve::calendar() const {
        return basis_->iborIndex()->fixingCalendar();
    }

    Natural TenorBasisForwardRateCurve::settlementDays() const {
        return basis_->iborIndex()->fixingDays();
    }

    Date TenorBasisForwardRateCurve::maxDate() const {
        return basis_->baseCurve()->maxDate();
    }

    Rate TenorBasisForwardRateCurve::forwardRate(Time t, bool extrapolate) const {
        return basis_->tenorForwardRate(t);
        }

    DiscountCorrectedTermStructure::DiscountCorrectedTermStructure(
        const Handle<YieldTermStructure>& baseCurve,
        const std::vector<boost::shared_ptr<RateHelper> >& instruments,
        Real accuracy)
    : baseCurve_(baseCurve), instruments_(instruments), accuracy_(accuracy) {
        registerWith(baseCurve_);
        bootstrap_.setup(this);
    }

    const Date& DiscountCorrectedTermStructure::referenceDate() const {
        return baseCurve_->referenceDate();
    }

    DayCounter DiscountCorrectedTermStructure::dayCounter() const {
        return baseCurve_->dayCounter();
    }

    Calendar DiscountCorrectedTermStructure::calendar() const {
        return baseCurve_->calendar();
    }

    Natural DiscountCorrectedTermStructure::settlementDays() const{
        return baseCurve_->settlementDays();
    }

    Date DiscountCorrectedTermStructure::maxDate() const {
        return baseCurve_->maxDate();
    }

    const std::vector<Time>& DiscountCorrectedTermStructure::times() const {
        calculate();
        return times_;
    }

    const std::vector<Date>& DiscountCorrectedTermStructure::dates() const {
        calculate();
        return dates_;
    }

    const std::vector<Real>& DiscountCorrectedTermStructure::data() const {
        calculate();
        return data_;
    }

    void DiscountCorrectedTermStructure::update() {
        LazyObject::update();
    }

    DiscountFactor DiscountCorrectedTermStructure::discountImpl(Time t) const {
        DiscountFactor B = baseCurve_->discount(t, true);
        Real k = interpolation_(t, true);
        return k*B;
    }

    void DiscountCorrectedTermStructure::performCalculations() const {
        bootstrap_.calculate();
    }

    ForwardCorrectedTermStructure::ForwardCorrectedTermStructure(
        const Handle<ForwardRateCurve>& baseCurve,
        const std::vector<boost::shared_ptr<ForwardHelper> >& instruments,
        Real accuracy)
    : /*baseCurve_(baseCurve),*/ instruments_(instruments), accuracy_(accuracy) {
        baseCurve_ = baseCurve;
        registerWith(baseCurve_);
        bootstrap_.setup(this);
    }

    const Date& ForwardCorrectedTermStructure::referenceDate() const {
        return baseCurve_->referenceDate();
    }

    DayCounter ForwardCorrectedTermStructure::dayCounter() const {
        return baseCurve_->dayCounter();
    }

    Calendar ForwardCorrectedTermStructure::calendar() const {
        return baseCurve_->calendar();
    }

    Natural ForwardCorrectedTermStructure::settlementDays() const{
        return baseCurve_->settlementDays();
    }

    Date ForwardCorrectedTermStructure::maxDate() const {
        return baseCurve_->maxDate();
    }

    const std::vector<Time>& ForwardCorrectedTermStructure::times() const {
        calculate();
        return times_;
    }

    const std::vector<Date>& ForwardCorrectedTermStructure::dates() const {
        calculate();
        return dates_;
    }

    const std::vector<Real>& ForwardCorrectedTermStructure::data() const {
        calculate();
        return data_;
    }

    void ForwardCorrectedTermStructure::update() {
        LazyObject::update();
    }

    Rate ForwardCorrectedTermStructure::forwardRate(Time t, bool extrapolate) const {
        Rate F = baseCurve_->forwardRate(t, true);
        Real k = interpolation_(t, true);
        return k*F;
    }

    void ForwardCorrectedTermStructure::performCalculations() const {
        bootstrap_.calculate();
    }


}

