/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2013-07-13

  (C) Copyright 2013 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstMultiTask/mtsQtWidgetIntervalStatistics.h>


mtsQtWidgetIntervalStatistics::mtsQtWidgetIntervalStatistics(void)
{
    // create a layout for the widgets
    MainLayout = new QGridLayout(this);
    MainLayout->setRowStretch(0, 1);
    MainLayout->setColumnStretch(1, 1);

    QLAverage = new QLabel("Avg: n/a");
    MainLayout->addWidget(QLAverage, 0, 0);
    QLStdDev = new QLabel("Dev: n/a");
    MainLayout->addWidget(QLStdDev, 1, 0);
    QLMin = new QLabel("Min: n/a");
    MainLayout->addWidget(QLMin, 2, 0);
    QLMax = new QLabel("Max: n/a");
    MainLayout->addWidget(QLMax, 3, 0);
}

void mtsQtWidgetIntervalStatistics::SetValue(const mtsIntervalStatistics & newValue)
{
    QLAverage->setText(QString("Avg: ") + QString::number(newValue.GetAvg() * 1000.0));
    QLStdDev->setText(QString("Dev: ") + QString::number(newValue.GetStdDev() * 1000.0));
    QLMin->setText(QString("Min: ") + QString::number(newValue.GetMin() * 1000.0));
    QLMax->setText(QString("Max: ") + QString::number(newValue.GetMax() * 1000.0));
}