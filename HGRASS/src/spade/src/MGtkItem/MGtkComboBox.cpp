
#include "MGtkComboBox.hpp"
void MGtkComboBox::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sTEHasFrame_IB))
      this->hasFrame = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sTEEditable_IB))
      this->editable = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sCBNItems_IB))
      this->numItems = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sTECharW_IB))
      this->charWidth = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sCBItems_IB))
    {
      if(numItems == -1)
	numItems = MAX_COMBOBOXITEM;
      items = (char**)new char* [numItems];
      for(int i=0;i<numItems;i++)
	items[i] = NULL;
      char* itemList = (char*)value;
      for(int i=0;i<numItems;)
      {
        char* tmp = strsep(&itemList," |");
	if(strcmp(tmp,""))
	{
	  items[i] = strdup(tmp);
	  i++;
	}
      }
    }

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkComboBox::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  if(   strcmp(pevent->event,"move_active")
     && strcmp(pevent->event,"changed")
     && strcmp(pevent->event,"activate")
     && strcmp(pevent->event,"popdown")
     && strcmp(pevent->event,"popup")
     && 1/*ToDo*/)
    return false;
  if(   strcmp(pevent->func,"MGtkComboBoxMoveActive")
     && strcmp(pevent->func,"MGtkComboBoxChanged")
     && strcmp(pevent->func,"MGtkComboBoxEntryActivate")
     && strcmp(pevent->func,"MGtkComboBoxPopdown")
     && strcmp(pevent->func,"MGtkComboBoxPopup")
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkComboBox::genGtkGUI()
{
   //ptr = gtk_combo_box_new();
   if(editable)
   {
     ptr = gtk_combo_box_entry_new_text();
     if(charWidth != -1)
     {
       GtkEntry* pEntry = (GtkEntry*)gtk_bin_get_child(GTK_BIN(ptr));
       if(pEntry)
       {
         gtk_entry_set_width_chars(GTK_ENTRY(pEntry),charWidth);
       }
     }
   }
   else
     ptr = gtk_combo_box_new_text();
   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);

   for(int i=0;i<numItems;i++)
   {
     gtk_combo_box_append_text(GTK_COMBO_BOX(ptr),items[i]);
   }
   if(numItems != -1)
     gtk_combo_box_set_active(GTK_COMBO_BOX(ptr),0);
   /*if(label)*/
   
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
	   else if(!strcmp(pevent->func, "MGtkComboBoxMoveActive"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxMoveActive);
	   else if(!strcmp(pevent->func, "MGtkComboBoxEntryActivate"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxEntryActivate);
	   else if(!strcmp(pevent->func, "MGtkComboBoxChanged"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxChanged);
	   else if(!strcmp(pevent->func, "MGtkComboBoxPopup"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxPopup);
	   else if(!strcmp(pevent->func, "MGtkComboBoxPopdown"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxPopdown);
           else
             ;//ToDo
         }
         else
         {
	   if(!strcmp(pevent->event, "destroy"))
	     pGtkFunc = G_CALLBACK(MGtkDestroy);
	   else if(!strcmp(pevent->event, "active"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxMoveActive);
	   else if(!strcmp(pevent->event, "activate"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxEntryActivate);
	   else if(!strcmp(pevent->event, "changed"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxChanged);
	   else if(!strcmp(pevent->event, "popup"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxPopup);
	   else if(!strcmp(pevent->event, "popdown"))
	     pGtkFunc = G_CALLBACK(MGtkComboBoxPopdown);
	   else
	     ;//ToDo       
         }
	 if(editable && 
	   (!strcmp(pevent->func, "MGtkComboBoxEntryActivate") || !strcmp(pevent->event, "activate")))
	 {
           GtkEntry* pEntry = (GtkEntry*)gtk_bin_get_child(GTK_BIN(ptr));
           if(pEntry)
	     g_signal_connect(G_OBJECT(pEntry), pevent->event, pGtkFunc, pevent->mData);
	 }
	 else
	   g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
       }
     }
   }
   assert(pFa);
   assert(pFa->getGtkWidgetPtr());
   if(pFa)
   {
     if(pFa->isContainer())
       gtk_container_add(GTK_CONTAINER(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr));

     else if(pFa->isBox())
     {
       if(seq)
         gtk_box_pack_end(GTK_BOX(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr), expand, fill, padding);
       else
         gtk_box_pack_start(GTK_BOX(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr), expand, fill, padding);
     }
   }
   if(visibility)
     gtk_widget_show(ptr);
}
