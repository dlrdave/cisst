/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: vidDC1394Source.h,v 1.10 2008/11/07 23:14:54 vagvoba Exp $
  
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

#ifndef _vidDC1394Source_h
#define _vidDC1394Source_h

#include <cisstStereoVision/svlVideoCaptureSource.h>
#include "svlImageBuffer.h"


/*********************************************************************
** LibDC1394                                                        **
**  - compatibility: DC1394 2.0.2                                   **
**  - get it from:                                                  **
**    http://sourceforge.net/project/showfiles.php?group_id=8157    **
*********************************************************************/
#include "dc1394/dc1394.h"

/*******************************************
** Video modes                            **
********************************************
typedef enum {
    DC1394_VIDEO_MODE_160x120_YUV444    = 64,
    DC1394_VIDEO_MODE_320x240_YUV422    = 65,
    DC1394_VIDEO_MODE_640x480_YUV411    = 66,
    DC1394_VIDEO_MODE_640x480_YUV422    = 67,
    DC1394_VIDEO_MODE_640x480_RGB8      = 68,
    DC1394_VIDEO_MODE_640x480_MONO8     = 69,
    DC1394_VIDEO_MODE_640x480_MONO16    = 70,
    DC1394_VIDEO_MODE_800x600_YUV422    = 71,
    DC1394_VIDEO_MODE_800x600_RGB8      = 72,
    DC1394_VIDEO_MODE_800x600_MONO8     = 73,
    DC1394_VIDEO_MODE_1024x768_YUV422   = 74,
    DC1394_VIDEO_MODE_1024x768_RGB8     = 75,
    DC1394_VIDEO_MODE_1024x768_MONO8    = 76,
    DC1394_VIDEO_MODE_800x600_MONO16    = 77,
    DC1394_VIDEO_MODE_1024x768_MONO16   = 78,
    DC1394_VIDEO_MODE_1280x960_YUV422   = 79,
    DC1394_VIDEO_MODE_1280x960_RGB8     = 80,
    DC1394_VIDEO_MODE_1280x960_MONO8    = 81,
    DC1394_VIDEO_MODE_1600x1200_YUV422  = 82,
    DC1394_VIDEO_MODE_1600x1200_RGB8    = 83,
    DC1394_VIDEO_MODE_1600x1200_MONO8   = 84,
    DC1394_VIDEO_MODE_1280x960_MONO16   = 85,
    DC1394_VIDEO_MODE_1600x1200_MONO16  = 86,
    DC1394_VIDEO_MODE_EXIF              = 87,
    DC1394_VIDEO_MODE_FORMAT7_0         = 88,
    DC1394_VIDEO_MODE_FORMAT7_1         = 89,
    DC1394_VIDEO_MODE_FORMAT7_2         = 90,
    DC1394_VIDEO_MODE_FORMAT7_3         = 91,
    DC1394_VIDEO_MODE_FORMAT7_4         = 92,
    DC1394_VIDEO_MODE_FORMAT7_5         = 93,
    DC1394_VIDEO_MODE_FORMAT7_6         = 94,
    DC1394_VIDEO_MODE_FORMAT7_7         = 95
} dc1394video_mode_t;
*******************************************/

/*******************************************
** Framerates                             **
********************************************
typedef enum {
    DC1394_FRAMERATE_1_875 = 32,
    DC1394_FRAMERATE_3_75  = 33,
    DC1394_FRAMERATE_7_5   = 34,
    DC1394_FRAMERATE_15    = 35,
    DC1394_FRAMERATE_30    = 36,
    DC1394_FRAMERATE_60    = 37,
    DC1394_FRAMERATE_120   = 38,
    DC1394_FRAMERATE_240   = 39
} dc1394framerate_t;
*******************************************/


class svlDC1394Context
{
public:
    static svlDC1394Context* Instance();

    dc1394_t* GetContext();
	int GetDeviceList(svlVideoCaptureSource::DeviceInfo **deviceinfo);
	dc1394camera_t** GetCameras();
	unsigned int GetNumberOfCameras();
    dc1394operation_mode_t* GetBestOpMode();
    dc1394speed_t* GetBestISOSpeed();

private:
    svlDC1394Context();
    ~svlDC1394Context();

    void Enumerate();
    int TestIEEE1394Interface(dc1394camera_t* camera, dc1394operation_mode_t opmode, dc1394speed_t isospeed);
    void ReleaseEnumeration();

    dc1394_t* Context;
    dc1394camera_list_t* CameraList;
    dc1394camera_t** Cameras;
    svlVideoCaptureSource::DeviceInfo* DeviceInfos;
    unsigned int NumberOfCameras;
    dc1394operation_mode_t* BestOpMode;
    dc1394speed_t* BestISOSpeed;
};


class CDC1394SourceThread;

class CDC1394Source : public CVideoCaptureSourceBase
{
friend class CDC1394SourceThread;

public:
    CDC1394Source();
	~CDC1394Source();

public:
    svlVideoCaptureSource::PlatformType GetPlatformType();
    int SetStreamCount(unsigned int numofstreams);
	int GetDeviceList(svlVideoCaptureSource::DeviceInfo **deviceinfo);
	int Open();
	void Close();
	int Start();
    svlImageRGB* GetLatestFrame(bool waitfornew, unsigned int videoch = 0);
	int Stop();
	bool IsRunning();
    int SetDevice(int devid, int inid, unsigned int videoch = 0);
	int GetWidth(unsigned int videoch = 0);
	int GetHeight(unsigned int videoch = 0);

    int GetFormatList(unsigned int deviceid, svlVideoCaptureSource::ImageFormat **formatlist);
    int SetFormat(svlVideoCaptureSource::ImageFormat& format, unsigned int videoch = 0);
    int GetFormat(svlVideoCaptureSource::ImageFormat& format, unsigned int videoch = 0);
    int SetImageProperties(svlVideoCaptureSource::ImageProperties& properties, unsigned int videoch = 0);
    int GetImageProperties(svlVideoCaptureSource::ImageProperties& properties, unsigned int videoch = 0);

private:
    unsigned int NumOfStreams;
    bool Initialized;
    bool Running;

    CDC1394SourceThread** CaptureProc;
    osaThread** CaptureThread;

    int* CameraFileNo;
    fd_set* CameraFDSet;
    int* CameraNFDS;

    osaMutex Mutex;
    dc1394_t* Context;
    dc1394camera_list_t* CameraList;
    dc1394camera_t** Cameras;
    unsigned int NumberOfCameras;
    dc1394operation_mode_t* BestOpMode;
    dc1394speed_t* BestISOSpeed;
    int* DeviceID;
    svlVideoCaptureSource::ImageFormat** Format;
    int* Width;
    int* Height;
    unsigned int* ColorCoding;
    dc1394video_frame_t** Frame;
    svlImageBuffer** OutputBuffer;

    int CaptureFrame(unsigned int videoch);

    void Release();
    int GetModeFromFormat(unsigned int width, unsigned int height, svlVideoCaptureSource::PixelType colspc, unsigned int& mode);
    int GetSupportedFrameratesForFormat(unsigned int devid, svlVideoCaptureSource::ImageFormat& format, double **fpslist, unsigned int& listsize);
    int GetFramerateFromFPS(double fps, unsigned int& framerate);
    int GetFormatFromMode(unsigned int mode, svlVideoCaptureSource::ImageFormat& format);
    void SwapRGBBuffer(unsigned char* buffer, const unsigned int numberofpixels);
};


class CDC1394SourceThread
{
public:
    CDC1394SourceThread(int streamid) { StreamID = streamid; InitSuccess = false; }
    ~CDC1394SourceThread() {}
    void* Proc(CDC1394Source* baseref);

    bool WaitForInit() { InitEvent.Wait(); return InitSuccess; }
    bool IsError() { return Error; }

private:
    int StreamID;
    bool Error;
    osaThreadSignal InitEvent;
    bool InitSuccess;
};


#endif // _vidDC1394Source_h

