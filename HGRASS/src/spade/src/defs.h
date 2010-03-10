#ifndef DEFS_H
#define DEFS_H

//MACROS
#define MAX_CHARS 200
#define MAX_EVENTS 20
#define MAX_WINDOWS 10
#define MAX_TOOLITEM 20
#define MAX_COMBOBOXITEM 100
#define MAX_EVENTIDS 100
#define MAX_WINDOW_IDS 100
//Const
#include <libxml/parser.h>
#include <libxml/tree.h>

//For GUIContainer
#define sGTK_GC		BAD_CAST "GTK"
#define sWindow_GC	BAD_CAST "Window"
#define sBox_GC		BAD_CAST "Box"
#define sFrame_GC	BAD_CAST "Frame"
#define sButton_GC	BAD_CAST "Button"
#define sFCButton_GC	BAD_CAST "FileChooserButton"
#define sLabel_GC	BAD_CAST "Label"
#define sComboBox_GC	BAD_CAST "ComboBox"
#define sTextEntry_GC   BAD_CAST "TextEntry"
#define sMenuShell_GC	BAD_CAST "MenuShell"
#define sMenu_GC	BAD_CAST "Menu"
#define sEvent_GC	BAD_CAST "Event"
#define sEMask_GC	BAD_CAST "EventMask"
#define sTBar_GC	BAD_CAST "ToolBar"
#define sTItem_GC	BAD_CAST "ToolItem"
///MGtkItemBase
#define sID_IB		BAD_CAST "id"
#define sFixSize_IB	BAD_CAST "size"
#define sWidth_IB	BAD_CAST "width"
#define sHeight_IB	BAD_CAST "height"
#define sVisibility_IB	BAD_CAST "visibility"
#define sResEnable_IB	BAD_CAST "responseEnable"
#define sLabel_IB	BAD_CAST "label"
#define sBorderW_IB	BAD_CAST "borderWidth"
#define sBorderH_IB	BAD_CAST "borderHeight"
///Event
#define sEName_IB	BAD_CAST "name"
#define sEHandler_IB	BAD_CAST "handler"
#define sEData_IB	BAD_CAST "data"
#define sEID_IB		BAD_CAST "id"
#define sENumID_IB	BAD_CAST "nid"
///Event Type
#define sEMType_IB	BAD_CAST "type"
//For MGtkWindow
#define sWinType_IB	BAD_CAST "windowType"
//For Box
#define sBoxHSpace_IB	BAD_CAST "homospace"
#define sBoxSpace_IB	BAD_CAST "spacing"
#define sBoxDir_IB	BAD_CAST "dir"
#define sBoxExp_IB	BAD_CAST "expand"
#define sBoxFill_IB	BAD_CAST "fill"
#define sBoxPad_IB	BAD_CAST "padding"
#define sBoxAdDir_IB    BAD_CAST "sequence"
//For Frame
#define sFrameXA_IB     BAD_CAST "xalign"
#define sFrameYA_IB     BAD_CAST "yalign"
#define sFrameShadow_IB BAD_CAST "shadow"
//For ToolBar
#define sTBarStyle_IB	BAD_CAST "style"
#define sTBarOrien_IB	BAD_CAST "orientation"
//For ToolItem
#define sTItemTip_IB	BAD_CAST "tip"
#define sTItemPTip_IB	BAD_CAST "priTip"
#define sTItemIcon_IB	BAD_CAST "icon"
#define sTItemPos_IB	BAD_CAST "position"
#define sTItemHColor_IB	BAD_CAST "highlight_color"
#define sTItemDraw_IB	BAD_CAST "draw"
//For Menu & Button
#define sMBMnemonic_IB	BAD_CAST "mnemonic"
#define sMBStock_IB	BAD_CAST "stock"
#define sBImage_IB	BAD_CAST "image"
#define sBImageP_IB	BAD_CAST "image_position"
#define sBRelief_IB	BAD_CAST "relief"
//For TextEntry
#define sTECharW_IB	BAD_CAST "char_width"
#define sTEMaxLen_IB	BAD_CAST "max_length"
#define sTEHasFrame_IB	BAD_CAST "frame"
#define sTEEditable_IB	BAD_CAST "editable"
//For ComboBox
#define sCBItems_IB	BAD_CAST "items"
#define sCBNItems_IB	BAD_CAST "nitems"
//For Label
#define sLEllipsize_IB	BAD_CAST "ellipsize"
#define sLWrap_IB	BAD_CAST "wrap"

#define XML_CONFIG	"src/spade/xml/SPADE.xml"
#define DOC_HELP	"doc/help/index.html"
#define SCRIPT_CMD_VA	"script/IsCommandValid.tcl"
#define SCRIPT_CMD_RUN	"script/Run.tcl"
#define GRASS_CONF	"src/grass/GrassConfs"
#define HSPICE_ERR	"rpt/spiceerrlis.rpt"
#define GRASS_3D_ERR	"rpt/grass3derr.rpt"
#define GRASS_3D_RUN	"rpt/grass3drun.rpt"
#define GRASS_2D_ERR	"rpt/grass2derr.rpt"
#define GRASS_2D_RUN	"rpt/grass2drun.rpt"

//MACRO FUNCTION
#ifndef MAX
#define MAX(a,b) (a)>(b)?(a):(b)
#endif
#ifndef MIN
#define MIN(a,b) (a)<(b)?(a):(b)
#endif

#define CHECK_COMMAND_VALIDITY(cmd,msg) \
        char* mScriptTmp = SCRIPT_CMD_VA;\
        char mCmdTmp [MAX_CHARS];\
        sprintf(mCmdTmp,"%s %s",mScriptTmp,(cmd));\
        int mRetTmp = system(mCmdTmp);\
        if(WEXITSTATUS(mRetTmp))\
        {\
          POPUP_ERROR((msg));\
	  return;\
	}

#define CHECK_FILE_VALIDITY(file,msg) \
        if(access(file,0))\
        {\
          POPUP_ERROR((msg));\
	  return;\
	}

//Dialogs
//Infos
#define POPUP_INFORMATION(str) \
        GtkWidget *dialog;  \
        dialog = gtk_message_dialog_new(NULL,\
                 GTK_DIALOG_DESTROY_WITH_PARENT,\
		 GTK_MESSAGE_INFO,\
		 GTK_BUTTONS_OK,\
		 (str));\
        gtk_window_set_title(GTK_WINDOW(dialog),"Information");\
	gtk_dialog_run(GTK_DIALOG(dialog));\
	gtk_widget_destroy(dialog)
//Warnings
#define POPUP_WARNING(str) \
        GtkWidget *dialog;  \
        dialog = gtk_message_dialog_new(NULL,\
                 GTK_DIALOG_DESTROY_WITH_PARENT,\
		 GTK_MESSAGE_WARNING,\
		 GTK_BUTTONS_OK,\
		 (str));\
        gtk_window_set_title(GTK_WINDOW(dialog),"Warning");\
	gtk_dialog_run(GTK_DIALOG(dialog));\
	gtk_widget_destroy(dialog)
//Errors
#define POPUP_ERROR(str) \
        GtkWidget *dialog;  \
        dialog = gtk_message_dialog_new(NULL,\
                 GTK_DIALOG_DESTROY_WITH_PARENT,\
		 GTK_MESSAGE_ERROR,\
		 GTK_BUTTONS_OK,\
		 (str));\
        gtk_window_set_title(GTK_WINDOW(dialog),"Error");\
	gtk_dialog_run(GTK_DIALOG(dialog));\
	gtk_widget_destroy(dialog)
//Questions
#define POPUP_QUESTION(str) \
        GtkWidget *dialog;  \
        dialog = gtk_message_dialog_new(NULL,\
                 GTK_DIALOG_DESTROY_WITH_PARENT,\
		 GTK_MESSAGE_QUESTION,\
		 GTK_BUTTONS_YES_NO,\
		 (str));\
        gtk_window_set_title(GTK_WINDOW(dialog),"YES_NO");\
	gtk_dialog_run(GTK_DIALOG(dialog));\
	gtk_widget_destroy(dialog)
//End of Dialogs


//IDs
#define CONFIGURE_BUTTON_LOAD		201
#define CONFIGURE_BUTTON_SAVE		202
#define CONFIGURE_BUTTON_SAVE_AS	203
#define CONFIGURE_BUTTON_RESET		204
#define CONFIGURE_LABEL_CONF_PATH	205
#define CONFIGURE_LABEL_CIR_PATH	206
#define CONFIGURE_BUTTON_CIR_OPEN	207
#define CONFIGURE_LABEL_LIB_PATH	208
#define CONFIGURE_BUTTON_LIB_OPEN	209
#define CONFIGURE_COMBOBOX_MODEL	210
#define CONFIGURE_COMBOBOX_VIEW		211
#define CONFIGURE_ENTRY_FREQ_FROM	212
#define CONFIGURE_ENTRY_FREQ_TO		213
#define CONFIGURE_ENTRY_FREQ_N		214
#define CONFIGURE_COMBOBOX_P1_NAME	215
#define CONFIGURE_COMBOENTRY_P1_ALIAS	216
#define CONFIGURE_ENTRY_P1_VAL_FROM	217
#define CONFIGURE_ENTRY_P1_VAL_TO	218
#define CONFIGURE_ENTRY_P1_VAL_N	219
#define CONFIGURE_ENTRY_P1_VAL_SIGMA	220
#define CONFIGURE_COMBOBOX_P2_NAME	221
#define CONFIGURE_COMBOENTRY_P2_ALIAS	222
#define CONFIGURE_ENTRY_P2_VAL_FROM	223
#define CONFIGURE_ENTRY_P2_VAL_TO	224
#define CONFIGURE_ENTRY_P2_VAL_N	225
#define CONFIGURE_ENTRY_P2_VAL_SIGMA	226
#define CONFIGURE_ENTRY_CORRELATION	227

#endif
