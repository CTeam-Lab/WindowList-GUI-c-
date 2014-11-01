#include "WindowList.h"
#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	WindowList gui;
	gtk_init (&argc, &argv);
	gui.create();
	gtk_main();
	return 0;
}
