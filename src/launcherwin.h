#pragma once

#include "launcher.h"
#include <gio/gio.h>
#include <gtk/gtk.h>

#define MMLAUNCHER_APP_WINDOW_TYPE (mmlauncher_app_window_get_type())
G_DECLARE_FINAL_TYPE (MMLauncherAppWindow, mmlauncher_app_window, MMLAUNCHER, WINDOW_APP, GtkApplicationWindow);

MMLauncherAppWindow* mmlauncher_app_window_new(MMLauncherApp* app);
void mmlauncher_app_window_open(MMLauncherAppWindow* win, GFile* file);
