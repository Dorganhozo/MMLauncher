#include "installerapp.h"
#include "installerappwin.h"
#include <asm-generic/fcntl.h>
#include <asm/fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/system.h>
#include <linux/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zip.h>
#include <curl/typecheck-gcc.h>
#include <gio/gio.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <zipconf.h>

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

void mkdir_p(const char* path){
	int status = mkdir(path, S_IRWXU);
	if(status){
		char* bar = strrchr(path, '/');
		if(bar == NULL)return;
		*bar = '\0';
		mkdir_p(path);
		*bar = '/';
		mkdir(path, S_IRWXU);
	}
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
		zip_error_init_with_code(&zerror, errorp);
		g_printerr("zip: %s\n", zip_error_strerror(&zerror));
		zip_error_fini(&zerror);
		return;
	}


	if(chdir(g_get_home_dir())){
		perror("Error entering HOME folder");
		return;
	}

	if(mkdir(".jre", S_IRWXU) & chdir(".jre")){
		perror("Error in JRE folder");
		return;
	}

	FILE* output_file;
	zip_file_t* input_file;
	mode_t mode;

	zip_uint32_t attrs;
	zip_uint8_t opsys;
	uint8_t buffer[1024];
	int valread;

	enum{
		FA_RONLY = 0x1,
		FA_DIRECTORY = 0x10,
	};

	for(int i=0; i < zip_get_num_entries(zip, ZIP_FL_UNCHANGED); i++){
		const char* name = zip_get_name(zip, i, ZIP_FL_UNCHANGED);
		zip_file_get_external_attributes(zip, i, ZIP_FL_CENTRAL, &opsys, &attrs);
		if(attrs & FA_DIRECTORY){
			mkdir_p(name);
			continue;
		}

	
		if(opsys & ~ZIP_OPSYS_DEFAULT)continue;

		mode = (attrs >> 16) & 0b111111111;


		output_file = fopen(name, "wr");
	
		if(output_file == NULL){
			perror(name);
			continue;
		}

		input_file = zip_fopen_index(zip, i, ZIP_FL_UNCHANGED);

		while((valread = zip_fread(input_file, buffer, 1024)) > 0){
			fwrite(buffer, sizeof *buffer, valread, output_file);
			fflush(output_file);
		}

		fclose(output_file);

		if(chmod(name, mode)){
			perror("error changing file mode");
			continue;
		}

	}

	fclose(app->download_file);

	zip_close(zip);

	installer_message(app->window, "Java Instalado com sucesso!");

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
