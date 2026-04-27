#include "launcherapp.h"
#include "launcherappwin.h"

struct _LauncherApp{
	GtkApplication parent;
};

G_DEFINE_TYPE(LauncherApp, launcher_app, GTK_TYPE_APPLICATION);
static void launcher_app_init(LauncherApp* app){}

static void launcher_app_activate(GApplication* app){

	LauncherAppWindow* win = launcher_app_window_new(LAUNCHER_APP(app));

	gtk_window_present(GTK_WINDOW(win));
}

static void launcher_app_open(GApplication* app, GFile** files, gint n_files, const gchar* hint){
	launcher_app_activate(app);
}

static void launcher_app_class_init(LauncherAppClass* class){
	G_APPLICATION_CLASS(class)->activate = launcher_app_activate;
	G_APPLICATION_CLASS(class)->open = launcher_app_open;
}

LauncherApp* launcher_app_new(void){
	return g_object_new(LAUNCHER_APP_TYPE, "application-id", "com.minimine.launcher", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
