#include <iostream>
#include <math.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <unistd.h>

#include "paperSoccer.h"
#include "board.h"

int main(int argc, char *argv[])
{
    paperSoccer paperSoccerGame;
	// init graphics
    paperSoccer::init(argc, argv);
    // create GUI
    paperSoccer::initGUI();
	// run GTK loop
	gtk_widget_show_all(paperSoccer::window);
	gtk_main();

	return 0;
}
