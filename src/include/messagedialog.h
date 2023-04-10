#pragma once

#define MaxMessageLength (200000)
#define MessageExpireTime (3000)
#define MaxMessageLine (4)

void setMessage(char *s);

void emplaceMessage(char *s);

int isMessageEmpty();

void clearMessage();

void drawMessageDialog();