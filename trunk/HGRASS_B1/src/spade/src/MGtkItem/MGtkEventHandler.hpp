#ifndef MGTK_EVENT_HANDLER_H
#define MGTK_EVENT_HANDLER_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
}

#include <gtk/gtk.h>
#include "ConfigureParameters.hpp"

class GUIContainer;

extern ConfigureParameters confParas;
/******************************* MGtkBox *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
/******************************* MGtkBox *********************************/

/******************************* MGtkButton *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
void MGtkButtonPressed(GtkWidget *widget, gpointer data);
void MGtkButtonReleased(GtkWidget *widget, gpointer data);
void MGtkButtonClicked(GtkWidget *widget, gpointer data);
void MGtkButtonEntered(GtkWidget *widget, gpointer data);
void MGtkButtonLeft(GtkWidget *widget, gpointer data);
void MGtkButtonDoubleClicked(GtkWidget *widget, gpointer data);
void MGtkButtonActivated(GtkWidget *widget, gpointer data);
int GetItemListFromCirFile(char* fileName, char** &itemList);
void MSetItemListAccordingToFile(char* fileName, GUIContainer* pGUI, int windowID, int nid, int* pids);
/******************************* MGtkButton *********************************/

/******************************* MGtkComboBox *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
void MGtkComboBoxMoveActive(GtkWidget *widget, gpointer data);
void MGtkComboBoxEntryActivate(GtkWidget *widget, gpointer data);
void MGtkComboBoxChanged(GtkWidget *widget, gpointer data);
void MGtkComboBoxPopup(GtkWidget *widget, gpointer data);
void MGtkComboBoxPopdown(GtkWidget *widget, gpointer data);
/******************************* MGtkComboBox *********************************/

/******************************* MGtkFrame *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
/******************************* MGtkFrame *********************************/

/******************************* MGtkLabel *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
/******************************* MGtkLabel *********************************/

/******************************* MGtkMenu *********************************/
void MGtkMenuActive(GtkWidget *widget, gpointer  data);
/*
void MGtkMenuGenNetlist(GtkWidget *widget, gpointer  data);
void MGtkMenuSetAnalysis(GtkWidget *widget, gpointer  data);
void MGtkMenuOpenSEDFile(GtkWidget *widget, gpointer  data);
void MGtkMenuSaveSEDFile(GtkWidget *widget, gpointer  data);
void MGtkMenuSavAsSEDFile(GtkWidget *widget, gpointer  data);
void MGtkMenuFileSelOKSave(GtkWidget *widget, gpointer  data);
void MGtkMenuFileSelOKOpen(GtkWidget *widget, gpointer  data);
void MGtkMenuFileSelNO(GtkWidget *widget, gpointer  data);
void MGtkMenuFileSelDestroy(GtkWidget *widget, gpointer  data);
void makeFileSelection(char* info, char* defName, gpointer  data);
void saveSEDFile(gpointer  data);
void readSEDFile(gpointer  data);

void MGtkAnalyWinDestroy(GtkWidget *widget, gpointer  data);
void MGtkAnalyWinSaveProp(GtkWidget *widget, gpointer  data);
void MGtkAnalyWinCancelProp(GtkWidget *widget, gpointer  data);
void MGtkAnalyWinToggleDisplay(GtkWidget *widget, gpointer  data);
void MGtkAnalyWinSetPara(GtkWidget *widget, gpointer  data);
void MGtkAnalyWinEmitEntry(GtkWidget *widget, gpointer  data);
void MGtkAnalyWinToggleEntryCtrl(GtkWidget *widget, gpointer  data);
*/
/******************************* MGtkMenu *********************************/

/******************************* MGtkMenuShell *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
/******************************* MGtkMenuShell *********************************/

/******************************* MGtkTextEntry *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
void MGtkTextEntryActivate(GtkWidget *widget, gpointer data);
void MGtkTextEntryChanged(GtkWidget *widget, gpointer data);
/******************************* MGtkTextEntry *********************************/


/******************************* MGtkToolBar *********************************/
//void MGtkDestroy(GtkWidget *widget, gpointer  data);
/******************************* MGtkToolBar *********************************/


/******************************* MGtkToolItem *********************************/
void MGtkBarItemClicked(GtkWidget *widget, gpointer  data);
void MGtkToolItemClicked(GtkWidget *widget, gpointer  data);
void MGtkToolItemPressed(GtkWidget *widget, gpointer  data);
void MGtkToolItemReleased(GtkWidget *widget, gpointer  data);
void MPopupFormula(gpointer data);
void MPopupSchematicEditor(gpointer data);
void MPopupNetlistEditor(gpointer data);
void MPopupConfigure(gpointer data);
void MPopupSimulate(gpointer data);
void MPopupHelp(gpointer data);
void MProcessSimulateError(int errorNumber);
void MConfigureLoad(GtkWidget *widget, gpointer  data);
void MConfigureSave(GtkWidget *widget, gpointer  data);
void MConfigureSaveAs(GtkWidget *widget, gpointer  data);
void MConfigureReset(GtkWidget *widget, gpointer  data);
void MSaveConfigureSettings(GtkWidget *widget, gpointer  data, char* fileName);
void MLoadConfigureSettings(char* fileName);
void MUpdateConfigureSettings(GtkWidget *widget, gpointer  data);
void MEmitSignals(GtkWidget *widget, gpointer  data);
void MSaveToFile(char* fileName);
void MSaveComboBoxToConfigure(GtkWidget *widget, gpointer  data);
void MSaveLabelToConfigure(GtkWidget *widget, gpointer  data);
void MSetComboBoxFromConfigure(GtkWidget *widget, gpointer  data);
void MSetTextEntryFromConfigure(GtkWidget *widget, gpointer  data);
void MSetLabelFromConfigure(GtkWidget *widget, gpointer  data, 
			    GUIContainer* pGUI, int windowID, int nid, int* pIDs);
char* MPopupFileChooserDialogOpen(char* msg, char** filterName, int* filterNum, char* defFileName = NULL);
char* MPopupFileChooserDialogSave(char* msg, char** filterName, int* filterNum, char* defFileName = NULL);
/******************************* MGtkToolItem *********************************/


/******************************* MGtkWindow *********************************/
void MGtkDestroy(GtkWidget *widget, gpointer  data);
//void MGtkKeyPressed(GtkWidget *widget, GdkEventKey *event, gpointer  data);
//void MGtkKeyReleased(GtkWidget *widget, GdkEventKey *event, gpointer  data);
/******************************* MGtkWindow *********************************/

#endif
