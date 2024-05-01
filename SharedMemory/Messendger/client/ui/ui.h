#ifndef UI_H_
#define UI_H_

#include <curses.h>
#include "../../custom_type.h"

void MessageWindow(WINDOW *wnd, MessageStorage *msg);
void InputMessageWindow(WINDOW *wnd, Message *msg);
void SigWinch(int signo);
void Register();
void UserWindow(WINDOW *wnd, NameList *list);

#endif //UI_H