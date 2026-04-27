#include "launcherappwin.h"
#include "launcherapp.h"

struct _LauncherAppWindow{
	GtkApplicationWindow parent;
};


G_DEFINE_TYPE(LauncherAppWindow, launcher_app_window, GTK_TYPE_APPLICATION_WINDOW);
static void launcher_app_window_init(LauncherAppWindow* win){
	gtk_window_set_default_icon_name("mmlauncher");
	gtk_widget_init_template(GTK_WIDGET(win));
}




static void launcher_app_window_class_init(LauncherAppWindowClass* class){
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/minimine/launcher/layout/launcher.ui");
}

LauncherAppWindow* launcher_app_window_new(LauncherApp* app){
	return g_object_new(LAUNCHER_APP_WINDOW_TYPE, "application", app, NULL);
}

void launcher_app_window_open(LauncherAppWindow* win, GFile* file){

}
