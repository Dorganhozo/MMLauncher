#include <gio/gio.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "launcherapp.h"

int main(int argc, char* argv[]){
	if(chdir(getenv("HOME"))){
		perror("Error ao entrar na pasta HOME");
		return EXIT_FAILURE;
	}
	char* mmlpath = realpath(".local/bin/mmlauncher", NULL);
	if(mmlpath == NULL){
		perror("Error ao se encontrar no PATH");
		return EXIT_FAILURE;
	}
	
	*strrchr(mmlpath, '/') = '\0';
	if(chdir(mmlpath)){
		free(mmlpath);
		perror("Error ao entrar no diretorio");
		return EXIT_FAILURE;
	}
	free(mmlpath);
	return g_application_run(G_APPLICATION(launcher_app_new()), 0, NULL);
}
