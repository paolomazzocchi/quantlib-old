
/*  
 Copyright (C) 2007 Eric Ehlers
 Copyright (C) 2006 Plamen Neykov
 
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
#include <rp/serializationfactory.hpp>

#include <sstream>

#define XLL_DEC extern "C"
#define SET_SESSION_ID

XLL_DEC OPER *ohObjectLoad(
        char *Directory,
        OPER *Pattern,
        OPER *Recurse,
        OPER *Overwrite,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectLoad"));

        if (functionCall->calledByFunctionWizard())
            return 0;

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::string PatternCpp = reposit::convert2<std::string>(
            reposit::ConvertOper(*Pattern), "Pattern", ".*\\.xml");

        bool RecurseCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*Recurse), "Recurse", false);

        bool OverwriteCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*Overwrite), "Overwrite", false);

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::SerializationFactory::instance().loadObject(
                Directory,
                PatternCpp,
                RecurseCpp,
                OverwriteCpp);

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
XLL_DEC OPER *ohObjectLoadString(
        char *Xml,
        OPER *Overwrite,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectLoadString"));

        if (functionCall->calledByFunctionWizard())
            return 0;

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        bool OverwriteCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*Overwrite), "Overwrite", false);

        // invoke the utility function

        std::vector<std::string> returnValue = reposit::SerializationFactory::instance().loadObjectString(
                Xml,
                OverwriteCpp);

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
XLL_DEC long *ohObjectSave(
        OPER *ObjectList,
        char *Filename,
        OPER *Overwrite,
        OPER *IncludeGroups,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectSave"));

        if (functionCall->calledByFunctionWizard())
            return 0;

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectListCpp =
            reposit::operToVector<std::string>(
                *ObjectList, "ObjectList");

        bool OverwriteCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*Overwrite), "Overwrite", false);

        bool IncludeGroupsCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*IncludeGroups), "IncludeGroups", true);

        // invoke the utility function

        static long returnValue;
        returnValue = reposit::SerializationFactory::instance().saveObject(
                ObjectListCpp,
                Filename,
                OverwriteCpp,
                IncludeGroupsCpp);

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
XLL_DEC char *ohObjectSaveString(
        OPER *ObjectList,
        OPER *Overwrite,
        OPER *Trigger) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<reposit::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object

        functionCall = boost::shared_ptr<reposit::FunctionCall>(
            new reposit::FunctionCall("ohObjectSaveString"));

        if (functionCall->calledByFunctionWizard())
            return 0;

        reposit::validateRange(Trigger, "Trigger");

        // initialize the session ID (if enabled)

        SET_SESSION_ID

        // convert input datatypes to C++ datatypes

        std::vector<std::string> ObjectListCpp =
            reposit::operToVector<std::string>(
                *ObjectList, "ObjectList");

        bool OverwriteCpp = reposit::convert2<bool>(
            reposit::ConvertOper(*Overwrite), "Overwrite", false);

        // convert input datatypes to Object references

        std::vector<boost::shared_ptr<reposit::Object> > ObjectListObj =
            reposit::getObjectVector<reposit::Object>(ObjectListCpp);

        // invoke the utility function

        std::string returnValue = reposit::SerializationFactory::instance().saveObjectString(
                ObjectListObj,
                OverwriteCpp);

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
