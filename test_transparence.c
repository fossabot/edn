
// gcc test_transparence.c -o out `pkg-config --cflags --libs gtk+-3.0` -DUSE_GTK_VERSION_3_0
// gcc test_transparence.c -o out `pkg-config --cflags --libs gtk+-2.0` -DUSE_GTK_VERSION_2_0

// includes system, malloc, EXIT_SUCCESS
#include <stdlib.h>
// includes fopen, fwrite, fseek, ftell
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>


#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <cairo.h>

static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer user_data);
static gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
static void clicked(GtkWindow *win, GdkEventButton *event, gpointer user_data);


#	if USE_GTK_VERSION_3_0
const GdkRGBA color = { 1.0, 1.0, 0.0, 0.0};
#endif

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Alpha Demo");
    g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);

    gtk_widget_set_app_paintable(window, TRUE);

#	if USE_GTK_VERSION_3_0
	g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(expose), NULL);
#	elif USE_GTK_VERSION_2_0
    g_signal_connect(G_OBJECT(window), "expose-event", G_CALLBACK(expose), NULL);
#	endif
    g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(screen_changed), NULL);
#	if USE_GTK_VERSION_3_0
	GtkStyleContext *context;
	GdkRGBA rgba;
	context = gtk_widget_get_style_context(window);
	gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
	                                       &rgba);
	gtk_widget_override_background_color(window,
	                                     GTK_STATE_FLAG_NORMAL, &rgba);
/*
    gtk_widget_override_background_color(window, 
	                                     GTK_STATE_FLAG_NORMAL,// | GTK_STATE_FLAG_ACTIVE | GTK_STATE_FLAG_PRELIGHT | GTK_STATE_FLAG_SELECTED | GTK_STATE_FLAG_INSENSITIVE | GTK_STATE_FLAG_INCONSISTENT | GTK_STATE_FLAG_FOCUSED,
	                                     &color);
	
	gtk_widget_override_color(window, 
	                                     GTK_STATE_FLAG_NORMAL,// | GTK_STATE_FLAG_ACTIVE | GTK_STATE_FLAG_PRELIGHT | GTK_STATE_FLAG_SELECTED | GTK_STATE_FLAG_INSENSITIVE | GTK_STATE_FLAG_INCONSISTENT | GTK_STATE_FLAG_FOCUSED,
	                                     &color);
*/
#endif
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(clicked), NULL);

    GtkWidget* fixed_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed_container);
    GtkWidget* button = gtk_button_new_with_label("button1");
    gtk_widget_set_size_request(button, 100, 100);
    gtk_container_add(GTK_CONTAINER(fixed_container), button);

    screen_changed(window, NULL, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

	
gboolean supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
    /* To check if the display supports alpha channels, get the colormap */
#	ifdef USE_GTK_VERSION_2_0
    GdkScreen *screen = gtk_widget_get_screen(widget);
    
    GdkColormap *colormap = gdk_screen_get_rgba_colormap(screen);

    if (!colormap)
    {
        printf("Your screen does not support alpha channels!\n");
        colormap = gdk_screen_get_rgb_colormap(screen);
        supports_alpha = FALSE;
    }
    else
    {
        printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    gtk_widget_set_colormap(widget, colormap);
#endif
   /*
    gtk_widget_override_background_color(widget, 
	                                     GTK_STATE_FLAG_NORMAL,// | GTK_STATE_FLAG_ACTIVE | GTK_STATE_FLAG_PRELIGHT | GTK_STATE_FLAG_SELECTED | GTK_STATE_FLAG_INSENSITIVE | GTK_STATE_FLAG_INCONSISTENT | GTK_STATE_FLAG_FOCUSED,
	                                     &color);
    gtk_widget_override_color(widget, 
	                                     GTK_STATE_FLAG_NORMAL,// | GTK_STATE_FLAG_ACTIVE | GTK_STATE_FLAG_PRELIGHT | GTK_STATE_FLAG_SELECTED | GTK_STATE_FLAG_INSENSITIVE | GTK_STATE_FLAG_INCONSISTENT | GTK_STATE_FLAG_FOCUSED,
	                                     &color);
*/
}

static gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer userdata)
{
   return FALSE;
#	if USE_GTK_VERSION_3_0
	cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(widget));
#	elif USE_GTK_VERSION_2_0
	cairo_t *cr = gdk_cairo_create(widget->window);
#	endif
    if (TRUE == supports_alpha)
        cairo_set_source_rgba (cr, 0.0, 0.0, 1.0, 0.3); /* transparent */
    else
        cairo_set_source_rgb (cr, 1.0, 0.0, 0.0); /* opaque white */

    /* draw the background */
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);

    cairo_destroy(cr);

    return FALSE;
}

static void clicked(GtkWindow *win, GdkEventButton *event, gpointer user_data)
{
    /* toggle window manager frames */
    gtk_window_set_decorated(win, !gtk_window_get_decorated(win));
}


