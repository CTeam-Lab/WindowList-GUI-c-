#include "WindowList.h"

gboolean WindowList::key_event(GtkWidget *widget, GdkEventKey *event)
{
	if (event->keyval == GDK_KEY_Escape){
		gtk_main_quit();
	}
    return FALSE;
}

void WindowList::print_hello (GtkWidget *widget, WindowInfo data)
{
	WindowsHelper helper;

	//activate_window(data.disp, data.window);

	//helper.client_msg(data.disp, win, "_NET_ACTIVE_WINDOW",0, 0, 0, 0, 0);
	//helper.client_msg(data.disp, data.window, "_NET_ACTIVE_WINDOW",0, 0, 0, 0, 0);
}

void WindowList::create()
{
		vector<Window> v_wClientList;

		if (! (disp = XOpenDisplay(NULL))) {
				fputs("Cannot open display.\n", stderr);
				return;
		}

		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

		gtk_window_set_decorated(GTK_WINDOW(window), false);
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		gtk_window_set_title(GTK_WINDOW(window), "WindowListGui");

		g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
		g_signal_connect(window, "key-release-event", G_CALLBACK(key_event), NULL);

		/*
		g_signal_connect (window, "button_press_event", G_CALLBACK (menu_popup), NULL);
		g_signal_connect (window, "visibility_notify_event", G_CALLBACK (window_visibility_event), NULL);
		*/

		grid = gtk_grid_new();
		gtk_container_add (GTK_CONTAINER (window), grid);

		WindowInfo windowinfo;

		helper.GetClientList(disp, v_wClientList);

		mainBox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

		for ( auto &i : v_wClientList ) {

		   helper.GetWindowDesktop(disp, i, desktop_id);

		   if(desktop_id == -1){
				continue;
		   }

		   helper.GetWindowTitle(disp, i, title);

		   windowinfo.disp = disp;
		   windowinfo.window =  i;

		   /*image = Gtk.Image()
		           image.set_from_pixbuf(wnck_window.get_icon())

		           btn1 = Gtk.Button('')
		           btn1.set_tooltip_text (win_title);
		           btn1.set_always_show_image(True)
		           btn1.set_image(image)
		           btn1.win_id = win_id*/

		   button = gtk_button_new_with_label (title.c_str());
		   gtk_widget_set_size_request(button, 50,50);
		   //g_signal_connect (button, "clicked", G_CALLBACK (print_hello), &windowinfo);
		   gtk_container_add (GTK_CONTAINER (mainBox), button);
		}

		gtk_grid_attach (GTK_GRID (grid), mainBox, 0, 6, GTK_POS_RIGHT, 1);
		gtk_widget_show_all(window);
}

void WindowList::CreateMainWindow()
{

}

void WindowList::addButtons()
{

}
