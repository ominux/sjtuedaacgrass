#ifndef MGTK_TOOLITEM_H
#define MGTK_TOOLITEM_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class GUIContainer;
class MGtkToolItem : public MGtkItemBase
{
protected:
  int pos;
  char* tip;
  char* priTip;
  char* icon;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  char* highlightColor;
  bool draw;
  GtkWidget* image;
  int imageWidth;
  int imageHeight;
  GtkButton* button;
  bool isValidEvent(MGtkEvent* pevent);
  bool isValidIcon();
public:
  MGtkToolItem(): MGtkItemBase()
  {
    pos = -1;
    tip = new char[MAX_CHARS];
    priTip = new char[MAX_CHARS];
    icon = new char[MAX_CHARS];
    tip = "";
    priTip = "Private";
    icon = NULL;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    button = NULL;
    highlightColor = strdup("red");
    image =NULL;
    imageWidth = 0;
    imageHeight = 0;
    draw = true;
    this->type = TOOLITEM;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);

  char*         getHLColor()		{return highlightColor;}
  void		setHLColor(char* c)	{if(highlightColor) delete highlightColor;
                                         highlightColor = strdup(c);}
  GtkWidget*	getImage()		{return image;}
  int		getImageWidth()		{return imageWidth;}
  int		getImageHeight()	{return imageHeight;}

  /*
  bool isRES(){return label ? !strcmp(label, "RES") : false;}
  bool isCAP(){return label ? !strcmp(label, "CAP") : false;}
  bool isIND(){return label ? !strcmp(label, "IND") : false;}
  bool isVOL(){return label ? !strcmp(label, "VOL") : false;}
  bool isCUR(){return label ? !strcmp(label, "CUR") : false;}
  bool isFEL(){return label ? !strcmp(label, "FEL") : false;}
  bool isEEL(){return label ? !strcmp(label, "EEL") : false;}
  bool isHEL(){return label ? !strcmp(label, "HEL") : false;}
  bool isGEL(){return label ? !strcmp(label, "GEL") : false;}
  bool isDIO(){return label ? !strcmp(label, "DIO") : false;}
  bool isGND(){return label ? !strcmp(label, "GND") : false;}
  bool isMNF(){return label ? !strcmp(label, "MNF") : false;}
  bool isMPF(){return label ? !strcmp(label, "MPF") : false;}
  bool isWIRE(){return label ? !strcmp(label, "WIRE") : false;}
  */
};

#endif
