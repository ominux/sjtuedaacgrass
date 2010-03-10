#ifndef MGTK_WINDOW_H
#define MGTK_WINDOW_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"
#include "../MGUI/GUIContainer.hpp"

class MGtkWindow : public MGtkItemBase
{
protected:
  GtkWindowType gtkWinT;
  int winID;
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkWindow(int id):MGtkItemBase()
  {
    gtkWinT = GTK_WINDOW_TOPLEVEL;
    winID = id;
    this->type = WINDOW;  
  }
  void getXmlNodeProperties(xmlNode* node);
  //void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
  bool isTopLevel(void) {return gtkWinT == GTK_WINDOW_TOPLEVEL;}
  int getWinID(void)	{return winID;}
  /*
  MGtkWindow(GtkWidgetType type, GtkWidget* ptr = NULL,
	     int width = 0, int height = 0,
	     bool visibility = true,
	     char* label = NULL, MGtkEvent* events = NULL)
    :MGtkItemBase(type,ptr,width,height,visibility,label,events){}
    */
};

#endif
