#pragma once

#include "utils.h"

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
} LandEventData;

typedef struct LandEvent {
  int type;
  int arg;
} LandEvent;

LandEventData landEventsData[MaxLandEventTypeNumber + 1];

void initLandEvents() {
  strcpy(landEventsData[None].name, "No Event");
  landEventsData[None].permanent = 0;
  landEventsData[None].defaultArg = 0;

  strcpy(landEventsData[Lock].name, "Lock");
  landEventsData[Lock].permanent = 0;
  landEventsData[Lock].defaultArg = 1;

  strcpy(landEventsData[Damage].name, "Damage");
  landEventsData[Damage].permanent = 1;
  landEventsData[Damage].defaultArg = 10;

  strcpy(landEventsData[DamageOT].name, "Damage (one-off)");
  landEventsData[DamageOT].permanent = 0;
  landEventsData[DamageOT].defaultArg = 10;

  strcpy(landEventsData[HealOT].name, "Heal (one-off)");
  landEventsData[HealOT].permanent = 0;
  landEventsData[HealOT].defaultArg = 10;
}