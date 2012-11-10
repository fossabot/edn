/**
 *******************************************************************************
 * @file appl/globalMsg.h
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

////////////////////////////////////////////////////////////////////////
// Event of the gui request something :
////////////////////////////////////////////////////////////////////////
	extern const char* const ednMsgGuiNew;        // data : ""
	extern const char* const ednMsgGuiOpen;       // data : ""
	extern const char* const ednMsgGuiClose;      // data : "current" "All"
	extern const char* const ednMsgGuiSave;       // data : ""
	extern const char* const ednMsgGuiSaveAs;     // data : ""
	extern const char* const ednMsgProperties;    // data : ""
	
	extern const char* const ednMsgGuiUndo;       // data : ""
	extern const char* const ednMsgGuiRedo;       // data : ""
	extern const char* const ednMsgGuiCopy;       // data : "STD" "Middle" "1" ... "9"
	extern const char* const ednMsgGuiCut;        // data : "STD" "Middle" "1" ... "9"
	extern const char* const ednMsgGuiPaste;      // data : "STD" "Middle" "1" ... "9"
	extern const char* const ednMsgGuiRm;         // data : "Word" "Line" "Paragraph"
	extern const char* const ednMsgGuiSelect;     // data : "ALL" "NONE"
	extern const char* const ednMsgGuiGotoLine;   // data : "???" / "1" ... "999999999999"
	
	extern const char* const ednMsgGuiSearch;     // data : ""
	extern const char* const ednMsgGuiReplace;    // data : "Normal" "All"
	extern const char* const ednMsgGuiFind;       // data : "Next" "Previous" "All" "None"
	
	extern const char* const ednMsgGuiChangeColor;   // data : "Black" "White"
	extern const char* const ednMsgGuiChangeCharset; // data : "UTF-8" "ISO-8859-1" "ISO-8859-15"
	extern const char* const ednMsgGuiShowSpaces;    // data : "enable" "disable"
	extern const char* const ednMsgGuiShowEndOfLine; // data : "enable" "disable"
	
	extern const char* const ednMsgGuiCtags;         // data : "Load" "ReLoad" "Jump" "Back"
	extern const char* const ednMsgCtagsLoadFile;    // data : "filename of the ctags file"
	
	extern const char* const ednMsgGuiReloadShader;  // data : ""


////////////////////////////////////////////////////////////////////////
// Event internal :
////////////////////////////////////////////////////////////////////////
	extern const char* const ednMsgBufferState;        // data : "Saved" "Modify" "HasHistory" "HasNotHistory" "HasFutureHistory" "HasNotFutureHistory"
	extern const char* const ednMsgBufferName;         // data : "filename"
	extern const char* const ednMsgBufferId;           // data : "0" ... "99999999999"
	extern const char* const ednMsgCodeViewSelectedId; // data : "0" ... "99999999999"
	extern const char* const ednMsgOpenFile;           // data : "/Compleate/file/name.xx"
	
	extern const char* const ednMsgBufferListChange; // data : ""
	
	extern const char* const ednMsgBufferColor;      // data : "new"


#endif

