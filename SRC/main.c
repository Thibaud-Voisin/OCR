#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "pre_processing.h"
#include "display.h"
#include "segmentation.h"
#include "tools.h"

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
            app_wdgts -> image = SDL_LoadBMP(file_name);

            if(app_wdgts -> image != NULL)
            {
            
                GtkAllocation *allocation = g_new(GtkAllocation, 1);
                gtk_widget_get_allocation(app_wdgts -> w_img_main, allocation);
                int width = allocation -> width;
                int height = allocation -> height;
                if(app_wdgts -> image -> w < width)
                    width = app_wdgts -> image -> w;
                if(app_wdgts -> image -> h < height)
                    height = app_wdgts -> image -> h;
                if(app_wdgts -> image -> w > width || app_wdgts -> image -> h > height)
                {
                    //app_wdgts -> image = Resize_Image(app_wdgts -> image, width, height);
                }
                reload_image(app_wdgts);
                gtk_widget_set_visible(app_wdgts->w_btn_scan,TRUE);
                gtk_widget_set_visible(app_wdgts->w_btn_training,TRUE);
                gtk_widget_set_visible(app_wdgts->box,TRUE);
            
                gtk_widget_set_sensitive(app_wdgts->w_spin,TRUE);
                gtk_widget_set_sensitive(app_wdgts->w_btn_rmvpict,TRUE);
                gtk_widget_set_sensitive(app_wdgts->w_btn_grscale,TRUE);
                gtk_widget_set_sensitive(app_wdgts->w_btn_blckwhte,TRUE);
                gtk_widget_set_sensitive(app_wdgts->w_btn_noise,TRUE);
                gtk_widget_set_sensitive(app_wdgts->w_btn_contrast,TRUE);
                gtk_widget_set_visible(app_wdgts->w_menuitm_open,FALSE);
                gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_scan),"Waiting for scan...");
            }
            else
                gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_scan),"Invalid Image");



        }
        g_free(file_name);
    }

    // Finished with the "Open Image" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}

void on_btn_rmvpict_clicked(GtkMenuItem *btn_rmvpict, app_widgets *app_wdgts)
{
    btn_rmvpict = btn_rmvpict;
    
   

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_spin),0);
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_degree),"Degree of rotation");
    gtk_widget_set_visible(app_wdgts->w_menuitm_open,TRUE);
    gtk_widget_set_visible(app_wdgts->w_btn_scan,FALSE);
    gtk_widget_set_visible(app_wdgts->w_btn_training,FALSE);
    gtk_widget_set_visible(app_wdgts->box,FALSE);
    
    gtk_widget_set_sensitive(app_wdgts->w_spin,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_rmvpict,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_grscale,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_blckwhte,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_noise,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_contrast,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_drlines,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_drwords,FALSE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_drletters,FALSE);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->w_btn_drlines),FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->w_btn_drwords),FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->w_btn_drletters),FALSE);

    gtk_image_clear(GTK_IMAGE(app_wdgts->w_img_main));
    remove("tmp.bmp");
    remove("tmp2.bmp");
    remove("tmp3.bmp");

    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_scan),"Waiting for scan...");
}

void on_spin_value_changed(GtkMenuItem *btn_left, app_widgets *app_wdgts)
{
    btn_left = btn_left;
    
    SDL_Surface *test = SDL_LoadBMP("tmp3.bmp");
    if(test == NULL)
        SDL_SaveBMP(app_wdgts -> image, "tmp3.bmp");
    else
        app_wdgts -> image = test;

    gint32 quantity = 0;
    gchar out_str[100] = {0};
    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_spin));
    g_snprintf(out_str,sizeof(out_str),"%d",quantity);
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_degree),out_str);

    image_rotation(app_wdgts -> image, quantity);

    reload_image(app_wdgts);
}

void on_btn_scan_clicked(GtkMenuItem *btn_scan, app_widgets *app_wdgts)
{
    gtk_widget_set_sensitive(app_wdgts->w_btn_drlines,TRUE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_drwords,TRUE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_drletters,TRUE);
    btn_scan = btn_scan;

    blackwhite(app_wdgts -> image);
    Matrix matrix = binarize_image(app_wdgts -> image);
    gchar txt[50] = ""; 
    gchar *tst = Segmentation(matrix, app_wdgts -> image, txt, app_wdgts);
    gtk_label_set_label(GTK_LABEL(app_wdgts->w_lbl_scan),tst);

	free(matrix.matrix_data);
    reload_image(app_wdgts);
}

void on_btn_training_clicked(GtkMenuItem *btn_training, app_widgets *app_wdgts)
{
    btn_training = btn_training;
    
    training(app_wdgts);

    reload_image(app_wdgts);
}


void on_btn_grscale_clicked(GtkMenuItem *btn_grscale, app_widgets *app_wdgts)
{
    btn_grscale = btn_grscale;
 
	grayscale(app_wdgts -> image);

    reload_image(app_wdgts);
}

void on_btn_blckwhte_clicked(GtkMenuItem *btn_blckwhte, app_widgets *app_wdgts)
{
    btn_blckwhte = btn_blckwhte;

	blackwhite(app_wdgts -> image);

    reload_image(app_wdgts);
}

void on_btn_noise_clicked(GtkMenuItem *btn_noise, app_widgets *app_wdgts)
{
    btn_noise = btn_noise;

	noise_reduction(app_wdgts -> image);

    reload_image(app_wdgts);
}

void on_btn_contrast_clicked(GtkMenuItem *btn_contrast, app_widgets *app_wdgts)
{
    btn_contrast = btn_contrast;

	contrast(app_wdgts -> image);

    reload_image(app_wdgts);
}

void on_dr_change(app_widgets *app_wdgts)
{
    SDL_Surface *test = SDL_LoadBMP("tmp2.bmp");
    if(test == NULL)
        SDL_SaveBMP(app_wdgts -> image, "tmp2.bmp");
    else
        app_wdgts -> image = test;

    Matrix matrix = binarize_image(app_wdgts -> image);

    gchar txt[50]; 
    gchar *tst = Segmentation(matrix, app_wdgts -> image, txt, app_wdgts);
    gtk_label_set_label(GTK_LABEL(app_wdgts->w_lbl_scan),tst);

    reload_image(app_wdgts);
}

void on_btn_drlines_toggled(GtkToggleButton *btn_drlines, app_widgets *app_wdgts)
{
    btn_drlines = btn_drlines;
    on_dr_change(app_wdgts);
}

void on_btn_drwords_toggled(GtkToggleButton *btn_drwords, app_widgets *app_wdgts)
{
    btn_drwords = btn_drwords;
    on_dr_change(app_wdgts);
}

void on_btn_drletters_toggled(GtkToggleButton *btn_drletters, app_widgets *app_wdgts)
{
    btn_drletters = btn_drletters;
    on_dr_change(app_wdgts);
}


// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		printf("Too many arguments !\n");
		return EXIT_FAILURE;
	}

    srand(time(NULL));

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
    widgets->box = GTK_WIDGET(gtk_builder_get_object(builder, "box"));
    widgets->w_btn_training = GTK_WIDGET(gtk_builder_get_object(builder, "btn_training"));
    widgets->w_btn_grscale = GTK_WIDGET(gtk_builder_get_object(builder, "btn_grscale"));
    widgets->w_btn_blckwhte = GTK_WIDGET(gtk_builder_get_object(builder, "btn_blckwhte"));
    widgets->w_btn_noise = GTK_WIDGET(gtk_builder_get_object(builder, "btn_noise"));
    widgets->w_btn_contrast = GTK_WIDGET(gtk_builder_get_object(builder, "btn_contrast"));
    widgets->w_btn_drlines = GTK_WIDGET(gtk_builder_get_object(builder,"btn_drlines"));
    widgets->w_btn_drwords = GTK_WIDGET(gtk_builder_get_object(builder,"btn_drwords"));
    widgets->w_btn_drletters = GTK_WIDGET(gtk_builder_get_object(builder,"btn_drletters"));
    
    
    gtk_builder_connect_signals(builder, widgets);
    
    g_object_unref(builder);


    gtk_widget_show(window);

    gtk_main();
    g_slice_free(app_widgets, widgets);

    remove("tmp.bmp");
    remove("tmp2.bmp");
    remove("tmp3.bmp");

    return 0;
}

