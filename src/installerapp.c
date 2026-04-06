#include "installerapp.h"
#include "installerappwin.h"
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <gio/gio.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>
#include <stddef.h>
#include <stdio.h>

struct _InstallerApp{
	GtkApplication parent;
	FILE* download_file;
	CURL* curl;
	InstallerAppWindow* window;
	GCancellable* setup_cancel;
};
#define ZULU_JRE_LINK  "https://cdn.azul.com/zulu/bin/zulu8.92.0.21-ca-jre8.0.482-linux_i686.zip"

G_DEFINE_TYPE(InstallerApp, installer_app, GTK_TYPE_APPLICATION);
static void installer_app_init(InstallerApp* app){}

static size_t data_handler(char* buffer, size_t itemsize, size_t n_items, void* data){
	InstallerApp* app = data;
	size_t bytes = itemsize * n_items;
	curl_off_t received, length;
	curl_easy_getinfo(app->curl, CURLINFO_SIZE_DOWNLOAD_T, &received);
	curl_easy_getinfo(app->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &length);



	gdouble progress = (float)received/(float)length;

	installer_set_progress_value(app->window, progress);
	gtk_main_iteration();

	fwrite(buffer, sizeof *buffer, bytes, app->download_file);
	fflush(app->download_file);

	return bytes;
}

// TODO: O extrair ícone do app, arquivo de entrada desktop em ~/.local/share/
// Copiar o executvel para o local escolhido
static void start_installation(InstallerAppWindow* win, gpointer data){
	InstallerApp* app = INSTALLER_APP(gtk_window_get_application(GTK_WINDOW(win)));
	app->curl = curl_easy_init();


	app->download_file = fopen(P_tmpdir"/zulujre.zip", "wr");
	curl_easy_setopt(app->curl, CURLOPT_URL, ZULU_JRE_LINK);
	curl_easy_setopt(app->curl, CURLOPT_WRITEFUNCTION, data_handler);
	curl_easy_setopt(app->curl, CURLOPT_WRITEDATA, app);

	installer_message(win, "Baixando o java 8..");
	CURLcode result = curl_easy_perform(app->curl);



	if(result != CURLE_OK){
		GString* str = g_string_new(NULL);
		g_string_printf(str, "<span foreground=\"red\">Error ao baixar o java: %s</span>", curl_easy_strerror(result));
		installer_message(win, str->str);
		g_printerr("curl: %s\n", curl_easy_strerror(result));
		g_string_free(str, TRUE);
		return;
	}


	installer_message(win, "Extraindo..");

	fclose(app->download_file);
	curl_easy_cleanup(app->curl);
}

static void cancel_installation(InstallerAppWindow* win, gpointer data){
	InstallerApp* app = INSTALLER_APP(gtk_window_get_application(GTK_WINDOW(win)));
}


// TODO tenta usar g_resources_open_stream()
static void installer_app_activate(GApplication* app){
	InstallerAppWindow* win;
	INSTALLER_APP(app)->window = installer_app_window_new(INSTALLER_APP(app));
	win = INSTALLER_APP(app)->window;

	g_signal_connect(win, "on_install_launcher", G_CALLBACK(start_installation), NULL);
	g_signal_connect(win, "on_cancel_installation", G_CALLBACK(cancel_installation), NULL);
	gtk_window_present(GTK_WINDOW(win));
}

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
