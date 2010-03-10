
#include "MGtkToolItem.hpp"

void MGtkToolItem::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sTItemTip_IB))
      this->tip = (char*)xmlStrdup(value);
    else if(!xmlStrcmp(attPtr->name, sTItemPTip_IB))
      this->priTip = (char*)xmlStrdup(value);
    else if(!xmlStrcmp(attPtr->name, sTItemIcon_IB))
      this->icon = (char*)xmlStrdup(value);
    else if(!xmlStrcmp(attPtr->name, sTItemPos_IB))
      this->pos = atoi((char*)value);
    else if(!xmlStrcmp(attPtr->name, sTItemHColor_IB))
      this->highlightColor = (char*)xmlStrdup(value);
    else if(!xmlStrcmp(attPtr->name, sTItemDraw_IB))
      this->draw = !xmlStrcmp(value, BAD_CAST "true") ? true : false;

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkToolItem::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  if( strcmp(pevent->event,"clicked")
     &&1/*ToDo*/)
    return false;
  if( strcmp(pevent->func,"MGtkBarItemClicked")
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkToolItem::genGtkGUI()
{
   if(label && icon)
   {
     GtkWidget* image = gtk_image_new_from_file(icon);
     ptr = (GtkWidget*)gtk_tool_button_new(image, label);
   }
   else if(!strcmp(label, "SEP"))
   {
     ptr = (GtkWidget*)gtk_separator_tool_item_new();
     gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(ptr),draw);
     gtk_tool_item_set_expand(GTK_TOOL_ITEM(ptr),expand);
   }
   else
     ptr = (GtkWidget*)gtk_tool_item_new();

   GtkWidget* gtkTip = NULL;
   GList* childs = NULL;
   GtkButton* button = NULL;
   if(strcmp(label, "SEP"))
   {
     gtkTip = (GtkWidget*)gtk_tooltips_new();
     gtk_tooltips_enable(GTK_TOOLTIPS(gtkTip));
     gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(ptr),GTK_TOOLTIPS(gtkTip),tip,priTip);
     if(isFixedSize)
       gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
     if(!responseEnable)
       gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
     childs = gtk_container_get_children(GTK_CONTAINER(ptr));
     assert(childs);
     button = (GtkButton*)childs->data;
   }
   
   if(numEvents != 0)
   {
     for(int i = 0; i < numEvents; i++)
     {
       MGtkEvent* pevent = events[i];
       if(isValidEvent(pevent))
       {
	 GCallback pGtkFunc = NULL;
/*	 GCallback pGtkButtonFuncPressed = NULL;
	 GCallback pGtkButtonFuncReleased = NULL;
	 GCallback pGtkButtonFuncClicked = NULL;*/
         if(pevent->func != NULL)
         {
           if(!strcmp(pevent->func, "MGtkBarItemClicked"))
	   {
	     pGtkFunc = G_CALLBACK(MGtkBarItemClicked);
/*	     pGtkButtonFuncPressed = G_CALLBACK(MGtkToolItemPressed);
	     pGtkButtonFuncReleased = G_CALLBACK(MGtkToolItemReleased);
	     pGtkButtonFuncClicked = G_CALLBACK(MGtkToolItemClicked);*/
	   }
           else
             ;//ToDo
         }
         else
         {
	   if(!strcmp(pevent->event, "clicked"))
	   {
	     pGtkFunc = G_CALLBACK(MGtkBarItemClicked);
/*	     pGtkButtonFuncPressed = G_CALLBACK(MGtkToolItemPressed);
	     pGtkButtonFuncReleased = G_CALLBACK(MGtkToolItemReleased);
	     pGtkButtonFuncClicked = G_CALLBACK(MGtkToolItemClicked);*/
	   }
	   else
	     ;//ToDo       
         }
         g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
//         g_signal_connect(G_OBJECT(button), pevent->event, pGtkButtonFuncPressed, pevent->mData);
//         g_signal_connect(G_OBJECT(button), pevent->event, pGtkButtonFuncReleased, pevent->mData);
//         g_signal_connect(G_OBJECT(button), pevent->event, pGtkButtonFuncClicked, pevent->mData);
       }
     }
   }   

   assert(pFa);
   assert(pFa->getGtkWidgetPtr());
   assert(pFa->isToolBar());
   gtk_toolbar_insert(GTK_TOOLBAR(pFa->getGtkWidgetPtr()),GTK_TOOL_ITEM(ptr),pos);

   if(visibility)
     gtk_widget_show_all(ptr);
}
