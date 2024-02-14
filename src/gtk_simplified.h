extern GtkWidget * main_window;
extern GtkWidget * main_grid;

void create_window(gchar * title, gint x, gint y);
void create_grid(gint padding);
GtkWidget * create_button(const gchar * text, void (*function)(), gint x, gint y, gint w, gint h);
GtkWidget * create_entry(const gchar * text, void (*function)(), gint x, gint y, gint w, gint h);
GtkWidget * create_label(const gchar * text, gint x, gint y, gint w, gint h);
GtkWidget * create_text_view(gint size_x, gint size_y, GtkTextBuffer * buffer, gint x, gint y, gint w, gint h);
GtkWidget * create_spin_button(gdouble min, gdouble max, gdouble step, gint x, gint y, gint w, gint h);
GtkWidget * create_check_button(const gchar * text, gint x, gint y, gint w, gint h);
