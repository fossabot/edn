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

#include <tools_debug.h>
#include <tools_globals.h>
#include <MsgBroadcast.h>


// new list of edn event generic : 
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
	extern const char* const ednMsgBufferState      = "edn-Msg-Buffer-State";
	extern const char* const ednMsgBufferName       = "edn-Msg-Buffer-Name";
	extern const char* const ednMsgBufferId         = "edn-Msg-Buffer-Id";
	extern const char* const ednMsgOpenFile         = "edn-Msg-OpenFile";
	
	extern const char* const ednMsgBufferListChange = "edn-Msg-BufferListChange";
	
	extern const char* const ednMsgBufferColor      = "edn-Msg-Buffer-Color";


// old ... 

extern const char* const ednMsgBufferChangeCurrent           = "edn-Msg-Buffer-Change-Current";
extern const char* const ednMsgUserDisplayChange             = "edn-Msg-User-Display-Change";

extern const char* const ednMsgBufferRemove                  = "edn-Msg-Buffer-Remove";
extern const char* const ednMsgBufferRemoveAll               = "edn-Msg-Buffer-Remove-All";
extern const char* const ednMsgBufferAdd                     = "edn-Msg-Buffer-Add";
extern const char* const ednMsgBufferChangeState             = "edn-Msg-Buffer-Change-State";
extern const char* const ednMsgBufferChangeName              = "edn-Msg-Buffer-Change-Name";
extern const char* const ednMsgBufferModify                  = "edn-Msg-Buffer-Modify";
extern const char* const ednMsgBufferHasHistory              = "edn-Msg-Buffer-Has-History";
extern const char* const ednMsgBufferHasNotHistory           = "edn-Msg-Buffer-Has-Not-History";
extern const char* const ednMsgBufferHasFutureHistory        = "edn-Msg-Buffer-Has-Future-History";
extern const char* const ednMsgBufferHasNotFutureHistory     = "edn-Msg-Buffer-Has-Not-Future-History";

extern const char* const ednMsgBufferManagerNewFile          = "edn-Msg-Buffer-Manager-New-File";
extern const char* const ednMsgBufferManagerSaveAll          = "edn-Msg-Buffer-Manager-Save-All";
extern const char* const ednMsgBufferManagerCloseAll         = "edn-Msg-Buffer-Manager-Close-All";
extern const char* const ednMsgBufferManagerClose            = "edn-Msg-Buffer-Manager-Close";
extern const char* const ednMsgBufferManagerSave             = "edn-Msg-Buffer-Manager-Save";

extern const char* const ednMsgCodeViewCurrentChangeBufferId = "edn-Msg-CodeView-Current-Change-Buffer-Id";
extern const char* const ednMsgCodeViewCurrentSave           = "edn-Msg-CodeView-Current-Save";
extern const char* const ednMsgCodeViewCurrentSaveAs         = "edn-Msg-CodeView-Current-Save-As";
extern const char* const ednMsgCodeViewCurrentSelectAll      = "edn-Msg-CodeView-Current-Select-All";
extern const char* const ednMsgCodeViewCurrentRemoveLine     = "edn-Msg-CodeView-Current-Remove-Line";
extern const char* const ednMsgCodeViewCurrentUnSelect       = "edn-Msg-CodeView-Current-Un-Select";
extern const char* const ednMsgCodeViewCurrentCopy           = "edn-Msg-CodeView-Current-Copy";
extern const char* const ednMsgCodeViewCurrentCut            = "edn-Msg-CodeView-Current-Cut";
extern const char* const ednMsgCodeViewCurrentPaste          = "edn-Msg-CodeView-Current-Paste";
extern const char* const ednMsgCodeViewCurrentFindPrevious   = "edn-Msg-CodeView-Current-Find-Previous";
extern const char* const ednMsgCodeViewCurrentFindNext       = "edn-Msg-CodeView-Current-Find-Next";
extern const char* const ednMsgCodeViewCurrentFindOldNext    = "edn-Msg-CodeView-Current-Find-Old-Next";
extern const char* const ednMsgCodeViewCurrentReplace        = "edn-Msg-CodeView-Current-Replace";
extern const char* const ednMsgCodeViewCurrentReplaceAll     = "edn-Msg-CodeView-Current-Replace-All";
extern const char* const ednMsgCodeViewCurrentClose          = "edn-Msg-CodeView-Current-Close";
extern const char* const ednMsgCodeViewCurrentUndo           = "edn-Msg-CodeView-Current-Undo";
extern const char* const ednMsgCodeViewCurrentRedo           = "edn-Msg-CodeView-Current-Redo";
extern const char* const ednMsgCodeViewCurrentGotoLine       = "edn-Msg-CodeView-Current-Goto-Line";
extern const char* const ednMsgCodeViewCurrentSetCharset     = "edn-Msg-CodeView-Current-Set-Charset";

extern const char* const ednMsgCtagsOpen                     = "edn-Msg-Ctags-Open";
extern const char* const ednMsgCtagsReload                   = "edn-Msg-Ctags-Reload";
extern const char* const ednMsgCtagsJumpCurrentSelection     = "edn-Msg-Ctags-Jump-Current-Selection";
extern const char* const ednMsgCtagsJumpBack                 = "edn-Msg-Ctags-Jump-Back";

extern const char* const ednMsgColorHasChange                = "edn-Msg-Color-Has-Change";

extern const char* const ednMsgReloadColorFile               = "edn-Msg-Reload-Color-File";

