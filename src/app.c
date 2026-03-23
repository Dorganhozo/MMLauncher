#include <gio/gio.h>
#include <gtk/gtk.h>

#include "launcher.h"

int main(int argc, char *argv[]){
	return g_application_run(G_APPLICATION(mmlauncher_app_new()), argc, argv);
}
