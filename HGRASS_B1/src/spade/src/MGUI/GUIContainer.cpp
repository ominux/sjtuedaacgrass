#include "GUIContainer.hpp"

void GUIContainer::processNode(xmlNode * node, MGtkItemBase* gtkFaNode)
{
	xmlNode* curNode = NULL;

	for (curNode = node; curNode; curNode = curNode->next) 
	{
	  MGtkItemBase* gtkNode = NULL;
	  if (curNode->type == XML_ELEMENT_NODE) 
	  {
	    if(!xmlStrcmp(curNode->name, sWindow_GC))
	    {
	      gtkNode = new MGtkWindow(numWindows);
	      numWindows++;
	    }
	    else if(!xmlStrcmp(curNode->name, sBox_GC))
	      gtkNode = new MGtkBox();
	    else if(!xmlStrcmp(curNode->name, sMenuShell_GC))
	      gtkNode = new MGtkMenuShell();
	    else if(!xmlStrcmp(curNode->name, sMenu_GC))
	      gtkNode = new MGtkMenu();
	    else if(!xmlStrcmp(curNode->name, sTBar_GC))
	      gtkNode = new MGtkToolBar();
	    else if(!xmlStrcmp(curNode->name, sTItem_GC))
	    {
	      gtkNode = new MGtkToolItem();
	      gtkNode->addTItem((MGtkToolItem*)gtkNode);
	    }
	    else if(!xmlStrcmp(curNode->name, sFrame_GC))
	      gtkNode = new MGtkFrame();
	    else if(!xmlStrcmp(curNode->name, sButton_GC))
	      gtkNode = new MGtkButton();
	    else if(!xmlStrcmp(curNode->name, sLabel_GC))
	      gtkNode = new MGtkLabel();
	    else if(!xmlStrcmp(curNode->name, sTextEntry_GC))
	      gtkNode = new MGtkTextEntry();
	    else if(!xmlStrcmp(curNode->name, sComboBox_GC))
	      gtkNode = new MGtkComboBox();
	    //Add Your Class Object New Code Here
	    else if(!xmlStrcmp(curNode->name, sEMask_GC))
	    {
		if(gtkFaNode == NULL)
		{
		  printf("[Error] Parse XML Configuration Error, Event Found No Recognized Body!\n");
		  //ToDo, Release Memory
		  exit(0);
		}
		gtkFaNode->getXmlNodeEventMaskProperties(curNode, this);
	    }
	    else if(!xmlStrcmp(curNode->name, sEvent_GC))
	    {
	      if(gtkFaNode == NULL)
	      {
	        printf("[Error] Parse XML Configuration Error, Event Found No Recognized Body!\n");
		//ToDo, Release Memory
		exit(0);
	      }
	      gtkFaNode->getXmlNodeEventProperties(curNode, this);
	    }
	    if(gtkNode)
	    {
	      //printf("node type: Element, name: %s\n", curNode->name);
	      assert(numWindows > 0);
	      int curWindow = numWindows - 1;
	      gtkNode->setPtrFa(gtkFaNode);
	      items[curWindow][numItems[curWindow]] = gtkNode;
	      gtkNode->getXmlNodeProperties(curNode);
	      (numItems[curWindow])++;
	    }
	  }
	  processNode(curNode->children, gtkNode);
	}
}

int GUIContainer::doParseXML(void)
{
	pXmlDoc = xmlReadFile(XML_CONFIG,NULL,0);
	if(pXmlDoc == NULL)
	{
	  printf("[Error] XML Configure File Not Found!\n");
	  xmlFreeDoc(pXmlDoc);
	  exit(0);
	}
	xmlNode* root = xmlDocGetRootElement(pXmlDoc);
	if(xmlStrcmp(root->name, sGTK_GC))
	{
		printf("[Error] XML Parse Error, No GTK Definition Found!");
		xmlFreeDoc(pXmlDoc);
		exit(0);
	}
	processNode(root, NULL);
	xmlFreeDoc(pXmlDoc);
	return 1;
}

int GUIContainer::constructGUI(void)
{
	if(this->numWindows != 0)
	{
	  constructWindow(0);
	  enableWindow(0);
	  return 1;
	}
	return 0;
}

int GUIContainer::constructWindow(int index)
{
        assert(index < numWindows);
	assert((items[index][0]));
	assert((items[index][0])->isWindow());
	for(int i = 0; i < numItems[index]; i++)
	{
	  MGtkItemBase* pMGtkItem = this->items[index][i];
          if(pMGtkItem != NULL)
	  {
	    pMGtkItem->genGtkGUI();
	  }
	}
	//(items[index][0])->genGtkGUI();
	//MGtkWindow* p = (MGtkWindow*)items[index][0];
	//if(p->isTopLevel())
	  //gtk_main();
	return 1;
}

void GUIContainer::enableWindow(int index)
{
	MGtkWindow* p = (MGtkWindow*)items[index][0];
	if(p->isTopLevel())
	  gtk_main();
}

int GUIContainer::init(void)
{
	int res = 1;
	res &= doParseXML();
	res &= constructGUI();
	return res;
}

MGtkItemBase* GUIContainer::getMGtkItem(int windowID, int ID)
{
	if(ID == -1)
	  return NULL;
	if(windowID >= numWindows)
	  return NULL;

	for(int i=0;i<numItems[windowID]; i++)
	{
	  MGtkItemBase* pMGtkItem = this->items[windowID][i];
	  int tmp = pMGtkItem->getID();
	  if(tmp == -1)
	    continue;
	  if(tmp == ID)
	    return pMGtkItem;
	}
	return NULL;
}
