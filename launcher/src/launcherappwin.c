#include "launcherappwin.h"
#include "launcherapp.h"

struct _LauncherAppWindow{
	GtkApplicationWindow parent;
};


enum signal_types {
	PLAY_GAME,
	END_SIGNAL
};
static guint signals[END_SIGNAL];

G_DEFINE_TYPE(LauncherAppWindow, launcher_app_window, GTK_TYPE_APPLICATION_WINDOW);
static void launcher_app_window_init(LauncherAppWindow* win){
	gtk_window_set_default_icon_name("mmlauncher");
	gtk_widget_init_template(GTK_WIDGET(win));
}


static void on_play_button_clicked(GtkButton* button, LauncherAppWindow* win){
	g_signal_emit(win, signals[PLAY_GAME], 0, G_APPLICATION(gtk_window_get_application(GTK_WINDOW(win))));
}


static void launcher_app_window_class_init(LauncherAppWindowClass* class){
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/minimine/launcher/layout/launcher.ui");
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), on_play_button_clicked);
	signals[PLAY_GAME] = g_signal_new("play_game",
			G_TYPE_FROM_CLASS(class),
			G_SIGNAL_RUN_LAST|G_SIGNAL_RUN_FIRST,
			0,
			NULL, NULL, NULL,
			G_TYPE_NONE, 1, G_TYPE_APPLICATION);
	
}

LauncherAppWindow* launcher_app_window_new(LauncherApp* app){
	return g_object_new(LAUNCHER_APP_WINDOW_TYPE, "application", app, NULL);
}
