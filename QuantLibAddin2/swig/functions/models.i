
%group(models);

%insert(models_library_hpp) %{
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>
#include <ql/models/shortrate/onefactormodels/hullwhite.hpp>
%}

%insert(models_serialization_cpp) %{
#include <qlo/objmanual_indexes.hpp>
#include <qlo/obj_termstructures.hpp>
%}

namespace QuantLib {

    class CalibrationHelper {
      public:
        void setPricingEngine(const boost::shared_ptr<PricingEngine>& engine);
        Volatility impliedVolatility(Real targetValue,
                             Real accuracy,
                             Size maxEvaluations,
                             Volatility minVol,
                             Volatility maxVol);
    };

    class SwaptionHelper : public CalibrationHelper {
      public:
        SwaptionHelper(const Period& maturity,
                       const Period& length,
                       const Handle<Quote>& volatility,
                       const boost::shared_ptr<IborIndex>& index,
                       const Period& fixedLegTenor,
                       const DayCounter& fixedLegDayCounter,
                       const DayCounter& floatingLegDayCounter,
                       const Handle<YieldTermStructure>& termStructure/*,
                       CalibrationHelper::CalibrationErrorType errorType
                                      = CalibrationHelper::RelativePriceError,
                       const Real strike = Null<Real>(),
                       const Real nominal = 1.0,
                       const Real shift = 0.0*/);
          Real modelValue();
    };
        
    class CalibratedModel {
      public:
        virtual void calibrate(
                const std::vector<boost::shared_ptr<CalibrationHelper> >& x,
                OptimizationMethod& method,
                const EndCriteria& endCriteria/*,
                const Constraint& constraint = Constraint(),
                const std::vector<Real>& weights = std::vector<Real>(),
                const std::vector<bool>& fixParameters = std::vector<bool>()*/);
    };
    
    class ShortRateModel : public CalibratedModel {};
    class OneFactorAffineModel : public ShortRateModel {};
    
    class HullWhite : public /*Vasicek, public TermStructureConsistentModel*/OneFactorAffineModel {
      public:
        HullWhite(const Handle<YieldTermStructure>& termStructure/*,
                  Real a = 0.1, Real sigma = 0.01*/);
    };
}
