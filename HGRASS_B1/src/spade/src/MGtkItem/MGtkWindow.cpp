
#include "MGtkWindow.hpp"

void MGtkWindow::getXmlNodeProperties(xmlNode* node)
{
  xmlAttrPtr attPtr = node->properties;
  while(attPtr != NULL)
  {
    xmlChar* value;
    if(attPtr->name != NULL)
      value = xmlGetProp(node, attPtr->name);
    if(!xmlStrcmp(attPtr->name, sWidth_IB))
      this->width = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sHeight_IB))
      this->height = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sID_IB))
      this->id = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sFixSize_IB))
      this->isFixedSize = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sVisibility_IB))
      this->visibility = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sResEnable_IB))
      this->responseEnable = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sLabel_IB))
      this->label = (char*)xmlStrdup(value);
    else if(!xmlStrcmp(attPtr->name, sWinType_IB))
      this->gtkWinT = !xmlStrcmp(value, BAD_CAST "Popup") ? GTK_WINDOW_POPUP : GTK_WINDOW_TOPLEVEL;
    else if(!xmlStrcmp(attPtr->name, sBorderW_IB))
      this->borderWidth = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sBorderH_IB))
      this->borderHeight = atoi((char*)value);

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}
bool MGtkWindow::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  if(  strcmp(pevent->event,"destroy")
     //&&strcmp(pevent->event,"configure_event")  
     //&&strcmp(pevent->event,"expose_event")  
     //&&(strcmp(pevent->event,"button_press_event") || !buttonPressMasked) 
     //&&(strcmp(pevent->event,"button_release_event") || !buttonReleaseMasked)    
     //&&(strcmp(pevent->event,"key_press_event") || !keyPressMasked) 
     //&&(strcmp(pevent->event,"key_release_event") || !keyReleaseMasked)    
     //&&strcmp(pevent->event,"key_press_event")
     //&&strcmp(pevent->event,"key_release_event")
     //&&(strcmp(pevent->event,"motion_notify_event") || !pointerMotionMasked)
     &&1/*ToDo*/)
    return false;
  if(  strcmp(pevent->func,"MGtkDestroy")
/*     &&strcmp(pevent->func,"MGtkKeyPressed")
     &&strcmp(pevent->func,"MGtkKeyReleased")*/
       /*Diming Ma: ToDo: Add Default Function, NYI
     &&strcmp(pevent->func,"")
     &&strcmp(pevent->func,"configure_event")
     &&strcmp(pevent->func,"configure_event")
     &&strcmp(pevent->func,"configure_event")
     */
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkWindow::genGtkGUI()
{
   ptr = gtk_window_new(gtkWinT); 
   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
   if(label)
     gtk_window_set_title(GTK_WINDOW(ptr), label);
   if(numEvents != 0)
   {
     for(int i = 0; i < numEvents; i++)
     {
       MGtkEvent* pevent = events[i];
       if(isValidEvent(pevent))
       {
	 GCallback pGtkFunc = NULL;
         if(pevent->func != NULL)
         {
           if(!strcmp(pevent->func, "MGtkDestroy"))
	     pGtkFunc = G_CALLBACK(MGtkDestroy);
           /*else if(!strcmp(pevent->func, "MGtkKeyPressed"))
             pGtkFunc = G_CALLBACK(MGtkKeyPressed);
           else if(!strcmp(pevent->func, "MGtkKeyReleased"))
             pGtkFunc = G_CALLBACK(MGtkKeyReleased);*/
           else
             ;//ToDo
         }
         else
         {
	   if(!strcmp(pevent->event, "destroy"))
	     pGtkFunc = G_CALLBACK(MGtkDestroy);
           /*else if(!strcmp(pevent->event, "key_press_event"))
             pGtkFunc = G_CALLBACK(MGtkKeyPressed);
           else if(!strcmp(pevent->event, "key_release_event"))
             pGtkFunc = G_CALLBACK(MGtkKeyReleased);*/
	   else
	     ;//ToDo       
         }
         g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
       }
     }
   }
  
   if(visibility)
     gtk_widget_show(ptr);
}
