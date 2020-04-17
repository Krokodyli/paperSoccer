#include "paperSoccer.h"

// variables
GtkWidget* paperSoccer::window;
board paperSoccer::b;
int paperSoccer::startingPlayer = 1;
bool paperSoccer::playersAIEnabled[3] = {false, true, false};
bool paperSoccer::gamePaused = false;

// functions
void paperSoccer::init(int argc, char *argv[]){
    b = board();
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_timeout_add(paperSoccer::AIInterval, paperSoccer::AI_turn, window);
}

void paperSoccer::initGUI(){
    // create window elements
	GtkWidget* darea = gtk_drawing_area_new();
	GtkWidget* grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_grid_attach(GTK_GRID(grid), darea, 0, 1, 1, 1);

    GtkWidget* vBox =  gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget* menuBar = gtk_menu_bar_new();
	GtkWidget* optionItem = gtk_menu_item_new_with_mnemonic("Options");
	GtkWidget* optionsMenu = gtk_menu_new();
	GtkWidget* AIMenuItem = gtk_menu_item_new_with_mnemonic("AI");
	GtkWidget* AIMenu = gtk_menu_new();
	GtkWidget* AIPlayer1Item = gtk_check_menu_item_new_with_mnemonic("Set player 1 to AI");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(AIPlayer1Item), TRUE);
	GtkWidget* AIPlayer2Item = gtk_check_menu_item_new_with_mnemonic("Set player 2 to AI");
	GtkWidget* startingPlayerItem = gtk_menu_item_new_with_mnemonic("First move: Player 1");
	GtkWidget* quitMenuItem = gtk_menu_item_new_with_mnemonic("Quit");
	GtkWidget* newGameMenuItem = gtk_menu_item_new_with_mnemonic("New game");
	gtk_menu_shell_append(GTK_MENU_SHELL(AIMenu), AIPlayer1Item);
	gtk_menu_shell_append(GTK_MENU_SHELL(AIMenu), AIPlayer2Item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(AIMenuItem), AIMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), newGameMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), AIMenuItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), startingPlayerItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), quitMenuItem);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(optionItem), optionsMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), optionItem);
	gtk_box_pack_start(GTK_BOX(vBox), menuBar, TRUE, TRUE, 0);
	gtk_grid_attach(GTK_GRID(grid), vBox, 0, 0, 2, 1);

    // add listeners
    gtk_widget_set_events (darea, gtk_widget_get_events (darea)
		                     | GDK_BUTTON_PRESS_MASK);
	g_signal_connect(darea, "button_press_event", G_CALLBACK(paperSoccer::onClick), NULL);
	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(paperSoccer::onDraw), NULL);
	g_signal_connect(G_OBJECT(newGameMenuItem), "button_press_event", G_CALLBACK(paperSoccer::onNewGameButtonPress), NULL);
	g_signal_connect(G_OBJECT(AIPlayer1Item), "button_press_event", G_CALLBACK(paperSoccer::OnAIPlayer1ButtonPress), NULL);
	g_signal_connect(G_OBJECT(AIPlayer2Item), "button_press_event", G_CALLBACK(paperSoccer::OnAIPlayer2ButtonPress), NULL);
	g_signal_connect(G_OBJECT(startingPlayerItem), "button_press_event", G_CALLBACK(paperSoccer::toggleStartingPlayer), NULL);
	g_signal_connect(G_OBJECT(quitMenuItem), "button_press_event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK(paperSoccer::onKeyPress), NULL);

	// configure window
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), paperSoccer::cellsize*BOARDWIDTH+paperSoccer::boardOffsetX, paperSoccer::cellsize*BOARDHEIGHT+paperSoccer::boardOffsetY);
	gtk_window_set_title(GTK_WINDOW(window), "Paper Soccer");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_set_size_request(darea, paperSoccer::boardOffsetX+paperSoccer::cellsize*BOARDWIDTH, paperSoccer::cellsize*BOARDHEIGHT+paperSoccer::boardOffsetY);
}

void paperSoccer::drawBoard(cairo_t *cr){
	int cs = paperSoccer::cellsize;
	int box = paperSoccer::boardOffsetX;
	int boy = paperSoccer::boardOffsetY;

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_select_font_face(cr, "Monospace",
		  CAIRO_FONT_SLANT_NORMAL,
		  CAIRO_FONT_WEIGHT_BOLD);

	cairo_set_font_size(cr, paperSoccer::playerTextFont);

	if(b.getTurn() == 1){
		cairo_move_to(cr, box + cs*BOARDGOALWIDTH/2 + cs*BOARDWIDTH/2 + cs, boy+cs/2);
		cairo_show_text(cr, " #2");
		cairo_move_to(cr, box + cs*BOARDGOALWIDTH/2 + cs*BOARDWIDTH/2 + cs, boy+(BOARDHEIGHT-2)*cs+2*cs/3);
		cairo_show_text(cr, ">#1");
	}
	else{
		cairo_move_to(cr, box + cs*BOARDGOALWIDTH/2 + cs*BOARDWIDTH/2 + cs, boy+cs/2);
		cairo_show_text(cr, ">#2");
		cairo_move_to(cr, box + cs*BOARDGOALWIDTH/2 + cs*BOARDWIDTH/2 + cs, boy+(BOARDHEIGHT-2)*cs+2*cs/3);
		cairo_show_text(cr, " #1");
	}

	// Big rectangle
	cairo_set_source_rgb(cr, 0, 0.5, 0);
	cairo_rectangle(cr, box+0, boy+50, 8*cs, 10*cs);
	cairo_stroke_preserve(cr);

	// Upper goal
	cairo_rectangle(cr, box+3*cs, boy, 2*cs, cs);

	// Lower goal
	cairo_rectangle(cr, box+3*cs, boy+11*cs, 2*cs, cs);
	cairo_fill(cr);
	cairo_stroke_preserve(cr);

	// dots
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_set_line_width(cr, 2);
	for(int y = 1; y < BOARDHEIGHT - 1; y++){
		for(int x = 1; x < BOARDWIDTH; x++){
			cairo_rectangle(cr, box+x*cs, boy+y*cs, 1, 1);
			cairo_fill(cr);
		}
	}

	// lines
	for(int y = 0; y < BOARDHEIGHT; y++){
		for(int x = 0; x < BOARDWIDTH; x++){
			int pos = y*BOARDWIDTH+x;
			cairo_move_to(cr, box+cs*x, boy+cs*y);
			if(b(pos, 1) == 1){
				cairo_line_to(cr, box+cs*x-cs, boy+cs*y-cs);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 2) == 1){
				cairo_line_to(cr, box+cs*x, boy+cs*y-cs);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 3) == 1){
				cairo_line_to(cr, box+cs*x+cs, boy+cs*y-cs);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 4) == 1){
				cairo_line_to(cr, box+cs*x-cs, boy+cs*y);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 6) == 1){
				cairo_line_to(cr, box+cs*x+cs, boy+cs*y);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 7) == 1){
				cairo_line_to(cr, box+cs*x-cs, boy+cs*y+cs);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 8) == 1){
				cairo_line_to(cr, box+cs*x, boy+cs*y+cs);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
			if(b(pos, 9) == 1){
				cairo_line_to(cr, box+cs*x+cs, boy+cs*y+cs);
				cairo_move_to(cr, box+cs*x, boy+cs*y);
			}
		}
	}
	cairo_stroke(cr);

	// last moves
	cairo_set_line_width(cr, 4);
	cairo_set_source_rgb(cr, 1, 1, 1);
	int pos = b.getLastMovePosition();
	for(int i = 0; i < b.getLastMovesListSize(); i++){
		int lx = pos % BOARDWIDTH;
		int ly = pos / BOARDWIDTH;
		cairo_move_to(cr, box+cs*lx, boy+cs*ly);
		int nPos = b.modifyPos(pos, b.getLastMoves(i));
		int nx = nPos % BOARDWIDTH;
		int ny = nPos / BOARDWIDTH;
		cairo_line_to(cr, box+cs*nx, boy+cs*ny);
		pos = nPos;
	}
	cairo_stroke(cr);

	// ball
	pos = b.getCurrentPosition();
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_arc(cr, box + cs*(pos%BOARDWIDTH), boy + cs*(pos/BOARDWIDTH), 5, 0, 2*G_PI);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}

void paperSoccer::messageDialogBox (GtkWindow *parent, char* title, char *message){
	GtkWidget *dialog, *label, *content_area;
	GtkDialogFlags flags;
	gamePaused = true;
	flags = GTK_DIALOG_DESTROY_WITH_PARENT;
	dialog = gtk_dialog_new_with_buttons (title, parent, flags, ("_OK"), GTK_RESPONSE_NONE, NULL);
    gtk_widget_set_size_request(dialog, 120, 50);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	label = gtk_label_new (message);
	g_signal_connect_swapped (dialog, "response", G_CALLBACK(paperSoccer::onDialogBoxResponse), dialog);
	gtk_container_add (GTK_CONTAINER (content_area), label);
	gtk_widget_show_all (dialog);
}

void paperSoccer::showDialogBox(GtkWindow* window, int win){
	if(win == 0) return;
	if(win == 1){
		messageDialogBox(window, "Paper soccer", "Player 1 won");
	}
	else {
		messageDialogBox(window, "Paper soccer", "Player 2 won");
	}
}

gboolean paperSoccer::AI_turn(gpointer user_data){
	if(!gamePaused){
		if(playersAIEnabled[b.getTurn()])
            paperSoccer::b.moveBall(b.getBestMove(paperSoccer::AIDepth));
		gtk_widget_queue_draw((GtkWidget*)user_data);
		int win = b.checkForWin();
		showDialogBox(GTK_WINDOW(user_data), win);
	}
	return TRUE;
}

gboolean paperSoccer::onDialogBoxResponse(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	gamePaused = false;
	gtk_widget_destroy(widget);
	b.setupBoard();
	return FALSE;
}

gboolean paperSoccer::onKeyPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	int dir;
	switch(event->keyval){
	case 'q':
		dir = 1;
		break;
	case 'w':
		dir = 2;
		break;
	case 'e':
		dir = 3;
		break;
	case 'a':
		dir = 4;
		break;
	case 'd':
		dir = 6;
		break;
	case 'z':
		dir = 7;
		break;
	case 'x':
		dir = 8;
		break;
	case 'c':
		dir = 9;
		break;
	default:
		dir = event->keyval-'0';
	}
	if(dir > 0 and dir <= 9 and dir != 5){
		if(!gamePaused and !playersAIEnabled[b.getTurn()]){
			b.moveBall(dir);
			int win = b.checkForWin();
			showDialogBox(GTK_WINDOW(widget), win);
			gtk_widget_queue_draw(widget);
		}
	}
	return FALSE;
}


gboolean paperSoccer::onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
	int possMoves[10];
	for(int i = 1; i <= 9; i++)
		if(i != 5) possMoves[i] = b.modifyPos(b.getBallPos(), i);

	for(int i = 1; i <= 9; i++){
		if(i == 5) continue;
		int dx = abs(possMoves[i]%BOARDWIDTH*paperSoccer::cellsize - event->x + paperSoccer::boardOffsetX);
		int dy = abs(possMoves[i]/BOARDWIDTH*paperSoccer::cellsize - event->y + paperSoccer::boardOffsetY);
		float distance = sqrt(dx*dx+dy*dy);
		if(distance < paperSoccer::cellsize/4 and !gamePaused and !playersAIEnabled[b.getTurn()]){
            b.moveBall(i);
            int win = b.checkForWin();
            showDialogBox(GTK_WINDOW(widget), win);
            gtk_widget_queue_draw(widget);
        }
	}
	return FALSE;
}

gboolean paperSoccer::onNewGameButtonPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	b.setupBoard();
	return FALSE;
}

gboolean paperSoccer::toggleStartingPlayer(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	startingPlayer = startingPlayer==1?2:1;
	if(startingPlayer == 1)
		gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "First move: Player 1");
	else
		gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "First move: Player 2");
	b.setStartingPlayer(startingPlayer);
	return FALSE;
}

gboolean paperSoccer::OnAIPlayer1ButtonPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
	playersAIEnabled[1] = (active?false:true);
	return FALSE;
}

gboolean paperSoccer::OnAIPlayer2ButtonPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
	playersAIEnabled[2] = (active?false:true);
	return FALSE;
}

gboolean paperSoccer::onDraw(GtkWidget *widget, cairo_t *cr, gpointer user_data){
	drawBoard(cr);
	return FALSE;
}
