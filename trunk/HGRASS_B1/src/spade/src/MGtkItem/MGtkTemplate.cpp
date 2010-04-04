
#include "MGtkTemplate.hpp"

void MGtkTemplate::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sBoxHSpace_IB))
      this->homoSpace = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sBoxSpace_IB))
      this->spacing = atoi((char*)value);
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
    //Add Your New Properties Parsing Code Here

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkTemplate::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  //Judge Event Name, say clicked, activate
  if(1
     &&1/*ToDo*/)
    return false;
  //Judge Event Handler Name, say MGtkDestroy, etc.
  if(1
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkTemplate::genGtkGUI()
{
   //New Your GtkWidget* Here
   //ptr = dir == 1 ? gtk_vbox_new(homoSpace, spacing) : gtk_hbox_new(homoSpace, spacing); 
   assert(ptr);
   //Do Not Remove the Codes Related to isFixedSize and responseEnable, Just Keep
   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
   //End
   //Event Binding Code
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
	   //Event Handler Name & Function Binding
	   //Remember open the comments if you are in need
           //if(!strcmp(pevent->func, "MGtkDestroy"))
	   //  pGtkFunc = G_CALLBACK(MGtkDestroy);
	   //Add "else if" Here
           //else
           //  ;//ToDo
         }
         else
         {
	   //Event Name & Function Binding
	   //Remember open the comments if you are in need
	   //Not Necessary, But as a Backup
	   //if(!strcmp(pevent->event, "destroy"))
	   //  pGtkFunc = G_CALLBACK(MGtkDestroy);
	   //Add "else if" Here
	   //else
	   //  ;//ToDo       
         }
         g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
       }
     }
   }
   //End
   //GtkWidget Pointer Package Codes, Just Keep
   assert(pFa);
   assert(pFa->getGtkWidgetPtr());
   if(pFa)
   {
     if(pFa->isContainer())
       gtk_container_add(GTK_CONTAINER(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr));

     else if(pFa->isTemplate())
     {
       if(seq)
         gtk_box_pack_end(GTK_BOX(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr), expand, fill, padding);
       else
         gtk_box_pack_start(GTK_BOX(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr), expand, fill, padding);
     }
   }
   //End
   //Display Codes, Just Keep
   if(visibility)
     gtk_widget_show(ptr);
   //End
}
