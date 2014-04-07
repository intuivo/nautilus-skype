/*
 * Copyright 2014 Intuivo, Inc.
 *
 * nautilus-skype-hooks.h
 * Header file for nautilus-skype-hooks.c
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

#ifndef NAUTILUS_SKYPE_HOOKS_H
#define NAUTILUS_SKYPE_HOOKS_H

#include <glib.h>

G_BEGIN_DECLS

typedef void (*SkypeUpdateHook)(GHashTable *, gpointer);
typedef void (*SkypeHookClientConnectHook)(gpointer);

typedef struct {
  GIOChannel *chan;
  int socket;
  struct {
    int line;
    gchar *command_name;
    GHashTable *command_args;
    int numargs;
  } hhsi;
  gboolean connected;
  guint event_source;
  GHashTable *dispatch_table;
  GHookList ondisconnect_hooklist;
  GHookList onconnect_hooklist;
} NautilusSkypeHookserv;

void
nautilus_skype_hooks_setup(NautilusSkypeHookserv *);

void
nautilus_skype_hooks_start(NautilusSkypeHookserv *);

gboolean
nautilus_skype_hooks_is_connected(NautilusSkypeHookserv *);

gboolean
nautilus_skype_hooks_force_reconnect(NautilusSkypeHookserv *);

void
nautilus_skype_hooks_add(NautilusSkypeHookserv *ndhs,
			   const gchar *hook_name,
			   SkypeUpdateHook hook, gpointer ud);
void
nautilus_skype_hooks_add_on_disconnect_hook(NautilusSkypeHookserv *hookserv,
					      SkypeHookClientConnectHook dhcch,
					      gpointer ud);

void
nautilus_skype_hooks_add_on_connect_hook(NautilusSkypeHookserv *hookserv,
					   SkypeHookClientConnectHook dhcch,
					   gpointer ud);


G_END_DECLS

#endif
