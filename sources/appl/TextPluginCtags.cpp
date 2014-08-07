/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginCtags.h>
#include <ewol/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/context/Context.h>
#include <appl/Gui/TagFileSelection.h>

#undef __class__
#define __class__ "TextPluginCtags"


appl::TextPluginCtags::TextPluginCtags() :
  m_tagFolderBase(""),
  m_tagFilename(""),
  m_ctagFile(nullptr) {
	m_activateOnReceiveMessage = true;
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
	addObjectType("appl::TextPluginCtags");
}

void appl::TextPluginCtags::init() {
	appl::TextViewerPlugin::init();
}


appl::TextPluginCtags::~TextPluginCtags() {
	
}

const char* eventJumpDestination = "event-plugin-ctags-jump-destination";
const char* eventJumpBack = "event-plugin-ctags-jump-back";
const char* eventOpenCtagsFile = "event-plugin-ctags-open";
const char* eventOpenCtagsOpenFileReturn = "event-plugin-ctags-open-file-return";
const char* eventOpenCtagsSelectReturn = "event-plugin-ctags-select-file-return";

void appl::TextPluginCtags::onPluginEnable(appl::TextViewer& _textDrawer) {
	// Add local event of this object (no dependency with the viewer ...
	/*
	registerMultiCast(ednMsgGuiCtags);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(ednMsgCtagsLoadFile);
	*/
	// add event :
	_textDrawer.ext_registerMultiCast(eventJumpDestination);
	_textDrawer.ext_registerMultiCast(eventJumpBack);
	_textDrawer.ext_registerMultiCast(eventOpenCtagsFile);
	_textDrawer.ext_shortCutAdd("ctrl+d", eventJumpDestination);
	_textDrawer.ext_shortCutAdd("ctrl+shift+d", eventJumpBack);
	_textDrawer.ext_shortCutAdd("ctrl+alt+d", eventOpenCtagsFile);
}

void appl::TextPluginCtags::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

void appl::TextPluginCtags::jumpTo(const std::string& _name) {
	if (m_ctagFile == nullptr) {
		APPL_WARNING("No ctags file open");
		return;
	}
	tagEntry entry;
	APPL_INFO("try to find the tag : " << _name);
	if (tagsFind (m_ctagFile, &entry, _name.c_str(), 0) != TagSuccess) {
		APPL_INFO("no tag find ...");
		return;
	}
	tagEntry entrySave = entry;
	int32_t numberOfTags = 0;
	
	// For all tags : Save in an internal Structure :
	std::string tmpFile(m_tagFolderBase + "/" + entry.file);
	etk::FSNode myfile(tmpFile);
	int32_t lineID = entry.address.lineNumber;
	printTag(&entry);
	
	if (tagsFindNext (m_ctagFile, &entry) == TagSuccess) {
		APPL_INFO("Multiple file destination ...");
		std::shared_ptr<appl::TagFileSelection> tmpWidget = appl::TagFileSelection::create();
		if (nullptr == tmpWidget) {
			APPL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			tmpWidget->addCtagsNewItem(myfile.getFileSystemName(), lineID);
			do {
				tmpFile = m_tagFolderBase + "/" + entry.file;
				myfile = tmpFile;
				lineID = entry.address.lineNumber;
				printTag(&entry);
				tmpWidget->addCtagsNewItem(myfile.getFileSystemName(), lineID);
			} while (tagsFindNext (m_ctagFile, &entry) == TagSuccess);
			ewol::getContext().getWindows()->popUpWidgetPush(tmpWidget);
			tmpWidget->registerOnEvent(shared_from_this(), applEventctagsSelection, eventOpenCtagsSelectReturn);
		}
	} else {
		jumpFile(myfile.getName(), lineID - 1);
	}
}

void appl::TextPluginCtags::jumpFile(const std::string& _filename, int64_t _lineId) {
	// save the current file in the history
	// TODO : registerHistory();
	if (m_bufferManager != nullptr) {
		m_bufferManager->open(_filename);
	}
	sendMultiCast(appl::MsgSelectChange, _filename);
	sendMultiCast(appl::MsgSelectGotoLineSelect, std::to_string(_lineId));
}

void appl::TextPluginCtags::loadTagFile() {
	tagFileInfo info;
	// close previous tag file
	if (nullptr != m_ctagFile) {
		tagsClose(m_ctagFile);
		m_ctagFile = nullptr;
	}
	if (m_tagFilename == "") {
		return;
	}
	// load (open) the tag file : 
	APPL_INFO("try to open tag file : " << m_tagFilename);
	m_ctagFile = tagsOpen(m_tagFilename.c_str(), &info);
	if (nullptr != m_ctagFile) {
		APPL_INFO("open exuberant Ctags file is OK ...");
	} else {
		APPL_INFO("Error to open ctags file ...");
	}
}

void appl::TextPluginCtags::printTag(const tagEntry *_entry) {
	#if 1
		APPL_INFO("find Tag file : name=\"" << _entry->name << "\" in file=\"" << _entry->file
			<< "\" at line="<< (int32_t)_entry->address.lineNumber);
	#else
		APPL_INFO("find Tag file : name=\"" << _entry->name << "\" in file=\"" << _entry->file
			<< "\" pattern=\"" << _entry->address.pattern 
			<< "\" at line="<< (int32_t)_entry->address.lineNumber);
		
		APPL_INFO("Extention field : ");
		if (_entry->kind != nullptr  && _entry->kind [0] != '\0') {
			APPL_INFO("        kind : " << _entry->kind);
		}
		if (_entry->fileScope) {
			APPL_INFO("        file : ");
		}
		for (int32_t iii = 0 ; iii < _entry->fields.count ; ++iii) {
			APPL_INFO("               " << _entry->fields.list[iii].key << ":" << _entry->fields.list[iii].value );
		}
	#endif
}

void appl::TextPluginCtags::onReceiveMessage(const ewol::object::Message& _msg) {
	if (_msg.getMessage() == eventOpenCtagsOpenFileReturn) {
		// open the new one :
		etk::FSNode tmpFilename = _msg.getData();
		m_tagFilename = tmpFilename.getNameFile();
		m_tagFolderBase = tmpFilename.getNameFolder();
		APPL_INFO("Receive load Ctags file : " << m_tagFolderBase << "/" << m_tagFilename << " ");
		loadTagFile();
	} else if (_msg.getMessage() == eventOpenCtagsSelectReturn) {
		// parse the input data
		char tmp[4096];
		int32_t lineID;
		// TODO : Review this ...
		sscanf(_msg.getData().c_str(), "%d:%s", &lineID, tmp);
		jumpFile(tmp, lineID - 1);
	}
}
bool appl::TextPluginCtags::onReceiveMessageViewer(appl::TextViewer& _textDrawer,
                                                   const ewol::object::Message& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (_msg.getMessage() == eventOpenCtagsFile) {
		APPL_INFO("Request opening ctag file");
		std::shared_ptr<ewol::widget::FileChooser> tmpWidget = ewol::widget::FileChooser::create();
		if (nullptr == tmpWidget) {
			APPL_ERROR("Can not allocate widget  == > display might be in error");
			return true;
		}
		tmpWidget->setTitle("Open Exuberant Ctags file");
		tmpWidget->setValidateLabel("Open");
		// try to get the current folder :
		std::string path = _textDrawer.getBufferPath();
		APPL_ERROR("get path : '" << path << "'");
		if (path != "") {
			tmpWidget->setFolder(path);
		}
		ewol::getContext().getWindows()->popUpWidgetPush(tmpWidget);
		tmpWidget->registerOnEvent(shared_from_this(), "validate", eventOpenCtagsOpenFileReturn);
		return true;
	} else if (_msg.getMessage() == eventJumpDestination) {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		std::string textToSearch;
		if (_textDrawer.hasTextSelected() == true) {
			_textDrawer.copy(textToSearch, _textDrawer.selectStart(), _textDrawer.selectStop() );
		} else {
			appl::Buffer::Iterator _beginPos;
			appl::Buffer::Iterator _endPos;
			if (_textDrawer.getPosAround(_textDrawer.cursor(), _beginPos, _endPos) == false) {
				APPL_WARNING("Can not get data around...");
				return true;
			}
			_textDrawer.copy(textToSearch, _beginPos, _endPos);
		}
		jumpTo(textToSearch);
		return true;
	} else if (_msg.getMessage() == eventJumpBack) {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		return true;
	}
	return false;
}
