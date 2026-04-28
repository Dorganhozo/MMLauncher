#include <gio/gio.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "launcherapp.h"

int main(int argc, char* argv[]){
	char* mmlpath = realpath("/usr/local/bin/mmlauncher", NULL);
	
	*strrchr(mmlpath, '/') = '\0';
	if(chdir(mmlpath)){
		free(mmlpath);
		perror("Error ao entrar no diretorio");
		return EXIT_FAILURE;
	}
	free(mmlpath);
	return g_application_run(G_APPLICATION(launcher_app_new()), 0, NULL);
}
