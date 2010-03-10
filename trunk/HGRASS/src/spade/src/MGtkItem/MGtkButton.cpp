
#include "MGtkButton.hpp"

void MGtkButton::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sMBMnemonic_IB))
      this->mnemonic = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
    else if(!xmlStrcmp(attPtr->name, sMBStock_IB))
      this->stock = (char*)xmlStrdup(value);
    else if(!xmlStrcmp(attPtr->name, sBRelief_IB))
      this->relief = !xmlStrcmp(value, BAD_CAST "Normal") ? GTK_RELIEF_NORMAL
	           : !xmlStrcmp(value, BAD_CAST "Half") ? GTK_RELIEF_HALF
		   : !xmlStrcmp(value, BAD_CAST "None") ? GTK_RELIEF_NONE
		   : GTK_RELIEF_NORMAL;
#ifdef GTK2_4
    else if(!xmlStrcmp(attPtr->name, sFrameXA_IB))
      this->xAlign = atof((char*)value);
    else if(!xmlStrcmp(attPtr->name, sFrameYA_IB))
      this->yAlign = atof((char*)value);
#endif
#ifdef GTK2_6
    else if(!xmlStrcmp(attPtr->name, sBImage_IB))
      this->image = gtk_image_new_from_file((char*)value);
    else if(!xmlStrcmp(attPtr->name, sBImageP_IB))
      this->imagePosition = !xmlStrcmp(value, BAD_CAST "Left") ? GTK_POS_LEFT
	                  : !xmlStrcmp(value, BAD_CAST "Right") ? GTK_POS_RIGHT
			  : !xmlStrcmp(value, BAD_CAST "Top") ? GTK_POS_TOP
			  : !xmlStrcmp(value, BAD_CAST "Bottom") ? GTK_POS_BOTTOM
			  : GTK_POS_LEFT;
#endif

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkButton::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  if(   strcmp(pevent->event,"activate")
     && strcmp(pevent->event,"clicked")
     && strcmp(pevent->event,"enter")
     && strcmp(pevent->event,"leave")
     && strcmp(pevent->event,"pressed")
     && strcmp(pevent->event,"released")
     &&1/*ToDo*/)
    return false;
  if(   strcmp(pevent->func,"MGtkButtonActivated")
     && strcmp(pevent->func,"MGtkButtonPressed")
     && strcmp(pevent->func,"MGtkButtonReleased")
     && strcmp(pevent->func,"MGtkButtonClicked")
     && strcmp(pevent->func,"MGtkButtonEntered")
     && strcmp(pevent->func,"MGtkButtonLeft")
     && strcmp(pevent->func,"MGtkButtonDoubleClicked")
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkButton::genGtkGUI()
{
   if(stock)
   {
     ptr = gtk_button_new_from_stock(stock);
   }
   else if(label)
   {
     if(mnemonic)
       ptr = gtk_button_new_with_mnemonic(label);
     else
       ptr = gtk_button_new_with_label(label);
   }
   else
     ptr = gtk_button_new();
   assert(pFa);
   assert(pFa->getGtkWidgetPtr());

   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
   gtk_button_set_relief(GTK_BUTTON(ptr),relief);
#ifdef GTK2_4
   gtk_button_set_align(GTK_BUTTON(ptr),xAlign,yAlign);
#endif
#ifdef GTK2_6
   gtk_button_set_image(GTK_BUTTON(ptr),image);
   gtk_button_set_image_position(GTK_BUTTON(ptr),imagePosition);
#endif

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
           if(!strcmp(pevent->func, "MGtkButtonActivated"))
             pGtkFunc = G_CALLBACK(MGtkButtonActivated);
           else if(!strcmp(pevent->func, "MGtkButtonPressed"))
             pGtkFunc = G_CALLBACK(MGtkButtonPressed);
           else if(!strcmp(pevent->func, "MGtkButtonReleased"))
             pGtkFunc = G_CALLBACK(MGtkButtonReleased);
           else if(!strcmp(pevent->func, "MGtkButtonClicked"))
             pGtkFunc = G_CALLBACK(MGtkButtonClicked);
           else if(!strcmp(pevent->func, "MGtkButtonEntered"))
             pGtkFunc = G_CALLBACK(MGtkButtonEntered);
           else if(!strcmp(pevent->func, "MGtkButtonLeft"))
             pGtkFunc = G_CALLBACK(MGtkButtonLeft);
           else if(!strcmp(pevent->func, "MGtkButtonDoubleClicked"))
             pGtkFunc = G_CALLBACK(MGtkButtonDoubleClicked);
           else
             ;//ToDo
         }
         else
         {
           if(!strcmp(pevent->event, "activate"))
             pGtkFunc = G_CALLBACK(MGtkButtonActivated);
           else if(!strcmp(pevent->event, "pressed"))
             pGtkFunc = G_CALLBACK(MGtkButtonPressed);
           else if(!strcmp(pevent->event, "released"))
             pGtkFunc = G_CALLBACK(MGtkButtonReleased);
           else if(!strcmp(pevent->event, "enter"))
             pGtkFunc = G_CALLBACK(MGtkButtonEntered);
           else if(!strcmp(pevent->event, "leave"))
             pGtkFunc = G_CALLBACK(MGtkButtonLeft);
           else if(!strcmp(pevent->event, "clicked"))
             pGtkFunc = G_CALLBACK(MGtkButtonClicked);
           else
             ;//ToDo       
         }
         g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
       }
     }
   }

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
