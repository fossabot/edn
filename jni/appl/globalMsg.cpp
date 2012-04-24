/**
 *******************************************************************************
 * @file MsgBroadcast.cpp
 * @brief Editeur De N'ours : message beetween thread and GUI elements ... (Souces)
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

#include <appl/globalMsg.h>


////////////////////////////////////////////////////////////////////////
// Event of the gui request something :
////////////////////////////////////////////////////////////////////////
extern const char* const ednMsgGuiNew        = "edn-Msg-Gui-New";
extern const char* const ednMsgGuiOpen       = "edn-Msg-Gui-Open";
extern const char* const ednMsgGuiClose      = "edn-Msg-Gui-Close";
extern const char* const ednMsgGuiSave       = "edn-Msg-Gui-Save";
extern const char* const ednMsgGuiSaveAs     = "edn-Msg-Gui-SaveAs";

extern const char* const ednMsgGuiUndo       = "edn-Msg-Gui-Undo";
extern const char* const ednMsgGuiRedo       = "edn-Msg-Gui-Redo";
extern const char* const ednMsgGuiCopy       = "edn-Msg-Gui-Copy";
extern const char* const ednMsgGuiCut        = "edn-Msg-Gui-Cut";
extern const char* const ednMsgGuiPaste      = "edn-Msg-Gui-Paste";
extern const char* const ednMsgGuiRm         = "edn-Msg-Gui-Rm";
extern const char* const ednMsgGuiSelect     = "edn-Msg-Gui-Select";
extern const char* const ednMsgGuiGotoLine   = "edn-Msg-Gui-GotoLine";

extern const char* const ednMsgGuiSearch     = "edn-Msg-Gui-Search";
extern const char* const ednMsgGuiReplace    = "edn-Msg-Gui-Replace";
extern const char* const ednMsgGuiFind       = "edn-Msg-Gui-Find";

extern const char* const ednMsgGuiChangeColor   = "edn-Msg-Gui-ChangeColor";
extern const char* const ednMsgGuiChangeCharset = "edn-Msg-Gui-ChangeCharset";
extern const char* const ednMsgGuiShowSpaces    = "edn-Msg-Gui-ShowSpaces";
extern const char* const ednMsgGuiShowEndOfLine = "edn-Msg-Gui-ShowEndOfLine";

extern const char* const ednMsgGuiCtags         = "edn-Msg-Gui-CTags";

extern const char* const ednMsgGuiAbout         = "edn-Msg-Gui-About";


////////////////////////////////////////////////////////////////////////
// Event internal :
////////////////////////////////////////////////////////////////////////
extern const char* const ednMsgBufferState         = "edn-Msg-Buffer-State";
extern const char* const ednMsgBufferName          = "edn-Msg-Buffer-Name";
extern const char* const ednMsgBufferId            = "edn-Msg-Buffer-Id";
extern const char* const ednMsgCodeViewSelectedId  = "edn-Msg-CodeView-Select-Id";
extern const char* const ednMsgOpenFile            = "edn-Msg-OpenFile";

extern const char* const ednMsgBufferListChange    = "edn-Msg-BufferListChange";

extern const char* const ednMsgBufferColor         = "edn-Msg-Buffer-Color";


