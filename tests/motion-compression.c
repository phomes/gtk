#include <gtk/gtk.h>
#include <math.h>

GtkAdjustment *adjustment;
int cursor_x, cursor_y;

static void
on_motion_notify (GtkWidget      *window,
                  GdkEventMotion *event)
{
  if (event->window == gtk_widget_get_window (window))
    {
      float processing_ms = gtk_adjustment_get_value (adjustment);
      g_usleep (processing_ms * 1000);
      cursor_x = event->x;
      cursor_y = event->y;
      gtk_widget_queue_draw (window);
    }
}

static void
on_draw (GtkDrawingArea *da,
         cairo_t        *cr,
         int             width,
         int             height,
         gpointer        data)
{
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_set_source_rgb (cr, 0, 0.5, 0.5);

  cairo_arc (cr, cursor_x, cursor_y, 10, 0, 2 * M_PI);
  cairo_stroke (cr);
}

int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *scale;
  GtkWidget *da;

  gtk_init ();

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
  gtk_widget_add_events (window, GDK_POINTER_MOTION_MASK);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  adjustment = gtk_adjustment_new (20, 0, 200, 1, 10, 0);
  scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adjustment);
  gtk_box_pack_end (GTK_BOX (vbox), scale, FALSE, FALSE);

  label = gtk_label_new ("Event processing time (ms):");
  gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
  gtk_box_pack_end (GTK_BOX (vbox), label, FALSE, FALSE);

  da = gtk_drawing_area_new ();
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (da), on_draw, NULL, NULL);
  gtk_box_pack_end (GTK_BOX (vbox), da, TRUE, TRUE);
  
  g_signal_connect (window, "motion-notify-event",
                    G_CALLBACK (on_motion_notify), NULL);
  g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);

  gtk_widget_show (window);
  gtk_main ();

  return 0;
}
