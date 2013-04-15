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
#define __class__	"TagFileSelection"


extern const char * const applEventctagsSelection           = "appl-event-ctags-validate";
extern const char * const applEventctagsCancel              = "appl-event-ctags-cancel";


appl::TagFileSelection::TagFileSelection(void)
{
	AddEventId(applEventctagsSelection);
	AddEventId(applEventctagsCancel);
	
	widget::Label*      myWidgetTitle = NULL;
	widget::Button*     myWidgetValidate = NULL;
	widget::Button*     myWidgetCancel = NULL;
	
	widget::Sizer * mySizerVert = NULL;
	widget::Sizer * mySizerHori = NULL;
	widget::Spacer *    mySpacer = NULL;
	#if defined(__TARGET_OS__Android)
		SetMinSize(ewol::Dimension(vec2(90,90),ewol::Dimension::Pourcent));
	#elif defined(__TARGET_OS__Windows)
		SetMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#else
		SetMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#endif
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		mySizerVert->LockExpand(bvec2(true,true));
		// set it in the pop-up-system : 
		SetSubWidget(mySizerVert);
		
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpand(bvec2(true,false));
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			myWidgetValidate = new widget::Button();
			if (NULL == myWidgetValidate) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myWidgetValidate->SetSubWidget(
				    new widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:icon/Load.svg\" fill=\"true\" size=\"70,70mm\"/>\n"
				        "		<label>Jump</label>\n"
				        "	</sizer>\n"
				        "</composer\n"));
				
				myWidgetValidate->RegisterOnEvent(this, ewolEventButtonPressed, applEventctagsSelection);
				mySizerHori->SubWidgetAdd(myWidgetValidate);
			}
			myWidgetCancel = new widget::Button();
			if (NULL == myWidgetCancel) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myWidgetCancel->SetSubWidget(
				    new widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:icon/Remove.svg\" fill=\"true\" size=\"70,70mm\"/>\n"
				        "		<label>Cancel</label>\n"
				        "	</sizer>\n"
				        "</composer\n"));
				myWidgetCancel->RegisterOnEvent(this, ewolEventButtonPressed, applEventctagsCancel);
				mySizerHori->SubWidgetAdd(myWidgetCancel);
			}
		}
		m_listTag = new appl::TagFileList();
		if (NULL == m_listTag) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			m_listTag->RegisterOnEvent(this, applEventCtagsListValidate);
			m_listTag->RegisterOnEvent(this, applEventCtagsListSelect);
			m_listTag->RegisterOnEvent(this, applEventCtagsListUnSelect);
			m_listTag->SetExpand(bvec2(true,true));
			m_listTag->SetFill(bvec2(true,true));
			mySizerVert->SubWidgetAdd(m_listTag);
		}
		
		myWidgetTitle = new widget::Label("Ctags Jump Selection ...");
		if (NULL == myWidgetTitle) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(myWidgetTitle);
		}
	}
}


appl::TagFileSelection::~TagFileSelection(void)
{
	
}

void appl::TagFileSelection::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data)
{
	EWOL_INFO("ctags LIST ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventId == applEventctagsSelection) {
		if (m_eventNamed!="") {
			GenerateEventId(applEventctagsSelection, m_eventNamed);
			//==> Auto remove ...
			AutoDestroy();
		}
	} else if (eventId == applEventCtagsListSelect) {
		m_eventNamed = data;
		
	} else if (eventId == applEventCtagsListUnSelect) {
		m_eventNamed = "";
	} else if (eventId == applEventCtagsListValidate) {
		GenerateEventId(applEventctagsSelection, data);
		//==> Auto remove ...
		AutoDestroy();
	} else if (eventId == applEventctagsCancel) {
		GenerateEventId(applEventctagsCancel, "");
		//==> Auto remove ...
		AutoDestroy();
	}
	return;
};


/**
 * @brief Add a Ctags item on the curent list
 * @param[in] file Compleate file name
 * @param[in] jump line id
 */
void appl::TagFileSelection::AddCtagsNewItem(etk::UString file, int32_t line)
{
	if (NULL != m_listTag) {
		m_listTag->Add(file, line);
	}
}

void appl::TagFileSelection::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	widget::PopUp::OnObjectRemove(m_listTag);
	// second step find if in all the elements ...
	if(removeObject == m_listTag) {
		m_listTag = NULL;
	}
}





