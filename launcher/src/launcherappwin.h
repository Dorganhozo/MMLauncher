#pragma once

#include "launcherapp.h"
#include <gio/gio.h>
#include <gtk/gtk.h>

#define LAUNCHER_APP_WINDOW_TYPE (launcher_app_window_get_type())
G_DECLARE_FINAL_TYPE (LauncherAppWindow, launcher_app_window, LAUNCHER, WINDOW_APP, GtkApplicationWindow);

LauncherAppWindow* launcher_app_window_new(LauncherApp* app);
void launcher_app_window_open(LauncherAppWindow* win, GFile* file);
