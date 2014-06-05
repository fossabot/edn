/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Gui/TagFileSelection.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/Manager.h>
#include <vector>
#include <etk/tool.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Composer.h>

#include <ewol/ewol.h>



#undef __class__
#define __class__ "TagFileSelection"


extern const char * const applEventctagsSelection = "appl-event-ctags-validate";
extern const char * const applEventctagsCancel    = "appl-event-ctags-cancel";


appl::TagFileSelection::TagFileSelection() {
	addObjectType("appl::TagFileSelection");
	addEventId(applEventctagsSelection);
	addEventId(applEventctagsCancel);
	
	ewol::object::Shared<ewol::widget::Label> myWidgetTitle;
	
	ewol::object::Shared<ewol::widget::Sizer> mySizerVert;
	#if defined(__TARGET_OS__Android)
		setMinSize(ewol::Dimension(vec2(90,90),ewol::Dimension::Pourcent));
	#elif defined(__TARGET_OS__Windows)
		setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#else
		setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#endif
	
	mySizerVert = ewol::object::makeShared(new ewol::widget::Sizer(ewol::widget::Sizer::modeVert));
	if (nullptr == mySizerVert) {
		EWOL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		mySizerVert->lockExpand(bvec2(true,true));
		mySizerVert->setExpand(bvec2(true,true));
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		ewol::object::Shared<ewol::widget::Composer> compose = ewol::object::makeShared(new ewol::widget::Composer(ewol::widget::Composer::String,
		   "<sizer mode=\"hori\" expand=\"true,false\" lock=\"false,true\">\n"
		   "	<spacer expand=\"true,false\"/>\n"
		   "	<button name=\"PLUGIN-CTAGS-jump\" expand=\"false\" fill=\"true\">"
		   "		<sizer mode=\"hori\">\n"
		   "			<image src=\"THEME:GUI:Load.svg\" fill=\"true\" size=\"10,10mm\"/>\n"
		   "			<label>Jump</label>\n"
		   "		</sizer>\n"
		   "	</button>\n"
		   "	<button name=\"PLUGIN-CTAGS-cancel\" expand=\"false\" fill=\"true\">"
		   "		<sizer mode=\"hori\">\n"
		   "			<image src=\"THEME:GUI:Remove.svg\" fill=\"true\" size=\"10,10mm\"/>\n"
		   "			<label>Cancel</label>\n"
		   "		</sizer>\n"
		   "	</button>\n"
		   "</sizer>\n"));
		compose->setExpand(bvec2(true,false));
		compose->setFill(bvec2(true,true));
		mySizerVert->subWidgetAdd(compose);
		compose->registerOnEventNameWidget(this, "PLUGIN-CTAGS-jump", "pressed", applEventctagsSelection);
		compose->registerOnEventNameWidget(this, "PLUGIN-CTAGS-cancel", "pressed", applEventctagsCancel);
			
		m_listTag = ewol::object::makeShared(new appl::TagFileList());
		if (nullptr == m_listTag) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			m_listTag->registerOnEvent(this, applEventCtagsListValidate);
			m_listTag->registerOnEvent(this, applEventCtagsListSelect);
			m_listTag->registerOnEvent(this, applEventCtagsListUnSelect);
			m_listTag->setExpand(bvec2(true,true));
			m_listTag->setFill(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_listTag);
		}
		
		myWidgetTitle = new ewol::widget::Label("Ctags Jump Selection ...");
		if (nullptr == myWidgetTitle) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(myWidgetTitle);
		}
	}
}


appl::TagFileSelection::~TagFileSelection() {
	
}

void appl::TagFileSelection::onReceiveMessage(const ewol::object::Message& _msg) {
	EWOL_INFO("ctags LIST ... : " << _msg );
	if (_msg.getMessage() == applEventctagsSelection) {
		if (m_eventNamed!="") {
			generateEventId(applEventctagsSelection, m_eventNamed);
			// == > Auto remove ...
			autoDestroy();
		}
	} else if (_msg.getMessage() == applEventCtagsListSelect) {
		m_eventNamed = _msg.getData();
		
	} else if (_msg.getMessage() == applEventCtagsListUnSelect) {
		m_eventNamed = "";
	} else if (_msg.getMessage() == applEventCtagsListValidate) {
		generateEventId(applEventctagsSelection, _msg.getData());
		// == > Auto remove ...
		autoDestroy();
	} else if (_msg.getMessage() == applEventctagsCancel) {
		generateEventId(applEventctagsCancel, "");
		// == > Auto remove ...
		autoDestroy();
	}
	return;
};


/**
 * @brief add a Ctags item on the curent list
 * @param[in] file Compleate file name
 * @param[in] jump line id
 */
void appl::TagFileSelection::addCtagsNewItem(std::string _file, int32_t _line) {
	if (nullptr != m_listTag) {
		m_listTag->add(_file, _line);
	}
}

void appl::TagFileSelection::onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject) {
	// First step call parrent : 
	ewol::widget::PopUp::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	if(_removeObject == m_listTag) {
		m_listTag = nullptr;
	}
}





