#include "client.h"
#include <gtk/gtk.h>

#define W_G(builder,type,name) name=GTK_##type(gtk_builder_get_object(builder,#name))
#define NCALLBACK_(event_name) void event_name(GtkWidget *widget, gpointer user_data)


GtkWindow *login_win;
GtkButton *login_btn;
GtkButton *cancel_btn;
GtkDialog *aboutdialog1;

void get_widgets(GtkBuilder* gb)
{
    W_G(gb,WINDOW,login_win);
    W_G(gb,BUTTON,login_btn);
    W_G(gb,BUTTON,cancel_btn);
    W_G(gb,DIALOG,aboutdialog1);
}

NCALLBACK_(on_login_btn_clicked){
    gtk_widget_show(GTK_WIDGET(aboutdialog1));
}

NCALLBACK_(on_login_win_destroy) {
    gtk_main_quit();
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


    gtk_widget_show(GTK_WIDGET(login_win));

    gtk_main();
    return 0;
}
