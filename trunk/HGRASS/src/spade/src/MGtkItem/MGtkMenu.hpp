#ifndef MGTK_MENU_H
#define MGTK_MENU_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

class MGtkMenu : public MGtkItemBase
{
protected:
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  bool isValidEvent(MGtkEvent* pevent);
  //static char* curSEDFile;
  //static bool tranEnable;
  //static double tranStep;
  //static double tranFinal;
  //static UNIT tranTUnit;
public:
  MGtkMenu():MGtkItemBase()
  {
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    this->type = MENU;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
  /*static char* getCurSEDFile();
  static void  setCurSEDFile(char* name);
  static bool  getTranEnable();
  static void  setTranEnable(bool b);
  static double getTranStep();
  static void  setTranStep(double v);
  static double getTranFinal();
  static void  setTranFinal(double v);
  static UNIT  getTranTUnit();
  static void  setTranTUnit(UNIT tU);*/
};

#endif
