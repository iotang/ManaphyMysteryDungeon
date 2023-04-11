#include "landevents.h"

#include "utils.h"

LandEventData landEventsData[MaxLandEventTypeNumber + 1];

void initLandEvents() {
  strcpy(landEventsData[None].name, "No Event");
  landEventsData[None].permanent = 0;
  landEventsData[None].defaultArg = 0;

  strcpy(landEventsData[Lock].name, "Lock");
  landEventsData[Lock].permanent = 0;
  landEventsData[Lock].defaultArg = 1;
  landEventsData[Lock].sprite = readBmpImage("assets/landevents/Lock.bmp");

  strcpy(landEventsData[Damage].name, "Damage");
  landEventsData[Damage].permanent = 1;
  landEventsData[Damage].defaultArg = 10;
  landEventsData[Damage].sprite = readBmpImage("assets/landevents/Damage.bmp");

  strcpy(landEventsData[DamageOT].name, "Damage (one-off)");
  landEventsData[DamageOT].permanent = 0;
  landEventsData[DamageOT].defaultArg = 10;
  landEventsData[DamageOT].sprite =
      readBmpImage("assets/landevents/DamageOT.bmp");

  strcpy(landEventsData[HealOT].name, "Heal (one-off)");
  landEventsData[HealOT].permanent = 0;
  landEventsData[HealOT].defaultArg = 10;
  landEventsData[HealOT].sprite = readBmpImage("assets/landevents/HealOT.bmp");
}