#pragma once

#include "imagesupport.h"

#define MaxLandEventTypeNumber (4)
#define MaxLandEventNameLength (16)

typedef enum LandEventType {
  None,
  Lock,
  Damage,
  DamageOT,
  HealOT
} LandEventType;

typedef struct LandEventData {
  char name[MaxLandEventNameLength + 1];
  int permanent;
  int defaultArg;
  HBITMAP sprite;
} LandEventData;

typedef struct LandEvent {
  int type;
  int arg;
} LandEvent;

extern LandEventData landEventsData[MaxLandEventTypeNumber + 1];

void initLandEvents();