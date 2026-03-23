#include "launcherwin.h"
#include "launcher.h"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

struct _MMLauncherAppWindow{
	GtkApplicationWindow parent;
	GtkStack* menu_stack;
};

G_DEFINE_TYPE(MMLauncherAppWindow, mmlauncher_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void quit_program(GtkButton* button, MMLauncherAppWindow* win){
	gtk_window_close(GTK_WINDOW(win));
}

static char* child_names[] = { "welcome", "choose", "loading" };
static char** current_child = child_names;

static void next(GtkButton* button, MMLauncherAppWindow* win){
	gtk_stack_set_visible_child_full(win->menu_stack, *++current_child, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);

}
static void  back(GtkButton* button, MMLauncherAppWindow* win){
	gtk_stack_set_visible_child_full(win->menu_stack, *--current_child, GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
}

static void mmlauncher_app_window_init(MMLauncherAppWindow* win){
	gtk_widget_init_template(GTK_WIDGET(win));

	// g_signal_connect(win->quit_button, "clicked", G_CALLBACK(quit_program), win);
	

	
}


static void mmlauncher_app_window_class_init(MMLauncherAppWindowClass* class){
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/minimine/launcher/layout/welcome.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), MMLauncherAppWindow, menu_stack);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), quit_program);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), next);
	gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), back);
}

MMLauncherAppWindow* mmlauncher_app_window_new(MMLauncherApp* app){
	return g_object_new(MMLAUNCHER_APP_WINDOW_TYPE, "application", app, NULL);
}

void mmlauncher_app_window_open(MMLauncherAppWindow* win, GFile* file){

}
