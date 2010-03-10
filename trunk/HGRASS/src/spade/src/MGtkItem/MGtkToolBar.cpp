
#include "MGtkToolBar.hpp"

void MGtkToolBar::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sTBarStyle_IB))
      this->style = !xmlStrcmp(value, BAD_CAST "Both") ? GTK_TOOLBAR_BOTH :
		    !xmlStrcmp(value, BAD_CAST "BothH")? GTK_TOOLBAR_BOTH_HORIZ :
		    !xmlStrcmp(value, BAD_CAST "Icon") ? GTK_TOOLBAR_ICONS :
		  GTK_TOOLBAR_TEXT;
    else if(!xmlStrcmp(attPtr->name, sTBarOrien_IB))
      this->orien = !xmlStrcmp(value, BAD_CAST "H") ? GTK_ORIENTATION_HORIZONTAL
				: GTK_ORIENTATION_VERTICAL;

    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkToolBar::isValidEvent(MGtkEvent* pevent)
{
  if(pevent == NULL || pevent->event == NULL)
    return false;
  if(1
     &&1/*ToDo*/)
    return false;
  if(1
     &&1/*ToDo*/)
    return false;
  return true;
}

void MGtkToolBar::genGtkGUI()
{
   ptr = gtk_toolbar_new();
   /*if(label)*/
   /*
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
           else
             ;//ToDo
         }
         else
         {
	   if(!strcmp(pevent->event, "destroy"))
	     pGtkFunc = G_CALLBACK(MGtkDestroy);
	   else
	     ;//ToDo       
         }
         g_signal_connect(G_OBJECT(ptr), pevent->event, pGtkFunc, pevent->mData);
       }
     }
   }
   */

   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
   gtk_toolbar_set_orientation(GTK_TOOLBAR(ptr), orien);
   gtk_toolbar_set_style(GTK_TOOLBAR(ptr), style);
   gtk_container_set_border_width(GTK_CONTAINER(ptr), borderWidth);

   assert(pFa);
   assert(pFa->getGtkWidgetPtr());

   if(pFa->isContainer())
     gtk_container_add(GTK_CONTAINER(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr));

   else if(pFa->isBox())
   {
     if(seq)
       gtk_box_pack_end(GTK_BOX(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr), expand, fill, padding);
     else
       gtk_box_pack_start(GTK_BOX(pFa->getGtkWidgetPtr()), GTK_WIDGET(ptr), expand, fill, padding);
   }

   if(visibility)
     gtk_widget_show(ptr);
}
