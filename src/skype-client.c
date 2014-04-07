/*
 * Copyright 2014 Intuivo, Inc.
 *
 * skype-client.c
 * Implements connection handling and C interface for interfacing with the Skype daemon.
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

#include <glib.h>

#include "g-util.h"
#include "skype-command-client.h"
#include "nautilus-skype-hooks.h"
#include "skype-client.h"

static void
hook_on_connect(SkypeClient *dc) {
  dc->hook_connect_called = TRUE;
  
  if (dc->command_connect_called) {
    debug("client connection");
    g_hook_list_invoke(&(dc->onconnect_hooklist), FALSE);
    /* reset flags */
    dc->hook_connect_called = dc->command_connect_called = FALSE;
  }
}

static void
command_on_connect(SkypeClient *dc) {
  dc->command_connect_called = TRUE;
  
  if (dc->hook_connect_called) {
    debug("client connection");
    g_hook_list_invoke(&(dc->onconnect_hooklist), FALSE);
    /* reset flags */
    dc->hook_connect_called = dc->command_connect_called = FALSE;
  }
}

static void
command_on_disconnect(SkypeClient *dc) {
  dc->command_disconnect_called = TRUE;
  
  if (dc->hook_disconnect_called) {
    debug("client disconnect");
    g_hook_list_invoke(&(dc->ondisconnect_hooklist), FALSE);
    /* reset flags */
    dc->hook_disconnect_called = dc->command_disconnect_called = FALSE;
  }
  else {
    nautilus_skype_hooks_force_reconnect(&(dc->hookserv));
  }
}

static void
hook_on_disconnect(SkypeClient *dc) {
  dc->hook_disconnect_called = TRUE;
  
  if (dc->command_disconnect_called) {
    debug("client disconnect");
    g_hook_list_invoke(&(dc->ondisconnect_hooklist), FALSE);
    /* reset flags */
    dc->hook_disconnect_called = dc->command_disconnect_called = FALSE;
  }
  else {
    skype_command_client_force_reconnect(&(dc->dcc));
  }
}

gboolean
skype_client_is_connected(SkypeClient *dc) {
  return (skype_command_client_is_connected(&(dc->dcc)) &&
	  nautilus_skype_hooks_is_connected(&(dc->hookserv)));
}

void
skype_client_force_reconnect(SkypeClient *dc) {
  if (skype_client_is_connected(dc) == TRUE) {
    debug("forcing client to reconnect");
    skype_command_client_force_reconnect(&(dc->dcc));
    nautilus_skype_hooks_force_reconnect(&(dc->hookserv));
  }
}

/* should only be called once on initialization */
void
skype_client_setup(SkypeClient *dc) {
  nautilus_skype_hooks_setup(&(dc->hookserv));
  skype_command_client_setup(&(dc->dcc));

  g_hook_list_init(&(dc->ondisconnect_hooklist), sizeof(GHook));
  g_hook_list_init(&(dc->onconnect_hooklist), sizeof(GHook));

  dc->hook_disconnect_called = dc->command_disconnect_called = FALSE;
  dc->hook_connect_called = dc->command_connect_called = FALSE;

  nautilus_skype_hooks_add_on_connect_hook(&(dc->hookserv), 
					     (SkypeHookClientConnectHook)
					     hook_on_connect, dc);
  
  skype_command_client_add_on_connect_hook(&(dc->dcc),
					     (SkypeCommandClientConnectHook)
					     command_on_connect, dc);
  
  nautilus_skype_hooks_add_on_disconnect_hook(&(dc->hookserv), 
						(SkypeHookClientConnectHook)
						hook_on_disconnect, dc);
  
  skype_command_client_add_on_disconnect_hook(&(dc->dcc),
						(SkypeCommandClientConnectHook)
						command_on_disconnect, dc);
}

/* not thread safe */
void
skype_client_add_on_disconnect_hook(SkypeClient *dc,
				      SkypeClientConnectHook dhcch,
				      gpointer ud) {
  GHook *newhook;
  
  newhook = g_hook_alloc(&(dc->ondisconnect_hooklist));
  newhook->func = dhcch;
  newhook->data = ud;
  
  g_hook_append(&(dc->ondisconnect_hooklist), newhook);
}

/* not thread safe */
void
skype_client_add_on_connect_hook(SkypeClient *dc,
				   SkypeClientConnectHook dhcch,
				   gpointer ud) {
  GHook *newhook;
  
  newhook = g_hook_alloc(&(dc->onconnect_hooklist));
  newhook->func = dhcch;
  newhook->data = ud;
  
  g_hook_append(&(dc->onconnect_hooklist), newhook);
}

/* not thread safe */
void
skype_client_add_connection_attempt_hook(SkypeClient *dc,
					   SkypeClientConnectionAttemptHook dhcch,
					   gpointer ud) {
  debug("shouldn't be here...");

  skype_command_client_add_connection_attempt_hook(&(dc->dcc),
						     dhcch, ud);
}

/* should only be called once on initialization */
void
skype_client_start(SkypeClient *dc) {
  debug("starting connections");
  nautilus_skype_hooks_start(&(dc->hookserv));
  skype_command_client_start(&(dc->dcc));
}
