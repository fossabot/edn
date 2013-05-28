/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/global.h>
#include <ColorizeManager.h>
#include <appl/globalMsg.h>
#include <ewol/eObject/EObject.h>
#include <ewol/renderer/ResourceManager.h>
#include <etk/os/FSNode.h>
#include <ewol/UserConfig.h>

#undef __class__
#define __class__	"globals"

class myParamGlobal : public ewol::EObject
{
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
			m_static = true; // Note : Set the object static notification( Must be set or assert at the end of process)
			SetName("edn_global_param");
			m_displayEOL=false;
			m_AutoIndent = true;
			m_displayTabChar = true;
			m_displaySpaceChar = true;
			RegisterConfig(configEOL,           "bool", NULL, "Display end of line character");
			RegisterConfig(configAutoIndent,    "bool", NULL, "Auto indent when create new line");
			RegisterConfig(configShowTabChar,   "bool", NULL, "Display the Tab char");
			RegisterConfig(configShowSpaceChar, "bool", NULL, "Display the space char");
		}
		
		bool OnSetConfig(const ewol::EConfig& _conf)
		{
			// Not set the EObject node parameter (name ==> not change ...)
			if (_conf.GetConfig() == configEOL) {
				m_displayEOL = _conf.GetData().ToBool();
				return true;
			}
			if (_conf.GetConfig() == configAutoIndent) {
				m_AutoIndent = _conf.GetData().ToBool();
				return true;
			}
			if (_conf.GetConfig() == configShowTabChar) {
				m_displayTabChar = _conf.GetData().ToBool();
				return true;
			}
			if (_conf.GetConfig() == configShowSpaceChar) {
				m_displaySpaceChar = _conf.GetData().ToBool();
				return true;
			}
			return false;
		}
		bool OnGetConfig(const char* _config, etk::UString& _result) const
		{
			// Not set the EObject node parameter (name ==> not change ...)
			if (_config == configEOL) {
				if (true==m_displayEOL) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			if (_config == configAutoIndent) {
				if (true==m_AutoIndent) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			if (_config == configShowTabChar) {
				if (true==m_displayTabChar) {
					_result = "true";
				} else {
					_result = "false";
				}
				return true;
			}
			if (_config == configShowSpaceChar) {
				if (true==m_displaySpaceChar) {
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

static myParamGlobal& l_obj(void)
{
	static myParamGlobal s_obj;
	return s_obj;
}



void globals::Init(void)
{
	ewol::userConfig::AddUserConfig(&l_obj());
}

void globals::UnInit(void)
{
	// nothing to do ...
	//ewol::userConfig::RmUserConfig(&l_obj());
}


// -----------------------------------------------------------
bool globals::IsSetDisplayEndOfLine(void)
{
	return l_obj().m_displayEOL;
}

void globals::SetDisplayEndOfLine(bool newVal)
{
	l_obj().m_displayEOL = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
bool globals::IsSetDisplaySpaceChar(void)
{
	return l_obj().m_displaySpaceChar;
}

void globals::SetDisplaySpaceChar(bool newVal)
{
	l_obj().m_displaySpaceChar = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}
// -----------------------------------------------------------
bool globals::IsSetDisplayTabChar(void)
{
	return l_obj().m_displayTabChar;
}

void globals::SetDisplayTabChar(bool newVal)
{
	l_obj().m_displayTabChar = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
bool globals::IsSetAutoIndent(void)
{
	return l_obj().m_AutoIndent;
}

void globals::SetAutoIndent(bool newVal)
{
	l_obj().m_AutoIndent = newVal;
}

// -----------------------------------------------------------


bool globals::OrderTheBufferList(void)
{
	return true;
}

// -----------------------------------------------------------


int32_t globals::getNbColoneBorder(void)
{
	return 6;
}

int32_t globals::getNbLineBorder(void)
{
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
	widget::Sizer(widget::Sizer::modeVert)
{
	widget::CheckBox* myCheckbox = NULL;
	widget::Spacer* mySpacer = NULL;
	
	mySpacer = new widget::Spacer();
	if (NULL == mySpacer) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		mySpacer->SetExpand(bvec2(true,true));
		SubWidgetAdd(mySpacer);
	}
	myCheckbox = new widget::CheckBox("Automatic Indentation");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpand(bvec2(true,false));
		myCheckbox->SetValue(IsSetAutoIndent());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeIndentation);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("Display space char (' ')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpand(bvec2(true,false));
		myCheckbox->SetValue(IsSetDisplaySpaceChar());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeSpace);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("Display tabulation char ('\\t')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpand(bvec2(true,false));
		myCheckbox->SetValue(IsSetDisplayTabChar());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeTabulation);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("Display end of line ('\\n')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpand(bvec2(true,false));
		myCheckbox->SetValue(IsSetDisplayEndOfLine());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeEndOfLine);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new widget::CheckBox("switch Rounded/default");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpand(bvec2(true,false));
		myCheckbox->SetValue(IsSetDisplayEndOfLine());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeRounded);
		SubWidgetAdd(myCheckbox);
	}
}

globals::ParameterGlobalsGui::~ParameterGlobalsGui(void) 
{
	
}


void globals::ParameterGlobalsGui::OnReceiveMessage(const ewol::EMessage& _msg)
{
	widget::Sizer::OnReceiveMessage(_msg);
	
	if (_msg.GetMessage() == l_changeEndOfLine) {
		if (_msg.GetData() == "true") {
			SetDisplayEndOfLine(true);
		} else {
			SetDisplayEndOfLine(false);
		}
	} else if (_msg.GetMessage() == l_changeIndentation) {
		if (_msg.GetData() == "true") {
			SetAutoIndent(true);
		} else {
			SetAutoIndent(false);
		}
	} else if (_msg.GetMessage() == l_changeSpace) {
		if (_msg.GetData() == "true") {
			SetDisplaySpaceChar(true);
		} else {
			SetDisplaySpaceChar(false);
		}
	} else if (_msg.GetMessage() == l_changeTabulation) {
		if (_msg.GetData() == "true") {
			SetDisplayTabChar(true);
		} else {
			SetDisplayTabChar(false);
		}
	} else if (_msg.GetMessage() == l_changeRounded) {
		if (_msg.GetData() == "true") {
			etk::theme::SetName("GUI", "rounded");;
		} else {
			etk::theme::SetName("GUI", "default");;
		}
		// Reload shaders and graphic system ...
		ewol::resource::ReLoadResources();
	}
	
}

