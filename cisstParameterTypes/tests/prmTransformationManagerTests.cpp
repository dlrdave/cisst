/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  
  Author(s):  Rajesh Kumar
  Created on: 2008-03-03
  
  (C) Copyright 2005-2008 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include "prmTransformationManagerTests.h"

#include <cisstVector/vctTypes.h>

//A simple graph
//  theWorld
//    |
//  offset
//    |
//   -+------+-------+------+--------+
//   suj1    suj2   suj3   suj4     suj5
//    |       |      |      |        |
//   sr1     sr2    cam    ms1      ms2

//some frames
prmTransformationFixed *dummy;
prmTransformationFixed *offset;

prmTransformationFixed *suj1;
prmTransformationFixed *suj2;
prmTransformationFixed *suj3;
prmTransformationFixed *suj4;
prmTransformationFixed *suj5;

prmTransformationFixed *sr1;
prmTransformationFixed *sr2;
prmTransformationFixed *cam;
prmTransformationFixed *ms1;
prmTransformationFixed *ms2;


void prmTransformationManagerTest::setUp(void)
{
    // this is the first test run, so clear the tree to allow multiple runs
    prmTransformationManager::Clear();

	dummy = new prmTransformationFixed("DummyFrame");
	offset = new prmTransformationFixed("OffsetFrame");

	suj1 = new prmTransformationFixed("SetupJoints1");
	suj2 = new prmTransformationFixed("SetupJoints2");
	suj3 = new prmTransformationFixed("SetupJoints3");
	suj4 = new prmTransformationFixed("SetupJoints4");
	suj5 = new prmTransformationFixed("SetupJoints5");

	sr1 = new prmTransformationFixed("SlaveRobot1");
	sr2 = new prmTransformationFixed("SlaveRobot2");
	cam = new prmTransformationFixed("CameraRobot");
	ms1 = new prmTransformationFixed("MasterRobot1");
	ms2 = new prmTransformationFixed("MasterRobot2");

	// test addition to graph
    offset->SetReferenceFrame(&prmTransformationManager::TheWorld);
	
    // set up the rest of the graph
    suj1->SetReferenceFrame("OffsetFrame"); 
    suj2->SetReferenceFrame(offset);
    suj3->SetReferenceFrame(offset); 
    suj4->SetReferenceFrame(offset);
    suj5->SetReferenceFrame(offset);
    
    sr1->SetReferenceFrame(suj1);
	sr2->SetReferenceFrame(suj2);
	cam->SetReferenceFrame(suj3);
	ms1->SetReferenceFrame(suj4);
	ms2->SetReferenceFrame(suj5);

    // fill in some values
    vct3 trans(0.0, 0.0, 0.0);
    vct3 tmp(1.0, 0.0, 0.0);
    tmp.NormalizedSelf();
    vctRot3 rotation(vctAxAnRot3(tmp, cmnPI / 2.0));
	
    ms1->SetTransformation(vctFrm3(rotation,trans));

    rotation = vctRot3(vctAxAnRot3(tmp, cmnPI / 4.0));
	suj4->SetTransformation(vctFrm3(rotation, trans));

    rotation = vctRot3(vctAxAnRot3(tmp, cmnPI / 4.0));
	suj3->SetTransformation(vctFrm3(rotation, trans));

    rotation = vctRot3(vctAxAnRot3(tmp, cmnPI / 2.0));
	cam->SetTransformation(vctFrm3(rotation, trans));
}


void prmTransformationManagerTest::tearDown(void)
{
	//A simple graph
	//  theWorld
	//    |
	//  offset
	//    |
	//   -+------+-------+------+--------+
	//   suj1    suj2   suj3   suj4     suj5
	//    |       |      |      |        |
	//   sr1     sr2    cam    ms1      ms2

	//
	// It's important to delete prmTransformationFixed objects in "leaf first" order
	// since the destructor prmTransformationBase::~prmTransformationBase() calls
	// prmTransformationManager::Detach(this).
	//
	// So this delete list is in the opposite order of the create/SetReferenceFrame
	// call order in setUp...
	//
	delete ms2;
	delete ms1;
	delete cam;
	delete sr2;
	delete sr1;

	delete suj5;
	delete suj4;
	delete suj3;
	delete suj2;
	delete suj1;

	delete offset;
	delete dummy;

	// done with test run, so clear the tree to allow multiple runs
	prmTransformationManager::Clear();
}


void prmTransformationManagerTest::TestAddNode(void)
{
    //This is the first test run, so clear the tree to allow multiple runs
    prmTransformationManager::Clear();

    //test addition to graph
    CPPUNIT_ASSERT(dummy->SetReferenceFrame(&prmTransformationManager::TheWorld));
}


void prmTransformationManagerTest::TestDeleteNode(void)
{
    prmTransformationFixed *test = new prmTransformationFixed("testframe");

    //attach to graph
    test->SetReferenceFrame(&prmTransformationManager::TheWorld);
    
    //this should get rid of the frame
    delete test;   
    //there is no mechanism to test this fully: we will still perform this test to check syntax/symbol definition.
    // ADV this leads to seg fault  ---- CPPUNIT_ASSERT(!prmTransformationManager::FindPath(testPointer, &prmTransformationManager::TheWorld));
}


void prmTransformationManagerTest::TestNullPtr(void)
{
    CPPUNIT_ASSERT(!ms1->SetReferenceFrame(NULL));
    CPPUNIT_ASSERT(!prmTransformationManager::TheWorld.SetReferenceFrame(ms1));
}


void prmTransformationManagerTest::TestPathFind(void)
{
    //can't search with NULL ptrs as usual, so this checks the transformation manager directly 
    CPPUNIT_ASSERT(!prmTransformationManager::FindPath(&prmTransformationManager::TheWorld, NULL));
    CPPUNIT_ASSERT(!prmTransformationManager::FindPath(NULL, &prmTransformationManager::TheWorld));
    CPPUNIT_ASSERT(!prmTransformationManager::FindPath(NULL, &prmTransformationManager::TheWorld));
    CPPUNIT_ASSERT(prmTransformationManager::FindPath(prmTransformationManager::GetTransformationNodePtr("MasterRobot1"), &prmTransformationManager::TheWorld));
    CPPUNIT_ASSERT(prmTransformationManager::GetTransformationNodePtr("BadString") == NULL);
    CPPUNIT_ASSERT(!prmTransformationManager::FindPath(NULL, NULL));
}


void prmTransformationManagerTest::TestDotCreate(void)
{
    std::ofstream dotFile1("example1.dot");
    prmTransformationManager::ToStreamDot(dotFile1);
}


void prmTransformationManagerTest::TestStringNameTreeEval(void)
{
    vctFrm3 transformation;
    transformation = prmWRTReference("MasterRobot1", "CameraRobot");
    CPPUNIT_ASSERT(transformation.AlmostEquivalent(vctFrm3::Identity(),0.01));   
}


void prmTransformationManagerTest::TestTreeEval(void)
{
    vctFrm3 transformation, transformation1;
    transformation = prmWRTReference(ms1, cam);
    CPPUNIT_ASSERT(transformation.AlmostEquivalent(vctFrm3::Identity(),0.01));

    transformation1 = prmWRTReference("MasterRobot1", "CameraRobot");
    CPPUNIT_ASSERT(transformation.AlmostEquivalent(transformation1));   
}
