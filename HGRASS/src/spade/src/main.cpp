#include "MGUI/GUIContainer.hpp"
extern "C"
{
  #include <gtk/gtk.h>
}


int main(int argc, char **argv)
{
  GUIContainer* gc = new GUIContainer();
  if(gc)
  {
    gtk_init(&argc, &argv);
    gc->init();
  }
  delete gc;
  return 1;
}
