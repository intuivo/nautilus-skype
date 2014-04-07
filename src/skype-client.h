/*
 * Copyright 2014 Intuivo, Inc.
 *
 * skype-client.h
 * Header file for skype-client.c
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

#ifndef SKYPE_CLIENT_H
#define SKYPE_CLIENT_H

#include <glib.h>
#include "skype-command-client.h"
#include "nautilus-skype-hooks.h"

G_BEGIN_DECLS

typedef struct {
  SkypeCommandClient dcc;
  NautilusSkypeHookserv hookserv;
  GHookList onconnect_hooklist;
  GHookList ondisconnect_hooklist;
  gboolean hook_connect_called;
  gboolean command_connect_called;
  gboolean hook_disconnect_called;
  gboolean command_disconnect_called;
} SkypeClient;

typedef void (*SkypeClientConnectionAttemptHook)(guint, gpointer);
typedef GHookFunc SkypeClientConnectHook;

void
skype_client_setup(SkypeClient *dc);

void
skype_client_start(SkypeClient *dc);

gboolean
skype_client_is_connected(SkypeClient *dc);

void
skype_client_force_reconnect(SkypeClient *dc);

void
skype_client_add_on_connect_hook(SkypeClient *dc,
				   SkypeClientConnectHook dhcch,
				   gpointer ud);

void
skype_client_add_on_disconnect_hook(SkypeClient *dc,
				      SkypeClientConnectHook dhcch,
				      gpointer ud);

void
skype_client_add_connection_attempt_hook(SkypeClient *dc,
					   SkypeClientConnectionAttemptHook dhcch,
					   gpointer ud);

G_END_DECLS

#endif
