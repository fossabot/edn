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
#include <ewol/widget/WidgetManager.h>
#include <etk/Vector.h>
#include <etk/tool.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Composer.h>

#include <ewol/ewol.h>



#undef __class__
#define __class__ "TagFileSelection"


extern const char * const applEventctagsSelection = "appl-event-ctags-validate";
extern const char * const applEventctagsCancel    = "appl-event-ctags-cancel";


appl::TagFileSelection::TagFileSelection(void)
{
	addEventId(applEventctagsSelection);
	addEventId(applEventctagsCancel);
	
	widget::Label*      myWidgetTitle = NULL;
	widget::Button*     myWidgetValidate = NULL;
	widget::Button*     myWidgetCancel = NULL;
	
	widget::Sizer * mySizerVert = NULL;
	widget::Sizer * mySizerHori = NULL;
	widget::Spacer *    mySpacer = NULL;
	#if defined(__TARGET_OS__Android)
		setMinSize(ewol::Dimension(vec2(90,90),ewol::Dimension::Pourcent));
	#elif defined(__TARGET_OS__Windows)
		setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#else
		setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#endif
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		mySizerVert->lockExpand(bvec2(true,true));
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setExpand(bvec2(true,false));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			myWidgetValidate = new widget::Button();
			if (NULL == myWidgetValidate) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				myWidgetValidate->setSubWidget(
				    new widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:icon/Load.svg\" fill=\"true\" size=\"70,70mm\"/>\n"
				        "		<label>Jump</label>\n"
				        "	</sizer>\n"
				        "</composer\n"));
				
				myWidgetValidate->registerOnEvent(this, widget::Button::eventPressed, applEventctagsSelection);
				mySizerHori->subWidgetAdd(myWidgetValidate);
			}
			myWidgetCancel = new widget::Button();
			if (NULL == myWidgetCancel) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				myWidgetCancel->setSubWidget(
				    new widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:icon/Remove.svg\" fill=\"true\" size=\"70,70mm\"/>\n"
				        "		<label>Cancel</label>\n"
				        "	</sizer>\n"
				        "</composer\n"));
				myWidgetCancel->registerOnEvent(this, widget::Button::eventPressed, applEventctagsCancel);
				mySizerHori->subWidgetAdd(myWidgetCancel);
			}
		}
		m_listTag = new appl::TagFileList();
		if (NULL == m_listTag) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			m_listTag->registerOnEvent(this, applEventCtagsListValidate);
			m_listTag->registerOnEvent(this, applEventCtagsListSelect);
			m_listTag->registerOnEvent(this, applEventCtagsListUnSelect);
			m_listTag->setExpand(bvec2(true,true));
			m_listTag->setFill(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_listTag);
		}
		
		myWidgetTitle = new widget::Label("Ctags Jump Selection ...");
		if (NULL == myWidgetTitle) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(myWidgetTitle);
		}
	}
}


appl::TagFileSelection::~TagFileSelection(void) {
	
}

void appl::TagFileSelection::onReceiveMessage(const ewol::EMessage& _msg) {
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
void appl::TagFileSelection::addCtagsNewItem(etk::UString _file, int32_t _line) {
	if (NULL != m_listTag) {
		m_listTag->add(_file, _line);
	}
}

void appl::TagFileSelection::onObjectRemove(ewol::EObject * _removeObject) {
	// First step call parrent : 
	widget::PopUp::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	if(_removeObject == m_listTag) {
		m_listTag = NULL;
	}
}





