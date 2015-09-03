
%feature("rp:group", "instruments");
%feature("rp:obj_include") %{
#include <ql/instruments/vanillaoption.hpp>
#include <ql/cashflow.hpp>
#include <ql/instruments/swap.hpp>
%}
%feature("rp:add_include") "#include \"qlo/obj_pricingengines.hpp\"
#include \"qlo/obj_payoffs.hpp\"
#include \"qlo/obj_exercise.hpp\""

%feature("rp:generate_countify") QuantLib::VanillaOption::VanillaOption;
%feature("rp:generate_countify") QuantLib::Instrument::setPricingEngine;
%feature("rp:generate_countify") QuantLib::Instrument::NPV;

namespace QuantLib {

    class Instrument {
      public:
        //Instrument();
        void setPricingEngine(const boost::shared_ptr<QuantLib::PricingEngine>& engine);
        QuantLib::Real NPV();
    };
    
    class VanillaOption : public Instrument {
      public:
        VanillaOption(const boost::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
                      const boost::shared_ptr<QuantLib::Exercise>& exercise);
    };
    
    class Swap : public Instrument {
      public:
        Swap(const std::vector<QuantLib::Leg>& legs,
            const std::vector<bool>& payer);
    };
}

%feature("rp:obj_include", "");
%feature("rp:add_include", "");
%feature("rp:group", "");

