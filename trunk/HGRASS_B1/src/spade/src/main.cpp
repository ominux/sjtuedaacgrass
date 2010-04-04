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
    int ret = system(SCRIPT_CMD_INIT);
    if(ret != 0)
    {
      POPUP_ERROR("Initialization of the environment failed!\nPlease contact your lab administrator and retry later");
      delete gc;
      return 1;
    }
    gc->init();
    delete gc;
  }
  return 0;
}
