
/*
 Copyright (C) 2015 StatPro Italia srl

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

#ifndef quantlib_tenor_basis_i
#define quantlib_tenor_basis_i

%include indexes.i
%include optimizers.i

%{
using QuantLib::TenorBasis;
%}

%ignore TenorBasis;
class TenorBasis {
  public:
    Spread value(Date d) const;
    Spread value(Time t) const;

    Rate tenorForwardRate(Date d) const;
    Rate tenorForwardRate(Time t) const;

    Rate forwardRate(Date d) const;
    Rate forwardRate(Date d1,
                     Date d2) const;
    Rate forwardRate(Time t1,
                     Time t2) const;

    const std::vector<Real>& coefficients() const;
    const std::vector<Real>& instCoefficients() const;

    void calibrate(
            const std::vector<boost::shared_ptr<RateHelper> >&,
            OptimizationMethod& method,
            const EndCriteria& endCriteria 
                             = EndCriteria(1000, 100, 1.0e-8, 0.3e-4, 0.3e-4),
            const std::vector<Real>& weights = std::vector<Real>(),
            const std::vector<bool>& fixParameters = std::vector<bool>());
};
%template(TenorBasis) boost::shared_ptr<TenorBasis>;

%{
using QuantLib::AbcdTenorBasis;
typedef boost::shared_ptr<TenorBasis> AbcdTenorBasisPtr;
%}

%rename(AbcdTenorBasis) AbcdTenorBasisPtr;
class AbcdTenorBasisPtr : public boost::shared_ptr<TenorBasis> {
  public:
    %extend {
      AbcdTenorBasisPtr(IborIndexPtr index,
                        const Handle<YieldTermStructure>& baseCurve,
                        Date referenceDate,
                        bool isSimple,
                        const std::vector<Real>& coefficients) {
          boost::shared_ptr<IborIndex> libor =
              boost::dynamic_pointer_cast<IborIndex>(index);
          return new AbcdTenorBasisPtr(
                           new AbcdTenorBasis(libor, baseCurve, referenceDate,
                                              isSimple, coefficients));
      }
    }
};


%{
using QuantLib::PolynomialTenorBasis;
typedef boost::shared_ptr<TenorBasis> PolynomialTenorBasisPtr;
%}

%rename(PolynomialTenorBasis) PolynomialTenorBasisPtr;
class PolynomialTenorBasisPtr : public boost::shared_ptr<TenorBasis> {
  public:
    %extend {
      PolynomialTenorBasisPtr(IborIndexPtr index,
                              const Handle<YieldTermStructure>& baseCurve,
                              Date referenceDate,
                              bool isSimple,
                              const std::vector<Real>& coefficients) {
          boost::shared_ptr<IborIndex> libor =
              boost::dynamic_pointer_cast<IborIndex>(index);
          return new PolynomialTenorBasisPtr(
                           new PolynomialTenorBasis(libor, baseCurve, referenceDate,
                                                    isSimple, coefficients));
      }
    }
};


#endif
