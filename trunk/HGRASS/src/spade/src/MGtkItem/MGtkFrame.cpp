
#include "MGtkFrame.hpp"

void MGtkFrame::getXmlNodeProperties(xmlNode* node)
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
    else if(!xmlStrcmp(attPtr->name, sFrameXA_IB))
      this->xAlign = atof((char*)value);
    else if(!xmlStrcmp(attPtr->name, sFrameYA_IB))
      this->yAlign = atof((char*)value);
    else if(!xmlStrcmp(attPtr->name, sFrameShadow_IB))
      this->shadowType = !xmlStrcmp(value, BAD_CAST "NONE") ? GTK_SHADOW_NONE
		       : !xmlStrcmp(value, BAD_CAST "IN") ? GTK_SHADOW_IN
		       : !xmlStrcmp(value, BAD_CAST "OUT") ? GTK_SHADOW_OUT
		       : !xmlStrcmp(value, BAD_CAST "3DIN") ? GTK_SHADOW_ETCHED_IN
		       : !xmlStrcmp(value, BAD_CAST "3OUT") ? GTK_SHADOW_ETCHED_OUT 
		       : GTK_SHADOW_NONE;
    if(value != NULL)
      xmlFree(value);
    attPtr = attPtr->next;
  }

}

bool MGtkFrame::isValidEvent(MGtkEvent* pevent)
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

void MGtkFrame::genGtkGUI()
{
   ptr = gtk_frame_new(label);
   assert(ptr);
   if(isFixedSize)
     gtk_widget_set_size_request(GTK_WIDGET(ptr), width, height);
   if(!responseEnable)
     gtk_widget_set_sensitive(GTK_WIDGET(ptr),responseEnable);
   gtk_frame_set_label_align(GTK_FRAME(ptr),xAlign,yAlign);
   gtk_frame_set_shadow_type(GTK_FRAME(ptr),shadowType);
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
