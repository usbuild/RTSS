#include "client.h"
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <config.h>
#include <service.h>
#include <rpc_fifo_client.h>

#define W_G(type,name) name=GTK_##type(gtk_builder_get_object(gb,#name))
#define N_CALLBACK(event_name) void event_name(GtkWidget *widget, gpointer user_data)

conn_t *conn;
t_user *user = NULL;

//login win
GtkWindow *login_win;
GtkButton *login_btn;
GtkButton *cancel_btn;
GtkButton *signup_btn;
GtkWindow *main_win;
GtkWindow *admin_win;
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
GtkTreeView *ticket_table;
GtkListStore *search_result_store;
GtkListStore *buy_store;
GtkEntry *et_start;
GtkEntry *et_end;
GtkTreeView *buy_ticket_table;


GtkEntry *et_tkt_id;
GtkEntry *et_tkt_start;
GtkEntry *et_tkt_end;
GtkEntry *et_tkt_stime;
GtkEntry *et_tkt_etime;
GtkEntry *et_tkt_distance;
GtkEntry *et_tkt_price;
GtkEntry *et_tkt_num;
GtkTreeView *station_table;
GtkListStore *station_store;

void get_widgets(GtkBuilder* gb)
{
    W_G(WINDOW, login_win);
    W_G(BUTTON, login_btn);
    W_G(BUTTON, cancel_btn);
    W_G(WINDOW, main_win);
    W_G(ENTRY, l_username);
    W_G(ENTRY, l_password);
    W_G(LABEL, lbl_username);
    W_G(LABEL, lbl_password);
    W_G(LABEL, lbl_card);
    W_G(LABEL, lbl_phone);
    W_G(ENTRY, et_password);
    W_G(ENTRY, et_repassword);
    W_G(ENTRY, et_card);
    W_G(ENTRY, et_phone);
    W_G(TREE_VIEW, ticket_table);
    W_G(LIST_STORE, search_result_store);
    W_G(ENTRY, et_start);
    W_G(ENTRY, et_end);
    W_G(TREE_VIEW, buy_ticket_table);
    W_G(LIST_STORE, buy_store);
    W_G(WINDOW, admin_win);
    W_G(ENTRY, et_tkt_id);
    W_G(ENTRY, et_tkt_start);
    W_G(ENTRY, et_tkt_end);
    W_G(ENTRY, et_tkt_stime);
    W_G(ENTRY, et_tkt_etime);
    W_G(ENTRY, et_tkt_price);
    W_G(ENTRY, et_tkt_distance);
    W_G(ENTRY, et_tkt_num);
    W_G(TREE_VIEW, station_table);
    W_G(LIST_STORE, station_store);
}

static void
format_double(GtkTreeViewColumn *tree_column, GtkCellRenderer *cell, GtkTreeModel *tree_model, GtkTreeIter *iter, gpointer data) {
    double price;
    char buf[10];
    gtk_tree_model_get(tree_model, iter, 5, &price, -1);
    sprintf(buf, "%.2lf", price);
    g_object_set(cell, "text", buf, NULL);

}
static void
setup_tree_view(GtkWidget *treeview) {/*{{{*/
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Start Site", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("End Site", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Start Time", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


    column = gtk_tree_view_column_new_with_attributes("End Time", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Price", renderer, "text", 5, NULL);
    gtk_tree_view_column_set_cell_data_func(column, renderer, format_double, NULL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Distance", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Num", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}/*}}}*/

N_CALLBACK(on_login_btn_clicked){
    if(login((char*)gtk_entry_get_text(l_username), (char*)gtk_entry_get_text(l_password), conn) == 0) {
        user = user_info(conn);
        gtk_widget_hide(GTK_WIDGET(login_win));
        if(user->type) {
            gtk_widget_show(GTK_WIDGET(admin_win));
        } else {
            gtk_widget_show(GTK_WIDGET(main_win));
        }
    } else {
        GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Wrong username or password!"
                ));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
    }
}

N_CALLBACK(on_signup_btn_clicked) {
    if(signup((char*)gtk_entry_get_text(l_username), (char*)gtk_entry_get_text(l_password), conn) == 0) {
        GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Signup successfully! Please Press login button to login"
                ));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Signup Failed!"
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
} N_CALLBACK(on_admin_win_destroy) {
    gtk_main_quit();
}

N_CALLBACK(on_login_win_show) {
    conn = connect_server();    
}

N_CALLBACK(on_admin_win_show) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(station_table), column);

    column = gtk_tree_view_column_new_with_attributes("name", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(station_table), column);
}


void reset_user_info() {/*{{{*/
    if(user != NULL) free(user);
    user = user_info(conn);
    gtk_label_set_text(lbl_username, user->id);
    gtk_label_set_text(lbl_card, user->card);
    gtk_label_set_text(lbl_phone, user->phone);

    gtk_entry_set_text(et_password, user->password);
    gtk_entry_set_text(et_repassword, user->password);
    gtk_entry_set_text(et_card, user->card);
    gtk_entry_set_text(et_phone, user->phone);
}/*}}}*/


static void
update_buy_table() {
    t_ticket_list *list = query_buy_ticket(conn);

    GtkTreeIter iter;
    gtk_list_store_clear(buy_store);
    int i;
    for (i = 0; i < list->num; i++) {
        gtk_list_store_append(buy_store, &iter);
        gtk_list_store_set(buy_store, &iter,
            0, list->data[i].id,
            1, list->data[i].start,
            2, list->data[i].end,
            3, list->data[i].stime,
            4, list->data[i].etime,
            5, list->data[i].price,
            6, list->data[i].distance,
            7, list->data[i].num,
            -1);
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(buy_ticket_table), GTK_TREE_MODEL(buy_store));
}

N_CALLBACK(on_main_win_show) {
    //for test
    //conn = connect_server();    
    //login("abc", "abc", conn);
    reset_user_info();
    setup_tree_view(GTK_WIDGET(ticket_table));
    setup_tree_view(GTK_WIDGET(buy_ticket_table));
    update_buy_table();
}


N_CALLBACK(on_reset_info_btn_clicked) {
    gtk_entry_set_text(et_password, user->password);
    gtk_entry_set_text(et_repassword, user->password);
    gtk_entry_set_text(et_card, user->card);
    gtk_entry_set_text(et_phone, user->phone);
}

N_CALLBACK(on_save_info_btn_clicked) {/*{{{*/
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

gboolean on_admin_win_delete_event(GtkWindow *window, gpointer user_data) {
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
}/*}}}*/

static void 
update_search_result() {
    t_ticket_list *list = query_ticket((char*)gtk_entry_get_text(et_start), (char*)gtk_entry_get_text(et_end), conn);
    GtkTreeIter iter;
    gtk_list_store_clear(search_result_store);
    int i;
    for (i = 0; i < list->num; i++) {
        gtk_list_store_append(search_result_store, &iter);
        gtk_list_store_set(search_result_store, &iter,
            0, list->data[i].id,
            1, list->data[i].start,
            2, list->data[i].end,
            3, list->data[i].stime,
            4, list->data[i].etime,
            5, list->data[i].price,
            6, list->data[i].distance,
            7, list->data[i].num,
            -1);
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(ticket_table), GTK_TREE_MODEL(search_result_store));
}
N_CALLBACK(on_search_btn_clicked) {/*{{{*/
    update_search_result();
}/*}}}*/


void on_ticket_table_row_activated( GtkTreeView *treeview,
                                    GtkTreePath *path,
                                    GtkTreeViewColumn *col,
                                    gpointer userdata) {
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    gchar *id;
    if(gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        GtkMessageDialog *dialog;
        if(buy(id, conn) == 0) {
            update_buy_table();
            update_search_result();
            dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                    login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Buy ticket successfully"
                    ));
        } else {
            dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                    login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Buy ticket Failed"
                    ));
            
        }
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(GTK_WIDGET(dialog));
        g_free(id);
    }
}

void on_buy_ticket_table_row_activated( GtkTreeView *treeview,/*{{{*/
                                    GtkTreePath *path,
                                    GtkTreeViewColumn *col,
                                    gpointer userdata) {
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    gchar *id;
    if(gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        GtkMessageDialog *dialog;
        if(refund(id, conn) == 0) {
            update_buy_table();
            update_search_result();
            dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                    login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Refund successfully"
                    ));
        } else {
            dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                    login_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Refund Failed"
                    ));
            
        }
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
        g_free(id);
    }
    
}/*}}}*/

//admin win
char old_tkt_id[10]={0};
N_CALLBACK(on_load_tkt_btn_clicked) {
    t_ticket *tkt = load_ticket((char*)gtk_entry_get_text(et_tkt_id), conn);
    if(tkt == NULL) {
        GtkMessageDialog *dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                admin_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"No Ticket Found"
                ));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
        strcpy(old_tkt_id, "");
    } else {
        strcpy(old_tkt_id, tkt->id);
        char tmp[10];
        gtk_entry_set_text(et_tkt_id, tkt->id);        
        sprintf(tmp, "%d", tkt->start_id);
        gtk_entry_set_text(et_tkt_start, tmp);        
        sprintf(tmp, "%d", tkt->end_id);
        gtk_entry_set_text(et_tkt_end, tmp);        
        gtk_entry_set_text(et_tkt_stime, tkt->stime);        
        gtk_entry_set_text(et_tkt_etime, tkt->etime);        

        sprintf(tmp, "%2.lf", tkt->price);
        gtk_entry_set_text(et_tkt_price, tmp);        

        sprintf(tmp, "%d", tkt->distance);
        gtk_entry_set_text(et_tkt_distance, tmp);        

        sprintf(tmp, "%d", tkt->num);
        gtk_entry_set_text(et_tkt_num, tmp);        
        free(tkt);
    }
}

N_CALLBACK(on_save_tkt_btn_clicked) {
    GtkMessageDialog *dialog;
    if( update_tkt(
            old_tkt_id,
            (char*)gtk_entry_get_text(et_tkt_id),
            (char*)gtk_entry_get_text(et_tkt_start),
            (char*)gtk_entry_get_text(et_tkt_end),
            (char*)gtk_entry_get_text(et_tkt_stime),
            (char*)gtk_entry_get_text(et_tkt_etime),
            (char*)gtk_entry_get_text(et_tkt_price),
            (char*)gtk_entry_get_text(et_tkt_distance),
            (char*)gtk_entry_get_text(et_tkt_num),
            conn
            ) == 0)  {
        dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                admin_win,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Save successfully"
                ));

    } else {
        dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
                admin_win,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Save Failed"
                ));

    }
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

GtkBuilder* gtk_load_glade(gchar* filename) {/*{{{*/
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


    //gtk_widget_show(GTK_WIDGET(main_win));

    gtk_main();
    return 0;
}/*}}}*/
