
#include "MGtkItemBase.hpp"
#include "MGtkToolItem.hpp"

int MGtkItemBase::numTItem = 0;
MGtkToolItem** MGtkItemBase::pTItem = new MGtkToolItem* [MAX_TOOLITEM];

int MGtkItemBase::getNumTItem(){return numTItem;}

void MGtkItemBase::setNumTItem(int n){numTItem = n;}

MGtkToolItem** MGtkItemBase::getTItemHead(){return pTItem;}

void MGtkItemBase::addTItem(MGtkToolItem* p){pTItem[numTItem++]=p;}

/*
MGtkToolItem* MGtkItemBase::findTItem(MEleType sType)
{
  switch(sType)
  {
    case RESISTOR:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isRES()) return pTItem[i]; break;
    case CAPACITANCE:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isCAP()) return pTItem[i]; break;
    case INDUCTOR:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isIND()) return pTItem[i]; break;
    case VOLTAGES:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isVOL()) return pTItem[i]; break;
    case CURRENTS:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isCUR()) return pTItem[i]; break;
    case DIODE:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isDIO()) return pTItem[i]; break;
    case VCVS:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isEEL()) return pTItem[i]; break;
    case CCCS:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isFEL()) return pTItem[i]; break;
    case VCCS:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isGEL()) return pTItem[i]; break;
    case CCVS:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isHEL()) return pTItem[i]; break;
    case NMOSFET:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isMNF()) return pTItem[i]; break;
    case PMOSFET:	
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isMPF()) return pTItem[i]; break;
    case GND:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isGND()) return pTItem[i]; break;
    case WIRE:		
      for(int i=0;i<numTItem;i++) if(pTItem[i]->isWIRE()) return pTItem[i]; break;
    default: return pTItem[0]; break;
  }
}
*/
void MGtkItemBase::getXmlNodeEventProperties(xmlNode* node, GUIContainer* containers)
{
	assert(events);
	assert(numEvents < MAX_EVENTS);
	xmlAttrPtr attPtr = node->properties;
	MGtkEvent* pe = NULL;
	if(attPtr != NULL)
	  pe = new MGtkEvent();
	while(attPtr != NULL)
	{
	  xmlChar* value;
	  if(attPtr->name != NULL)
	    value = xmlGetProp(node,attPtr->name);
	
	  if(!xmlStrcmp(attPtr->name, sEName_IB))
	    pe->event = (char*)(xmlStrdup(value));
	  else if(!xmlStrcmp(attPtr->name, sEHandler_IB))
	    pe->func = (char*)(xmlStrdup(value));
	  else if(!xmlStrcmp(attPtr->name, sEData_IB))
	  {
	    void* pdata = NULL;
	    if(!xmlStrcmp(value, BAD_CAST "NULL"))
	      pdata = (void*)NULL;
	    else
	      pdata = (void*)(xmlStrdup(value));        
	    if(pe->mData)
	      pe->mData->data = pdata;
	    else
	      pe->mData = new GtkCFData(containers, this, pdata);
	  }
	  else if(!xmlStrcmp(attPtr->name, sENumID_IB))
	  {
	    int nID = atoi((char*)value);
	    if(pe->mData)
	      pe->mData->numID = nID;
	    else
	      pe->mData = new GtkCFData(containers, this, NULL, nID);
	  }
	  else if(!xmlStrcmp(attPtr->name, sEID_IB))
	  {
	    int* pID = NULL;
	    int nID = 1;
	    if(!pe->mData || pe->mData->numID == 0)
	    {
	      pID = new int [1];
	      pID[0] = atoi((char*)value);
	    }
	    else
	    {
	      nID = pe->mData->numID;
	      pID = new int [nID];
	      char* idList = (char*)value;
	      for(int i=0;i<nID;)
	      {
		char* tmp = strsep(&idList," ,");
		if(strcmp(tmp,""))
		{
	          pID[i] = atoi(tmp);
		  i++;
		}
	      }
	    }
	    if(pe->mData)
	    {
	      pe->mData->numID = nID;
	      pe->mData->interactiveID = pID;
	    }
	    else
	    {
	      pe->mData = new GtkCFData(containers, this, NULL, nID, pID);
	    }
	  }

	  if(value != NULL)
	    xmlFree(value);
	  attPtr = attPtr->next;
	}
	
	if(pe != NULL)
	{
	  if(isValidEvent(pe))
	  {
	    events[numEvents] = pe;
	    numEvents++;
	  }
	  else
	    delete pe;
	}
}

void MGtkItemBase::getXmlNodeEventMaskProperties(xmlNode* node, GUIContainer* containers)
{
	xmlAttrPtr attPtr = node->properties;
	while(attPtr != NULL)
	{
	  xmlChar* value;
	  if(attPtr->name != NULL)
	    value = xmlGetProp(node,attPtr->name);
	
	  if(!xmlStrcmp(attPtr->name, sEMType_IB))
	  {
	    assert(this->ptr);
	    if(!xmlStrcmp(value, BAD_CAST "GDK_BUTTON_PRESS_MASK"))
	    { 
	      buttonPressMasked = true;
	      gtk_widget_add_events(this->ptr, GDK_BUTTON_PRESS_MASK);
	    }
	    else if(!xmlStrcmp(value, BAD_CAST "GDK_BUTTON_RELEASE_MASK"))
	    {
	      buttonReleaseMasked = true;
	      gtk_widget_add_events(this->ptr, GDK_BUTTON_RELEASE_MASK);
	    }
	    else if(!xmlStrcmp(value, BAD_CAST "GDK_KEY_PRESS_MASK"))
	    {
	      keyPressMasked = true;
              GTK_WIDGET_SET_FLAGS(this->ptr, GTK_CAN_FOCUS); 
	      gtk_widget_add_events(this->ptr, GDK_KEY_PRESS_MASK);
	    }
	    else if(!xmlStrcmp(value, BAD_CAST "GDK_KEY_RELEASE_MASK"))
	    {
	      keyReleaseMasked = true;
              GTK_WIDGET_SET_FLAGS(this->ptr, GTK_CAN_FOCUS); 
	      gtk_widget_add_events(this->ptr, GDK_KEY_RELEASE_MASK);
	    }
            else if(!xmlStrcmp(value, BAD_CAST "GDK_POINTER_MOTION_MASK"))
	    {
	      pointerMotionMasked = true;
              GTK_WIDGET_SET_FLAGS(this->ptr, GTK_CAN_FOCUS); 
	      gtk_widget_add_events(this->ptr, GDK_POINTER_MOTION_MASK);
	    }
	  }
	  if(value != NULL)
	    xmlFree(value);
	  attPtr = attPtr->next;
	}
}
