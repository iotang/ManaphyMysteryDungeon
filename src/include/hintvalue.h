#pragma once

#define MaxHintLength (200000)
#define HintExpireTime (3000)
#define MaxHintLine (4)

void setHint(char *s);

void emplaceHint(char *s);

int isHintEmpty();

void clearHint();

void drawHintDialog();