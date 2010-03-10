#ifndef MGTK_COMBOBOX_H
#define MGTK_COMBOBOX_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkComboBox : public MGtkItemBase
{
protected:
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  bool hasFrame;
  int numItems;
  bool editable;
  int charWidth;
  char** items;
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkComboBox():MGtkItemBase()
  {
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    hasFrame = true;
    numItems = -1;
    items = NULL;
    editable = false;
    charWidth = -1;
    this->type = COMBOBOX;  
  }
  ~MGtkComboBox()
  {
    if(items)
    {
      for(int i=0;i<numItems;i++)
        delete [] items[i];
      delete [] items;
    }
  }

  bool isEditable()	{return editable;}
  char** getItems()	{return items;}
  void setItems(char** i) {items = i;}
  int getNumItems()	{return numItems;}
  void setNumItems(int i) {numItems = i;}

  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
