#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"

void smInit();
bool smIsStateEmpty();
int smStateCount();
AppState *smStateTop();
void callCtor(AppState *now);
void callProc(AppState *now);
void callDtor(AppState *now);
void smLastProc();
void smTopChanging();
void smRebuildTop();
void smPushState(AppState *state);
void smBarePushState(AppState *state);
void smPopState();
void smBarePopState();
void smPopStateUntil(int target);
void smClearState();