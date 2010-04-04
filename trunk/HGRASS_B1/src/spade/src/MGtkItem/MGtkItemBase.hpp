#ifndef MGTK_ITEM_BASE_H
#define MGTK_ITEM_BASE_H

#include "../defs.h"
extern "C"
{
	#include <assert.h>
	#include <stdio.h>
	#include <gtk/gtk.h>
	#include <string.h>
}
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "MGtkEventHandler.hpp"
class GUIContainer;
class MGtkItemBase;
class MGtkToolItem;

enum GtkWidgetType 
{
  BASE, 
  WINDOW, 
  BOX, 
  BUTTON, 
  MENUSHELL,
  TOOLBAR,
  TOOLITEM,
  MENU, 
  ICON,
  FRAME,
  LABEL,
  TEXTENTRY,
  COMBOBOX,
  TEMPLATE,
};
class GtkCFData
{
public:
	GUIContainer* pcon;
	MGtkItemBase* pmGtk;
	void *data;
	int numID;
	int* interactiveID;

	GtkCFData()
	{
	  pcon = NULL;
	  data = NULL;
	  pmGtk = NULL;
	  numID = 0;
	  interactiveID = NULL;
	}
	GtkCFData(GUIContainer* c, MGtkItemBase* p, void *d, int nID = 0, int* ID = NULL)
	{
	  pcon = c;
	  data = d;
	  pmGtk = p;
	  numID = nID;
	  interactiveID = ID;
	}
	~GtkCFData()
	{
	  if(interactiveID)
	  {
	    if(numID == 1)
	      delete interactiveID;
	    else if(numID != 0)
	      delete [] interactiveID;
	  }
	}
};
class MGtkEvent
{
public:
	char* event;
	char* func;
	GtkCFData* mData;

	MGtkEvent()
	{
	  event = NULL;
	  func = NULL;
	  //mData = new GtkCFData();
	  mData = NULL;
	}
};

class MGtkItemBase
{
protected:
        int id;
        bool isFixedSize;
	GtkWidgetType type;
	GtkWidget* ptr;
	MGtkItemBase* pFa;
	GtkStateType widgetState;
	int width;
	int height;
	int borderWidth;
	int borderHeight;
	bool visibility;
	bool responseEnable;
	char* label;
	MGtkEvent** events;
	int numEvents;
	bool buttonPressMasked;
	bool buttonReleaseMasked;
	bool keyPressMasked;
	bool keyReleaseMasked;
	bool pointerMotionMasked;
	static MGtkToolItem** pTItem;
	static int numTItem;

	virtual bool isValidEvent(MGtkEvent* pevent) = 0;

public:
	MGtkItemBase()
	{
	  id = -1;
	  isFixedSize = false;
	  type = BASE;
	  ptr = NULL;
	  pFa = NULL;
	  width = 0;
	  height = 0;
	  borderWidth = 0;
	  borderHeight = 0;
	  visibility = true;
	  responseEnable = true;
	  label = new char [MAX_CHARS];
	  events = new MGtkEvent* [MAX_EVENTS];
	  numEvents = 0;
	  widgetState = GTK_STATE_NORMAL;
	}
	virtual ~MGtkItemBase()
	{
	  ptr = NULL;
	  pFa = NULL;
	  width = 0;
	  height = 0;
	  visibility = true;
	  if(label)
	    delete [] label;
	  label = NULL;
	  if(events)
	  {	    
	    MGtkEvent** tmp = events;
	    for(int i = 0;tmp && i < numEvents; tmp++,i++)
		if(*tmp)
		{
		  if((*tmp)->mData)
		  {
		    delete (*tmp)->mData;
		    (*tmp)->mData = NULL;
		  }
		  delete *tmp;
		  *tmp = NULL;
		}
	    delete [] events;
	  }
	  events = NULL;
	  numEvents = 0;
	}
	virtual void getXmlNodeProperties(xmlNode* node)=0;
	void getXmlNodeEventProperties(xmlNode* node,GUIContainer* containers);
	void getXmlNodeEventMaskProperties(xmlNode* node,GUIContainer* containers);
	virtual void genGtkGUI(void)=0;

	int		getID()				{return id;}
	void		setPtrFa(MGtkItemBase* p)	{pFa = p;}
	GtkWidget*	getGtkWidgetPtr()		{return ptr;}
	GtkStateType	getWidgetState()		{return widgetState;}
	void		setWidgetState(GtkStateType s)	{widgetState = s;}
static int getNumTItem();
static void setNumTItem(int n);
static MGtkToolItem** getTItemHead();
static void addTItem(MGtkToolItem* p);
//static MGtkToolItem* findTItem(MEleType sType);

	bool isWindow()			{return type == WINDOW;}
	bool isContainer()		{return type == WINDOW || type == FRAME || 0 /*ToDo*/;}
	bool isBox()			{return type == BOX;}
	bool isMenu()			{return type == MENU;}
	bool isMenuShell()		{return type == MENUSHELL;}
	bool isToolBar()		{return type == TOOLBAR;}
	bool isToolItem()		{return type == TOOLITEM;}
	bool isFrame()			{return type == FRAME;}
	bool isButton()			{return type == BUTTON;}
	bool isLabel()			{return type == LABEL;}
	bool isTextEntry()		{return type == TEXTENTRY;}
	bool isComboBox()		{return type == COMBOBOX;}
	bool isTemplate()		{return type == TEMPLATE;}

	bool isButtonPressMasked()	{return buttonPressMasked;}
	bool isBUttonReleaseMasked()	{return buttonReleaseMasked;}
	bool isKeyPressMasked()		{return keyPressMasked;}
	bool isKeyReleaseMasked()	{return keyReleaseMasked;}
	bool isPointerMotionMasked()	{return pointerMotionMasked;}
};

#endif
