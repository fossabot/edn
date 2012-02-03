/**
 *******************************************************************************
 * @file MsgBroadcast.h
 * @brief Editeur De N'ours : message beetween thread and GUI elements ... (header)
 * @author Edouard DUPIN
 * @date 04/02/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
 
#ifndef __MSG_BROADCAST_H__
#define __MSG_BROADCAST_H__

#include <tools_debug.h>
#include <etk/String.h>


// new list of edn event generic : 
////////////////////////////////////////////////////////////////////////
// Event of the gui request something :
////////////////////////////////////////////////////////////////////////
	extern const char* const ednMsgGuiNew;        // data : ""
	extern const char* const ednMsgGuiOpen;       // data : ""
	extern const char* const ednMsgGuiClose;      // data : "current" "All"
	extern const char* const ednMsgGuiSave;       // data : ""
	extern const char* const ednMsgGuiSaveAs;     // data : ""
	
	extern const char* const ednMsgGuiUndo;       // data : ""
	extern const char* const ednMsgGuiRedo;       // data : ""
	extern const char* const ednMsgGuiCopy;       // data : "STD" "Middle" "1" ... "9"
	extern const char* const ednMsgGuiCut;        // data : "STD" "Middle" "1" ... "9"
	extern const char* const ednMsgGuiPaste;      // data : "STD" "Middle" "1" ... "9"
	extern const char* const ednMsgGuiRm;         // data : "Word" "Line" "Paragraph"
	extern const char* const ednMsgGuiSelect;     // data : "ALL" "NONE"
	extern const char* const ednMsgGuiGotoLine;   // data : "???" / "1" ... "999999999999"
	
	extern const char* const ednMsgGuiSearch;     // data : ""
	extern const char* const ednMsgGuiReplace;    // data : ""
	extern const char* const ednMsgGuiFind;       // data : "Next" "Previous"
	
	extern const char* const ednMsgGuiChangeColor;   // data : "Black" "White"
	extern const char* const ednMsgGuiChangeCharset; // data : "UTF-8" "ISO-8859-1" "ISO-8859-15"
	extern const char* const ednMsgGuiShowSpaces;    // data : "enable" "disable"
	extern const char* const ednMsgGuiShowEndOfLine; // data : "enable" "disable"
	
	extern const char* const ednMsgGuiCtagsLoad;     // data : "Load" "ReLoad" "Jump" "Back"
	
	extern const char* const ednMsgGuiAbout;         // data : ""


////////////////////////////////////////////////////////////////////////
// Event internal :
////////////////////////////////////////////////////////////////////////
	extern const char* const ednMsgBufferState;      // data : "Saved" "Modify" "HasHistory" "HasNotHistory" "HasFutureHistory" "HasNotFutureHistory"
	extern const char* const ednMsgBufferName;       // data : "filename"
	extern const char* const ednMsgBufferId;         // data : "0" ... "99999999999"
	extern const char* const ednMsgOpenFile;         // data : "/Compleate/file/name.xx"
	
	extern const char* const ednMsgBufferListChange; // data : ""
	
	extern const char* const ednMsgBufferColor;      // data : "new"




// old list : 

extern const char* const ednMsgBufferChangeCurrent;  // set the new current BUFFER ...
extern const char* const ednMsgUserDisplayChange;    // User change the display ==> need to reload all the display depending on color internal

extern const char* const ednMsgBufferRemove;
extern const char* const ednMsgBufferRemoveAll;
extern const char* const ednMsgBufferAdd;
extern const char* const ednMsgBufferChangeState;
extern const char* const ednMsgBufferChangeName;
extern const char* const ednMsgBufferModify;
extern const char* const ednMsgBufferHasHistory;
extern const char* const ednMsgBufferHasNotHistory;
extern const char* const ednMsgBufferHasFutureHistory;
extern const char* const ednMsgBufferHasNotFutureHistory;

extern const char* const ednMsgBufferManagerNewFile;
extern const char* const ednMsgBufferManagerSaveAll;
extern const char* const ednMsgBufferManagerCloseAll;
extern const char* const ednMsgBufferManagerClose;
extern const char* const ednMsgBufferManagerSave;

extern const char* const ednMsgCodeViewCurrentChangeBufferId;
extern const char* const ednMsgCodeViewCurrentSave;
extern const char* const ednMsgCodeViewCurrentSaveAs;
extern const char* const ednMsgCodeViewCurrentSelectAll;
extern const char* const ednMsgCodeViewCurrentRemoveLine;
extern const char* const ednMsgCodeViewCurrentUnSelect;
extern const char* const ednMsgCodeViewCurrentCopy;
extern const char* const ednMsgCodeViewCurrentCut;
extern const char* const ednMsgCodeViewCurrentPaste;
extern const char* const ednMsgCodeViewCurrentFindPrevious;
extern const char* const ednMsgCodeViewCurrentFindNext;
extern const char* const ednMsgCodeViewCurrentFindOldNext;
extern const char* const ednMsgCodeViewCurrentReplace;
extern const char* const ednMsgCodeViewCurrentReplaceAll;
extern const char* const ednMsgCodeViewCurrentClose;
extern const char* const ednMsgCodeViewCurrentUndo;
extern const char* const ednMsgCodeViewCurrentRedo;
extern const char* const ednMsgCodeViewCurrentGotoLine;
extern const char* const ednMsgCodeViewCurrentSetCharset;

extern const char* const ednMsgCtagsOpen;
extern const char* const ednMsgCtagsReload;
extern const char* const ednMsgCtagsJumpCurrentSelection;
extern const char* const ednMsgCtagsJumpBack;

extern const char* const ednMsgColorHasChange;

extern const char* const ednMsgReloadColorFile;


#endif

