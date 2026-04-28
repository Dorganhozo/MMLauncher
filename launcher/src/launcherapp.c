#include "launcherapp.h"
#include "launcherappwin.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct _LauncherApp{
	GtkApplication parent;
};

G_DEFINE_TYPE(LauncherApp, launcher_app, GTK_TYPE_APPLICATION);
static void launcher_app_init(LauncherApp* app){}

void play_game(GtkButton* button, GApplication* app){
	// TODO: Baixe a ultima versão do MineMine para a pasta versoes
	g_print("Starting the GAME!\n");

	char* java_path = realpath(g_get_home_dir(), NULL);

	if(java_path == NULL){
		perror("Erro ao encontrar java");
		return;
	}

	strcat(java_path, "/.jre/zulu8.92.0.21-ca-jre8.0.482-linux_i686/bin/java");

	g_print("%s\n", java_path);
	char* argv[] = {java_path, "-jar", "versoes/minimine.jar"};
	if(execve(java_path, argv, g_get_environ())){
		free(java_path);
		perror("Erro ao inicializar\n");
	}
	free(java_path);

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
