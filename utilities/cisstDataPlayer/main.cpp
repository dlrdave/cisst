/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */
/*
  $Id$
  
  Author(s): Marcin Balicki
  Created on: 2011-02-10

  (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstMultiTask.h>
#include <cisstCommon.h>
#include <cisstOSAbstraction.h>
#include <QApplication>
#include "cdpPlayerExample.h"
#include "cdpPlayerManager.h"
#include "cdpPlayerVideo.h"


int main(int argc, char *argv[])
{
   // log configuration
    cmnLogger::SetMask(CMN_LOG_ALLOW_ALL);
    cmnLogger::SetMaskDefaultLog(CMN_LOG_ALLOW_ALL);
    cmnLogger::AddChannel(std::cout, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);

    // create our components
    mtsComponentManager * componentManager;
    componentManager = mtsManagerLocal::GetInstance();

    QApplication application(argc, argv);
    svlInitialize();
    cdpPlayerManager * playerManager = new cdpPlayerManager("PlayerManager", 1.0 * cmn_ms);
    cdpPlayerExample * player1 = new cdpPlayerExample("Player1", 1.0 * cmn_ms);
    cdpPlayerVideo   * vplayer = new cdpPlayerVideo("VideoPlayer", 1.0 * cmn_ms);
    cdpPlayerVideo   * vplayer2 = new cdpPlayerVideo("VideoPlayer2", 1.0 * cmn_ms);


    componentManager->AddComponent(playerManager);
    componentManager->AddComponent(player1);
    componentManager->AddComponent(vplayer);
    componentManager->AddComponent(vplayer2);


    // create the components, i.e. find the commands
    componentManager->CreateAll();
    componentManager->WaitForStateAll(mtsComponentState::READY);

    // start the periodic Run
    componentManager->StartAll();
    componentManager->WaitForStateAll(mtsComponentState::ACTIVE);

    playerManager->AddPlayer(player1);
    playerManager->AddPlayer(vplayer);
    playerManager->AddPlayer(vplayer2);

    playerManager->Configure();
    player1->Configure();

    //these should be configured after the pipeline is started.
    vplayer->Configure();
    vplayer2->Configure();

    application.setStyle("Plastique");
    application.exec();

    componentManager->KillAll();
    componentManager->WaitForStateAll(mtsComponentState::FINISHED, 2.0 * cmn_s);

    componentManager->Cleanup();

    return 0;
}