/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/TextPlugin.h>
#include <appl/Debug.h>


appl::TextViewerPlugin::TextViewerPlugin(void) :
  m_isEnable(false),
  m_activateOnEventEntry(false),
  m_activateOnEventInput(false),
  m_activateOnWrite(false),
  m_activateOnReplace(false),
  m_activateOnRemove(false),
  m_activateOnReceiveMessage(false),
  m_activateOnCursorMove(false) {
	
}

appl::TextViewerPlugin::~TextViewerPlugin(void) {
	if (m_isEnable == false) {
		return;
	}
	m_isEnable = false;
	onPluginDisable();
}

void appl::TextViewerPlugin::setEnableStatus(bool _status) {
	if (_status == m_isEnable) {
		return;
	}
	m_isEnable = _status;
	if (m_isEnable == true) {
		onPluginEnable();
	} else {
		onPluginDisable();
	}
}

