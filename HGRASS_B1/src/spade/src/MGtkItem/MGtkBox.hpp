#ifndef MGTK_BOX_H
#define MGTK_BOX_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkBox : public MGtkItemBase
{
protected:
  int dir;	//dir = 1 == VBOX | dir = 0 == HBOX
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkBox():MGtkItemBase()
  {
    dir = 1;
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    this->type = BOX;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
  bool isHBox(void)
  {
    return dir == 1;
  }
};

#endif
