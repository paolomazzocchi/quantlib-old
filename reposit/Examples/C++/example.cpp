
/*!
 Copyright (C) 2004, 2005, 2006, 2007 Eric Ehlers
 Copyright (C) 2006 Plamen Neykov
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

/* Use BOOST_MSVC instead of _MSC_VER since some other vendors (Metrowerks,
   for example) also #define _MSC_VER
*/

#ifdef BOOST_MSVC
#  define BOOST_LIB_DIAGNOSTIC
#  include <rp/auto_link.hpp>
#  undef BOOST_LIB_DIAGNOSTIC
#endif
#include <sstream>
#include <iostream>
#include <exception>
#include <rp/reposit.hpp>
#include <ExampleObjects/accountexample.hpp>
#include <Examples/ExampleObjects/Serialization/serializationfactory.hpp>

void makeCustomer(
    const std::string &objectID,
    const std::string &name,
    const long &age) {

    boost::shared_ptr <reposit::ValueObject> valueObject(
        new AccountExample::CustomerValueObject(objectID, name, age, false));

    boost::shared_ptr<reposit::Object> object(
        new AccountExample::CustomerObject(valueObject, name, age, false));

    reposit::Repository::instance().storeObject(objectID, object, true);
}

void makeAccount(
    const std::string &objectID,
    const std::string &customer,
    const std::string &type,
    const long &number,
    reposit::property_t balance = reposit::property_t(),
    bool overwrite = false) {

    RP_GET_REFERENCE(customerRef, customer,
        AccountExample::CustomerObject, AccountExample::Customer)

    boost::shared_ptr <reposit::ValueObject> valueObject(
        new AccountExample::AccountValueObject(objectID, customer, type, number, balance, false));

    AccountExample::Account::Type typeEnum =
        reposit::Create<AccountExample::Account::Type>()(type);

    double accountBalance = reposit::convert2<double>(balance, "balance", 100.00);

    boost::shared_ptr<reposit::Object> object(
        new AccountExample::AccountObject(
            valueObject, customerRef, typeEnum, number, accountBalance, false));

    reposit::Repository::instance().storeObject(objectID, object, overwrite);

}


int main() {

    // Instantiate the reposit Repository
    reposit::Repository repository;
    // Instantiate the Enumerated Type Registry
    reposit::EnumTypeRegistry enumTypeRegistry;
    // Instantiate the Processor Factory
    reposit::ProcessorFactory processorFactory;
    // Instantiate the Serialization Factory
    AccountExample::SerializationFactory factory;

    try {

        // Specify log file
        reposit::logSetFile("./example.log");
        // Also direct log messages to stdout
        reposit::logSetConsole(1);
        RP_LOG_MESSAGE("begin example program");

    } catch (const std::exception &e) {

        std::cout << "Unable to initialize logging: " << e.what() << std::endl;
        return 1;

    } catch (...) {

        std::cout << "Unable to initialize logging." << std::endl;
        return 1;

    }

    try {
       AccountExample::registerEnumeratedTypes();

        // Construct some objects and store them in the object handler
        makeCustomer("customer1", "Joe", 40);
        makeAccount("account1", "customer1", "Savings", 123456789);
        makeAccount("account2", "customer1", "Current", 987654321, 100.00);


        // High level interrogation
        RP_LOG_MESSAGE("High level interrogation - after constructor");
        reposit::logObject("account2");

        // Retrieve an object and update it
        RP_GET_OBJECT(accountObject2_retrieve,
            "account2", AccountExample::AccountObject)
        accountObject2_retrieve->setBalance(100.00);

        // Low-level interrogation
        RP_LOG_MESSAGE("Low-level interrogation - after update");
        RP_GET_REFERENCE(accountObjectUnderlying, "account2",
            AccountExample::AccountObject, AccountExample::Account)
        RP_LOG_MESSAGE("Result of getBalance on underlying = "
            << accountObjectUnderlying->balance());

        // Delete an object
        reposit::Repository::instance().deleteObject("account2");

        // Log all objects
        RP_LOG_MESSAGE("Log all objects after deleting account2:");
        reposit::logAllObjects();

        // Serialize an object
        std::vector<boost::shared_ptr<reposit::Object> > objectList;
        objectList.push_back(accountObject2_retrieve);
        reposit::SerializationFactory::instance().saveObject(
            objectList, "./account.xml", true);

        // Deserialize an object
        reposit::SerializationFactory::instance().loadObject(
            ".", "account.xml", false, true);

        // Manipulate the deserialized object
        RP_GET_OBJECT(accountObject1_load,
            "account2", AccountExample::AccountObject)
        accountObject1_load->setBalance(200.00);
        RP_LOG_MESSAGE("Balance of account account2 = "
            << accountObject1_load->balance());

        // initially time
        RP_LOG_MESSAGE("The initially time of creating account2 is ");
        std::vector<std::string> vecOb;
        vecOb.push_back("account2");
        vecOb.push_back("account1");
        std::vector<double> vecTime;
        vecTime = reposit::Repository::instance().creationTime(vecOb);
         for(unsigned int i = 0; i < vecTime.size(); ++i)
            RP_LOG_MESSAGE(vecOb[i] <<"  "<< reposit::formatTime(vecTime[i]));
        //sleep(1);
        makeAccount("account2", "customer1", "Current", 987654321, 100.00, true);
       // last time
        RP_LOG_MESSAGE("The last time of creating account2 is ");
        vecTime.clear();
        vecTime = reposit::Repository::instance().updateTime(vecOb);
         for(unsigned int i = 0; i < vecTime.size(); ++i)
            RP_LOG_MESSAGE(vecOb[i] << "  "<< reposit::formatTime(vecTime[i]));

        // relation the  objects list
        RP_LOG_MESSAGE("relation the objects list is ");
        std::vector<std::string> relationIDs = reposit::Repository::instance().precedentIDs("account2");
        std::string show;
        for(unsigned int i = 0; i < relationIDs.size(); ++i)
            show += relationIDs[i];
        RP_LOG_MESSAGE(show);

        // Delete all objects
        reposit::Repository::instance().deleteAllObjects();

        RP_LOG_MESSAGE("End example program");
        return 0;

    } catch (const std::exception &e) {

        RP_LOG_ERROR("Error: " << e.what());
        return 1;

    } catch (...) {

        RP_LOG_ERROR("Error");
        return 1;

    }
}

