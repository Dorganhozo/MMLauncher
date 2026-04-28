#pragma once

#include "installerapp.h"
#include <gio/gio.h>
#include <gtk/gtk.h>

#define INSTALLER_APP_WINDOW_TYPE (installer_app_window_get_type())
G_DECLARE_FINAL_TYPE (InstallerAppWindow, installer_app_window, INSTALLER, WINDOW_APP, GtkApplicationWindow);

InstallerAppWindow* installer_app_window_new(InstallerApp* app);
void installer_set_progress_value(InstallerAppWindow* win, gdouble progress);
void installer_message(InstallerAppWindow* win, const gchar* message);
