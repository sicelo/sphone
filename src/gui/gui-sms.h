/*
 * sphone
 * Copyright (C) Ahmed Abdel-Hamid 2010 <ahmedam@mail.usa.com>
 * 
 * sphone is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * sphone is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GUI_SMS_H_
#define _GUI_SMS_H_
#include <stdbool.h>
#include "types.h"
 
void gtk_gui_sms_init(void);
void gtk_gui_sms_exit(void);
bool gtk_gui_sms_send_show(const MessageProperties *message);
void gui_sms_receive_show(const MessageProperties *message);

#endif
