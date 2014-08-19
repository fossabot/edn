/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/global.h>
#include <appl/globalMsg.h>
#include <ewol/object/Object.h>
#include <ewol/context/Context.h>
#include <ewol/resource/Manager.h>
#include <etk/os/FSNode.h>

#undef __class__
#define __class__ "globals"

class myParamGlobal : public ewol::Object {
	public:
		ewol::object::Param<bool> m_displayEOL;
		ewol::object::Param<bool> m_AutoIndent;
		ewol::object::Param<bool> m_displayTabChar;
		ewol::object::Param<bool> m_displaySpaceChar;
	public : 
		myParamGlobal() :
		  m_displayEOL(*this, "eol", false, "Display end of line character"),
		  m_AutoIndent(*this, "auto-indent", true, "Auto indent when create new line"),
		  m_displayTabChar(*this, "display-tab", true, "Display the Tab char"),
		  m_displaySpaceChar(*this, "display-space", true, "Display the space char") {
			m_static = true; // Note : set the object static notification( Must be set or assert at the end of process)
			setName("edn_global_param");
		}
		/*
		bool onSetConfig(const ewol::object::Config& _conf) {
			// Not set the EObject node parameter (name  == > not change ...)
			if (_conf.getConfig() == configEOL) {
				m_displayEOL = etk::string_to_bool(_conf.getData());
				return true;
			}
			if (_conf.getConfig() == configAutoIndent) {
				m_AutoIndent = etk::string_to_bool(_conf.getData());
				return true;
			}
			if (_conf.getConfig() == configShowTabChar) {
				m_displayTabChar = etk::string_to_bool(_conf.getData());
				return true;
			}
			if (_conf.getConfig() == configShowSpaceChar) {
				m_displaySpaceChar = etk::string_to_bool(_conf.getData());
				return true;
			}
			return false;
		}
		bool onGetConfig(const char* _config, std::string& _result) const {
			// Not set the EObject node parameter (name  == > not change ...)
			if (_config == configEOL) {
				if (true == m_displayEOL) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			if (_config == configAutoIndent) {
				if (true == m_AutoIndent) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			if (_config == configShowTabChar) {
				if (true == m_displayTabChar) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			if (_config == configShowSpaceChar) {
				if (true == m_displaySpaceChar) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			return false;
		}
		*/
};


static myParamGlobal& l_obj() {
	static myParamGlobal s_obj;
	return s_obj;
}

void globals::init() {
	//ewol::userConfig::addUserConfig(&l_obj());
}

void globals::UnInit() {
	// nothing to do ...
	//ewol::userConfig::RmUserConfig(&l_obj());
}

// -----------------------------------------------------------
bool globals::isSetDisplayEndOfLine() {
	return l_obj().m_displayEOL;
}

void globals::setDisplayEndOfLine(bool newVal) {
	l_obj().m_displayEOL = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
bool globals::isSetDisplaySpaceChar() {
	return l_obj().m_displaySpaceChar;
}

void globals::setDisplaySpaceChar(bool _newVal) {
	l_obj().m_displaySpaceChar = _newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}
// -----------------------------------------------------------
bool globals::isSetDisplayTabChar() {
	return l_obj().m_displayTabChar;
}

void globals::setDisplayTabChar(bool _newVal) {
	l_obj().m_displayTabChar = _newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
bool globals::isSetAutoIndent() {
	return l_obj().m_AutoIndent;
}

void globals::setAutoIndent(bool _newVal) {
	l_obj().m_AutoIndent = _newVal;
}

// -----------------------------------------------------------
bool globals::OrderTheBufferList() {
	return true;
}

// -----------------------------------------------------------
int32_t globals::getNbColoneBorder() {
	return 6;
}

int32_t globals::getNbLineBorder() {
	return 3;
}


#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Spacer.h>
static const char * const l_changeIndentation = "edn-event-change-indentation";
static const char * const l_changeSpace       = "edn-event-change-spaces";
static const char * const l_changeTabulation  = "edn-event-change-tabulation";
static const char * const l_changeEndOfLine   = "edn-event-change-endOfLine";
static const char * const l_changeRounded     = "edn-event-change-rounded";

globals::ParameterGlobalsGui::ParameterGlobalsGui() {
	addObjectType("globals::ParameterGlobalsGui");
}

void globals::ParameterGlobalsGui::init() {
	ewol::widget::Sizer::init(ewol::widget::Sizer::modeVert);
	std::shared_ptr<ewol::widget::CheckBox> myCheckbox;
	std::shared_ptr<ewol::widget::Spacer> mySpacer;
	
	mySpacer = ewol::widget::Spacer::create();
	if (nullptr == mySpacer) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		mySpacer->setExpand(bvec2(true,true));
		subWidgetAdd(mySpacer);
	}
	myCheckbox = ewol::widget::CheckBox::create("Automatic Indentation");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetAutoIndent());
		myCheckbox->registerOnEvent(shared_from_this(), "clicked", l_changeIndentation);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("Display space char (' ')");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplaySpaceChar());
		myCheckbox->registerOnEvent(shared_from_this(), "clicked", l_changeSpace);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("Display tabulation char ('\\t')");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayTabChar());
		myCheckbox->registerOnEvent(shared_from_this(), "clicked", l_changeTabulation);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("Display end of line ('\\n')");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayEndOfLine());
		myCheckbox->registerOnEvent(shared_from_this(), "clicked", l_changeEndOfLine);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("switch Rounded/default");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayEndOfLine());
		myCheckbox->registerOnEvent(shared_from_this(), "clicked", l_changeRounded);
		subWidgetAdd(myCheckbox);
	}
}

globals::ParameterGlobalsGui::~ParameterGlobalsGui() {
	
}


void globals::ParameterGlobalsGui::onReceiveMessage(const ewol::object::Message& _msg) {
	ewol::widget::Sizer::onReceiveMessage(_msg);
	
	if (_msg.getMessage() == l_changeEndOfLine) {
		if (_msg.getData() == "true") {
			setDisplayEndOfLine(true);
		} else {
			setDisplayEndOfLine(false);
		}
	} else if (_msg.getMessage() == l_changeIndentation) {
		if (_msg.getData() == "true") {
			setAutoIndent(true);
		} else {
			setAutoIndent(false);
		}
	} else if (_msg.getMessage() == l_changeSpace) {
		if (_msg.getData() == "true") {
			setDisplaySpaceChar(true);
		} else {
			setDisplaySpaceChar(false);
		}
	} else if (_msg.getMessage() == l_changeTabulation) {
		if (_msg.getData() == "true") {
			setDisplayTabChar(true);
		} else {
			setDisplayTabChar(false);
		}
	} else if (_msg.getMessage() == l_changeRounded) {
		if (_msg.getData() == "true") {
			etk::theme::setName("GUI", "rounded");;
		} else {
			etk::theme::setName("GUI", "default");;
		}
		// Reload shaders and graphic system ...
		ewol::getContext().getResourcesManager().reLoadResources();
		ewol::getContext().forceRedrawAll();
	}
	
}

