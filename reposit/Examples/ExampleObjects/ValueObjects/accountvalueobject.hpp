
/*!
 Copyright (C) 2004, 2005, 2006, 2007 Eric Ehlers
 Copyright (C) 2006, 2008 Plamen Neykov
 Copyright (C) 2008 Nazcatech sprl Belgium

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

#ifndef example_account_valueobject_hpp
#define example_account_valueobject_hpp

#include <rp/valueobject.hpp>
#include <vector>
#include <string>

namespace AccountExample {

    class AccountValueObject : public reposit::ValueObject {
        friend class boost::serialization::access;

    public:

        AccountValueObject() {}
        AccountValueObject(
            const std::string &objectID,
            const std::string &customer,
            const std::string &type,
            const long &number,
            const reposit::property_t& balance,
            bool permanent)
            : reposit::ValueObject(objectID, "Account", permanent),
            customer_(customer), type_(type), number_(number), balance_(balance) {
            precedentIDs_.insert(customer);
        }

        const std::set<std::string>& getSystemPropertyNames() const;
        std::vector<std::string> getPropertyNamesVector() const;
        reposit::property_t getSystemProperty(const std::string &name) const;
        void setSystemProperty(const std::string& name, const reposit::property_t& value);

    private:

        static const char* mPropertyNames[];
        std::string customer_;
        std::string type_;
        long number_;
        reposit::property_t balance_;

        template<class Archive>
        void serialize(Archive& ar, const unsigned int) {
            boost::serialization::void_cast_register<AccountValueObject, ValueObject>(this, this);
            ar & boost::serialization::make_nvp("ObjectId", objectId_)
               & boost::serialization::make_nvp("ClassName", className_)
               & boost::serialization::make_nvp("Customer", customer_)
               & boost::serialization::make_nvp("Type", type_)
               & boost::serialization::make_nvp("Number", number_)
               & boost::serialization::make_nvp("Balance", balance_)
               & boost::serialization::make_nvp("Permanent", permanent_);
        }

    };

}

#endif

