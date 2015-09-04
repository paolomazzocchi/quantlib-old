
/*  
 Copyright (C) 2004, 2005 Ferdinando Ametrano
 Copyright (C) 2004, 2005, 2006 Eric Ehlers
 Copyright (C) 2005, 2006 Plamen Neykov
 Copyright (C) 2004 StatPro Italia srl
 
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

#include <rp/utilities.hpp>
#include <rp/exception.hpp>
#include <rpxl/repositoryxl.hpp>
#include <rpxl/conversions/all.hpp>
#include <rpxl/functioncall.hpp>
#include <rpxl/callingrange.hpp>
#include <rp/loop/loop_valueobjects.hpp>
#include <rpxl/loop.hpp>

#include <sstream>

#define XLL_DEC extern "C"
#define SET_SESSION_ID

XLL_DEC OPER *ohObjectPropertyNames(
        char *ObjectId,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectPropertyNames"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to Object references

        RP_GET_OBJECT(ObjectIdObj, ObjectId, reposit::Object)

        // invoke the member function

        std::vector<std::string> returnValue = ObjectIdObj->propertyNamesVector();

        // convert and return the return value

        static OPER xRet;
        reposit::vectorToOper(returnValue, xRet);
        return &xRet;

    } catch (const std::exception &e) {
        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    } catch (...) {
        reposit::RepositoryXL::instance().logError("unkown error type", functionCall);
        return 0;
    }

}
XLL_DEC OPER *ohObjectPropertyValues(
        char *ObjectId,
        OPER *PropertyName,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectPropertyValues"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to Object references

        RP_GET_OBJECT(ObjectIdObj, ObjectId, reposit::Object)

        // loop on the input parameter and populate the return vector

        static XLOPER returnValue;

        reposit::ohObjectPropertyValuesBind bindObject = 
            boost::bind((reposit::ohObjectPropertyValuesSignature)
                &reposit::Object::propertyValue,
                ObjectIdObj,
                _1);
        reposit::loop
            <reposit::ohObjectPropertyValuesBind, std::string, reposit::property_t>
            (functionCall, bindObject, PropertyName, returnValue);

        return &returnValue;

    } catch (const std::exception &e) {
        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    } catch (...) {
        reposit::RepositoryXL::instance().logError("unkown error type", functionCall);
        return 0;
    }

}
