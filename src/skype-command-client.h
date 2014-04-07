/*
 * Copyright 2014 Intuivo, Inc.
 *
 * skype-command-client.h
 * Header file for nautilus-skype-command.c
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

#ifndef SKYPE_COMMAND_CLIENT_H
#define SKYPE_COMMAND_CLIENT_H

#include <libnautilus-extension/nautilus-info-provider.h>
#include <libnautilus-extension/nautilus-file-info.h>

G_BEGIN_DECLS

/* command structs */
typedef enum {GET_FILE_INFO, GENERAL_COMMAND} NautilusSkypeRequestType;

typedef struct {
  NautilusSkypeRequestType request_type;
} SkypeCommand;

typedef struct {
  SkypeCommand dc;
  NautilusInfoProvider *provider;
  GClosure *update_complete;
  NautilusFileInfo *file;
  gboolean cancelled;
} SkypeFileInfoCommand;

typedef struct {
  SkypeFileInfoCommand *dfic;
  GHashTable *file_status_response;
  GHashTable *folder_tag_response;
  GHashTable *emblems_response;
} SkypeFileInfoCommandResponse;

typedef void (*NautilusSkypeCommandResponseHandler)(GHashTable *, gpointer);

typedef struct {
  SkypeCommand dc;
  gchar *command_name;
  GHashTable *command_args;
  NautilusSkypeCommandResponseHandler handler;
  gpointer handler_ud;
} SkypeGeneralCommand;

typedef void (*SkypeCommandClientConnectionAttemptHook)(guint, gpointer);
typedef GHookFunc SkypeCommandClientConnectHook;

typedef struct {
  GMutex *command_connected_mutex;
  gboolean command_connected;
  GAsyncQueue *command_queue; 
  GList *ca_hooklist;
  GHookList onconnect_hooklist;
  GHookList ondisconnect_hooklist;
} SkypeCommandClient;

gboolean skype_command_client_is_connected(SkypeCommandClient *dcc);

void skype_command_client_force_reconnect(SkypeCommandClient *dcc);

void
skype_command_client_request(SkypeCommandClient *dcc, SkypeCommand *dc);

void
skype_command_client_setup(SkypeCommandClient *dcc);

void
skype_command_client_start(SkypeCommandClient *dcc);

void skype_command_client_send_simple_command(SkypeCommandClient *dcc,
						const char *command);

void skype_command_client_send_command(SkypeCommandClient *dcc, 
					 NautilusSkypeCommandResponseHandler h,
					 gpointer ud,
					 const char *command, ...);
void
skype_command_client_add_on_connect_hook(SkypeCommandClient *dcc,
					   SkypeCommandClientConnectHook dhcch,
					   gpointer ud);

void
skype_command_client_add_on_disconnect_hook(SkypeCommandClient *dcc,
					      SkypeCommandClientConnectHook dhcch,
					      gpointer ud);

void
skype_command_client_add_connection_attempt_hook(SkypeCommandClient *dcc,
						   SkypeCommandClientConnectionAttemptHook dhcch,
						   gpointer ud);

G_END_DECLS

#endif
