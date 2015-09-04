
/*  
 Copyright (C) 2007 Eric Ehlers
 
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
#include <rp/range.hpp>
#include <rp/valueobjects/vo_range.hpp>

#include <sstream>

#define XLL_DEC extern "C"
#define SET_SESSION_ID

XLL_DEC char *rpRange(
        char *ObjectId,
        FP *Values,
        OPER *Permanent,
        OPER *Trigger,
        bool *Overwrite) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("rpRange"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::vector<double> > ValuesCpp =
            reposit::fpToMatrix<double>(*Values);

        bool PermanentCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*Permanent), "Permanent", false);

        // Strip the Excel cell update counter suffix from Object IDs
        
        std::string ObjectIdStrip = reposit::CallingRange::getStub(ObjectId);

        // Construct the Value Object

        boost::shared_ptr<reposit::ValueObject> valueObject(
            new reposit::ValueObjects::rpRange(
                ObjectIdStrip,
                ValuesCpp,
                PermanentCpp));

        // Construct the Object
        
        boost::shared_ptr<reposit::Object> object(
            new reposit::Range(
                valueObject,
                ValuesCpp,
                PermanentCpp));

        // Store the Object in the Repository

        std::string returnValue =
            reposit::RepositoryXL::instance().storeObject(ObjectIdStrip, object, *Overwrite, valueObject);

        // Convert and return the return value

        static char ret[XL_MAX_STR_LEN];
        reposit::stringToChar(returnValue, ret);
        return ret;

    } catch (const std::exception &e) {
        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    } catch (...) {
        reposit::RepositoryXL::instance().logError("unkown error type", functionCall);
        return 0;
    }

}
