
#include "MGtkButton.hpp"
#include "../MGUI/GUIContainer.hpp"
#include "MGtkEventHandler.hpp"
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
void MGtkButtonPressed(GtkWidget *widget, gpointer data)
{

}

void MGtkButtonReleased(GtkWidget *widget, gpointer data)
{

}

void MGtkButtonClicked(GtkWidget *widget, gpointer data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  char* pmdata = (char*)pdata->data;
  if(pmdata == NULL)
    return;
  if(strcmp(pmdata,"CirPath") && strcmp(pmdata,"LibPath"))
    return;
  int nid = pdata->numID;
  int* pids = pdata->interactiveID;
  if(nid < 1 || pids == NULL)
    return;
  GUIContainer* pGUI = pdata->pcon;
  assert(pGUI);
  MGtkButton* pmGtkButton = (MGtkButton*)pdata->pmGtk;
  assert(pmGtkButton);
  int windowID = pmGtkButton->getID();
  if(windowID == -1)
    return;
  MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,pids[0]);
  if(pGtkItem == NULL || !pGtkItem->isLabel())
    return;
  MGtkLabel* pGtkLabel = (MGtkLabel*)pGtkItem;
  assert(pGtkLabel);
  GtkWidget* pGtkWidget = pGtkLabel->getGtkWidgetPtr();
  assert(pGtkWidget);
  char* text = (char*)gtk_label_get_text(GTK_LABEL(pGtkWidget));
  char* fileName = NULL;
  if(text == NULL || !strcmp(text,"") || !strcmp(text,"(None)"))
    fileName = NULL;
  else
    fileName = text;
  if(!strcmp(pmdata,"CirPath"))
  {
    char* filterName [] = {"Circuit Files","All Files","*.sp","*.cir","*"};
    int filterNum [] = {2,2,1};
    fileName = MPopupFileChooserDialogOpen("Open the File...",filterName,filterNum,fileName);
  }
  else if(!strcmp(pmdata,"LibPath"))
  {
    char* filterName [] = {"Lib Files","All Files","*.lib","*.l","*"};
    int filterNum [] = {2,2,1};
    fileName = MPopupFileChooserDialogOpen("Open the File...",filterName,filterNum,fileName);
  }
  else
    return;
  if(fileName == NULL || !strcmp(fileName,text))
    return;
  gtk_label_set_text(GTK_LABEL(pGtkWidget),fileName);
  if(!strcmp(pmdata,"CirPath"))
  {
    MSetItemListAccordingToFile(fileName,pGUI,windowID,nid,pids);
  }
}

void MGtkButtonEntered(GtkWidget *widget, gpointer data)
{

}

void MGtkButtonLeft(GtkWidget *widget, gpointer data)
{

}

void MGtkButtonDoubleClicked(GtkWidget *widget, gpointer data)
{

}

void MGtkButtonActivated(GtkWidget *widget, gpointer data)
{

}

int GetItemListFromCirFile(char* fileName, char** &itemList)
{
   if(fileName == NULL)
     return 0;
   FILE* fp = NULL;
   if((fp = fopen(fileName,"r")) == NULL)
   {
     POPUP_ERROR("File Not Found!");
     return 0;
   }
   itemList = new char* [MAX_COMBOBOXITEM];
   for(int i=0;i<MAX_COMBOBOXITEM;i++)
     itemList[i] = NULL;
   int n = 0;
   char* tmp = new char[MAX_CHARS];
   fscanf(fp,"%[^\n]%*c\n",tmp);
   while(!feof(fp))
   {
     char* item = new char[MAX_CHARS];
     fscanf(fp,"%s %[^\n]%*c\n",item,tmp);
     if(item[0] != 'r' && item[0] != 'R' &&
	item[0] != 'c' && item[0] != 'C' &&
	item[0] != 'l' && item[0] != 'L' && 1)
     {
       delete [] item;
       continue;
     }
     itemList[n++] = item;
   }
   delete [] tmp;
   fclose(fp);
   return n;
}

void MSetItemListAccordingToFile(char* fileName, GUIContainer* pGUI, int windowID, int nid, int* pids)
{
  if(fileName == NULL || pGUI == NULL || pids == NULL || nid <= 0)
    return;
  char** itemList = NULL;;
  int n = GetItemListFromCirFile(fileName, itemList);
  if(itemList == NULL)
    return;
  for(int i=1;i<nid;i++)
  {
    if(pids[i] != CONFIGURE_COMBOBOX_P1_NAME && pids[i] != CONFIGURE_COMBOBOX_P2_NAME)
      continue;
    MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,pids[i]);
    if(pGtkItem == NULL || !pGtkItem->isComboBox())
      continue;
    MGtkComboBox* pGtkComboBox = (MGtkComboBox*)pGtkItem;
    assert(pGtkComboBox);
    GtkWidget* pGtkWidgetComboBox = pGtkComboBox->getGtkWidgetPtr();
    assert(pGtkWidgetComboBox);
    char** mItems = pGtkComboBox->getItems();
    if(mItems != NULL)
    {
      gtk_combo_box_set_active(GTK_COMBO_BOX(pGtkWidgetComboBox),-1);
      int nn = pGtkComboBox->getNumItems();
      for(int k=0;k<nn;k++)
      {
        delete [] mItems[k];
	gtk_combo_box_remove_text(GTK_COMBO_BOX(pGtkWidgetComboBox),0);
      }
      delete [] mItems;
    }
    mItems = new char* [n];
    for(int k=0;k<n;k++)
      mItems[k] = NULL;
    for(int j=0;j<n;j++)
    {
      mItems[j] = strdup(itemList[j]);
      gtk_combo_box_append_text(GTK_COMBO_BOX(pGtkWidgetComboBox),itemList[j]);
    }
    pGtkComboBox->setNumItems(n);
    pGtkComboBox->setItems(mItems);
    if(n != 0)
    {
      gtk_combo_box_set_active(GTK_COMBO_BOX(pGtkWidgetComboBox),0);
    }
  }
  if(itemList)
  {
    for(int j=0;j<n;j++)
      delete [] itemList[j];
    delete [] itemList;
  }
}
