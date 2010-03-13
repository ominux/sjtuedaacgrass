
#include "MGtkToolItem.hpp"
#include "../MGUI/GUIContainer.hpp"
#include "MGtkEventHandler.hpp"
#include <unistd.h>
#include <fcntl.h>

void MGtkBarItemClicked(GtkWidget *widget, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  MGtkToolItem* pGtkTItem = (MGtkToolItem*) pdata->pmGtk;
  assert(pGtkTItem);
  char* pLabel = (char*)pdata->data;
  assert(pLabel);

  if(!strcmp(pLabel,"formula"))
    MPopupFormula(data);
  else if(!strcmp(pLabel,"schematic"))
    MPopupSchematicEditor(data);
  else if(!strcmp(pLabel,"netlist"))
    MPopupNetlistEditor(data);
  else if(!strcmp(pLabel,"configure"))
    MPopupConfigure(data);
  else if(!strcmp(pLabel,"simulate"))
    MPopupSimulate(data);
  else if(!strcmp(pLabel,"help"))
    MPopupHelp(data);
  else if(!strcmp(pLabel,"load"))
    MConfigureLoad(widget,data);
  else if(!strcmp(pLabel,"save"))
    MConfigureSave(widget,data);
  else if(!strcmp(pLabel,"save as"))
    MConfigureSaveAs(widget,data);
  else if(!strcmp(pLabel,"reset"))
    MConfigureReset(widget,data);

  //ToDo: Add more methods if needed
  else
    ;

}

void MGtkToolItemClicked(GtkWidget *widget, gpointer  data)
{

}

void MGtkToolItemPressed(GtkWidget *widget, gpointer  data)
{
   
}

void MGtkToolItemReleased(GtkWidget *widget, gpointer  data)
{

}

void MPopupFormula(gpointer data)
{

}

void MPopupSchematicEditor(gpointer data)
{
   CHECK_COMMAND_VALIDITY("icfb","You have not installed Cadence ICFB yet, \nPlease contact your lab administrator and retry later");
   int ret = system("icfb &");
}

void MPopupNetlistEditor(gpointer data)
{
   char* filterName [] = {"Circuit Files","All Files","*.sp","*.cir","*"};
   int filterNum [] = {2,2,1};
   char* fileName = MPopupFileChooserDialogOpen("Open the File...",filterName,filterNum);
   if(fileName == NULL)
     return;

   CHECK_COMMAND_VALIDITY("gedit","You have not installed gedit yet, \nPlease contact your lab administrator and retry later");
   char* command = new char [MAX_CHARS];
   sprintf(command,"gedit %s &",fileName);
   int ret = system(command);
   g_free(fileName);
   delete [] command;
}

void MPopupConfigure(gpointer data)
{
   GtkCFData* pdata = (GtkCFData*)data;
   assert(pdata);
   GUIContainer* pGui = (GUIContainer*)pdata->pcon;
   assert(pGui);
   
   pGui->constructWindow(1);
   if(confParas.confPath && strcmp(confParas.confPath,"") && strcmp(confParas.confPath,"(NONE)"))
   {
     MGtkItemBase* pGtkItem = pGui->getMGtkItem(1,CONFIGURE_BUTTON_RESET);
     if(pGtkItem)
     {
       GtkWidget* pWidget = pGtkItem->getGtkWidgetPtr();
       if(pWidget)
         g_signal_emit_by_name(G_OBJECT(pWidget),"clicked",NULL,NULL);    
     }
   }
   pGui->enableWindow(1);
}

void MPopupSimulate(gpointer data)
{
   char* filterName [] = {"Configure Files","*.conf","*.cf"};
   int filterNum [] = {1,2};
   char* fileName = MPopupFileChooserDialogOpen("Open the File...",filterName,filterNum);
   if(fileName == NULL)
     return;

   CHECK_FILE_VALIDITY(fileName,"Cannot find the configure file, \nPlease set the configuration first and retry later");
   char* run = SCRIPT_CMD_RUN;
   //char* conf = GRASS_CONF;
   char* command = new char [MAX_CHARS];
   //sprintf(command,"%s %s %s &",run,fileName,conf);
   sprintf(command,"%s %s",run,fileName);
   int ret = system(command);
   int errNum = WEXITSTATUS(ret);
   MProcessSimulateError(errNum);
   g_free(fileName);
   delete [] command;
}

void MPopupHelp(gpointer data)
{
   char* fileName = DOC_HELP;
   CHECK_COMMAND_VALIDITY("firefox","You have not installed firefox yet, \nPlease contact your lab administrator and retry later");
   CHECK_FILE_VALIDITY(fileName,"Cannot find the help documents, \nPlease contact HGRASS designers and retry later");
   char* command = new char [MAX_CHARS];
   sprintf(command,"firefox %s &",fileName);
   int ret = system(command);
   delete [] command;
}

void MProcessSimulateError(int errorNumber)
{
   switch(errorNumber)
   {
     case 0://No Error
            break;
     case 1://Configure File Not Exists
            {
            POPUP_ERROR("The configure file cannot be found!\nPlease re-confirm the configure operation and retry later");
            }
            break;
     case 2://Grass Directory Not Exists
            {
            POPUP_ERROR("The GRASS directory cannot be found!\nPlease contact SPADE designers and retry later");
            }
            break;
     case 3://HSPICE Not Exists
            {
            POPUP_ERROR("The HSPICE tool cannot be found!\nPlease contact your lab administrator and retry later");
            }
            break;
     case 4://HSPICE Simulation Error
            {
            POPUP_ERROR("HSPICE simulation has detected your circuit file errors!\nPlease retry later after you fixed the errors");
            char* spiceErr = HSPICE_ERR;
            CHECK_FILE_VALIDITY(spiceErr,"Cannot find the HSPICE error report, \nPlease contact SPADE designers and retry later");
            CHECK_COMMAND_VALIDITY("gedit","You have not installed gedit yet, \nPlease contact your lab administrator and retry later");
            char* command = new char [MAX_CHARS];
            sprintf(command,"gedit %s &",spiceErr);
            int ret = system(command);
            delete [] command;
            }
            break;
     case 5://No Support View Found
            {
            POPUP_ERROR("The view model selection is not supported!\nPlease make sure your selection is within 3D-2D range");
            }
            break;
     case 6://TKY's Script Not Exists
            {
            POPUP_ERROR("The parameters extraction script cannot be found!\nPlease contact SPADE designers and retry later");
            }
            break;
     case 7://TKY's Script Run Error
            {
            POPUP_ERROR("The parameters extraction script has run to errors!\nPlease check out the error and contact SPADE designers");
            }
            break;
     case 8://Grass 3D Version Running Error
            {
            POPUP_ERROR("The GRASS 3D verision has run to errors!\nPlease contact SPADE designers and retry later");
            char* grass3DErr = GRASS_3D_RUN;
            CHECK_FILE_VALIDITY(grass3DErr,"Cannot find the HSPICE error report, \nPlease contact SPADE designers and retry later");
            CHECK_COMMAND_VALIDITY("gedit","You have not installed gedit yet, \nPlease contact your lab administrator and retry later");
            char* command = new char [MAX_CHARS];
            char* grass3DErr2 = GRASS_3D_ERR;
            if(access(grass3DErr2,0))
              sprintf(command,"gedit %s &",grass3DErr);
            else
              sprintf(command,"gedit %s %s &",grass3DErr,grass3DErr2);
            int ret = system(command);
            delete [] command;
            break;
            }
      case 9://Grass 3D Verision Reports Error
            {
            POPUP_ERROR("The GRASS 3D version has reported errors!\nPlease check out the error and contact SPADE designers");
            char* grass3DErr = GRASS_3D_ERR;
            CHECK_FILE_VALIDITY(grass3DErr,"Cannot find the HSPICE error report, \nPlease contact SPADE designers and retry later");
            CHECK_COMMAND_VALIDITY("gedit","You have not installed gedit yet, \nPlease contact your lab administrator and retry later");
            char* command = new char [MAX_CHARS];
            sprintf(command,"gedit %s &",grass3DErr);
            int ret = system(command);
            delete [] command;
            break;
            }
            break;
     case 10://Grass 2D Version Running Error
            {
            POPUP_ERROR("The GRASS 2D verision has run to errors!\nPlease contact SPADE designers and retry later");
            char* grass2DErr = GRASS_2D_RUN;
            CHECK_FILE_VALIDITY(grass2DErr,"Cannot find the HSPICE error report, \nPlease contact SPADE designers and retry later");
            CHECK_COMMAND_VALIDITY("gedit","You have not installed gedit yet, \nPlease contact your lab administrator and retry later");
            char* command = new char [MAX_CHARS];
            char* grass2DErr2 = GRASS_2D_ERR;
            if(access(grass2DErr2,0))
              sprintf(command,"gedit %s &",grass2DErr);
            else
              sprintf(command,"gedit %s %s &",grass2DErr,grass2DErr2);
            int ret = system(command);
            delete [] command;
            break;
            }
            break;
     case 11://Grass 2D Verision Reports Error
            {
            POPUP_ERROR("The GRASS 2D verision has run to errors!\nPlease contact SPADE designers and retry later");
            char* grass2DErr = GRASS_2D_ERR;
            CHECK_FILE_VALIDITY(grass2DErr,"Cannot find the HSPICE error report, \nPlease contact SPADE designers and retry later");
            CHECK_COMMAND_VALIDITY("gedit","You have not installed gedit yet, \nPlease contact your lab administrator and retry later");
            char* command = new char [MAX_CHARS];
            sprintf(command,"gedit %s &",grass2DErr);
            int ret = system(command);
            delete [] command;
            break;
            }
            break;
     case 12://Configure Information Not Enough
	    {
	    POPUP_ERROR("Too little configure information!\nPlease finish configure settings and retry later");
	    break;
	    }
     case 13://Spice Circuit File Not Exit
	    {
	    POPUP_ERROR("Cannot find the circuit file!\nPlease re-chech the file path setting and retry later");
	    break;
	    }
     default://Error Number Not Recognized
            {
            POPUP_ERROR("Error cannot be recognized!\nPlease contact SPADE designers and retry later");
            }
            break;
   }
}

void MConfigureLoad(GtkWidget *widget, gpointer  data)
{
  char* fileName = NULL;
  char* filterName [] = {"Configure Files","All Files","*.cf","*.conf","*"};
  int filterNum [] = {2,2,1};
  fileName = MPopupFileChooserDialogOpen("Open the File ...",filterName,filterNum);
  if(fileName == NULL)
    return;
  if(confParas.confPath)
    delete [] confParas.confPath;
  confParas.confPath = strdup(fileName);
  
  MLoadConfigureSettings(fileName);
  MUpdateConfigureSettings(widget,data);
}

void MConfigureSave(GtkWidget *widget, gpointer  data)
{
  char* fileName = NULL;
  bool isSaveAs = false;
  if(!confParas.confPath)
  {
    fileName = strdup("MyNewConfigure.conf");
    char* filterName [] = {"Configure Files","All Files","*.cf","*.conf","*"};
    int filterNum [] = {2,2,1};
    fileName = MPopupFileChooserDialogSave("Save the File As...",filterName,filterNum,fileName);
    if(fileName == NULL)
      return;
    if(confParas.confPath)
      delete [] confParas.confPath;
    confParas.confPath = strdup(fileName);
    isSaveAs = true;
  }
  else
    fileName = strdup(confParas.confPath);
  MSaveConfigureSettings(widget,data,fileName);
  if(isSaveAs)
  {
    GtkCFData* pdata = (GtkCFData*)data;
    assert(pdata);
    GUIContainer* pGUI = pdata->pcon;
    assert(pGUI);
    MGtkToolItem* pmGtkToolItem = (MGtkToolItem*)pdata->pmGtk;
    assert(pmGtkToolItem);
    int windowID = pmGtkToolItem->getID();
    MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,CONFIGURE_LABEL_CONF_PATH);
    if(pGtkItem && pGtkItem->isLabel())
    {
      GtkWidget* pWidget = pGtkItem->getGtkWidgetPtr();
      gtk_label_set_text(GTK_LABEL(pWidget),confParas.confPath);
    }
  }
}

void MConfigureSaveAs(GtkWidget *widget, gpointer  data)
{
  char* fileName = NULL;
  if(confParas.confPath)
    fileName = strdup(confParas.confPath);
  else
    fileName = strdup("MyNewConfigure.conf");
  char* filterName [] = {"Configure Files","All Files","*.cf","*.conf","*"};
  int filterNum [] = {2,2,1};
  fileName = MPopupFileChooserDialogSave("Save the File As...",filterName,filterNum,fileName);
  if(fileName == NULL)
    return;
  if(confParas.confPath)
    delete [] confParas.confPath;
  confParas.confPath = strdup(fileName);
  MSaveConfigureSettings(widget,data,fileName);

  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  GUIContainer* pGUI = pdata->pcon;
  assert(pGUI);
  MGtkToolItem* pmGtkToolItem = (MGtkToolItem*)pdata->pmGtk;
  assert(pmGtkToolItem);
  int windowID = pmGtkToolItem->getID();
  MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,CONFIGURE_LABEL_CONF_PATH);
  if(pGtkItem && pGtkItem->isLabel())
  {
    GtkWidget* pWidget = pGtkItem->getGtkWidgetPtr();
    gtk_label_set_text(GTK_LABEL(pWidget),confParas.confPath);
  }
}

void MConfigureReset(GtkWidget *widget, gpointer  data)
{
  if(confParas.confPath && access(confParas.confPath,0) == 0)
    MUpdateConfigureSettings(widget,data);
}

void MLoadConfigureSettings(char* fileName)
{
  if(fileName == NULL)
    return;
  FILE* fp = NULL;
  if((fp = fopen(fileName,"r")) == NULL)
  {
    POPUP_ERROR("Load from File Failed!");
    return;
  }
  confParas.readFromFile(fp);
  fclose(fp);
}

void MUpdateConfigureSettings(GtkWidget *widget, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  int nID = pdata->numID;
  int* pIDs = pdata->interactiveID;
  if(nID == 0 || pIDs == NULL)
    return;
  GUIContainer* pGUI = pdata->pcon;
  assert(pGUI);
  MGtkToolItem* pmGtkToolItem = (MGtkToolItem*)pdata->pmGtk;
  assert(pmGtkToolItem);
  int windowID = pmGtkToolItem->getID();
  if(windowID == -1)
    return;
  for(int i=0;i<nID;i++)
  {
    MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,pIDs[i]);
    if(pGtkItem == NULL)
      continue;
    GtkWidget* pWidget = pGtkItem->getGtkWidgetPtr();
    if(pGtkItem->isLabel())
    {
      MSetLabelFromConfigure(pWidget,pGtkItem,pGUI,windowID,nID,pIDs); 
    }
    else if(pGtkItem->isTextEntry())
    {
      MSetTextEntryFromConfigure(pWidget,pGtkItem); 
    }
    else if(pGtkItem->isComboBox())
    {
      MSetComboBoxFromConfigure(pWidget,pGtkItem); 
    }
  }
}

void MSaveConfigureSettings(GtkWidget *widget, gpointer  data, char* fileName)
{
  MEmitSignals(widget,data);
  MSaveToFile(fileName);
}

void MEmitSignals(GtkWidget *widget, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  int nID = pdata->numID;
  int* pIDs = pdata->interactiveID;
  if(nID == 0 || pIDs == NULL)
    return;
  GUIContainer* pGUI = pdata->pcon;
  assert(pGUI);
  MGtkToolItem* pmGtkToolItem = (MGtkToolItem*)pdata->pmGtk;
  assert(pmGtkToolItem);
  int windowID = pmGtkToolItem->getID();
  if(windowID == -1)
    return;
  for(int i=0;i<nID;i++)
  {
    MGtkItemBase* pGtkItem = pGUI->getMGtkItem(windowID / MAX_WINDOW_IDS - 1,pIDs[i]);
    if(pGtkItem == NULL)
      continue;
    GtkWidget* pWidget = pGtkItem->getGtkWidgetPtr();
    if(pGtkItem->isTextEntry())
      g_signal_emit_by_name(G_OBJECT(pWidget),"activate",NULL,NULL);
    else if(pGtkItem->isComboBox())
    {
      MGtkComboBox* pCombo = (MGtkComboBox*)pGtkItem;
      if(!pCombo)
	continue;
      if(pCombo->isEditable())
      {
	GtkWidget* pWidgetComboEntry = gtk_bin_get_child(GTK_BIN(pWidget));
        g_signal_emit_by_name(G_OBJECT(pWidgetComboEntry),"activate",NULL,NULL);
      }
      else
      {
        MSaveComboBoxToConfigure(pWidget,pGtkItem);
      }
    }
    else if(pGtkItem->isLabel())
    {
      MSaveLabelToConfigure(pWidget,pGtkItem);
    }
  }
}

void MSaveToFile(char* fileName)
{
  if(fileName == NULL)
    return;
  FILE* fp = NULL;
  if((fp = fopen(fileName,"w")) == NULL)
  {
    POPUP_ERROR("Save to File Failed!");
    return;
  }
  confParas.writeToFile(fp);
  fclose(fp);
}

void MSaveComboBoxToConfigure(GtkWidget *widget, gpointer  data)
{
  if(widget == NULL)
   return;
  MGtkComboBox* pdata = (MGtkComboBox*)data;
  assert(pdata);
  int id = pdata->getID();
  char** items = pdata->getItems();
  switch(id)
  {
    case CONFIGURE_COMBOBOX_MODEL:
      if(confParas.models && confParas.nModels > 0)
      {
        for(int i=0;i<confParas.nModels;i++)
	  delete [] confParas.models[i];
	delete [] confParas.models;
      }
      confParas.nModels = pdata->getNumItems();
      confParas.modelIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
      if(items == NULL)
        confParas.models = NULL;
      else
      {
        confParas.models = new char* [confParas.nModels];
        for(int i=0;i<confParas.nModels;i++)
	  confParas.models[i] = strdup(items[i]);
      }
      break;
    case CONFIGURE_COMBOBOX_VIEW:
      if(confParas.views && confParas.nViews > 0)
      {
        for(int i=0;i<confParas.nViews;i++)
	  delete [] confParas.views[i];
	delete [] confParas.views;
      }
      confParas.nViews = pdata->getNumItems();
      confParas.viewIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
      if(items == NULL)
        confParas.views = NULL;
      else
      {
        confParas.views = new char* [confParas.nViews];
        for(int i=0;i<confParas.nViews;i++)
	  confParas.views[i] = strdup(items[i]);
      }
      break;
    case CONFIGURE_COMBOBOX_P1_NAME:
      {
      if(confParas.p1Name)
	delete [] confParas.p1Name;
      int index = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
      if(items == NULL || items[index] == NULL)
	confParas.p1Name = NULL;
      else
        confParas.p1Name = strdup(items[index]);
      }
      break;
    case CONFIGURE_COMBOBOX_P2_NAME:
      {
      if(confParas.p2Name)
	delete [] confParas.p2Name;
      int index = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
      if(items == NULL || items[index] == NULL)
	confParas.p2Name = NULL;
      else
        confParas.p2Name = strdup(items[index]);
      }
      break;
    default:
      break;
  }
}

void MSaveLabelToConfigure(GtkWidget *widget, gpointer  data)
{
  if(widget == NULL)
    return;
  MGtkItemBase* pdata = (MGtkItemBase*)data;
  assert(pdata);
  int id = pdata->getID();
  char* text = (char*)gtk_label_get_text(GTK_LABEL(widget));
  char* value = NULL;
  if(text && strcmp(text,"") && strcmp(text,"(NONE)"))
    value = text;
  switch(id)
  {
    /*case CONFIGURE_LABEL_CONF_PATH:	if(confParas.confPath) 
					  delete [] confParas.confPath;
					if(value)
					  confParas.confPath = strdup(value);
					else
					  confParas.confPath = NULL;
					break;*/
    case CONFIGURE_LABEL_CIR_PATH:	if(confParas.cirPath)
					  delete [] confParas.cirPath;
					if(value)
					  confParas.cirPath = strdup(value);
					else
					  confParas.cirPath = NULL;
					break;
    case CONFIGURE_LABEL_LIB_PATH:	if(confParas.libPath)
					  delete [] confParas.libPath;
					if(value)
					  confParas.libPath = strdup(value);
					else
					  confParas.libPath = NULL;
					break;
    default:break;
  }
}

void MSetLabelFromConfigure(GtkWidget *widget, gpointer  data,
			    GUIContainer* pGUI, int windowID, int nid, int* pIDs)
{
  if(widget == NULL)
    return;
  MGtkItemBase* pdata = (MGtkItemBase*)data;
  assert(pdata);
  int id = pdata->getID();
  char* text = NULL;
  switch(id)
  {
    case CONFIGURE_LABEL_CONF_PATH:
     if(confParas.confPath)
       text = strdup(confParas.confPath);
     break;
    case CONFIGURE_LABEL_CIR_PATH:
     if(confParas.cirPath)
     {
       text = strdup(confParas.cirPath);
       MSetItemListAccordingToFile(confParas.cirPath,pGUI,windowID,nid,pIDs);
     }
     break; 
    case CONFIGURE_LABEL_LIB_PATH:
     if(confParas.libPath)
       text = strdup(confParas.libPath);
     break;
    default:
     break;
  }
  if(text == NULL)
    text = strdup("(NONE)");
  gtk_label_set_text(GTK_LABEL(widget),text);
  delete [] text;
}

void MSetTextEntryFromConfigure(GtkWidget *widget, gpointer  data)
{
  if(widget == NULL)
    return;
  MGtkItemBase* pdata = (MGtkItemBase*)data;
  assert(pdata);
  int id = pdata->getID();
  char* text = NULL;
  switch(id)
  {
    case CONFIGURE_ENTRY_FREQ_FROM:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.freqFrom);
     break;
    case CONFIGURE_ENTRY_FREQ_TO:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.freqTo);
     break; 
    case CONFIGURE_ENTRY_FREQ_N:
     text = new char [MAX_CHARS];
     sprintf(text,"%d",confParas.freqN);
     break;
    case CONFIGURE_ENTRY_P1_VAL_FROM:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.p1ValFrom);
     break;
    case CONFIGURE_ENTRY_P1_VAL_TO:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.p1ValTo);
     break;
    case CONFIGURE_ENTRY_P1_VAL_N:
     text = new char [MAX_CHARS];
     sprintf(text,"%d",confParas.p1ValN);
     break;
    case CONFIGURE_ENTRY_P1_VAL_SIGMA:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.p1ValSigma);
     break;
    case CONFIGURE_ENTRY_P2_VAL_FROM:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.p2ValFrom);
     break;
    case CONFIGURE_ENTRY_P2_VAL_TO:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.p2ValTo);
     break;
    case CONFIGURE_ENTRY_P2_VAL_N:
     text = new char [MAX_CHARS];
     sprintf(text,"%d",confParas.p2ValN);
     break;
    case CONFIGURE_ENTRY_P2_VAL_SIGMA:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.p2ValSigma);
     break;
    case CONFIGURE_ENTRY_CORRELATION:
     text = new char [MAX_CHARS];
     sprintf(text,"%g",confParas.correlation);
     break;
    default:
     break;
  }
  if(text == NULL)
    text = strdup("");
  gtk_entry_set_text(GTK_ENTRY(widget),text);
  delete [] text;
}

void MSetComboBoxFromConfigure(GtkWidget *widget, gpointer  data)
{
  if(widget == NULL)
    return;
  MGtkComboBox* pdata = (MGtkComboBox*)data;
  assert(pdata);
  int id = pdata->getID();
  char** items = pdata->getItems();
  int n = pdata->getNumItems();
  switch(id)
  {
    case CONFIGURE_COMBOBOX_MODEL:
      /*if(items)
      {
        for(int i=0;i<n;i++)
          delete [] items[i];
        delete [] items;
      }*/
      if(confParas.nModels <= 0)
      {
	//pdata->setItems(NULL);
	//pdata->setNumItems(0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);
      }
      else
      {
	/*items = new char* [confParas.nModels];
	for(int i=0;i<confParas.nModels;i++)
	  items[i] = strdup(confParas.models[i]);
        pdata->setItems(items);
	pdata->setNumItems(confParas.nModels);*/
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget),confParas.modelIndex);
      }
      break;
    case CONFIGURE_COMBOBOX_VIEW:
      /*if(items)
      {
        for(int i=0;i<n;i++)
          delete [] items[i];
        delete [] items;
      }*/
      if(confParas.nViews <= 0)
      {
	//pdata->setItems(NULL);
	//pdata->setNumItems(0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);
      }
      else
      {
	/*items = new char* [confParas.nViews];
	for(int i=0;i<confParas.nViews;i++)
	  items[i] = strdup(confParas.views[i]);
        pdata->setItems(items);
	pdata->setNumItems(confParas.nViews);*/
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget),confParas.viewIndex);
      }
      break;
    case CONFIGURE_COMBOBOX_P1_NAME:
      if(items == NULL)
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);
      else
      {
	bool found = false;
	if(confParas.p1Name)
	{
          for(int i=0;i<n;i++)
	  {
	    if(!strcmp(items[i],confParas.p1Name))
	    {
	      gtk_combo_box_set_active(GTK_COMBO_BOX(widget),i);
	      found = true;
	      break;
	    }
	  }
	}
	if(!found)
	  gtk_combo_box_set_active(GTK_COMBO_BOX(widget),0);
      }
      break;
    case CONFIGURE_COMBOENTRY_P1_ALIAS:
      if(confParas.p1Alias == NULL)
        gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);
      else
      {
        bool found = false;
	if(items)
	{
	  for(int i=0;i<n;i++)
	  {
	    if(!strcmp(items[i],confParas.p1Alias))
	    {
              gtk_combo_box_set_active(GTK_COMBO_BOX(widget),i);
	      found = true;
	      break;
	    }
	  }
	}
	if(!found)
	{
	  GtkEntry* pEntry = (GtkEntry*)gtk_bin_get_child(GTK_BIN(widget));
	  gtk_entry_set_text(GTK_ENTRY(pEntry),confParas.p1Alias);
	}
      }
      break;
    case CONFIGURE_COMBOBOX_P2_NAME:
      if(items == NULL)
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);
      else
      {
	bool found = false;
	if(confParas.p2Name)
	{
          for(int i=0;i<n;i++)
	  {
	    if(!strcmp(items[i],confParas.p2Name))
	    {
	      gtk_combo_box_set_active(GTK_COMBO_BOX(widget),i);
	      found = true;
	      break;
	    }
	  }
	}
	if(!found)
	  gtk_combo_box_set_active(GTK_COMBO_BOX(widget),0);
      }
      break;
    case CONFIGURE_COMBOENTRY_P2_ALIAS:
      if(confParas.p2Alias == NULL)
        gtk_combo_box_set_active(GTK_COMBO_BOX(widget),-1);
      else
      {
        bool found = false;
	if(items)
	{
	  for(int i=0;i<n;i++)
	  {
	    if(!strcmp(items[i],confParas.p2Alias))
	    {
              gtk_combo_box_set_active(GTK_COMBO_BOX(widget),i);
	      found = true;
	      break;
	    }
	  }
	}
	if(!found)
	{
	  GtkEntry* pEntry = (GtkEntry*)gtk_bin_get_child(GTK_BIN(widget));
	  gtk_entry_set_text(GTK_ENTRY(pEntry),confParas.p2Alias);
	}
      }
      break;
    default:
      break;
  }
}

char* MPopupFileChooserDialogOpen(char* msg, char** filterName, int* filterNum, char* defFileName)
{
  char* fileName = NULL;
  GtkWidget* dialog = gtk_file_chooser_dialog_new(msg,NULL,GTK_FILE_CHOOSER_ACTION_OPEN,
						  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
						  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						  NULL);
  if(filterName && filterNum)
  {
    for(int i=0;i<filterNum[0];i++)
    {
      GtkFileFilter* filter = gtk_file_filter_new();
      gtk_file_filter_set_name(filter,filterName[i]);
      for(int j=0;j<filterNum[i+1];j++)
      {
        gtk_file_filter_add_pattern(filter,filterName[filterNum[0]+j+i*filterNum[i]]);
      }
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter);
    }
  }
  if(defFileName)
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),defFileName);
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));    
  }
  gtk_widget_destroy(dialog);
  return fileName;
}

char* MPopupFileChooserDialogSave(char* msg, char** filterName, int* filterNum, char* defFileName)
{
  char* fileName = NULL;
  GtkWidget* dialog = gtk_file_chooser_dialog_new(msg,NULL,GTK_FILE_CHOOSER_ACTION_SAVE,
						  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
						  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						  NULL);
#ifdef GTK2_8
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),TRUE);
#endif
  if(filterName && filterNum)
  {
    for(int i=0;i<filterNum[0];i++)
    {
      GtkFileFilter* filter = gtk_file_filter_new();
      gtk_file_filter_set_name(filter,filterName[i]);
      for(int j=0;j<filterNum[i+1];j++)
      {
        gtk_file_filter_add_pattern(filter,filterName[filterNum[0]+j+i*filterNum[i]]);
      }
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter);
    }
  }
  if(defFileName)
  {
    if(access(defFileName,0) || !g_path_is_absolute(defFileName))
    {
      //gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), ".");
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), defFileName);
    }
    else
    {
      gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),defFileName);
    }
  }
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));    
  }
  gtk_widget_destroy(dialog);
  return fileName;
}
