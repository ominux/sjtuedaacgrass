
#include "MGtkEventHandler.hpp"
#include "MGtkWindow.hpp"
#include "../MGUI/GUIContainer.hpp"

void MGtkDestroy(GtkWidget *widget, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  MGtkWindow* pGtkWindow = (MGtkWindow*) pdata->pmGtk;
  assert(pGtkWindow);
  if(pGtkWindow->isTopLevel())
    gtk_main_quit();
  GUIContainer* pGUICon = (GUIContainer*) pdata->pcon;
  assert(pGUICon);
  int id = pGtkWindow->getWinID();
  if(id == 0)
  {
    for(int i=0;i<pGUICon->getNumWindows();i++)
      pGUICon->deleteWindow(i);
  }
}
/*
void MGtkKeyPressed(GtkWidget *widget, GdkEventKey *event, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  MGtkWindow* pGtkWindow = (MGtkWindow*) pdata->pmGtk;
  assert(pGtkWindow);
  MGtkDrawArea* pGtkDrawArea = (MGtkDrawArea*)pGtkWindow->getDrawArea();
  assert(pGtkDrawArea);

  char* key = gdk_keyval_name(event->keyval);
  guint ks = event->state;

  if((ks & GDK_CONTROL_MASK)
  || !strcmp(key,"Control_L") 
  || !strcmp(key,"Control_R"))     //Ctrl+ 
  {
    //printf("Ctrl Key Pressed!\n");
    pGtkDrawArea->setCanMultiSel(true); 
  }
  if(ks & GDK_SHIFT_MASK)       //Shift+
  {
    //printf("Shift Key Pressed!\n");

  }
  if(ks & GDK_MOD1_MASK)        //Alt+  
  {
    //printf("Alt Key Pressed!\n");

  }
  if(ks & GDK_LOCK_MASK)        //Lock+ 
  {
    //printf("Lock Key Pressed!\n");

  }
  //GDK_META_MASK is supported Since 2.10
}

void MGtkKeyReleased(GtkWidget *widget, GdkEventKey *event, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  MGtkWindow* pGtkWindow = (MGtkWindow*) pdata->pmGtk;
  assert(pGtkWindow);
  MGtkDrawArea* pGtkDrawArea = (MGtkDrawArea*)pGtkWindow->getDrawArea();
  assert(pGtkDrawArea);

  char* key = gdk_keyval_name(event->keyval);
  guint ks = event->state;
  if(ks & GDK_CONTROL_MASK
  || !strcmp(key,"Control_L")
  || !strcmp(key,"Control_R"))     //Ctrl+
  {
    //printf("Ctrl Key Released!\n");
    pGtkDrawArea->setCanMultiSel(false);
  }
  if(ks & GDK_SHIFT_MASK)       //Shift+
  {
    //printf("Shift Key Released!\n");

  }
  if(ks & GDK_MOD1_MASK)        //Alt+
  {
    //printf("Alt Key Released!\n");

  }
  if(ks & GDK_LOCK_MASK)        //Lock+
  {
    //printf("Lock Key Released!\n");

  }
  //GDK_META_MASK is supported Since 2.10
}
*/
