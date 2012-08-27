/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2012-07-09

  (C) Copyright 2012 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include "vctDataFunctionsFixedSizeMatrixTest.h"

#include <cisstVector/vctFixedSizeMatrix.h>
#include <cisstVector/vctDataFunctionsFixedSizeMatrix.h>
#include <cisstVector/vctRandomFixedSizeMatrix.h>

#include <cisstVector/vctDynamicMatrix.h>
#include <cisstVector/vctDataFunctionsDynamicMatrix.h>
#include <cisstVector/vctRandomDynamicMatrix.h>

void vctDataFunctionsFixedSizeMatrixTest::TestDataCopy(void)
{
    vctFixedSizeMatrix<double, 7, 3> source, destination;
    vctRandom(source, -1.0, 1.0);
    cmnDataCopy(destination, source);
    CPPUNIT_ASSERT(source.Equal(destination));
}


void vctDataFunctionsFixedSizeMatrixTest::TestBinarySerializationStream(void)
{
    cmnDataFormat local, remote;
    std::stringstream stream;
    vctFixedSizeMatrix<double, 6, 3> m1, m2, mReference;
    vctRandom(mReference, -10.0, 10.0);
    m1 = mReference;
    cmnDataSerializeBinary(stream, m1);
    m1.SetAll(0);
    cmnDataDeSerializeBinary(stream, m2, remote, local);
    CPPUNIT_ASSERT_EQUAL(mReference, m2);
}


void vctDataFunctionsFixedSizeMatrixTest::TestScalar(void)
{
    vctFixedSizeMatrix<int, 3, 6> mInt;
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3 * 6), cmnDataScalarNumber(mInt));
    CPPUNIT_ASSERT_EQUAL(true, cmnDataScalarNumberIsFixed(mInt));

    vctFixedSizeMatrix<vctFixedSizeMatrix<double, 3, 4>, 2, 3> mmDouble;
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3 * 4 * 2 * 3), cmnDataScalarNumber(mmDouble));
    CPPUNIT_ASSERT_EQUAL(true, cmnDataScalarNumberIsFixed(mmDouble));

    vctFixedSizeMatrix<std::string, 3, 2> mString;
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), cmnDataScalarNumber(mString));
    CPPUNIT_ASSERT_EQUAL(true, cmnDataScalarNumberIsFixed(mString));

    vctFixedSizeMatrix<vctDynamicMatrix<double>, 4, 2> mmMixed;
    size_t numberOfScalarsExpected = 0;
    for (size_t i = 0; i < mmMixed.rows(); i++) {
        for (size_t j = 0; j < mmMixed.cols(); j++) {
            mmMixed.Element(i, j).SetSize(i + 1, j + 1); // + 1 to make sure we don't have a 0 size matrix
            numberOfScalarsExpected += ((i + 1) * (j + 1));
        }
    }
    CPPUNIT_ASSERT_EQUAL(numberOfScalarsExpected, cmnDataScalarNumber(mmMixed));
    CPPUNIT_ASSERT_EQUAL(false, cmnDataScalarNumberIsFixed(mmMixed));

    size_t row, col, subRow, subCol, position;
    bool exceptionReceived = false;
    std::string description;

    // exception expected if index too high
    try {
        description = cmnDataScalarDescription(mInt, cmnDataScalarNumber(mInt) + 1);
    } catch (std::out_of_range) {
        exceptionReceived = true;
    }
    CPPUNIT_ASSERT(exceptionReceived);

    exceptionReceived = false;
    try {
        description = cmnDataScalarDescription(mmDouble, cmnDataScalarNumber(mmDouble) + 1);
    } catch (std::out_of_range) {
        exceptionReceived = true;
    }
    CPPUNIT_ASSERT(exceptionReceived);

    // exception expected for any index
    exceptionReceived = false;
    try {
        description = cmnDataScalarDescription(mString, cmnDataScalarNumber(mString));
    } catch (std::out_of_range) {
        exceptionReceived = true;
    }
    CPPUNIT_ASSERT(exceptionReceived);

    // get scalar
    position = 0;
    for (row = 0; row < mInt.rows(); ++row) {
        for (col = 0; col < mInt.cols(); ++col) {
            mInt.Element(row, col) = row * 10 + col;
            CPPUNIT_ASSERT_EQUAL(static_cast<double>(row * 10 + col), cmnDataScalar(mInt, position));
            position++;
        }
    }

    position = 0;
    for (row = 0; row < mmDouble.rows(); ++row) {
        for (col = 0; col < mmDouble.cols(); ++col) {
            for (subRow = 0; subRow < mmDouble.Element(row, col).rows(); ++subRow) {
                for (subCol = 0; subCol < mmDouble.Element(row, col).cols(); ++subCol) {
                    mmDouble.Element(row, col).Element(subRow, subCol) = row * col * 100 + subRow * subCol;
                    CPPUNIT_ASSERT_EQUAL(static_cast<double>(row * col * 100 + subRow * subCol),
                                         cmnDataScalar(mmDouble, position));
                    position++;
                }
            }
        }
    }

    // try with fixed size matrix of dynamic matrices
    position = 0;
    for (row = 0; row < mmMixed.rows(); ++row) {
        for (col = 0; col < mmMixed.cols(); ++col) {
            for (subRow = 0; subRow < mmMixed.Element(row, col).rows(); ++subRow) {
                for (subCol = 0; subCol < mmMixed.Element(row, col).cols(); ++subCol) {
                    mmMixed.Element(row, col).Element(subRow, subCol) = row * col * 100 + subRow * subCol;
                    CPPUNIT_ASSERT_EQUAL(static_cast<double>(row * col * 100 + subRow * subCol),
                                         cmnDataScalar(mmMixed, position));
                    position++;
                }
            }
        }
    }
}


CPPUNIT_TEST_SUITE_REGISTRATION(vctDataFunctionsFixedSizeMatrixTest);