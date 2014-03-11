/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/* 
 Copyright (C) 2006, 2007, 2011 Eric Ehlers

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

// Excel functions coded by hand, to be registered with Excel
// alongside the autogenerated functions.

#include <oh/utilities.hpp>
#include <oh/exception.hpp>
#include <ohxl/repositoryxl.hpp>
#include <ohxl/conversions/all.hpp>
#include <ohxl/functioncall.hpp>
#include <ohxl/functions/functioncount.hpp>
#include <ohxl/xloper.hpp>
#include <map>
#include <algorithm>

#define XLL_DEC extern "C"

void operToOper(OPER *xTarget, const OPER *xSource) {
    if (xSource->xltype == xltypeNum) {
        xTarget->xltype = xltypeNum;
        xTarget->val.num = xSource->val.num;
        return;
    } else if (xSource->xltype == xltypeStr) {
        // Must use type unsigned char (BYTE) to process the 0th byte of Excel byte-counted string
        unsigned char len = xSource->val.str[0];
        xTarget->val.str = new char[ len + 1 ];
        xTarget->xltype = xltypeStr | xlbitDLLFree;
        xTarget->val.str[0] = len;
        if (len)
            strncpy(xTarget->val.str + 1, xSource->val.str + 1, len);
        return;
    } else if (xSource->xltype == xltypeBool) {
        xTarget->xltype = xltypeBool;
        xTarget->val.boolean = xSource->val.boolean;
        return;
    } else if (xSource->xltype == xltypeErr) {
        xTarget->xltype = xltypeErr;
        xTarget->val.err = xSource->val.err;
        return;
    } else if (xSource->xltype == xltypeNil) {
        xTarget->xltype = xltypeNil;
        return;
    } else if (xSource->xltype == xltypeInt) {
        xTarget->xltype = xltypeInt;
        xTarget->val.w = xSource->val.w;
        return;
    } else {
        OH_FAIL("operToOper: unexpected OPER type: " << xSource->xltype);
    }
}

void validateReference(const XLOPER *xReference, const std::string &name) {
    OH_REQUIRE(xReference->xltype != xltypeErr && xReference->val.err != xlerrRef,
        "parameter '" << name << "' is not a valid range reference");
}

XLL_DEC long *ohTrigger(
        OPER *dummy0,
        OPER *dummy1,
        OPER *dummy2,
        OPER *dummy3,
        OPER *dummy4,
        OPER *dummy5,
        OPER *dummy6,
        OPER *dummy7,
        OPER *dummy8,
        OPER *dummy9) {

    boost::shared_ptr<ObjectHandler::FunctionCall> functionCall;

    try {
        functionCall = boost::shared_ptr<ObjectHandler::FunctionCall>
            (new ObjectHandler::FunctionCall("ohTrigger"));

        validateReference(dummy0, "dummy0");
        validateReference(dummy1, "dummy1");
        validateReference(dummy2, "dummy2");
        validateReference(dummy3, "dummy3");
        validateReference(dummy4, "dummy4");
        validateReference(dummy5, "dummy5");
        validateReference(dummy6, "dummy6");
        validateReference(dummy7, "dummy7");
        validateReference(dummy8, "dummy8");
        validateReference(dummy9, "dummy9");

        static long returnValue;
        static std::map<std::string, long> iterators;
        returnValue = iterators[ObjectHandler::FunctionCall::instance().addressString()]++;
        return &returnValue;

    } catch (const std::exception &e) {
        ObjectHandler::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    }
}

XLL_DEC long *ohFunctionCount() {
    static long returnValue = FUNCTION_COUNT;
    return &returnValue;
}

int countValidRows(const OPER &xMulti) {
    for (int numValidRows=xMulti.val.array.rows; numValidRows; --numValidRows) {
        for (int i=0; i<xMulti.val.array.columns; ++i) {
            int index = (numValidRows - 1) * xMulti.val.array.columns + i;
            if (!(xMulti.val.array.lparray[index].xltype & (xltypeErr | xltypeNil)))
                return numValidRows;
        }
    }
    return 1;
}

int countValidCols(const OPER &xMulti) {
    for (int numValidCols=xMulti.val.array.columns; numValidCols; --numValidCols) {
        for (int i=0; i<xMulti.val.array.rows; ++i) {
            int index = (numValidCols - 1) + i * xMulti.val.array.columns;
            if (!(xMulti.val.array.lparray[index].xltype & (xltypeErr | xltypeNil)))
                return numValidCols;
        }
    }
    return 1;
}

XLL_DEC OPER *ohPack(OPER *xInputRange) {

    // initialize Function Call object

    boost::shared_ptr<ObjectHandler::FunctionCall> functionCall;

    ObjectHandler::Xloper xMulti;
    static OPER xRet;
    xRet.val.array.lparray = 0;

    try {
        functionCall = boost::shared_ptr<ObjectHandler::FunctionCall>
            (new ObjectHandler::FunctionCall("ohPack"));

        Excel(xlCoerce, &xMulti, 2, xInputRange, TempInt(xltypeMulti));

        int numValidRows = countValidRows(xMulti());
        int numValidCols = countValidCols(xMulti());

        xRet.val.array.rows = numValidRows;
        xRet.val.array.columns = numValidCols;
        xRet.val.array.lparray = new OPER[numValidRows * numValidCols]; 
        xRet.xltype = xltypeMulti | xlbitDLLFree;

        for (int i=0; i<numValidRows; ++i) {
            for (int j=0; j<numValidCols; ++j) {
                int indexSource = i * xMulti->val.array.columns + j;
                int indexTarget = i * numValidCols + j;
                operToOper(&xRet.val.array.lparray[indexTarget], 
                    &xMulti->val.array.lparray[indexSource]);
            }
        }

        return &xRet;
    } catch (const std::exception &e) {

        // free any memory that may have been allocated

        if (xRet.xltype & xltypeMulti && xRet.val.array.lparray) {
            for (int i=0; i<xRet.val.array.columns * xRet.val.array.rows; ++i) {
                if (xRet.val.array.lparray[i].xltype & xltypeStr && xRet.val.array.lparray[i].val.str)
                    delete [] xRet.val.array.lparray[i].val.str;
            }
            delete [] xRet.val.array.lparray;
        }

        // log the exception and return a null pointer (#NUM!) to Excel

        ObjectHandler::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    }
}

bool rowIsValid(const OPER &xMulti, int i) {
    for (int j=0; j<xMulti.val.array.columns; ++j) {
        int index = i * xMulti.val.array.columns + j;
        if (xMulti.val.array.lparray[index].xltype & (xltypeErr | xltypeNil))
            return false;
    }
    return true;
}

int countValidRows2(const OPER &xMulti) {
    int ret = 0;
    for (int i=0; i<xMulti.val.array.rows; ++i)
        ret += rowIsValid(xMulti, i);
    return ret;
}

XLL_DEC OPER *ohRemoveInvalidRows(OPER *xInputRange) {

    // initialize Function Call object
    boost::shared_ptr<ObjectHandler::FunctionCall> functionCall;

    ObjectHandler::Xloper xMulti;
    static OPER xRet;
    xRet.val.array.lparray = 0;

    try {
        functionCall = boost::shared_ptr<ObjectHandler::FunctionCall>
            (new ObjectHandler::FunctionCall("ohRemoveInvalidRows"));

        Excel(xlCoerce, &xMulti, 2, xInputRange, TempInt(xltypeMulti));

        int numValidRows = countValidRows2(xMulti());
        if (!numValidRows) return 0;
        int numCols = xMulti->val.array.columns;

        xRet.val.array.rows = numValidRows;
        xRet.val.array.columns = numCols;
        xRet.val.array.lparray = new OPER[numValidRows * numCols]; 
        xRet.xltype = xltypeMulti | xlbitDLLFree;

        int i2 = 0;
        for (int i=0; i<xMulti->val.array.rows; ++i) {
            if (rowIsValid(xMulti(), i)) {
                for (int j=0; j<numCols; ++j) {
                    int indexSource = i * numCols + j;
                    int indexTarget = i2 * numCols + j;
                    operToOper(&xRet.val.array.lparray[indexTarget], 
                        &xMulti->val.array.lparray[indexSource]);
                }
                i2++;
            }
        }

        return &xRet;

    } catch (const std::exception &e) {

        // free any memory that may have been allocated

        if (xRet.xltype & xltypeMulti && xRet.val.array.lparray) {
            for (int i=0; i<xRet.val.array.columns * xRet.val.array.rows; ++i) {
                if (xRet.val.array.lparray[i].xltype & xltypeStr && xRet.val.array.lparray[i].val.str)
                    delete [] xRet.val.array.lparray[i].val.str;
            }
            delete [] xRet.val.array.lparray;
        }

        // log the exception and return a null pointer (#NUM!) to Excel

        ObjectHandler::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    }
}

bool columnIsValid(const OPER &xMulti, int j) {
    for (int i=0; i<xMulti.val.array.rows; ++i) {
        int index = i * xMulti.val.array.columns + j;
        if (xMulti.val.array.lparray[index].xltype & (xltypeErr | xltypeNil))
            return false;
    }
    return true;
}

int countValidColumns(const OPER &xMulti) {
    int ret = 0;
    for (int j=0; j<xMulti.val.array.columns; ++j)
        ret += columnIsValid(xMulti, j);
    return ret;
}

XLL_DEC OPER *ohRemoveInvalidColumns(OPER *xInputRange) {

    // initialize Function Call object
    boost::shared_ptr<ObjectHandler::FunctionCall> functionCall;

    ObjectHandler::Xloper xMulti;
    static OPER xRet;
    xRet.val.array.lparray = 0;

    try {
        functionCall = boost::shared_ptr<ObjectHandler::FunctionCall>
            (new ObjectHandler::FunctionCall("ohRemoveInvalidColumns"));

        Excel(xlCoerce, &xMulti, 2, xInputRange, TempInt(xltypeMulti));

        int numValidCols = countValidColumns(xMulti());
        if (!numValidCols) return 0;
        int numRows = xMulti->val.array.rows;

        xRet.val.array.rows = numRows;
        xRet.val.array.columns = numValidCols;
        xRet.val.array.lparray = new OPER[numRows * numValidCols]; 
        xRet.xltype = xltypeMulti | xlbitDLLFree;

        for (int i=0; i<xMulti->val.array.rows; ++i) {
			int j2 = 0;
            for (int j=0; j<xMulti->val.array.columns; ++j) {
				if (columnIsValid(xMulti(), j)) {
					int indexSource = i * xMulti->val.array.columns + j;
					int indexTarget = i * numValidCols + j2;
					operToOper(&xRet.val.array.lparray[indexTarget], 
						&xMulti->val.array.lparray[indexSource]);
					j2++;
				}
            }
        }

        return &xRet;

    } catch (const std::exception &e) {

        // free any memory that may have been allocated

        if (xRet.xltype & xltypeMulti && xRet.val.array.lparray) {
            for (int i=0; i<xRet.val.array.columns * xRet.val.array.rows; ++i) {
                if (xRet.val.array.lparray[i].xltype & xltypeStr && xRet.val.array.lparray[i].val.str)
                    delete [] xRet.val.array.lparray[i].val.str;
            }
            delete [] xRet.val.array.lparray;
        }

        // log the exception and return a null pointer (#NUM!) to Excel

        ObjectHandler::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    }
}

XLL_DEC OPER *ohFilter(
        OPER *xInput,
        OPER *flags) {

    // declare a shared pointer to the Function Call object

    boost::shared_ptr<ObjectHandler::FunctionCall> functionCall;

    try {

        // instantiate the Function Call object
    
        functionCall = boost::shared_ptr<ObjectHandler::FunctionCall>(
            new ObjectHandler::FunctionCall("ohFilter"));

        // convert input datatypes to C++ datatypes

        std::vector<bool> flagsCpp = ObjectHandler::operToVector<bool>(*flags, "flags");

        const OPER *xMulti;
        ObjectHandler::Xloper xTemp;
        if (xInput->xltype == xltypeMulti) {
            xMulti = xInput;
        } else {
            Excel(xlCoerce, &xTemp, 2, xInput, TempInt(xltypeMulti));
            xMulti = &xTemp;
        }

        int sizeInput = xMulti->val.array.rows * xMulti->val.array.columns;
        OH_REQUIRE(sizeInput == flagsCpp.size(),
            "size mismatch between value vector (" << sizeInput << 
            ") and flag vector (" << flagsCpp.size() << ")");


        static OPER xRet;
        xRet.val.array.rows = count(flagsCpp.begin(), flagsCpp.end(), true);
        xRet.val.array.columns = 1;
        xRet.val.array.lparray = new OPER[xRet.val.array.rows]; 
        xRet.xltype = xltypeMulti | xlbitDLLFree;

        int idx = 0;
        for (int i=0; i<sizeInput; i++) {
            if (flagsCpp[i]) {
                operToOper(&xRet.val.array.lparray[idx++], &xMulti->val.array.lparray[i]);
            }
        }

        return &xRet;

    } catch (const std::exception &e) {
        ObjectHandler::RepositoryXL::instance().logError(e.what(), functionCall);
        return 0;
    }
}

/*
ohRangeRetrieveError() - This implementation uses a couple of undocumented workarounds for an
undocumented Excel bug.
- ohRangeRetrieveError() requires macro capabilities (#)
- ohRangeRetrieveError() accepts an XLOPER* (range reference) as input
- Macro functions accepting XLOPER* are not recalculated reliably by Excel
- We implement 2 functions, ohRangeRetrieveError() is a non-macro function which uses xlUDF
  to call ohRetrieveErrorImpl() which is a macro function.  This indirection fools Excel
  into allowing ohRangeRetrieveError() to invoke macro privileges.  Thanks to Laurent Longre
  for publishing this technique.
- ohRangeRetrieveError() performs a dummy xlCoerce on the input XLOPER*.  If Excel has called 
  ohRangeRetrieveError() out of sequence, then the xlCoerce will return xlretUncalced, causing
  Excel to automatically re-call ohRangeRetrieveError() after the input range is updated.
*/

XLL_DEC XLOPER *ohRangeRetrieveError(XLOPER *xRange) {
    try {
        XLOPER xTemp;
        Excel(xlCoerce, &xTemp, 1, xRange);
        static XLOPER xRet;
        if (xTemp.xltype & xltypeErr) {
            Excel(xlUDF, &xRet, 2, TempStrNoSize("\x13""ohRetrieveErrorImpl"), xRange);
            xRet.xltype |= xlbitXLFree;
        } else {
            ObjectHandler::scalarToOper(std::string(), xRet);
        }
        Excel(xlFree, 0, 1, &xTemp);
        return &xRet;
    } catch (...) {
        return 0;
    }
}

XLL_DEC char *ohRetrieveErrorImpl(XLOPER *xRange) {
    try {
        std::string returnValue =
            ObjectHandler::RepositoryXL::instance().retrieveError(xRange);
        static char ret[XL_MAX_STR_LEN];
        ObjectHandler::stringToChar(returnValue, ret);
        return ret;
    } catch (...) {
        return 0;
    }
}

// The amount of space left on the stack.  Excel returns this value into an unsigned short.
// Usually the amount of available stack space exceeds USHRT_MAX (65535).  So normally this
// function always returns 65535.
XLL_DEC long *ohStack() {
    static long returnValue;
    XLOPER xRes;
    Excel4(xlStack, &xRes, 0);
    xRes.xltype = xltypeNum;
    returnValue = static_cast<unsigned short>(xRes.val.w);
    return &returnValue;
}
