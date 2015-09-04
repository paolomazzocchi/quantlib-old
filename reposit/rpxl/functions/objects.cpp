
/*  
 Copyright (C) 2008 Eric Ehlers
 
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
#include <rp/objectwrapper.hpp>

#include <sstream>

#define XLL_DEC extern "C"
#define SET_SESSION_ID

XLL_DEC OPER *ohObjectCallerAddress(
        OPER *ObjectId,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectCallerAddress"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIdCpp =
            reposit::operToVector<std::string>(
                *ObjectId, "ObjectId");

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::RepositoryXL::instance().callerAddress(
                ObjectIdCpp);

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
XLL_DEC OPER *ohObjectCallerKey(
        OPER *ObjectId,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectCallerKey"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIdCpp =
            reposit::operToVector<std::string>(
                *ObjectId, "ObjectId");

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::RepositoryXL::instance().callerKey(
                ObjectIdCpp);

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
XLL_DEC OPER *ohObjectClassName(
        OPER *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectClassName"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIDCpp =
            reposit::operToVector<std::string>(
                *ObjectID, "ObjectID");

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::RepositoryXL::instance().className(
                ObjectIDCpp);

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
XLL_DEC OPER *ohObjectCreationTime(
        OPER *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectCreationTime"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIDCpp =
            reposit::operToVector<std::string>(
                *ObjectID, "ObjectID");

        // invoke the utility function

        std::vector<double> returnValue = reposit::Repository::instance().creationTime(
                ObjectIDCpp);

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
XLL_DEC OPER *ohObjectExists(
        OPER *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectExists"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIDCpp =
            reposit::operToVector<std::string>(
                *ObjectID, "ObjectID");

        // invoke the utility function

        std::vector<bool> returnValue = reposit::RepositoryXL::instance().objectExists(
                ObjectIDCpp);

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
XLL_DEC OPER *ohObjectIsOrphan(
        OPER *ObjectId,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectIsOrphan"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIdCpp =
            reposit::operToVector<std::string>(
                *ObjectId, "ObjectId");

        // invoke the utility function

        std::vector<bool> returnValue = reposit::RepositoryXL::instance().isOrphan(
                ObjectIdCpp);

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
XLL_DEC OPER *ohObjectIsPermanent(
        OPER *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectIsPermanent"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIDCpp =
            reposit::operToVector<std::string>(
                *ObjectID, "ObjectID");

        // invoke the utility function

        std::vector<bool> returnValue = reposit::RepositoryXL::instance().isPermanent(
                ObjectIDCpp);

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
XLL_DEC OPER *ohObjectPrecedentIDs(
        char *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectPrecedentIDs"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::Repository::instance().precedentIDs(
                ObjectID);

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
XLL_DEC OPER *ohObjectUpdateCounter(
        OPER *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectUpdateCounter"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIDCpp =
            reposit::operToVector<std::string>(
                *ObjectID, "ObjectID");

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::RepositoryXL::instance().updateCounter(
                ObjectIDCpp);

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
XLL_DEC OPER *ohObjectUpdateTime(
        OPER *ObjectID,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectUpdateTime"));

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectIDCpp =
            reposit::operToVector<std::string>(
                *ObjectID, "ObjectID");

        // invoke the utility function

        std::vector<double> returnValue = reposit::Repository::instance().updateTime(
                ObjectIDCpp);

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
