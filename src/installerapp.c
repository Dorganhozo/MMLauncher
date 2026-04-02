#include "installerapp.h"
#include "installerappwin.h"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>

struct _InstallerApp{
	GtkApplication parent;
};

G_DEFINE_TYPE(InstallerApp, installer_app, GTK_TYPE_APPLICATION);
static void installer_app_init(InstallerApp* app){

}


// TODO: O extrair ícone do app, arquivo de entrada desktop em ~/.local/share/
// Copiar o executvel para o local escolhido
static void start_installation(InstallerAppWindow* win, gpointer data){
	gchar* path = data;
	g_print("%s\n", path);
	GError* error = NULL;
	GUri* uri = g_uri_parse("https://docs.gtk.org/glib/error-reporting.html", G_URI_FLAGS_ENCODED, &error);
	if(uri == NULL){
		g_printerr("%sError", error->message);
		return;
	}
	
}

static void cancel_installation(InstallerAppWindow* win, gpointer data){
	
}


// TODO tenta usar g_resources_open_stream()
static void installer_app_activate(GApplication* app){
	gtk_window_set_default_icon_name("installer");
	InstallerAppWindow* win;
	win = installer_app_window_new(INSTALLER_APP(app));
	g_signal_connect(win, "on_install_launcher", G_CALLBACK(start_installation), NULL);
	g_signal_connect(win, "on_cancel_installation", G_CALLBACK(cancel_installation), NULL);
	gtk_window_present(GTK_WINDOW(win));
}

// Preciso disso sempre?
static void installer_app_open(GApplication* app, GFile** files, gint n_files, const gchar* hint){
	installer_app_activate(app);
}

static void installer_app_class_init(InstallerAppClass* class){
	G_APPLICATION_CLASS(class)->activate = installer_app_activate;
	G_APPLICATION_CLASS(class)->open = installer_app_open;
}

InstallerApp* installer_app_new(void){
	return g_object_new(INSTALLER_APP_TYPE, "application-id", "com.minimine.launcher", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
