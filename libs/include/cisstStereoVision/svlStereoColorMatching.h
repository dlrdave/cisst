/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: svlStereoColorMatching.h,v 1.5 2008/10/17 17:44:38 vagvoba Exp $
  
  Author(s):  Balazs Vagvolgyi
  Created on: 2006 

  (C) Copyright 2006-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#ifndef _svlStereoColorMatching_h
#define _svlStereoColorMatching_h

#include <cisstStereoVision/svlStreamManager.h>

// Always include last!
#include <cisstStereoVision/svlExport.h>

class CISST_EXPORT svlStereoColorMatching : public svlFilterBase
{
public:
    svlStereoColorMatching();
    virtual ~svlStereoColorMatching();

    int SetRegionOfInterest(svlRect roi);
    int SetRegionOfInterest(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom);

    void RecomputeColorBalance(bool always = false);

protected:
    virtual int Initialize(svlSample* inputdata = 0);
    virtual int ProcessFrame(ProcInfo* procInfo, svlSample* inputdata = 0);
    virtual int Release();

private:
    svlRect ROI_Target;
    svlRect ROI_Actual;
    int ColBal_Red;
    int ColBal_Green;
    int ColBal_Blue;
    int RecomputeRatios;
};

#endif // _svlStereoColorMatching_h

