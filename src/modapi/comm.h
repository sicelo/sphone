/*
 * comm.h
 * Copyright (C) Carl Philipp Klemm 2021 <carl@uvos.xyz>
 * 
 * comm.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * comm.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <glib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	BACKEND_FLAG_MESSAGE = 1,
	BACKEND_FLAG_CALL = 1<<1,
	BACKEND_FLAG_CELLULAR = 1<<2
} BackendFlag;

typedef struct _Scheme {
	char* scheme;
	BackendFlag flags;
} Scheme;

typedef struct  _CommBackend {
	char* name;
	Scheme** schemes;
	BackendFlag flags;
	int id;
} CommBackend;

int sphone_comm_add_backend(const char* name, const Scheme** schemes, BackendFlag flags);

void sphone_comm_remove_backend(int id);

GSList *sphone_comm_get_backends(void);

CommBackend *sphone_comm_default_backend(void);

CommBackend *sphone_comm_get_backend(int id);

CommBackend *sphone_comm_get_backend_for_scheme(const char* scheme, BackendFlag requiredFlags);

bool sphone_comm_set_default_backend(int id);

int sphone_comm_find_backend_id(const char* name);

#ifdef __cplusplus
}
#endif
