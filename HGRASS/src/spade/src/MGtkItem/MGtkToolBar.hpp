#ifndef MGTK_TOOLBAR_H
#define MGTK_TOOLBAR_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkToolBar : public MGtkItemBase
{
protected:
  GtkOrientation orien; //GTK_ORIENTATION_HORIZONTAL | GTK_ORIENTATION_VERTICAL
  GtkToolbarStyle style; //GTK_TOOLBAR_ICONS | GTK_TOOLBAR_TEXT |
			 //GTK_TOOLBAR_BOTH  | GTK_TOOLBAR_BOTH_HORIZ
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkToolBar():MGtkItemBase()
  {
    orien = GTK_ORIENTATION_HORIZONTAL;
    style = GTK_TOOLBAR_BOTH;
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    this->type = TOOLBAR;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
