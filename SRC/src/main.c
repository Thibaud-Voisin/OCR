#include <gtk/gtk.h>

typedef struct {
    GtkWidget *w_dlg_file_choose;       // Pointer to file chooser dialog box
    GtkWidget *w_img_main;              // Pointer to image widget
    
    GtkWidget *w_lbl_scan;
    GtkWidget *w_lbl_degree;
    
    GtkWidget *w_spin;
    
    GtkWidget *w_btn_rmvpict;
    GtkWidget *w_menuitm_open;
    GtkWidget *w_btn_scan;
    GtkWidget *w_btn_grscale;
    GtkWidget *w_btn_blckwhte;
    GtkWidget *w_btn_noise;
    GtkWidget *w_btn_contrast;
} app_widgets;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    app_widgets     *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    
    widgets->w_dlg_file_choose = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_file_choose"));
    widgets->w_img_main = GTK_WIDGET(gtk_builder_get_object(builder, "img_main"));
    
    widgets->w_lbl_scan = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_scan"));
    widgets->w_lbl_degree = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_degree"));
    
    widgets->w_spin = GTK_WIDGET(gtk_builder_get_object(builder, "spin"));
    
    widgets->w_btn_rmvpict = GTK_WIDGET(gtk_builder_get_object(builder, "btn_rmvpict"));
    widgets->w_menuitm_open = GTK_WIDGET(gtk_builder_get_object(builder, "menuitm_open"));
    widgets->w_btn_scan = GTK_WIDGET(gtk_builder_get_object(builder, "btn_scan"));
    widgets->w_btn_grscale = GTK_WIDGET(gtk_builder_get_object(builder, "btn_grscale"));
    widgets->w_btn_blckwhte = GTK_WIDGET(gtk_builder_get_object(builder, "btn_blckwhte"));
    widgets->w_btn_noise = GTK_WIDGET(gtk_builder_get_object(builder, "btn_noise"));
    widgets->w_btn_contrast = GTK_WIDGET(gtk_builder_get_object(builder, "btn_contrast"));
    
    
    gtk_builder_connect_signals(builder, widgets);
    
    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

// File --> Open
void on_menuitm_open_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    gchar *file_name = NULL;        // Name of file to open from dialog box
    
    menuitem = menuitem;
    // Show the "Open Image" dialog box
    gtk_widget_show(app_wdgts->w_dlg_file_choose);
    
    // Check return value from Open Image dialog box to see if user clicked the Open button
    if (gtk_dialog_run(GTK_DIALOG (app_wdgts->w_dlg_file_choose)) == GTK_RESPONSE_OK) {
        // Get the file name from the dialog box
        file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(app_wdgts->w_dlg_file_choose));
        if (file_name != NULL) {
            gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_main), file_name);
            
            gtk_widget_set_visible(app_wdgts->w_menuitm_open,FALSE);
            gtk_widget_set_visible(app_wdgts->w_btn_scan,TRUE);
            
            gtk_widget_set_sensitive(app_wdgts->w_spin,TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_rmvpict,TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_grscale,TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_blckwhte,TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_noise,TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_contrast,TRUE);
        }
        g_free(file_name);
    }

    // Finished with the "Open Image" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}

void on_btn_rmvpict_clicked(GtkMenuItem *btn_rmvpict, app_widgets *app_wdgts)
{
    btn_rmvpict = btn_rmvpict;
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_main), "");
    
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_scan),"Waiting for scan...");
   

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_spin),0);
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_degree),"Degree of rotation");
    gtk_widget_set_visible(app_wdgts->w_menuitm_open,TRUE);
    gtk_widget_set_visible(app_wdgts->w_btn_scan,FALSE);
    
    gtk_widget_set_sensitive(app_wdgts->w_spin,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_rmvpict,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_grscale,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_blckwhte,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_noise,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_contrast,FALSE);
    

}

void on_spin_value_changed(GtkMenuItem *btn_left, app_widgets *app_wdgts)
{
    btn_left = btn_left;
    guint32 quantity = 0;
    gchar out_str[100] = {0};
    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_spin));
    g_snprintf(out_str,sizeof(out_str),"%d",quantity);
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_degree),out_str);
    g_print("%d\n",quantity);
}

void on_btn_scan_clicked(GtkMenuItem *btn_scan, app_widgets *app_wdgts)
{
    btn_scan = btn_scan;
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_scan),"cjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnvcjnnvkebvkbekvbkebve\nvnnvnwevnwnv");
}

void on_btn_grscale_clicked(GtkMenuItem *btn_grscale, app_widgets *app_wdgts)
{
    btn_grscale = btn_grscale;
    app_wdgts = app_wdgts;
    g_print("grayscale\n");
}

void on_btn_blckwhte_clicked(GtkMenuItem *btn_blckwhte, app_widgets *app_wdgts)
{
    btn_blckwhte = btn_blckwhte;
    app_wdgts = app_wdgts;
    g_print("Black and White\n");
}

void on_btn_noise_clicked(GtkMenuItem *btn_noise, app_widgets *app_wdgts)
{
    btn_noise = btn_noise;
    app_wdgts = app_wdgts;
    g_print("Noise reduction\n");
}

void on_btn_contrast_clicked(GtkMenuItem *btn_contrast, app_widgets *app_wdgts)
{
    btn_contrast = btn_contrast;
    app_wdgts = app_wdgts;
    g_print("Contrast\n");
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
