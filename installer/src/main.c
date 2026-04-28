#include <gio/gio.h>
#include <gtk/gtk.h>

#include "installerapp.h"

int main(int argc, char *argv[]){
	return g_application_run(G_APPLICATION(installer_app_new(argc, argv)), 0, NULL);
}
