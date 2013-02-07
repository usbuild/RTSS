#include "client.h"
#include <gtk/gtk.h>

#define W_G(builder,type,name) name=GTK_##type(gtk_builder_get_object(builder,#name))
#define N_CALLBACK(event_name) void event_name(GtkWidget *widget, gpointer user_data)


GtkWindow *login_win;
GtkButton *login_btn;
GtkButton *cancel_btn;
GtkWindow *main_win;

void get_widgets(GtkBuilder* gb)
{
    W_G(gb,WINDOW,login_win);
    W_G(gb,BUTTON,login_btn);
    W_G(gb,BUTTON,cancel_btn);
    W_G(gb,WINDOW,main_win);
}

N_CALLBACK(on_login_btn_clicked){
    gtk_widget_hide(GTK_WIDGET(login_win));
    gtk_widget_show(GTK_WIDGET(main_win));
}

N_CALLBACK(on_login_win_destroy) {
    gtk_main_quit();
}

N_CALLBACK(on_main_win_destroy) {
    gtk_main_quit();
}
gboolean on_main_win_delete_event(GtkWindow *window, gpointer user_data) {
    GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
        main_win,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"您真的要退出么?"
        ));
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
    if(result == GTK_RESPONSE_YES) {
        return FALSE;
    }
    return TRUE;
}

GtkBuilder* gtk_load_glade(gchar* filename) {
    GtkBuilder *gb;
    gb = gtk_builder_new();
    if(!gtk_builder_add_from_file(gb, filename, NULL)) {
        g_print("ERROR!!");
        return NULL;
    }
    gtk_builder_connect_signals(gb, NULL);
    return gb;
}


int main(int argc, char *argv[])
{
    GtkBuilder *gb;
    gtk_init(&argc, &argv);

    gb = gtk_load_glade(UI_FILE);
    get_widgets(gb);


    //gtk_widget_show(GTK_WIDGET(login_win));
    gtk_widget_show(GTK_WIDGET(main_win));

    gtk_main();
    return 0;
}
