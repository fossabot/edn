/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __MSG_BROADCAST_H__
#define __MSG_BROADCAST_H__
namespace appl {
	class Broadcast : public ewol::Resource {
		public:
			Broadcast();
			void init(const std::string& _uniqueName);
		public:
			DECLARE_RESOURCE_SINGLE_FACTORY(Broadcast, "???Broadcast???");
			virtual ~Broadcast() {};
		public:
			/*
			ewol::object::Signal<> signalGuiNew;        // data : ""
			ewol::object::Signal<> signalGuiOpen;       // data : ""
			ewol::object::Signal<> signalGuiClose;      // data : "current" "All"
			ewol::object::Signal<> signalGuiSave;       // data : ""
			ewol::object::Signal<> signalSaveAs;     // data : ""
			ewol::object::Signal<> signalProperties;    // data : ""
			ewol::object::Signal<> signalGuiExit;       // data : ""
			
			ewol::object::Signal<> signalGuiUndo;       // data : ""
			ewol::object::Signal<> signalGuiRedo;       // data : ""
			ewol::object::Signal<> signalGuiCopy;       // data : "STD" "Middle" "1" ... "9"
			ewol::object::Signal<> signalGuiCut;        // data : "STD" "Middle" "1" ... "9"
			ewol::object::Signal<> signalGuiPaste;      // data : "STD" "Middle" "1" ... "9"
			ewol::object::Signal<> signalGuiRm;         // data : "Word" "Line" "Paragraph"
			ewol::object::Signal<> signalGuiSelect;     // data : "ALL" "NONE"
			ewol::object::Signal<> signalGuiGotoLine;   // data : "???" / "1" ... "999999999999"
			
			ewol::object::Signal<> signalGuiSearch;     // data : ""
			ewol::object::Signal<> signalGuiReplace;    // data : "Normal" "All"
			ewol::object::Signal<> signalGuiFind;       // data : "Next" "Previous" "All" "None"
			
			ewol::object::Signal<> signalShowSpaces;    // data : "enable" "disable"
			ewol::object::Signal<> signalShowEndOfLine; // data : "enable" "disable"
			
			ewol::object::Signal<> signalGuiCtags;         // data : "Load" "ReLoad" "Jump" "Back"
			ewol::object::Signal<> signalCtagsLoadFile;    // data : "filename of the ctags file"
			
			ewol::object::Signal<> signalGuiReloadShader;  // data : ""
			*/

////////////////////////////////////////////////////////////////////////
// Event internal :
////////////////////////////////////////////////////////////////////////
			ewol::object::Signal<std::string> signalBufferState;        // data : "Saved" "Modify" "HasHistory" "HasNotHistory" "HasFutureHistory" "HasNotFutureHistory"
			ewol::object::Signal<std::string> signalBufferName;         // data : "filename"
			ewol::object::Signal<void> signalBufferId;           // data : "0" ... "99999999999"
			ewol::object::Signal<void> signalCodeViewSelectedId; // data : "0" ... "99999999999"
			ewol::object::Signal<std::string> signalOpenFile;           // data : "/Compleate/file/name.xx"
			
			ewol::object::Signal<void> signalBufferListChange; // data : ""
			
			ewol::object::Signal<std::string> signalBufferColor;      // data : "new"
			
			ewol::object::Signal<std::string> signalSelectNewFile;      // data : "buffer/name"
			ewol::object::Signal<void> signalSelectChange;       // data : ""
			ewol::object::Signal<void> signalNameChange;         // data : ""
			ewol::object::Signal<std::string> signalNameGuiChangeColor; // data : "Black" "White"
			ewol::object::Signal<int32_t> signalSelectGotoLine;     // data : "75822"
			ewol::object::Signal<int32_t> signalSelectGotoLineSelect; // data : "75822"
	};
};

#endif

