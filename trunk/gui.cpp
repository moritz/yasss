#include <gtk/gtk.h>
#include "sudoku.hpp"

/* Our callback.
 * The data passed to this function is printed to stdout */
/*static void callback( GtkWidget *widget,
                      gpointer   data )
{
    g_print ("Hello again - %s was pressed\n", (char *) data);
}

*/
/* This callback quits the program */
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    gtk_main_quit ();
    return FALSE;
}

int main(int argc, char** argv){
	GtkWidget *window;
	GtkWidget *table;
	GtkWidget *text;
    
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "YasSS");
	/* Set a handler for delete_event that immediately
	 * exits GTK. */
	g_signal_connect (G_OBJECT (window), "delete_event",
			G_CALLBACK (delete_event), NULL);

	/* Sets the border width of the window. */
	gtk_container_set_border_width (GTK_CONTAINER (window), 5);

	/* Create a 2x2 table */
	table = gtk_table_new (9, 9, TRUE);

	gtk_window_resize (GTK_WINDOW(window), 400, 400);

	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			text = gtk_label_new("0");
			gtk_table_attach (GTK_TABLE (table), text, x, x + 1, y, y + 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
			gtk_widget_show(text);

		}
	}

	/* Put the table in the main window */
	gtk_container_add (GTK_CONTAINER (window), table);

	gtk_widget_show (table);

    
	gtk_widget_show  (window);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
	gtk_main ();

	return 0;
}
