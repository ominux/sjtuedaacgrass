#ifndef MGTK_MENUSHELL_H
#define MGTK_MENUSHELL_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkMenuShell : public MGtkItemBase
{
protected:
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkMenuShell():MGtkItemBase()
  {
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    this->type = MENUSHELL;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
