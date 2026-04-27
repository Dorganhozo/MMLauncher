#include <gio/gio.h>
#include <gtk/gtk.h>

#include "installerapp.h"
#ifndef CONTENT_SIZE
#define CONTENT_SIZE 0
#endif

int main(int argc, char *argv[]){
	return g_application_run(G_APPLICATION(installer_app_new(argc, argv)), 0, NULL);
}
