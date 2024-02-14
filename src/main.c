/* Compile with:
	gcc main.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -o timer */

#include <gtk/gtk.h>
#include "gtk_simplified.h"

GtkWidget * pause_button;
GtkWidget * quit_button;

GTimer * timer;
gint64 end_time = 0;
gint64 time_remaining = 0;

GtkWidget * digits[6];

GtkWidget * colon_0;
GtkWidget * colon_1;

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

	if (seconds > 359999)
	{
		seconds = 359999;
	}

    return seconds;
}

// ---------------------------------------------------------------- Widget logic

gboolean on_timer_timeout(gpointer data)
{
	time_remaining = end_time - (gint32)g_timer_elapsed(timer, NULL);
	if (time_remaining < 0)
	{
		gtk_widget_destroy(main_window);
		return FALSE;
	}

	gint32 hour = time_remaining / 3600;
	gint32 minute = (time_remaining - (hour * 3600)) / 60;
	gint32 second = (time_remaining - (hour * 3600) - (minute * 60));

	const gchar * time_output = g_strdup_printf("%02d%02d%02d", hour, minute, second);

	for (int i = 0; i < 6; i++)
	{
		const gchar * file_name = g_strdup_printf("data/%c.png", time_output[i]);
		gtk_image_set_from_file(GTK_IMAGE(digits[i]), file_name);
		g_free( (gpointer)file_name );
	}

	g_free( (gpointer)time_output );

	return TRUE;
}

void on_pause_button_pressed(GtkWidget* widget, gpointer data)
{
	if (g_timer_is_active(timer))
	{
		g_timer_stop(timer);
		gtk_button_set_label( GTK_BUTTON(pause_button), "Resume");
	}
	else
	{
		g_timer_continue(timer);
		gtk_button_set_label(GTK_BUTTON(pause_button), "Pause");
	}
}

void on_quit_button_pressed(GtkWidget* widget, gpointer data)
{
	gtk_widget_destroy(main_window);
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

	timer = g_timer_new();

    gtk_init(&argc, &argv);

	create_window("Timer",265,113);
	create_grid(5);

	// ------------------------------------------------------------ Output label
	for (int i = 0, offset = 0; i < 6; i++)
	{
		if (i == 2 || i == 4)
		{
			offset++;
		}
		digits[i] = gtk_image_new_from_file("data/0.png");
		gtk_grid_attach(GTK_GRID(main_grid), digits[i], i+offset,0,1,1);		
	}

	colon_0 = gtk_image_new_from_file("data/colon.png");
	gtk_grid_attach(GTK_GRID(main_grid), colon_0, 2,0,1,1);

	colon_1 = gtk_image_new_from_file("data/colon.png");
	gtk_grid_attach(GTK_GRID(main_grid), colon_1, 5,0,1,1);

	pause_button = create_button("Pause", on_pause_button_pressed, 3,3,3,1);
	create_button("Quit", on_quit_button_pressed, 6,3,2,1);

	on_timer_timeout(NULL);

	g_timeout_add(1000, on_timer_timeout, NULL);

    gtk_widget_show_all(main_window);

    gtk_main();

	return 0;
}
