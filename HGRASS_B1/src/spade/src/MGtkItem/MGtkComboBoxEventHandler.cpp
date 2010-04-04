
#include "MGtkEventHandler.hpp"
#include "MGtkComboBox.hpp"
#include "../MGUI/GUIContainer.hpp"

/*
void MGtkDestroy(GtkWidget *widget, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  MGtkWindow* pGtkWindow = (MGtkWindow*) pdata->pmGtk;
  assert(pGtkWindow);
  delete pGtkWindow;
}
*/
void MGtkComboBoxMoveActive(GtkWidget *widget, gpointer data)
{

}

void MGtkComboBoxChanged(GtkWidget *widget, gpointer data)
{
    GtkCFData* pdata = (GtkCFData*)data;
    assert(pdata);
    int nID = pdata->numID;
    int* pIDs = pdata->interactiveID;
    if(nID == 0 || pIDs == NULL)
      return;
    GUIContainer* pGUI = pdata->pcon;
    assert(pGUI);
    MGtkComboBox* pmGtkComboBox = (MGtkComboBox*)pdata->pmGtk;
    assert(pmGtkComboBox);
    GtkWidget* pmGtkWidget = pmGtkComboBox->getGtkWidgetPtr();
    int id = gtk_combo_box_get_active(GTK_COMBO_BOX(pmGtkWidget));
    if(id == -1)
      return;
    char** items = pmGtkComboBox->getItems();
    if(items == NULL)
      return;
    int windowID = pmGtkComboBox->getID();
    if(windowID == -1)
      return;
    for(int i=0;i<nID;i++)
    {
      MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,pIDs[i]);
      if(pGtkItem == NULL)
	continue;
      if(pGtkItem->isComboBox())
      {
        MGtkComboBox* pGtkComboBox = (MGtkComboBox*)pGtkItem;
	assert(pGtkComboBox);
	if(pGtkComboBox->isEditable())
	{
	  GtkWidget* pGtkWidget = pGtkComboBox->getGtkWidgetPtr();
	  assert(pGtkWidget);
	  char** mItems = pGtkComboBox->getItems();
	  int n = pGtkComboBox->getNumItems();
	  bool itemExist = false;
          for(int j=0;mItems && j<n;j++)
	  {
	    if(!strcmp(mItems[j],items[id]))
	    {
	      gtk_combo_box_set_active(GTK_COMBO_BOX(pGtkWidget),j);
	      itemExist = true;
	      break;
	    }
	  }
	  if(!itemExist)
	  {
	    if(!mItems)
	    {
	      mItems = new char* [MAX_COMBOBOXITEM];
	      for(int j=0;j<MAX_COMBOBOXITEM;j++)
		mItems[j] = NULL;
	      n = 0;
	    }
	    gtk_combo_box_append_text(GTK_COMBO_BOX(pGtkWidget),items[id]);
	    //ToDo: Insure n never over MAX_COMBOBOXITEM (100)
	    mItems[n++] = strdup(items[id]);
	    pGtkComboBox->setNumItems(n);
	    pGtkComboBox->setItems(mItems);
            gtk_combo_box_set_active(GTK_COMBO_BOX(pGtkWidget),n-1);
	  }
	}
      }
    }
}

void MGtkComboBoxEntryActivate(GtkWidget *widget, gpointer data)
{
   GtkCFData* pdata = (GtkCFData*)data;
   assert(pdata);
   MGtkComboBox* pGtkComboBox = (MGtkComboBox*)pdata->pmGtk;
   assert(pGtkComboBox);
   if(!pGtkComboBox->isEditable())
     return;
   
   char* text = (char*)gtk_entry_get_text(GTK_ENTRY(widget));
   char* value = NULL;
   if(text && strcmp(text,""))
     value = text;
   int ID = pGtkComboBox->getID();
   switch(ID)
   {
     case CONFIGURE_COMBOENTRY_P1_ALIAS:
       if(confParas.p1Alias)
	 delete [] confParas.p1Alias;
       if(value == NULL)
	 confParas.p1Alias = NULL;
       else
         confParas.p1Alias = strdup(value);
       break;
     case CONFIGURE_COMBOENTRY_P2_ALIAS:
       if(confParas.p2Alias)
	 delete [] confParas.p2Alias;
       if(value == NULL)
	 confParas.p2Alias = NULL;
       else
         confParas.p2Alias = strdup(value);
       break;
     default:
       break;
   }
}

void MGtkComboBoxPopup(GtkWidget *widget, gpointer data)
{

}

void MGtkComboBoxPopdown(GtkWidget *widget, gpointer data)
{

}

