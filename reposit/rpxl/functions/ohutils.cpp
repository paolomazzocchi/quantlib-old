
/*  
 Copyright (C) 2004, 2005, 2006, 2007, 2008 Ferdinando Ametrano
 Copyright (C) 2004, 2005, 2006, 2007, 2011 Eric Ehlers
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
#include <rp/utilities.hpp>

#include <sstream>

#define XLL_DEC extern "C"
#define SET_SESSION_ID

XLL_DEC char *ohBoostVersion(
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohBoostVersion"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        std::string returnValue = reposit::boostVersion();

        // convert and return the return value

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
XLL_DEC char *ohStringConcatenate(
        OPER *Substrings,
        char *Delimiter) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohStringConcatenate"));


        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> SubstringsCpp =
            reposit::operToVector<std::string>(
                *Substrings, "Substrings");

        // invoke the utility function

        std::string returnValue = reposit::concatenate(
                SubstringsCpp,
                Delimiter);

        // convert and return the return value

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
XLL_DEC OPER *ohStringSplit(
        char *String,
        char *Delimiters,
        OPER *ConcatenateDelim) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohStringSplit"));


        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        bool ConcatenateDelimCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*ConcatenateDelim), "ConcatenateDelim", false);

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::split(
                String,
                Delimiters,
                ConcatenateDelimCpp);

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
XLL_DEC char *ohVbaError(
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohVbaError"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        std::string returnValue = reposit::RepositoryXL::instance().vbaError();

        // convert and return the return value

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
XLL_DEC char *ohVersion(
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohVersion"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        std::string returnValue = reposit::version();

        // convert and return the return value

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
