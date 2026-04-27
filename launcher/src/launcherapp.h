#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define LAUNCHER_APP_TYPE (launcher_app_get_type())
G_DECLARE_FINAL_TYPE(LauncherApp, launcher_app, LAUNCHER, APP, GtkApplication);
LauncherApp* launcher_app_new(void);
