
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
#include <rp/utilities.hpp>

#include <sstream>

#define XLL_DEC extern "C"
#define SET_SESSION_ID

XLL_DEC char *ohLogFile(
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohLogFile"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        std::string returnValue = reposit::logFile();

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
XLL_DEC long *ohLogLevel(
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohLogLevel"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        static long returnValue;
        returnValue = reposit::logLevel();

        // convert and return the return value

        return &returnValue;

    } catch (const std::exception &e) {
        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    } catch (...) {
        reposit::RepositoryXL::instance().logError("unkown error type", functionCall);
        return 0;
    }

}
XLL_DEC char *ohLogSetFile(
        char *LogFileName,
        OPER *LogLevel,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohLogSetFile"));

        if (functionCall->calledByFunctionWizard())
            return 0;

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        long LogLevelCpp = reposit::convert2<long>(
            reposit::ConvertOper(*LogLevel), "LogLevel", 4);

        // invoke the utility function

        std::string returnValue = reposit::logSetFile(
                LogFileName,
                LogLevelCpp);

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
XLL_DEC bool *ohLogSetLevel(
        long *LogLevel,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohLogSetLevel"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        static bool returnValue = true;
        reposit::logSetLevel(
                *LogLevel);

        // convert and return the return value

        return &returnValue;

    } catch (const std::exception &e) {
        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    } catch (...) {
        reposit::RepositoryXL::instance().logError("unkown error type", functionCall);
        return 0;
    }

}
XLL_DEC bool *ohLogWriteMessage(
        char *LogMessage,
        OPER *LogLevel,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohLogWriteMessage"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        long LogLevelCpp = reposit::convert2<long>(
            reposit::ConvertOper(*LogLevel), "LogLevel", 4);

        // invoke the utility function

        static bool returnValue = true;
        reposit::logWriteMessage(
                LogMessage,
                LogLevelCpp);

        // convert and return the return value

        return &returnValue;

    } catch (const std::exception &e) {
        reposit::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    } catch (...) {
        reposit::RepositoryXL::instance().logError("unkown error type", functionCall);
        return 0;
    }

}
