#ifndef MGTK_BUTTON_H
#define MGTK_BUTTON_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkButton : public MGtkItemBase
{
protected:
  bool mnemonic;
  char* stock;
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  GtkReliefStyle relief;
#ifdef GTK2_4
  float xAlign;
  float yAlign;
#endif
#ifdef GTK2_6
  GtkWidget* image;
  GtkPositionType imagePosition;
#endif
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkButton():MGtkItemBase()
  {
    mnemonic = false;
    stock = NULL;
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    relief = GTK_RELIEF_NORMAL;
#ifdef GTK2_4
    xAlign = 0.5;
    yAlign = 0.5;
#endif
#ifdef GTK2_6
    image = NULL;
    imagePosition = GTK_POS_LEFT;
#endif
    this->type = BUTTON;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
