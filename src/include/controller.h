#pragma once

#define MOVEGAP (150) // 长按移动键时，每隔多少毫秒移动一次。
#define USEMOVEGAP (500) // 长按招式键时，每隔多少毫秒使用一次招式。并未采用。

extern int controlPressed; // 是否按下了 Control。
extern int shiftPressed;   // 是否按下了 Shift。

void ScreenRender();

void bindPlayerMove(void (*_playerMove)(int));
void bindAutoMove(void (*_autoMove)(void));

void clearTimers();

void render(int id);

void setPauseBuffer();

void controlKeyboard(int key, int event);