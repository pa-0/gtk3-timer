/* Compile with:
	gcc main.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -o timer */

#include <gtk/gtk.h>

GtkWidget * window;
GtkWidget * grid;

GtkWidget * output_label;
GtkWidget * pause_button;
GtkWidget * quit_button;

GString * output;

GTimer * timer;
gint64 end_time = 0;
gint64 time_remaining = 0;

// ------------------------------------------------------------------- utilities
gint64 hms_to_seconds(gchar * time_input)
{
	gint8 colons = 0;
	for (gchar * index = time_input; *index != '\0'; index++)
	{
		colons += (*index == ':');
	}

	gchar ** time_array;
	gint64 seconds = 0;
	switch (colons)
	{
		case 0:
			seconds = g_ascii_strtoll(time_input, NULL, 10);
			break;
		case 1:
			time_array = g_strsplit(time_input, ":", -1);
			seconds += g_ascii_strtoll(time_array[0], NULL, 10) * 60;
			seconds += g_ascii_strtoll(time_array[1], NULL, 10);
		    g_strfreev(time_array);
			break;
		case 2:
			time_array = g_strsplit(time_input, ":", -1);
			seconds += g_ascii_strtoll(time_array[0], NULL, 10) * 3600;
			seconds += g_ascii_strtoll(time_array[1], NULL, 10) * 60;
			seconds += g_ascii_strtoll(time_array[2], NULL, 10);
		    g_strfreev(time_array);
			break;
		default:
			seconds = -1;
	}
    return seconds;
}

// ---------------------------------------------------------------- Widget logic

gboolean on_timer_timeout(gpointer data)
{
	time_remaining = end_time - (gint32)g_timer_elapsed(timer, NULL);
	if (time_remaining < 0)
	{
		gtk_widget_destroy(window);
		return FALSE;
	}

	gint32 hour = time_remaining / 3600;
	gint32 minute = (time_remaining - (hour * 3600)) / 60;
	gint32 second = (time_remaining - (hour * 3600) - (minute * 60));

    g_string_printf(output, "%02d:%02d:%02d", hour, minute, second);
    gtk_label_set_text( GTK_LABEL(output_label), output->str);

	return TRUE;
}

void on_pause_button_pressed(GtkWidget* widget, gpointer data)
{
	if (g_timer_is_active(timer))
	{
		g_timer_stop(timer);
		gtk_button_set_label( GTK_BUTTON(pause_button), "resume");
	}
	else
	{
		g_timer_continue(timer);
		gtk_button_set_label(GTK_BUTTON(pause_button), "pause");
	}
}

void on_quit_button_pressed(GtkWidget* widget, gpointer data)
{
	g_string_free(output, TRUE);
	gtk_widget_destroy(window);
}

// ------------------------------------------------------------------------ main
gint32 main (gint32 argc, gchar **argv)
{
	end_time = hms_to_seconds(argv[1]);
	time_remaining = end_time;
	if (time_remaining < 1)
	{
		return 1;
	}

	gint32 status;
	output = g_string_new(NULL);

	timer = g_timer_new();

	gint32 hour = time_remaining / 3600;
	gint32 minute = (time_remaining - (hour * 3600)) / 60;
	gint32 second = (time_remaining - (hour * 3600) - (minute * 60));
    g_string_printf(output, "%02d:%02d:%02d", hour, minute, second);

    gtk_init(&argc, &argv);

	// ------------------------------------------- Create and set up main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Timer");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	gtk_container_set_border_width (GTK_CONTAINER (window), 5);

	// ---------------------------------------- Create and pack grid into window
	grid = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (window), grid);
	gtk_grid_set_column_spacing( GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing( GTK_GRID(grid), 2);

	// ------------------------------------------------------------ Output label
	output_label = gtk_label_new(output->str);
	gtk_grid_attach (GTK_GRID (grid), output_label, 0,0,2,1);

	// ------------------------------------------------------------------- Pause
	pause_button = gtk_button_new_with_label("pause");
	gtk_grid_attach (GTK_GRID (grid), pause_button, 0,1,1,1);	
	g_signal_connect (pause_button, "clicked", G_CALLBACK (on_pause_button_pressed), NULL);

	// -------------------------------------------------------------------- Quit
	quit_button = gtk_button_new_with_label("quit");
	gtk_grid_attach (GTK_GRID (grid), quit_button, 1,1,1,1);	
	g_signal_connect (quit_button, "clicked", G_CALLBACK (on_quit_button_pressed), NULL);

	g_timeout_add(1000, on_timer_timeout, NULL);

    gtk_widget_show_all(window);

    gtk_main();

	return status;
}
