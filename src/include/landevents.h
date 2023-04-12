#pragma once

#include "imagesupport.h"

#define MaxLandEventTypeNumber (4)  // 最大场地事件编号。
#define MaxLandEventNameLength (16) // 场地事件名字的最大长度。

typedef enum LandEventType {
  None,
  Lock,
  Damage,
  DamageOT,
  HealOT
} LandEventType; // 场地事件编号定义。

typedef struct LandEventData {
  char name[MaxLandEventNameLength + 1];
  int permanent;
  int defaultArg;
  HBITMAP sprite;
} LandEventData; // 场地事件数据。

typedef struct LandEvent {
  int type;
  int arg;
} LandEvent; // 具体的场地事件。

extern LandEventData
    landEventsData[MaxLandEventTypeNumber + 1]; // 场地事件图鉴。

void initLandEvents();