/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005, 2006, 2007, 2009 Eric Ehlers
 Copyright (C) 2004 Ferdinando Ametrano

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

/*! \file
    \brief Preprocessor directives for reposit compilation
*/

#ifndef rp_defines_hpp
#define rp_defines_hpp

#include <boost/config.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION < 103600
    #error using an old version of Boost, please update to 1.36.0 or higher.
#endif

//! Version string.
#ifdef _DEBUG
    #define REPOSIT_VERSION "1.7.0-debug"
#else
    #define REPOSIT_VERSION "1.7.0"
#endif

//! Version hexadecimal number.
#define REPOSIT_HEX_VERSION 0x010700f0
//! Version string for output lib name.
#define REPOSIT_LIB_VERSION "1_7_0"

#include <cctype>
#if defined(BOOST_NO_STDC_NAMESPACE)
    namespace std { using ::tolower; using ::toupper; }
#endif

#ifdef XLL_EXPORTS
    #define DLL_API __declspec(dllexport)
    #define COMPILING_XLL_DYNAMIC
#elif XLL_IMPORTS
    #define DLL_API __declspec(dllimport)
    #define COMPILING_XLL_DYNAMIC
#elif XLL_STATIC
    #define DLL_API
#else
    #define DLL_API
#endif

#if defined BOOST_MSVC
#pragma warning(disable : 4996)     // Using a deprecated function
//#pragma warning(disable : 4244)     // An integer type is converted to a smaller integer type
#pragma warning(disable : 4267)     // Sending a size_t to an output stream
// non-ASCII characters - Disabling this warning here is ineffective
// and the change has been made instead under project properties
//#pragma warning(disable: 4819)

// behavior change: an object of POD type constructed with
// an initializer of the form () will be default-initialized
#pragma warning(disable:4345)

//#pragma warning(disable:4503)

#endif

#if defined BOOST_MSVC
#define STRICMP _stricmp
#else
#define STRICMP strcasecmp
#endif

//! Get a boost shared pointer to a class derived from Object.
#define RP_GET_OBJECT( NAME, ID, OBJECT_CLASS ) \
    boost::shared_ptr<OBJECT_CLASS > NAME; \
    reposit::Repository::instance().retrieveObject(NAME, ID);

//! Like RP_GET_OBJECT but only attempt retrieval if id supplied.
#define RP_GET_OBJECT_DEFAULT( NAME, ID, OBJECT_CLASS ) \
    boost::shared_ptr<OBJECT_CLASS > NAME; \
    if (!ID.empty()) { \
        reposit::Repository::instance().retrieveObject(NAME, ID); \
    }

//! Get a shared pointer to the library object referenced by an Object.
#define RP_GET_REFERENCE( NAME, ID, OBJECT_CLASS, LIBRARY_CLASS ) \
    RP_GET_OBJECT(NAME ## temp, ID, OBJECT_CLASS ) \
    boost::shared_ptr<LIBRARY_CLASS> NAME; \
    NAME ## temp->getLibraryObject(NAME);

//! Like RP_GET_REFERENCE but only attempt retrieval if id supplied.
#define RP_GET_REFERENCE_DEFAULT( NAME, ID, OBJECT_CLASS, LIBRARY_CLASS ) \
    boost::shared_ptr<LIBRARY_CLASS> NAME; \
    if (!ID.empty()) { \
        RP_GET_OBJECT(NAME ## temp, ID, OBJECT_CLASS ) \
        NAME ## temp->getLibraryObject(NAME); \
    }

//! Get a direct reference to the underlying object wrapped by the Object.
#define RP_GET_UNDERLYING( NAME, ID, OBJECT_CLASS, LIBRARY_CLASS ) \
    RP_GET_REFERENCE(NAME ## temp, ID, OBJECT_CLASS, LIBRARY_CLASS ) \
    const LIBRARY_CLASS &NAME = *(NAME ## temp.get());

//! Like RP_GET_UNDERLYING but without const qualifier.
#define RP_GET_UNDERLYING_NONCONST( NAME, ID, OBJECT_CLASS, LIBRARY_CLASS ) \
    RP_GET_REFERENCE(NAME ## temp, ID, OBJECT_CLASS, LIBRARY_CLASS ) \
    LIBRARY_CLASS &NAME = *(NAME ## temp.get());

//! Log the given message.
#define RP_LOG_MESSAGE(message) \
do { \
    std::ostringstream _rp_msg_stream; \
    _rp_msg_stream << message; \
    reposit::logWriteMessage(_rp_msg_stream.str()); \
} while (false)

//! Log the given error message.
#define RP_LOG_ERROR(message) \
do { \
    std::ostringstream _rp_msg_stream; \
    _rp_msg_stream << message; \
    reposit::logWriteMessage(_rp_msg_stream.str(), 1); \
} while (false)

//! An empty constructor for a class derived from Object.
#define RP_OBJ_CTOR(derived_class, base_class) \
derived_class( \
const boost::shared_ptr<reposit::ValueObject>& properties, \
bool permanent) \
: base_class(properties, permanent) {}

//! An empty class derived from Object.
#define RP_OBJ_CLASS(derived_class, base_class) \
class derived_class : \
public base_class { \
protected: \
RP_OBJ_CTOR(derived_class, base_class) \
}

//! An empty constructor for a class derived from LibraryObject.
#define RP_LIB_CTOR(derived_class, base_class) \
derived_class( \
const boost::shared_ptr<reposit::ValueObject>& properties, \
bool permanent) \
: reposit::LibraryObject<base_class>(properties, permanent) {}

//! An empty class derived from LibraryObject.
#define RP_LIB_CLASS(derived_class, base_class) \
class derived_class : \
public reposit::LibraryObject<base_class> { \
protected: \
RP_LIB_CTOR(derived_class, base_class) \
}

#endif
