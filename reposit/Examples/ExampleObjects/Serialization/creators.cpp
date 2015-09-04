
/*
 Copyright (C) 2007, 2008 Eric Ehlers
 Copyright (C) 2008 Plamen Neykov

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

#if defined(HAVE_CONFIG_H)     // Dynamically created by configure
    #include <rp/config.hpp>
#endif

#include <rp/property.hpp>
#include <ExampleObjects/Serialization/creators.hpp>
#include <ExampleObjects/Objects/customerobject.hpp>
#include <ExampleObjects/Objects/accountobject.hpp>
#include <ExampleObjects/Enumerations/accountenumerations.hpp>

namespace AccountExample {

    boost::shared_ptr<reposit::Object> createAccount(
        const boost::shared_ptr<reposit::ValueObject> &valueObject) {

        std::string customer = reposit::convert2<std::string>(valueObject->getProperty("Customer"));
        bool permanent = reposit::convert2<bool>(valueObject->getProperty("Permanent"));
        std::string type = reposit::convert2<std::string>(valueObject->getProperty("Type"));
        long number = reposit::convert2<long>(valueObject->getProperty("Number"));
        reposit::property_t balance = valueObject->getProperty("Balance");

        RP_GET_REFERENCE(customerRef, customer,
            AccountExample::CustomerObject, AccountExample::Customer)

        Account::Type typeEnum = reposit::Create<Account::Type>()(type);

        double accountBalance = reposit::convert2<double>(balance, "Balance", 100.00);

        boost::shared_ptr<reposit::Object> object(
            new AccountObject(valueObject, customerRef, typeEnum, number, accountBalance, permanent));
        return object;
    }

    boost::shared_ptr<reposit::Object> createCustomer(
        const boost::shared_ptr<reposit::ValueObject> &valueObject) {

        bool permanent = reposit::convert2<bool>(valueObject->getProperty("Permanent"));
        std::string name = reposit::convert2<std::string>(valueObject->getProperty("name"));
        long age = reposit::convert2<long>(valueObject->getProperty("age"));

        boost::shared_ptr<reposit::Object> object(
            new CustomerObject(valueObject, name, age, permanent));
        return object;
    }

}

