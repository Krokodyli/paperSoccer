#include <iostream>
#include <math.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "board.h"
#include <unistd.h>

class paperSoccer{
public:
    static const int AIInterval = 500;
    static const int AIDepth = 7;
    static const int cellsize = 50;
    static const int boardOffsetX = 50;
    static const int boardOffsetY = 50;
    static const int playerTextFont = 20;

    static GtkWidget* window;
    static board b;
    static int startingPlayer;
    static bool playersAIEnabled[3];
    static bool gamePaused;

    static void init(int argc, char* argv[]);
    static void initGUI();
    static void drawBoard(cairo_t *);
    static void messageDialogBox(GtkWindow *parent, gchar* title,  gchar *message);
    static void showDialogBox(GtkWindow* window, int win);
    static gboolean AI_turn(gpointer user_data);

    static gboolean onDialogBoxResponse(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
    static gboolean onKeyPress (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
    static gboolean onClick(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
    static gboolean onNewGameButtonPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
    static gboolean toggleStartingPlayer(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
    static gboolean OnAIPlayer1ButtonPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
    static gboolean OnAIPlayer2ButtonPress(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
    static gboolean onDraw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
};
