/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
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
extern const char* const ednMsgProperties    = "edn-Msg-Gui-Properties";
extern const char* const ednMsgGuiExit       = "edn-Msg-Gui-quit";

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
extern const char* const ednMsgCtagsLoadFile    = "edn-Msg-CTags-direct-load";

extern const char* const ednMsgGuiReloadShader  = "edn-Msg-Gui-ReloadOpenGlShader";


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


extern const char* const appl::MsgSelectNewFile    = "edn-msg-select-new-file";
extern const char* const appl::MsgSelectChange     = "edn-msg-select-change";


