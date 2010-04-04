#ifndef MGTK_LABEL_H
#define MGTK_LABEL_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkLabel : public MGtkItemBase
{
protected:
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  PangoEllipsizeMode ellipsizeMode;
  bool wrap;
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkLabel():MGtkItemBase()
  {
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    ellipsizeMode = PANGO_ELLIPSIZE_NONE;
    wrap = false;
    this->type = LABEL;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
