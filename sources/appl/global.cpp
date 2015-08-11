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
#include <gale/resource/Manager.h>
#include <etk/os/FSNode.h>

#undef __class__
#define __class__ "globals"

class myParamGlobal : public ewol::Object {
	public:
		ewol::parameter::Value<bool> m_displayEOL;
		ewol::parameter::Value<bool> m_AutoIndent;
		ewol::parameter::Value<bool> m_displayTabChar;
		ewol::parameter::Value<bool> m_displaySpaceChar;
	public : 
		myParamGlobal() :
		  m_displayEOL(*this, "eol", false, "Display end of line character"),
		  m_AutoIndent(*this, "auto-indent", true, "Auto indent when create new line"),
		  m_displayTabChar(*this, "display-tab", true, "Display the Tab char"),
		  m_displaySpaceChar(*this, "display-space", true, "Display the space char") {
			m_static = true; // Note : set the object static notification( Must be set or assert at the end of process)
			setName("edn_global_param");
		}
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
		myCheckbox->signalValue.bind(shared_from_this(), &globals::ParameterGlobalsGui::onCallbackIndentation);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("Display space char (' ')");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplaySpaceChar());
		myCheckbox->signalValue.bind(shared_from_this(), &globals::ParameterGlobalsGui::onCallbackSpace);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("Display tabulation char ('\\t')");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayTabChar());
		myCheckbox->signalValue.bind(shared_from_this(), &globals::ParameterGlobalsGui::onCallbackTabulation);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("Display end of line ('\\n')");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayEndOfLine());
		myCheckbox->signalValue.bind(shared_from_this(), &globals::ParameterGlobalsGui::onCallbackEndOfLine);
		subWidgetAdd(myCheckbox);
	}
	myCheckbox = ewol::widget::CheckBox::create("switch Rounded/default");
	if (nullptr == myCheckbox) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		myCheckbox->setExpand(bvec2(true,false));
		myCheckbox->setValue(isSetDisplayEndOfLine());
		myCheckbox->signalValue.bind(shared_from_this(), &globals::ParameterGlobalsGui::onCallbackRounded);
		subWidgetAdd(myCheckbox);
	}
}

globals::ParameterGlobalsGui::~ParameterGlobalsGui() {
	
}


void globals::ParameterGlobalsGui::onCallbackEndOfLine(const bool& _value) {
	setDisplayEndOfLine(_value);
}

void globals::ParameterGlobalsGui::onCallbackIndentation(const bool& _value) {
	setAutoIndent(_value);
}

void globals::ParameterGlobalsGui::onCallbackSpace(const bool& _value) {
	setDisplaySpaceChar(_value);
}
void globals::ParameterGlobalsGui::onCallbackTabulation(const bool& _value) {
	setDisplayTabChar(_value);
}
void globals::ParameterGlobalsGui::onCallbackRounded(const bool& _value) {
	if (_value == true) {
		etk::theme::setName("GUI", "rounded");;
	} else {
		etk::theme::setName("GUI", "default");;
	}
	// Reload shaders and graphic system ...
	ewol::getContext().getResourcesManager().reLoadResources();
	ewol::getContext().forceRedrawAll();
}

