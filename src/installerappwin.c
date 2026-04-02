#include "installerappwin.h"
#include "installerapp.h"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>

struct _InstallerAppWindow{
	GtkApplicationWindow parent;
	GtkStack* menu_stack;
	GtkFileChooserButton* choose_path_button;
	char** current_child;
};

static char* page_names[] = { "welcome", "choose", "loading" };
enum signal_types {
	START_INSTALLATION,
	CANCEL_INSTALLATION,
	END_SIGNAL
};
static guint signals[END_SIGNAL];

G_DEFINE_TYPE(InstallerAppWindow, installer_app_window, GTK_TYPE_APPLICATION_WINDOW);
static void installer_app_window_init(InstallerAppWindow* win){
	gtk_widget_init_template(GTK_WIDGET(win));
	win->current_child = page_names;
}


static void next(GtkButton* button, InstallerAppWindow* win){
	gtk_stack_set_visible_child_full(win->menu_stack, *++win->current_child, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);


	if(!strcmp(*win->current_child, "loading")){
		const gchar* path;

		GtkFileChooser* chooser = GTK_FILE_CHOOSER(win->choose_path_button);
		path = gtk_file_chooser_get_filename(chooser);
		g_signal_emit(win, signals[START_INSTALLATION], 0, path);
	}


}
static void  back(GtkButton* button, InstallerAppWindow* win){
	gtk_stack_set_visible_child_full(win->menu_stack, *--win->current_child, GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);

	if(!strcmp(*win->current_child, "choose")){
		// g_print("Cancelar instalação!\n");
		g_signal_emit(win, signals[CANCEL_INSTALLATION], 0);
	}
}


static void quit_program(GtkButton* button, InstallerAppWindow* win){
	gtk_window_close(GTK_WINDOW(win));
}



static void installer_app_window_class_init(InstallerAppWindowClass* class){
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/minimine/launcher/layout/installer.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), InstallerAppWindow, menu_stack);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), InstallerAppWindow, choose_path_button);

	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), next);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), back);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), quit_program);

	signals[START_INSTALLATION] = g_signal_new("on_install_launcher", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST|G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_STRING);
	signals[CANCEL_INSTALLATION] = g_signal_new("on_cancel_installation", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST|G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

InstallerAppWindow* installer_app_window_new(InstallerApp* app){
	return g_object_new(INSTALLER_APP_WINDOW_TYPE, "application", app, NULL);
}

void installer_app_window_open(InstallerAppWindow* win, GFile* file){

}
