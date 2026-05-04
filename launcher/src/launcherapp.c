#include "launcherapp.h"
#include "launcherappwin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <curl/curl.h>

struct _LauncherApp{
	GtkApplication parent;
	FILE* download_file;
	CURL* curl;
};

#define MINIMINE_LINK "https://focadoestudios.netlify.app/pacotes/minimine/MiniMine.jar"

G_DEFINE_TYPE(LauncherApp, launcher_app, GTK_TYPE_APPLICATION);
static void launcher_app_init(LauncherApp* app){}

static size_t download_handler(char* buffer, size_t itemsize, size_t n_items, void* data){
	LauncherApp* app = data;
	size_t bytes = itemsize * n_items;
	curl_off_t received, length;
	curl_easy_getinfo(app->curl, CURLINFO_SIZE_DOWNLOAD_T, &received);
	curl_easy_getinfo(app->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &length);

	if(length < 0)return 0;
	float progress = (float)received/(float)length;

	fwrite(buffer, sizeof *buffer, bytes, app->download_file);

	printf("\r%.2f%%", progress * 100);

	return bytes;
}

void play_game(GtkButton* button, GApplication* gapp){
	LauncherApp* app = LAUNCHER_APP(gapp);

	char* java_path = realpath(g_get_home_dir(), NULL);

	if(java_path == NULL){
		perror("Erro ao encontrar java");
		return;
	}

#ifdef __i686__
	strcat(java_path, "/.jre/zulu8.92.0.21-ca-jre8.0.482-linux_i686/bin/java");
#else
	strcat(java_path, "/.jre/zulu8.94.0.17-ca-jre8.0.492-linux_x64/bin/java");
#endif

	struct stat st = {};

	if(stat("versoes/minimine.jar", &st) == -1){
		if(mkdir("versoes", S_IRWXU)){
			perror("Error ao criar a pasta versoes");
			return;
		}
		app->curl = curl_easy_init();
		app->download_file = fopen("versoes/minimine.jar", "wr");

		curl_easy_setopt(app->curl, CURLOPT_URL, MINIMINE_LINK);
		curl_easy_setopt(app->curl, CURLOPT_WRITEFUNCTION, download_handler);
		curl_easy_setopt(app->curl, CURLOPT_WRITEDATA, app);

		CURLcode result = curl_easy_perform(app->curl);


		if(result != CURLE_OK){
			curl_easy_cleanup(app->curl);
			g_printerr("curl: %s\n", curl_easy_strerror(result));
			return;
		}
		fclose(app->download_file);

		g_print("\n");
	}


	g_print("Começando o jogo!\n");

	char* argv[] = {java_path, "-jar", "versoes/minimine.jar"};
	if(execve(java_path, argv, g_get_environ())){
		free(java_path);
		perror("Erro ao inicializar\n");
		return;
	}

}

static void launcher_app_activate(GApplication* app){

	LauncherAppWindow* win = launcher_app_window_new(LAUNCHER_APP(app));

	gtk_window_present(GTK_WINDOW(win));

	g_signal_connect(win, "play_game", G_CALLBACK(play_game), NULL);
}

static void launcher_app_class_init(LauncherAppClass* class){
	G_APPLICATION_CLASS(class)->activate = launcher_app_activate;
}

LauncherApp* launcher_app_new(void){
	return g_object_new(LAUNCHER_APP_TYPE, "application-id", "com.minimine.launcher", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
