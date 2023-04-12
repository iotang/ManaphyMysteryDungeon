#pragma once

#define MaxMessageLength (200000) // 消息的总长度最大值。
#define MessageExpireTime (3000)  // 消息会展示几秒。
#define MaxMessageLine (4)        // 一次最多展示消息的数量。

void setMessage(char *s);

void emplaceMessage(char *s);

int isMessageEmpty();

void clearMessage();

void drawMessageDialog();