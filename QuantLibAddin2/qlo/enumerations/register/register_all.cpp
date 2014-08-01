/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007, 2014 Eric Ehlers

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

#include <qlo/enumerations/register/register_all.hpp>
#include <qlo/enumerations/register/register_types.hpp>
#include <qlo/enumerations/register/register_classes.hpp>

namespace QuantLibAddin {

    void registerEnumerations() {

        registerEnumeratedClasses();
        //registerEnumeratedCurves();
        ////registerEnumeratedHistoricalForwardRatesAnalysis();

        registerTypesCalendars();
        registerTypesDayCounters();
        registerTypesOptionTypes();
    }

    void unregisterEnumerations() {

        unregisterTypesCalendars();
        unregisterTypesDayCounters();
        unregisterTypesOptionTypes();
    }

}

