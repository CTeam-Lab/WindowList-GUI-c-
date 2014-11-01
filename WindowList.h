#ifndef _WINDOWLIST_H_
#define _WINDOWLIST_H_

#include <glib.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <cstdlib>
#include <string>
#include "WindowsHelper.h"

using namespace std;

typedef struct {
		Display *disp;
		long unsigned window;
} WindowInfo;

class WindowList
{
	public:
		static gboolean key_event(GtkWidget *widget, GdkEventKey *event);
		static void print_hello (GtkWidget *widget, WindowInfo data);
		void create();
		void CreateMainWindow();
		void addButtons();

	private:
		Display *disp;
		GtkWidget *window;
		GtkWidget *button;
		GtkWidget *grid;
		GtkWidget *notebook;
		GtkWidget *image;
		GtkWidget *Hbox, *mainBox;
		GtkWidget *label;
		WindowsHelper helper;
		char bufferf[32];
		char bufferl[32];
		long signed desktop_id;
		string title;

};

#endif
