#include <gio/gio.h>
#include <gtk/gtk.h>

#include "launcherapp.h"

int main(int argc, char *argv[]){
	return g_application_run(G_APPLICATION(launcher_app_new()), argc, argv);
}
