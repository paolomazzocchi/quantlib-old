
/*
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

#include <rp/enumerations/typefactory.hpp>
#include <rpxl/repositxl.hpp>
#include <rpxl/register/register_all.hpp>
#include <rpxl/functions/export.hpp>
#include <rpxl/utilities/xlutilities.hpp>
#include <ExampleObjects/accountexample.hpp>
#include <rpxl/objectwrapperxl.hpp>

/* Use BOOST_MSVC instead of _MSC_VER since some other vendors (Metrowerks,
   for example) also #define _MSC_VER
*/
#ifdef BOOST_MSVC
#  define BOOST_LIB_DIAGNOSTIC
#  include <rp/auto_link.hpp>
#  undef BOOST_LIB_DIAGNOSTIC
#endif
#include <sstream>

DLLEXPORT int xlAutoOpen() {

    // Instantiate the reposit Repository
    static reposit::RepositoryXL repositoryXL;
    // Instantiate the Enumerated Type Registry
    static reposit::EnumTypeRegistry enumTypeRegistry;
    // Instantiate the Enumerated Class Registry
    static reposit::EnumClassRegistry enumClassRegistry;
    // Instantiate the Enumerated Pair Registry
    static reposit::EnumPairRegistry enumPairRegistry;
	//Instantiate the Processor Factory
	static reposit::ProcessorFactory processorFactory;
    // Instantiate the Serialization Factory
    static AccountExample::SerializationFactory factory;

    static XLOPER xDll;

    try {

        Excel(xlGetName, &xDll, 0);

        reposit::Configuration::instance().init();

        registerOhFunctions(xDll);
        AccountExample::registerEnumeratedTypes();

        Excel(xlfRegister, 0, 7, &xDll,
            TempStrNoSize("\x0A""ohCustomer"),          // function code name
            TempStrNoSize("\x06""CCCNL#"),              // parameter codes
            TempStrNoSize("\x0A""ohCustomer"),          // function display name
            TempStrNoSize("\x1B""ObjectID,Name,Age,Permanent"), // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x07""Example"));            // function category

        Excel(xlfRegister, 0, 7, &xDll,
            TempStrNoSize("\x09""ohAccount"),           // function code name
            TempStrNoSize("\x08""CCCCNPL#"),            // parameter codes
            TempStrNoSize("\x09""ohAccount"),           // function display name
            TempStrNoSize("\x2F""ObjectID,Customer,Type,Number,Balance,Permanent"), // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x07""Example"));            // function category

        Excel(xlfRegister, 0, 7, &xDll,
            TempStrNoSize("\x13""ohAccountSetBalance"), // function code name
            TempStrNoSize("\x04""LCE#"),                // parameter codes
            TempStrNoSize("\x13""ohAccountSetBalance"), // function display name
            TempStrNoSize("\x10""ObjectID,Balance"),    // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x07""Example"));            // function category

        Excel(xlfRegister, 0, 7, &xDll,
            TempStrNoSize("\x10""ohAccountBalance"),    // function code name
            TempStrNoSize("\x04""ECP#"),                // parameter codes
            TempStrNoSize("\x10""ohAccountBalance"),    // function display name
            TempStrNoSize("\x10""ObjectID,Trigger"),    // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x07""Example"));            // function category

        Excel(xlfRegister, 0, 7, &xDll,
            TempStrNoSize("\x0D""ohAccountType"),       // function code name
            TempStrNoSize("\x04""CCP#"),                // parameter codes
            TempStrNoSize("\x0D""ohAccountType"),       // function code name
            TempStrNoSize("\x10""ObjectID,Trigger"),    // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x07""Example"));            // function category

        Excel(xlfRegister, 0, 7, &xDll,
            TempStrNoSize("\x15""ohAccountCustomerName"),// function code name
            TempStrNoSize("\x04""CCP#"),                // parameter codes
            TempStrNoSize("\x15""ohAccountCustomerName"),// function display name
            TempStrNoSize("\x10""ObjectID,Trigger"),    // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x07""Example"));            // function category

        Excel(xlFree, 0, 1, &xDll);
        return 1;

    } catch (const std::exception &e) {

        std::ostringstream err;
        err << "Error loading ExampleXllStatic: " << e.what();
        Excel(xlcAlert, 0, 1, TempStrStl(err.str()));
        Excel(xlFree, 0, 1, &xDll);
        return 0;

    } catch (...) {

        Excel(xlFree, 0, 1, &xDll);
        return 0;

    }
}

DLLEXPORT int xlAutoClose() {

    static XLOPER xDll;

    try {

        Excel(xlGetName, &xDll, 0);

        AccountExample::unregisterEnumeratedTypes();
        unregisterOhFunctions(xDll);
        reposit::RepositoryXL::instance().clear();

        Excel(xlFree, 0, 1, &xDll);
        return 1;

    } catch (const std::exception &e) {

        std::ostringstream err;
        err << "Error unloading ExampleXllStatic: " << e.what();
        Excel(xlcAlert, 0, 1, TempStrStl(err.str()));
        Excel(xlFree, 0, 1, &xDll);
        return 0;

    } catch (...) {

        Excel(xlFree, 0, 1, &xDll);
        return 0;

    }

}

DLLEXPORT void xlAutoFree(XLOPER *px) {

    freeOper(px);

}

DLLEXPORT char *ohCustomer(
        char *objectID,
        char *name,
        long *age,
        bool *permanent) {

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        functionCall = boost::shared_ptr<reposit::FunctionCall>
            (new reposit::FunctionCall("ohCustomer"));

        boost::shared_ptr<reposit::ValueObject> valueObject(
            new AccountExample::CustomerValueObject(objectID, name, *age, *permanent));

        boost::shared_ptr<reposit::Object> object(
            new AccountExample::CustomerObject(valueObject, name, *age, *permanent));

        std::string returnValue = 
            reposit::RepositoryXL::instance().storeObject(objectID, object);

        static char ret[XL_MAX_STR_LEN];
        reposit::stringToChar(returnValue, ret);
        return ret;

    } catch (const std::exception &e) {

        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;

    }
}

DLLEXPORT char *ohAccount(
        char *objectID,
        char *customer,
        char *type,
        long *number,
        OPER *balance,
        bool *permanent) {

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        functionCall = boost::shared_ptr<reposit::FunctionCall>
            (new reposit::FunctionCall("ohAccount"));

        RP_GET_REFERENCE(customerRef, customer,
            AccountExample::CustomerObject, AccountExample::Customer)

        double balanceDouble = reposit::convert2<double>(
            reposit::ConvertOper(*balance), "balance", 100.00);

        reposit::property_t balanceProperty =
            reposit::convert2<reposit::property_t>(
                reposit::ConvertOper(*balance), "balance");

        AccountExample::Account::Type typeEnum =
            reposit::Create<AccountExample::Account::Type>()(type);

        boost::shared_ptr<reposit::ValueObject> valueObject(
            new AccountExample::AccountValueObject(objectID, customer, type, *number, balanceProperty, *permanent));

        boost::shared_ptr<reposit::Object> object(
            new AccountExample::AccountObject(valueObject, customerRef, typeEnum, *number, balanceDouble, *permanent));

    
        std::string returnValue = 
            reposit::RepositoryXL::instance().storeObject(objectID, object, true);

        static char ret[XL_MAX_STR_LEN];
        reposit::stringToChar(returnValue, ret);
        return ret;

    } catch (const std::exception &e) {

        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;

    }
}

DLLEXPORT short int *ohAccountSetBalance(char *objectID, double *balance) {

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        functionCall = boost::shared_ptr<reposit::FunctionCall>
            (new reposit::FunctionCall("ohAccountSetBalance"));

        RP_GET_OBJECT(accountObject, objectID, AccountExample::AccountObject)
        accountObject->setBalance(*balance);

        static short int ret = TRUE;
        return &ret;

    } catch (const std::exception &e) {

        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;

    }
}

DLLEXPORT double *ohAccountBalance(char *objectID, OPER *trigger) {

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        functionCall = boost::shared_ptr<reposit::FunctionCall>
            (new reposit::FunctionCall("ohAccountBalance"));

        RP_GET_OBJECT(accountObject, objectID, AccountExample::AccountObject)

        static double ret;
        ret = accountObject->balance();
        return &ret;

    } catch (const std::exception &e) {

        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;

    }
}

DLLEXPORT char *ohAccountCustomerName(char *objectID, OPER *trigger) {

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        functionCall = boost::shared_ptr<reposit::FunctionCall>
            (new reposit::FunctionCall("ohAccountCustomerName"));

        RP_GET_REFERENCE(accountRef, objectID,
            AccountExample::AccountObject, AccountExample::Account)

        static char ret[XL_MAX_STR_LEN];
        reposit::stringToChar(accountRef->customerName(), ret);
        return ret;

    } catch (const std::exception &e) {

        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;

    }
}

DLLEXPORT char *ohAccountType(char *objectID, OPER *trigger) {

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        functionCall = boost::shared_ptr<reposit::FunctionCall>
            (new reposit::FunctionCall("ohAccountType"));

        RP_GET_OBJECT(accountObject, objectID, AccountExample::AccountObject)

        static char ret[XL_MAX_STR_LEN];
        reposit::stringToChar(accountObject->type(), ret);
        return ret;

    } catch (const std::exception &e) {

        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;

    }
}
