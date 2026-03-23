#include "launcher.h"
#include "launcherwin.h"
#include <gio/gio.h>
#include <glib-object.h>
#include <gtk/gtk.h>

struct _MMLauncherApp{
	GtkApplication parent;
};

G_DEFINE_TYPE(MMLauncherApp, mmlauncher_app, GTK_TYPE_APPLICATION);

static void mmlauncher_app_init(MMLauncherApp* app){

}

static void mmlauncher_app_activate(GApplication* app){
	MMLauncherAppWindow* win;
	win = mmlauncher_app_window_new(MMLAUNCHER_APP(app));
	gtk_window_present(GTK_WINDOW(win));
}

static void mmlauncher_app_open(GApplication* app, GFile** files, gint n_files, const gchar* hint){
	GList* windows;
	MMLauncherAppWindow* win;
	windows = gtk_application_get_windows(GTK_APPLICATION(app));
	win = windows?
		MMLAUNCHER_WINDOW_APP(windows->data) :
		mmlauncher_app_window_new(GTK_APPLICATION((app)));

	for(int i=0; i < n_files; i++)
		mmlauncher_app_window_open(win, files[i]);

	gtk_window_present(GTK_WINDOW(win));
}

static void mmlauncher_app_class_init(MMLauncherAppClass* class){
	G_APPLICATION_CLASS(class)->activate = mmlauncher_app_activate;
	G_APPLICATION_CLASS(class)->open = mmlauncher_app_open;
}

MMLauncherApp* mmlauncher_app_new(void){
	return g_object_new(MMLAUNCHER_APP_TYPE, "application-id", "com.minimine.launcher", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
