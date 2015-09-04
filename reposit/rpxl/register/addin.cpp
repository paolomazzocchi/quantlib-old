/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2004, 2005, 2006, 2007 Eric Ehlers

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

#include <xlsdk/xlsdkdefines.hpp>
#include <rpxl/objecthandlerxl.hpp>
#include <rpxl/register/register_all.hpp>
#include <rpxl/functions/export.hpp>
#include <rpxl/conversions/all.hpp>
#include <rpxl/utilities/xlutilities.hpp>
#include <sstream>

// Instantiate the reposit Repository
reposit::RepositoryXL repositoryXL;
// Instantiate the Enumerated Type Registry
reposit::EnumTypeRegistry enumTypeRegistry;
// Instantiate the Enumerated Class Registry
reposit::EnumClassRegistry enumClassRegistry;
// Instantiate the Enumerated Pair Registry
reposit::EnumPairRegistry enumPairRegistry;

DLLEXPORT int xlAutoOpen() {

    static XLOPER xDll;

    try {

        Excel(xlGetName, &xDll, 0);

        reposit::Configuration::instance().init();
        registerOhFunctions(xDll);

        Excel(xlFree, 0, 1, &xDll);
        return 1;

    } catch (const std::exception &e) {

        std::ostringstream err;
        err << "Error loading reposit: " << e.what();
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
        // empty the reposit repository
        //Excel(xlUDF, 0, 1, TempStrNoSize("\x1c""ohRepositoryDeleteAllObjects"));

        // Get the DLL name.
        Excel(xlGetName, &xDll, 0);
        // Unregister the addin functions.
        unregisterOhFunctions(xDll);
        // Clear the state of the Repository.
        reposit::RepositoryXL::instance().clear();
        // Release the DLL name.
        Excel(xlFree, 0, 1, &xDll);

        return 1;

    } catch (const std::exception &e) {

        Excel(xlFree, 0, 1, &xDll);
        std::ostringstream err;
        err << "Error unloading reposit: " << e.what();
        Excel(xlcAlert, 0, 1, TempStrStl(err.str()));
        return 0;

    } catch (...) {

        Excel(xlFree, 0, 1, &xDll);
        return 0;

    }
}

DLLEXPORT void xlAutoFree(XLOPER *px) {

    freeOper(px);

}

DLLEXPORT XLOPER *xlAddInManagerInfo(XLOPER *xlAction) {

    XLOPER xlReturn;
    static XLOPER xlLongName;

    // Coerce the argument XLOPER to an integer.
    Excel(xlCoerce, &xlReturn, 2, xlAction, TempInt(xltypeInt));

    // The only valid argument value is 1. In this case we return the
    // long name for the XLL. Any other value should result in the
    // return of a #VALUE! error.
    if (1 == xlReturn.val.w) {
        reposit::scalarToOper(std::string("reposit 1.7.0"), xlLongName);
    } else {
        xlLongName.xltype = xltypeErr;
        xlLongName.val.err = xlerrValue;
    }

    return &xlLongName;
}

