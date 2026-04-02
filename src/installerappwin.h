#pragma once

#include "installerapp.h"
#include <gio/gio.h>
#include <gtk/gtk.h>

#define INSTALLER_APP_WINDOW_TYPE (installer_app_window_get_type())
G_DECLARE_FINAL_TYPE (InstallerAppWindow, installer_app_window, INSTALLER, WINDOW_APP, GtkApplicationWindow);

InstallerAppWindow* installer_app_window_new(InstallerApp* app);
void installer_app_window_open(InstallerAppWindow* win, GFile* file);
