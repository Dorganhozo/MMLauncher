#include "installerapp.h"
#include "installerappwin.h"
#include <asm-generic/fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/system.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
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

static size_t download_handler(char* buffer, size_t itemsize, size_t n_items, void* data){
	InstallerApp* app = data;
	size_t bytes = itemsize * n_items;
	curl_off_t received, length;
	curl_easy_getinfo(app->curl, CURLINFO_SIZE_DOWNLOAD_T, &received);
	curl_easy_getinfo(app->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &length);

	gdouble progress = (float)received/(float)length;

	fwrite(buffer, sizeof *buffer, bytes, app->download_file);

	installer_set_progress_value(app->window, progress);
	gtk_main_iteration();

	return bytes;
}

// TODO: O extrair ícone do app, arquivo de entrada desktop em ~/.local/share/
// Copiar o executvel para o local escolhido
static void start_installation(InstallerAppWindow* win, gpointer data){
	InstallerApp* app = INSTALLER_APP(gtk_window_get_application(GTK_WINDOW(win)));
	app->curl = curl_easy_init();
	app->download_file = tmpfile();

	curl_easy_setopt(app->curl, CURLOPT_URL, ZULU_JRE_LINK);
	curl_easy_setopt(app->curl, CURLOPT_WRITEFUNCTION, download_handler);
	curl_easy_setopt(app->curl, CURLOPT_WRITEDATA, app);

	installer_message(win, "Baixando o java 8..");
	CURLcode result = curl_easy_perform(app->curl);


	if(result != CURLE_OK){
		curl_easy_cleanup(app->curl);
		g_printerr("curl: %s\n", curl_easy_strerror(result));
		return;
	}
	curl_easy_cleanup(app->curl);
	rewind(app->download_file);

	installer_message(win, "Extraindo..");

	curl_off_t length = 0;
	curl_easy_getinfo(app->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &length);

	int errorp;
	zip_t* zip;
	zip_source_t* zsource;
	zip_error_t zerror;

	zip_error_init(&zerror);

	zsource = zip_source_filep_create(app->download_file, 0, length, &zerror);

	if(zsource == NULL){
		g_printerr("zsource: %s\n", zip_error_strerror(&zerror));
		zip_error_fini(&zerror);
		return;
	}

	zip = zip_open_from_source(zsource, ZIP_RDONLY, &zerror);

	if(zip == NULL){
		g_printerr("zip: %s\n", zip_error_strerror(&zerror));
		zip_error_fini(&zerror);
		return;
	}

	for(int i=0; i < zip_get_num_entries(zip, ZIP_FL_UNCHANGED); i++){
		const char* name = zip_get_name(zip, i, ZIP_FL_UNCHANGED);
		g_print("%s\n", name);
	}

	fclose(app->download_file);

	zip_close(zip);

}

static void cancel_installation(InstallerAppWindow* win, gpointer data){
	// InstallerApp* app = INSTALLER_APP(gtk_window_get_application(GTK_WINDOW(win)));
	g_print("Cancelado\n");
}


// TODO tenta usar g_resources_open_stream()
static void installer_app_activate(GApplication* app){
	INSTALLER_APP(app)->window = installer_app_window_new(INSTALLER_APP(app));

	InstallerAppWindow* win = INSTALLER_APP(app)->window;

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
