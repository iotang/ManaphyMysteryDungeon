#pragma once

#define MOVEGAP (150)
#define USEMOVEGAP (500)

void ScreenRender();

void bindPlayerMove(void (*_playerMove)(int));
void bindAutoMove(void (*_autoMove)(void));

void clearTimers();

void render(int id);

void setPauseBuffer();

void controlKeyboard(int key, int event);