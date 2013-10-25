/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/global.h>
//#include <ColorizeManager.h>
#include <appl/globalMsg.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/eContext.h>
#include <ewol/resources/ResourceManager.h>
#include <etk/os/FSNode.h>
//#include <ewol/UserConfig.h>

#undef __class__
#define __class__ "globals"

class myParamGlobal : public ewol::EObject {
	public:
		static const char * const configEOL;
		static const char * const configAutoIndent;
		static const char * const configShowTabChar;
		static const char * const configShowSpaceChar;
	public:
		bool m_displayEOL;
		bool m_AutoIndent;
		bool m_displayTabChar;
		bool m_displaySpaceChar;
	public : 
		myParamGlobal(void) {
			m_static = true; // Note : set the object static notification( Must be set or assert at the end of process)
			setName("edn_global_param");
			m_displayEOL=false;
			m_AutoIndent = true;
			m_displayTabChar = true;
			m_displaySpaceChar = true;
			registerConfig(configEOL,           "bool", NULL, "Display end of line character");
			registerConfig(configAutoIndent,    "bool", NULL, "Auto indent when create new line");
			registerConfig(configShowTabChar,   "bool", NULL, "Display the Tab char");
			registerConfig(configShowSpaceChar, "bool", NULL, "Display the space char");
		}
		
		bool onSetConfig(const ewol::EConfig& _conf) {
			// Not set the EObject node parameter (name  == > not change ...)
			if (_conf.getConfig() == configEOL) {
				m_displayEOL = _conf.getData().toBool();
				return true;
			}
			if (_conf.getConfig() == configAutoIndent) {
				m_AutoIndent = _conf.getData().toBool();
				return true;
			}
			if (_conf.getConfig() == configShowTabChar) {
				m_displayTabChar = _conf.getData().toBool();
				return true;
			}
			if (_conf.getConfig() == configShowSpaceChar) {
				m_displaySpaceChar = _conf.getData().toBool();
				return true;
			}
			return false;
		}
		bool onGetConfig(const char* _config, etk::UString& _result) const {
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
};

const char * const myParamGlobal::configEOL = "eol";
const char * const myParamGlobal::configAutoIndent = "auto-indent";
const char * const myParamGlobal::configShowTabChar = "display-tab";
const char * const myParamGlobal::configShowSpaceChar = "display-space";

static myParamGlobal& l_obj(void) {
	static myParamGlobal s_obj;
	return s_obj;
}

void globals::init(void) {
	//ewol::userConfig::addUserConfig(&l_obj());
}

void globals::UnInit(void) {
	// nothing to do ...
	//ewol::userConfig::RmUserConfig(&l_obj());
}

// -----------------------------------------------------------
bool globals::isSetDisplayEndOfLine(void) {
	return l_obj().m_displayEOL;
}

void globals::setDisplayEndOfLine(bool newVal) {
	l_obj().m_displayEOL = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
bool globals::isSetDisplaySpaceChar(void) {
	return l_obj().m_displaySpaceChar;
}

void globals::setDisplaySpaceChar(bool _newVal) {
	l_obj().m_displaySpaceChar = _newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}
// -----------------------------------------------------------
bool globals::isSetDisplayTabChar(void) {
	return l_obj().m_displayTabChar;
}

void globals::setDisplayTabChar(bool _newVal) {
	l_obj().m_displayTabChar = _newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
bool globals::isSetAutoIndent(void) {
	return l_obj().m_AutoIndent;
}

void globals::setAutoIndent(bool _newVal) {
	l_obj().m_AutoIndent = _newVal;
}

// -----------------------------------------------------------
bool globals::OrderTheBufferList(void) {
	return true;
}

// -----------------------------------------------------------
int32_t globals::getNbColoneBorder(void) {
	return 6;
}

int32_t globals::getNbLineBorder(void) {
	return 3;
}


#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Spacer.h>
static const char * const l_changeIndentation = "edn-event-change-indentation";
static const char * const l_changeSpace       = "edn-event-change-spaces";
static const char * const l_changeTabulation  = "edn-event-change-tabulation";
static const char * const l_changeEndOfLine   = "edn-event-change-endOfLine";
static const char * const l_changeRounded     = "edn-event-change-rounded";

globals::ParameterGlobalsGui::ParameterGlobalsGui(void) :
  widget::Sizer(widget::Sizer::modeVert) {
	widget::CheckBox* myCheckbox = NULL;
	widget::Spacer* mySpacer = NULL;
	
	mySpacer = new widget::Spacer();
	if (NULL == mySpacer) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		mySpacer->setExpand(bvec2(true,true));
		subWidgetAdd(mySpacer);
	}
	myCheckbox = new widget::CheckBox("Automatic Indentation");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetAutoIndent());
		myCheckbox->registerOnEvent(this, ewolEventCheckBoxClicked, l_changeIndentation);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("Display space char (' ')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplaySpaceChar());
		myCheckbox->registerOnEvent(this, ewolEventCheckBoxClicked, l_changeSpace);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("Display tabulation char ('\\t')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayTabChar());
		myCheckbox->registerOnEvent(this, ewolEventCheckBoxClicked, l_changeTabulation);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("Display end of line ('\\n')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayEndOfLine());
		myCheckbox->registerOnEvent(this, ewolEventCheckBoxClicked, l_changeEndOfLine);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("switch Rounded/default");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayEndOfLine());
		myCheckbox->registerOnEvent(this, ewolEventCheckBoxClicked, l_changeRounded);
		subWidgetAdd(myCheckbox);
	}
}

globals::ParameterGlobalsGui::~ParameterGlobalsGui(void) {
	
}


void globals::ParameterGlobalsGui::onReceiveMessage(const ewol::EMessage& _msg) {
	widget::Sizer::onReceiveMessage(_msg);
	
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

