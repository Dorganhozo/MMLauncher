#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define MMLAUNCHER_APP_TYPE (mmlauncher_app_get_type())
G_DECLARE_FINAL_TYPE(MMLauncherApp, mmlauncher_app, MMLAUNCHER, APP, GtkApplication);
MMLauncherApp* mmlauncher_app_new(void);
