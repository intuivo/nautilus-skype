/*
 * Copyright 2014 Intuivo, Inc.
 *
 * nautilus-skype.h
 * Header file for nautilus-skype.c
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

#ifndef NAUTILUS_SKYPE_H
#define NAUTILUS_SKYPE_H

#include <glib.h>
#include <glib-object.h>

#include <libnautilus-extension/nautilus-info-provider.h>

#include "skype-command-client.h"
#include "nautilus-skype-hooks.h"
#include "skype-client.h"

G_BEGIN_DECLS

/* Declarations for the skype extension object.  This object will be
 * instantiated by nautilus.  It implements the GInterfaces 
 * exported by libnautilus. */

#define NAUTILUS_TYPE_SKYPE	  (nautilus_skype_get_type ())
#define NAUTILUS_SKYPE(o)	  (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_TYPE_SKYPE, NautilusSkype))
#define NAUTILUS_IS_SKYPE(o)	  (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_TYPE_SKYPE))
typedef struct _NautilusSkype      NautilusSkype;
typedef struct _NautilusSkypeClass NautilusSkypeClass;

struct _NautilusSkype {
  GObject parent_slot;
  GHashTable *filename2obj;
  GHashTable *obj2filename;
  GMutex *emblem_paths_mutex;
  GHashTable *emblem_paths;
  SkypeClient dc;
};

struct _NautilusSkypeClass {
	GObjectClass parent_slot;
};

GType nautilus_skype_get_type(void);
void  nautilus_skype_register_type(GTypeModule *module);

extern gboolean skype_use_nautilus_submenu_workaround;
extern gboolean skype_use_operation_in_progress_workaround;

G_END_DECLS

#endif
