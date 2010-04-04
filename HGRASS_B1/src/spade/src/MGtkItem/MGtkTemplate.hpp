#ifndef MGTK_TEMPLATE_H
#define MGTK_TEMPLATE_H

#include "../defs.h"
extern "C"
{
	#include <stdio.h>
//	#include <gtk/gtk.h>
}

#include <gtk/gtk.h>
#include "MGtkItemBase.hpp"

/**************************** Create A New MGtk* Class Steps Instructions ************************************/
/// To Create A New MGtk* Class, Please Follow the Steps Below:
/// 1. Name the class name same as Gtk, say for GtkBox class, name MGtkBox.
/// 2. Copy the template files MGtkTemplate.cpp, MGtkTemplate.hpp, MGtkTemplateEventhandler.cpp as
///    MGtkBox.cpp, MGtkBox.hpp, MGtkBoxEventhandler.cpp
/// 3. Modify each file:
///      MGtkBox.hpp:
///        a. Replace all words "Template" with "Box"
///        b. Add new class properties into the class, the properties of template is the basic ones, 
///           I suggest you not remove them
///        c. Set type properties from TEMPLATE to BOX, and add the new type into enum GtkWidgetType
///           of file MGtkItembase.hpp
///      MGtkBox.cpp:
///        a. Replace all words "Template" with "Box", include the "#include "MGtkTemplate.hpp"" line
///        b. Add the new properties parsing code into the function ::getXmlNodeProperties(xmlNode* node)
///             Rules are:
///               I.	Add the key word of the properties into the file "../defs.h" as the format:
///                  #define s[ItemName][PropertyName]_IB BAD_CAST "[Property Key Word in XML]", say
///                  #define sBoxHSpace_IB   BAD_CAST "homospace"
///               II.	The new property is a new "else if" branch with property type:
///                  bool:	this->[Property] = !xmlStrcmp(value, BAD_CAST "true") ? true : false;
///                  int:	this->[Property] = atoi((char*)value);
///                  double:	this->[Property] = atof((char*)value);
///                  char*:	this->[Property] = (char*)xmlStrdup(value);
///                  Other:     decide by yourself
///        c. Implement the function ::isValidEvent(MGtkEvent* pevent)
///             Rules are:
///               I.	For the first if with "ToDo" comment, please add judgement of event name,
///                  say clicked, activate, etc. by strcmp API
///               II.	For the second if with "ToDo" comment, please add judgement of event handler name,
///                  say MGtkDestroy, MGtkButtonClicked, etc. by strcmp API
///        d. Implement the function :: genGtkGUI()
///             Rules are:
///               I.	Do GtkWidget* new operation here, say ptr/*The GtkWidget Pointer*/=gtk_menu_new()
///               II.	Set the properties of the widget, but DO rememeber NOT remove the codes related to
///                  isFixedSize and responseEnable
///               III.	Add the event and function binding codes into the if(numEvents != 0){} body as comments
///               IV.	Keep the package codes and display codes untouched
///       MGtkBoxEventHandler.cpp:
///         a. Add the event handler callback function declarasions to the file "MGtkEventHandler.hpp" 
///            embraced by /**..**/ comments like other event handlers
///         b. Implememt the handlers in the MGtkBoxEventHandler.cpp file, remember the gpointer data is pointer
///            of class GtkCFData defined in the file "MGtkItemBase.hpp", thus please try to obtain the wanted 
///            data you want by pointer reference
///              Class GtkCFData
///              {
///                GUIContainer* pcon;	//GUI Top Class Object Pointer
///                MGtkItemBase* pmGtk;	//The Pointer of the Class Object Emitted the Signal Itself
///                void *data;		//char* of XML Event Properties "data"
///                int numID;		//Event Related Item ID Number
///                int* interactiveID;	//Event Related Item ID Pointer
///              }
///          c. Remember to change the include line into your class hpp file
/// 4. Add is[TYPE]() function into MGtkItemBase into file MGtkItemBase.hpp
/// 5. Add class include hpp file into file "../MGUI/GUIContainer.hpp"
/// 6. Add class new object codes into function "void GUIContainer::processNode(xmlNode * node, MGtkItemBase* gtkFaNode)"
///    of file "../MGUI/GUIContainer.cpp", say 
///      else if(!xmlStrcmp(curNode->name, sComboBox_GC))
///        gtkNode = new MGtkComboBox();
/// 7. Some basic macro defined in "../defs.h"
///      MAX_CHARS 200 : for new char* say, "char* str = new char [MAX_CHARS];" Remember to delete the new char pointer with
///                      "delete [] str;"
///      MAX / MIN 
///      CHECK_COMMAND_VALIDITY(cmd,msg): check "cmd" existence and exit with msg if error detected
///      CHECK_FILE_VALIDITY(file,msg):   check "file" existence and exit with msg if error detected
///      POPUP_INFORMATION(str):          popup dialog with str as information
///      POPUP_WARNING(str):              popup dialog with str as warning
///      POPUP_ERROR(str):                popup dialog with str as error
///      POPUP_QUESTION(str):             popup dialog with str as question
///      [ITEM IDs]: for each window, start from x00 to (x+1)00, say 200 to 300, x is the window index in XML
/************************* End of Creation New MGtk* Class Steps Instructions *********************************/


class MGtkTemplate : public MGtkItemBase
{
protected:
  bool homoSpace; //true : equal Spcace | false : content Decided
  int spacing;
  bool expand;
  bool fill;
  int padding;
  int seq;	//0 : follow Sequence | 1 : anti-
  bool isValidEvent(MGtkEvent* pevent);
public:
  MGtkTemplate():MGtkItemBase()
  {
    homoSpace = false;
    spacing = 0;
    expand = false;
    fill = false;
    padding = 0;
    seq = 0;
    this->type = TEMPLATE;  
  }
  void getXmlNodeProperties(xmlNode* node);
  void getXmlNodeEventProperties(xmlNode* node);
  void genGtkGUI(void);
};

#endif
