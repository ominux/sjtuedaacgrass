#ifndef MGTK_FRAME_H
#define MGTK_FRAME_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkFrame : public MGtkItemBase
{
protected:
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  float xAlign;
  float yAlign;
  GtkShadowType shadowType;
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkFrame():MGtkItemBase()
  {
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    xAlign = 0.0;
    yAlign = 0.0;
    shadowType = GTK_SHADOW_IN;
    this->type = FRAME;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
