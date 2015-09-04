/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2014 Eric Ehlers

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
#include <rp/addin.hpp>
#include <rp/utilities.hpp>
#include <rp/repository.hpp>
#include <rp/serializationfactory.hpp>

std::string reposit::ohVersion() {
    return reposit::version();
}

void reposit::ohDeleteObject(const std::string &objectID) {
    reposit::Repository::instance().deleteObject(objectID);
}

std::string reposit::ohObjectSaveString(const std::string &objectId) {
    RP_GET_OBJECT(x, objectId, reposit::Object)
    return reposit::SerializationFactory::instance().saveObjectString(x);
}

void reposit::ohLoadObjectString(const std::string &xml, bool overwriteExisting) {
    reposit::SerializationFactory::instance().loadObjectString(xml, overwriteExisting);
}

