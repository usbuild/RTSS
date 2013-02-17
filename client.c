#include "client.h"
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <config.h>
#include <service.h>
#include <rpc_fifo_client.h>

#define W_G(builder,type,name) name=GTK_##type(gtk_builder_get_object(builder,#name))
#define N_CALLBACK(event_name) void event_name(GtkWidget *widget, gpointer user_data)

conn_t *conn;
t_user *user = NULL;

//login win
GtkWindow *login_win;
GtkButton *login_btn;
GtkButton *cancel_btn;
GtkWindow *main_win;
GtkEntry *l_username;
GtkEntry *l_password;

//main win
GtkLabel *lbl_username;
GtkLabel *lbl_password;
GtkLabel *lbl_card;
GtkLabel *lbl_phone;
GtkEntry *et_password;
GtkEntry *et_repassword;
GtkEntry *et_card;
GtkEntry *et_phone;

void get_widgets(GtkBuilder* gb)
{
    W_G(gb, WINDOW, login_win);
    W_G(gb, BUTTON, login_btn);
    W_G(gb, BUTTON, cancel_btn);
    W_G(gb, WINDOW, main_win);
    W_G(gb, ENTRY, l_username);
    W_G(gb, ENTRY, l_password);
    W_G(gb, LABEL, lbl_username);
    W_G(gb, LABEL, lbl_password);
    W_G(gb, LABEL, lbl_card);
    W_G(gb, LABEL, lbl_phone);
    W_G(gb, ENTRY, et_password);
    W_G(gb, ENTRY, et_repassword);
    W_G(gb, ENTRY, et_card);
    W_G(gb, ENTRY, et_phone);
}

N_CALLBACK(on_login_btn_clicked){
    if(login((char*)gtk_entry_get_text(l_username), (char*)gtk_entry_get_text(l_password), conn) == 0) {
        gtk_widget_hide(GTK_WIDGET(login_win));
        gtk_widget_show(GTK_WIDGET(main_win));
    } else {
        GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Wrong username or password!"
                ));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
    }
}

N_CALLBACK(on_login_win_destroy) {
    gtk_main_quit();
}

N_CALLBACK(on_main_win_destroy) {
    gtk_main_quit();
}

N_CALLBACK(on_login_win_show) {
    //conn = connect_server();    
}

void reset_user_info() {
    if(user != NULL) free(user);
    user = user_info(conn);
    gtk_label_set_text(lbl_username, user->id);
    gtk_label_set_text(lbl_card, user->card);
    gtk_label_set_text(lbl_phone, user->phone);

    gtk_entry_set_text(et_password, user->password);
    gtk_entry_set_text(et_repassword, user->password);
    gtk_entry_set_text(et_card, user->card);
    gtk_entry_set_text(et_phone, user->phone);

}

N_CALLBACK(on_main_win_show) {
    //for test
    conn = connect_server();    
    login("abc", "abc", conn);
    reset_user_info();
}


N_CALLBACK(on_reset_info_btn_clicked) {
    gtk_entry_set_text(et_password, user->password);
    gtk_entry_set_text(et_repassword, user->password);
    gtk_entry_set_text(et_card, user->card);
    gtk_entry_set_text(et_phone, user->phone);
}

N_CALLBACK(on_save_info_btn_clicked) {
    char *passwd = (char*)gtk_entry_get_text(et_password);
    char *repasswd = (char*)gtk_entry_get_text(et_repassword);
    char *card = (char*)gtk_entry_get_text(et_card);
    char *phone = (char*)gtk_entry_get_text(et_phone);
    if(strcmp(passwd, repasswd) != 0) {
        GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                main_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"password not match"
                ));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
        return;
    }
    if(update_user(passwd, card, phone, conn) == 0)  {
       reset_user_info(); 
    } else {
        GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                main_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Cannot update info"
                ));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
    }
}

gboolean on_main_win_delete_event(GtkWindow *window, gpointer user_data) {
    GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
            main_win,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Are you sure to quit?"
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
