#ifndef GUI_CONTAINER
#define GUI_CONTAINER

extern "C" 
{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <assert.h>
	#include <gtk/gtk.h>
	#include <X11/Xlib.h>
}

#include <map>

using namespace std;
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "../MGtkItem/MGtkItemBase.hpp"
#include "../MGtkItem/MGtkWindow.hpp"
#include "../MGtkItem/MGtkBox.hpp"
#include "../MGtkItem/MGtkFrame.hpp"
#include "../MGtkItem/MGtkMenuShell.hpp"
#include "../MGtkItem/MGtkMenu.hpp"
#include "../MGtkItem/MGtkToolBar.hpp"
#include "../MGtkItem/MGtkToolItem.hpp"
#include "../MGtkItem/MGtkLabel.hpp"
#include "../MGtkItem/MGtkButton.hpp"
#include "../MGtkItem/MGtkTextEntry.hpp"
#include "../MGtkItem/MGtkComboBox.hpp"


#include "../defs.h"

class GUIContainer
{
private:
	xmlDocPtr pXmlDoc;
	map<int, MGtkItemBase*> *items;
	int* numItems;
	int numWindows;
        char* pConfPath;

	void processNode(xmlNode * node, MGtkItemBase* gtkFaNode);
public:
	GUIContainer()
	{
	  pConfPath = NULL;
	  numItems = new int[MAX_WINDOWS];
	  for(int i = 0; i < MAX_WINDOWS; i++)
	    numItems[i] = 0;
	  numWindows = 0;
	  items = new map<int, MGtkItemBase*>[MAX_WINDOWS];
	}
	~GUIContainer()
	{
	  if(numWindows > 0)
	  {
	    assert(items);
	    for(int i = 0; i < numWindows; i++)
	      deleteWindow(i);
	    delete [] items;
	    items = NULL;
	  }
	  numWindows = 0;
	  if(pConfPath)
	    delete [] pConfPath;
	}

	void deleteWindow(int index)
	{
	  if(numItems[index] > 0)
	  {
	    assert(items[index][0]);
	    for(int i = 0; i < numItems[index]; i++)
	    {
	      if(items[index][i])
	        delete items[index][i];
	      items[index][i] = NULL;
	    }
	  }
	  numItems[index] = 0;
	}

	int getNumWindows()	{return numWindows;}
	char* getConfPath()	{return pConfPath;}
	void setConfPath(char* p)	{pConfPath = p;}

	int init(void);
	int doParseXML(void);
	int constructGUI(void); 
	int constructWindow(int index);
	void enableWindow(int index);
	MGtkItemBase* getMGtkItem(int windowID, int ID);
};

#endif
