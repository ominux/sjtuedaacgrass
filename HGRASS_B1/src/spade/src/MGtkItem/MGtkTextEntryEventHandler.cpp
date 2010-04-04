
#include "MGtkTextEntry.hpp"
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
void MGtkTextEntryActivate(GtkWidget *widget, gpointer data)
{
   char* text = (char*)gtk_entry_get_text(GTK_ENTRY(widget));
   char* value = NULL;
   if(text && strcmp(text,""))
     value = text;
   GtkCFData* pdata = (GtkCFData*)data;
   assert(pdata);
   MGtkItemBase* pmGtkItemBase = (MGtkItemBase*)pdata->pmGtk;
   assert(pmGtkItemBase);
   int ID = pmGtkItemBase->getID();
   switch(ID)
   {
     case CONFIGURE_ENTRY_FREQ_FROM:
       if(value == NULL)
	 confParas.freqFrom = 0;
       else
         confParas.freqFrom = atof(value);
       break;
     case CONFIGURE_ENTRY_FREQ_TO:
       if(value == NULL)
	 confParas.freqTo = 0;
       else
         confParas.freqTo = atof(value);
       break;
     case CONFIGURE_ENTRY_FREQ_N:
       if(value == NULL)
	 confParas.freqN = 0;
       else
         confParas.freqN = atoi(value);
       break;
     case CONFIGURE_ENTRY_P1_VAL_FROM:
       if(value == NULL)
	 confParas.p1ValFrom = 0;
       else
         confParas.p1ValFrom = atof(value);
       break;
     case CONFIGURE_ENTRY_P1_VAL_TO:
       if(value == NULL)
	 confParas.p1ValTo = 0;
       else
         confParas.p1ValTo = atof(value);
       break;
     case CONFIGURE_ENTRY_P1_VAL_N:
       if(value == NULL)
	 confParas.p1ValN = 0;
       else
         confParas.p1ValN = atoi(value);
       break;
     case CONFIGURE_ENTRY_P1_VAL_SIGMA:
       if(value == NULL)
	 confParas.p1ValSigma = 0;
       else
         confParas.p1ValSigma = atof(value);
       break;
     case CONFIGURE_ENTRY_P2_VAL_FROM:
       if(value == NULL)
	 confParas.p2ValFrom = 0;
       else
         confParas.p2ValFrom = atof(value);
       break;
     case CONFIGURE_ENTRY_P2_VAL_TO:
       if(value == NULL)
	 confParas.p2ValTo = 0;
       else
         confParas.p2ValTo = atof(value);
       break;
     case CONFIGURE_ENTRY_P2_VAL_N:
       if(value == NULL)
	 confParas.p2ValN = 0;
       else
         confParas.p2ValN = atoi(value);
       break;
     case CONFIGURE_ENTRY_P2_VAL_SIGMA:
       if(value == NULL)
	 confParas.p2ValSigma = 0;
       else
         confParas.p2ValSigma = atof(value);
       break;
     case CONFIGURE_ENTRY_CORRELATION:
       if(value == NULL)
	 confParas.correlation = 0;
       else
         confParas.correlation = atof(value);
       break;
     default:
       break;
   }
}

void MGtkTextEntryChanged(GtkWidget *widget, gpointer data)
{
   //printf("Change Detected!\n");
}
