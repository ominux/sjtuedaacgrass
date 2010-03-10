
#include "MGtkMenu.hpp"
#include "../MGUI/GUIContainer.hpp"
#include "MGtkEventHandler.hpp"
void MGtkMenuActive(GtkWidget *widget, gpointer  data)
{
  GtkCFData* pdata = (GtkCFData*)data;
  assert(pdata);
  MGtkMenu* pGtkMenu = (MGtkMenu*) pdata->pmGtk;
  assert(pGtkMenu);
  char* pMsg = (char*)pdata->data;
  assert(pMsg);
  printf("%s\n",pMsg);
}
