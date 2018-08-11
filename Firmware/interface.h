#ifndef INTERFACE_H
#define INTERFACE_H
/*  Copyright (C) 2018 Shrinath Nimare
    This file is part of CheatESP
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "oled_functions.h"
#define SHORT_PRESS 0
#define LONG_PRESS 1

extern MenuStruct GlobalMenuStruct;

void Interface_next(void);
void Interface_select(void);
void Interface_screenTimeoutFunction(void);
void Interface_buttonPressHandler(uint8);

#endif
