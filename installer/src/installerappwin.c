#include "installerappwin.h"
#include "installerapp.h"
#include <gdk/gdk.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>
#include <string.h>

struct _InstallerAppWindow{
	GtkApplicationWindow parent;
	GtkStack* menu_stack;
	GtkFileChooserButton* choose_path_button;
	GtkProgressBar* setup_progressbar;
	GtkTextView* messages_viewer;

	char** current_page;
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
	// TODO: E tbm coloque um icone de instalação na janela
	gtk_window_set_default_icon_name("mmlauncher");
	gtk_widget_init_template(GTK_WIDGET(win));
	win->current_page = page_names;
}

// TODO: No penultimo botão coloque deixe escrito install
static void next(GtkButton* button, InstallerAppWindow* win){
	const gchar* path;
	if(!strcmp(*win->current_page, "choose")){
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(win->choose_path_button);
		path = gtk_file_chooser_get_filename(chooser);
		if(path == NULL)return;
	}


	gtk_stack_set_visible_child_full(win->menu_stack, *++win->current_page, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);


	if(!strcmp(*win->current_page, "loading")){
		g_signal_emit(win, signals[START_INSTALLATION], 0, path);
	}


}
static void  back(GtkButton* button, InstallerAppWindow* win){
	gtk_stack_set_visible_child_full(win->menu_stack, *--win->current_page, GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);

	if(!strcmp(*win->current_page, "choose")){
		GtkTextBuffer* buffer = gtk_text_view_get_buffer(win->messages_viewer);
		GtkTextIter start = {}, end = {};
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);

		gtk_text_buffer_delete(buffer, &start, &end);

		gtk_progress_bar_set_fraction(win->setup_progressbar, 0);
		g_signal_emit(win, signals[CANCEL_INSTALLATION], 0);
	}
}


static void quit_program(GtkButton* button, InstallerAppWindow* win){
	GtkApplication* app = gtk_window_get_application(GTK_WINDOW(win));
	g_application_quit(G_APPLICATION(app));
}

void installer_set_progress_value(InstallerAppWindow* win, gdouble progress){

	gtk_progress_bar_set_fraction(win->setup_progressbar, progress);
}


void installer_message(InstallerAppWindow* win, const gchar* message){
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(win->messages_viewer);
	gtk_text_buffer_insert_at_cursor(buffer, message, -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\n", 1);

}

static void installer_app_window_class_init(InstallerAppWindowClass* class){
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/minimine/launcher/layout/installer.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), InstallerAppWindow, menu_stack);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), InstallerAppWindow, choose_path_button);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), InstallerAppWindow, setup_progressbar);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), InstallerAppWindow, messages_viewer);

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
