#ifndef UI_H_
#define UI_H_

#include <curses.h>
#include "../../config.h"

void MessageWindow(WINDOW *wnd, MessageStorage *msg , int y);
void InputMessageWindow(WINDOW *wnd, Message *msg);
void Register(Controller *cont, char *name_user);
void UserWindow(WINDOW *wnd, NameList *list);

#endif //UI_H