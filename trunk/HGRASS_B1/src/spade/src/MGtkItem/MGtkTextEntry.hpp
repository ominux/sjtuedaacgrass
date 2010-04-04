#ifndef MGTK_TEXTENTRY_H
#define MGTK_TEXTENTRY_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkTextEntry : public MGtkItemBase
{
protected:
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  int charWidth;
  int maxLength;
  bool hasFrame;
  bool editable;
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkTextEntry():MGtkItemBase()
  {
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    charWidth = -1;
    maxLength = 0;
    hasFrame = true;
    editable = true;
    this->type = TEXTENTRY;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
