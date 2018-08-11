#ifndef INTERRUPT_CONFIG_H
#define INTERRUPT_CONFIG_H

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

void InterruptConfig_gpioInterruptEnabler(void);
void InterruptConfig_postButtonTapTimerFunction(void);
void InterruptConfig_gpioInterruptHandlerFunction(void);
void InterruptConfig_setupInterrupt(void);

#endif
