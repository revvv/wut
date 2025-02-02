#pragma once
#include <wut.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAMERA_STREAM_WIDTH  640
#define CAMERA_STREAM_PITCH  768
#define CAMERA_STREAM_HEIGHT 480

#define CAMERA_Y_BUFFER_SIZE  (CAMERA_STREAM_PITCH * CAMERA_STREAM_HEIGHT)
#define CAMERA_UV_BUFFER_SIZE (CAMERA_STREAM_PITCH * CAMERA_STREAM_HEIGHT / 2)
#define CAMERA_YUV_BUFFER_SIZE (CAMERA_Y_BUFFER_SIZE + CAMERA_UV_BUFFER_SIZE)
#define CAMERA_YUV_BUFFER_ALIGNMENT 256

typedef int CAMHandle;

typedef struct CAMMode CAMMode;
typedef struct CAMStateInfo CAMStateInfo;
typedef struct CAMEventHandlerInput CAMEventHandlerInput;
typedef struct CAMWorkMem CAMWorkMem;
typedef struct CAMStreamInfo CAMStreamInfo;
typedef struct CAMSetupInfo CAMSetupInfo;
typedef struct CAMSurface CAMSurface;

typedef void (*CAMEventHandler)(CAMEventHandlerInput* eventHandlerInput);

typedef enum OSCoreId
{
   OS_CORE_0 = 0,
   OS_CORE_1 = 1,
   OS_CORE_2 = 2,

   OS_NUM_CORES = 3
} OSCoreId;

typedef enum OSCoreAffinity {
    OS_CORE_AFFINITY_0 = 1 << OS_CORE_0,
    OS_CORE_AFFINITY_1 = 1 << OS_CORE_1,
    OS_CORE_AFFINITY_2 = 1 << OS_CORE_2
} OSCoreAffinity;

typedef enum CAMError {
    CAMERA_ERROR_NONE                = 0,
    CAMERA_ERROR_INVALID_ARG         = -1,
    CAMERA_ERROR_INVALID_HANDLE      = -2,
    CAMERA_ERROR_INVALID_STATE       = -3,
    CAMERA_ERROR_QUEUE_FULL          = -4,
    CAMERA_ERROR_INSUFFICIENT_MEMORY = -5,
    CAMERA_ERROR_NOT_READY           = -6,
    CAMERA_ERROR_FSA_FAILURE         = -7,
    CAMERA_ERROR_UNIINITIALIZED      = -8,
    CAMERA_ERROR_DEVICE_NOT_READY    = -9,
    CAMERA_ERROR_UNKNOWN             = -10,
    CAMERA_ERROR_NOT_IMPLEMENTED     = -11,
    CAMERA_ERROR_DEVICE_IN_USE       = -12,
    CAMERA_ERROR_UVD_FAILURE         = -13,
    CAMERA_ERROR_NOT_SUPPORTED       = -14,
    CAMERA_ERROR_SEGMENT_VIOLATION   = -15
} CAMError;

typedef enum CAMState {
    CAM_STATE_DEVICE_POWER_MODE            = 0,
    CAM_STATE_AUTO_EXPOSURE_MODE           = 1,
    CAM_STATE_AUTO_EXPOSURE_TIME_ABSOLUTE  = 2,
    CAM_STATE_BRIGHTNESS                   = 3,
    CAM_STATE_CONTRAST                     = 4,
    CAM_STATE_GAIN                         = 5,
    CAM_STATE_POWER_LINE_FREQUENCY         = 6,
    CAM_STATE_HUE                          = 7,
    CAM_STATE_SATURATION                   = 8,
    CAM_STATE_SHARPNESS                    = 9,
    CAM_STATE_GAMMA                        = 10,
    CAM_STATE_DIGITAL_MULTIPLIER_STEP      = 11,
    CAM_STATE_DIGITAL_MULTIPLIER_LIMIT     = 12,
    CAM_STATE_WHITE_BALANCE_COMPONENT      = 13,
    CAM_STATE_WHITE_BALANCE_COMPONENT_AUTO = 14,
    CAM_STATE_DRC_CONNECTION_STATE         = 15,
    CAM_STATE_GENERATE_KEY_FRAME           = 16
} CAMState;

typedef enum CAMInstance {
    CAM_INSTANCE_0 = 0
} CAMInstance;

typedef enum CAMPixelFormat {
    CAM_FORMAT_NV12 = 0
} CAMPixelFormat;

typedef enum CAMTileMode {
    CAM_TILE_LINEAR = 0
} CAMTileMode;

typedef enum CAMEventType {
    CAM_EVENT_DECODE_DONE = 0,
    CAM_EVENT_DRC_DETACH  = 1
} CAMEventType;

typedef enum CAMDisplayMode {
    CAM_DISPLAY_MODE_DOWNSTREAM     = 0,
    CAM_DISPLAY_MODE_LOCAL_LOOPBACK = 1
} CAMDisplayMode;

typedef enum CAMFrameRate {
    CAM_FRAME_RATE_15 = 0,
    CAM_FRAME_RATE_30 = 1
} CAMFrameRate;

typedef enum CAMPowerLineFrequency {
    CAM_POWER_LINE_FREQUENCY_DISABLED = 0,
    CAM_POWER_LINE_FREQUENCY_50HZ     = 1,
    CAM_POWER_LINE_FREQUENCY_60HZ     = 2
} CAMPowerLineFrequency;

typedef enum CAMStreamType {
    CAM_STREAM_VIDEO = 0
} CAMStreamType;

struct CAMMode {
    CAMDisplayMode display;
    CAMFrameRate   fps;
};

struct CAMStateInfo {
    int max;
    int min;
    int res;
};

struct CAMEventHandlerInput {
    CAMEventType type;
    uint32_t          data0;
    uint32_t          data1;
    uint32_t          data2;
};

struct CAMWorkMem {
    int   size;
    void* mem;
};

struct CAMStreamInfo {
    CAMStreamType type;
    int           height;
    int           width;
};

struct CAMSetupInfo {
    CAMStreamInfo   streamInfo;
    CAMWorkMem      workMem;
    CAMEventHandler eventHandler;
    CAMMode         mode;
    OSCoreAffinity  threadAffinity;
    int             reserve[4];
};

struct CAMSurface {
    int            imageSize;
    void*          imagePtr;
    int            height;
    int            width;
    int            pitch;
    int            alignment;
    CAMTileMode    tileMode;
    CAMPixelFormat pixelFormat;
};

CAMHandle CAMInit(CAMInstance instance, CAMSetupInfo* setup, CAMError* err);
void CAMExit(CAMHandle cam);
CAMError CAMOpen(CAMHandle cam);
CAMError CAMClose(CAMHandle cam);
CAMError CAMGetMemReq(CAMStreamInfo* streamInfo);
CAMError CAMSubmitTargetSurface(CAMHandle cam, CAMSurface* surface);
CAMError CAMGetState(CAMHandle cam, CAMState state, int* val);
CAMError CAMGetStateInfo(CAMHandle cam, CAMState state, CAMStateInfo* info);
CAMError CAMSetState(CAMHandle cam, CAMState state, int* val);
CAMError CAMCheckMemSegmentation(void* mem, unsigned int size);

#ifdef __cplusplus
}
#endif
