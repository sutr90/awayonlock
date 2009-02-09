/*
 awayonlock - plugin to set away status on screensaver activation
 Copyright (C) 2009  Leo Antunes <leo@costela.net>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc.

*/

// purple
#include <debug.h>
#include <plugin.h>
#include <version.h>
#include <savedstatuses.h>

#include "i18n.h"
#include "callback.h"

static PurpleSavedStatus *status_saved = NULL;

void awayonlock_idle_changed_callback(DBusGProxy *proxy, gboolean screensaver_status, gpointer data) {
	purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("got message from screensaver: active=%u\n"), screensaver_status);

	PurpleSavedStatus *status_idle = purple_savedstatus_get_idleaway();

	PurpleSavedStatus *status_current = purple_savedstatus_get_current();
	PurpleStatusPrimitive status_type = purple_savedstatus_get_type(status_current);

	if(screensaver_status && (status_type != PURPLE_STATUS_OFFLINE && status_type != PURPLE_STATUS_INVISIBLE) && ! purple_savedstatus_is_idleaway()) {
		status_saved = status_current;
		purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("setting status as away and storing '%s'\n"), purple_savedstatus_get_title(status_saved));
		purple_savedstatus_activate(status_idle);
	}
	else if (!screensaver_status && status_saved != NULL && status_saved != status_idle) {
		purple_debug(PURPLE_DEBUG_INFO, PACKAGE, N_("restoring status '%s'\n"), purple_savedstatus_get_title(status_saved));
		purple_savedstatus_activate(status_saved);
		status_saved = NULL;
	}
}

