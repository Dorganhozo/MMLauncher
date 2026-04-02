#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define INSTALLER_APP_TYPE (installer_app_get_type())
G_DECLARE_FINAL_TYPE(InstallerApp, installer_app, INSTALLER, APP, GtkApplication);
InstallerApp* installer_app_new(void);
