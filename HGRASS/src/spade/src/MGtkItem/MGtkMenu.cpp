
#include "MGtkMenu.hpp"
void MGtkMenu::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sBoxExp_IB))
      this->expand = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sBoxFill_IB))
      this->fill = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sBoxPad_IB))
      this->padding = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sBoxAdDir_IB))
      this->seq = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sBorderW_IB))
      this->borderWidth = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sBorderH_IB))
      this->borderHeight = atoi((char*)value);

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkMenu::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  if( strcmp(pevent->event,"activate")
     &&1/*ToDo*/)
    return false;
  if( strcmp(pevent->func,"MGtkMenuActive")
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkMenu::genGtkGUI()
{
   if(label)
     ptr = gtk_menu_item_new_with_label(label);
   else
     ptr = gtk_menu_item_new();
   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
   /*if(label)
     gtk_menu_item_set_label(GTK_MENU(ptr), label);*/
   //printf("label = %s\n", label);
   
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
           if(!strcmp(pevent->func, "MGtkMenuActive"))
	     pGtkFunc = G_CALLBACK(MGtkMenuActive);
           /*else if(!strcmp(pevent->func, "MGtkMenuGenNetlist"))
             pGtkFunc = G_CALLBACK(MGtkMenuGenNetlist);
           else if(!strcmp(pevent->func, "MGtkMenuSetAnalysis"))
             pGtkFunc = G_CALLBACK(MGtkMenuSetAnalysis);
           else if(!strcmp(pevent->func, "MGtkMenuOpenSEDFile"))
             pGtkFunc = G_CALLBACK(MGtkMenuOpenSEDFile);
           else if(!strcmp(pevent->func, "MGtkMenuSaveSEDFile"))
             pGtkFunc = G_CALLBACK(MGtkMenuSaveSEDFile);
           else if(!strcmp(pevent->func, "MGtkMenuSavAsSEDFile"))
             pGtkFunc = G_CALLBACK(MGtkMenuSavAsSEDFile);*/
           else
             ;//ToDo
         }
         else
         {
	   if(!strcmp(pevent->event, "activate"))
	     pGtkFunc = G_CALLBACK(MGtkMenuActive);
	   else
	     ;//ToDo       
         }
         g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
       }
     }
   }   

   assert(pFa);
   assert(pFa->getGtkWidgetPtr());
   assert(pFa->isMenuShell());
   gtk_menu_shell_append(GTK_MENU_SHELL(pFa->getGtkWidgetPtr()), ptr);

   if(visibility)
     gtk_widget_show(ptr);
}
