/*
 * Copyright 2014 Intuivo, Inc.
 *
 * skype-client-util.h
 * Header file for skype-client-util.c
 *
 * This file is part of nautilus-skype.
 *
 * nautilus-skype is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nautilus-skype is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nautilus-skype.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SKYPE_CLIENT_UTIL_H
#define SKYPE_CLIENT_UTIL_H

#include <glib.h>

G_BEGIN_DECLS

gchar *skype_client_util_sanitize(const gchar *a);
gchar *skype_client_util_desanitize(const gchar *a);

gboolean
skype_client_util_command_parse_arg(const gchar *line, GHashTable *return_table);

G_END_DECLS

#endif
