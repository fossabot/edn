/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/TextPlugin.hpp>
#include <appl/TextPluginManager.hpp>
#include <appl/debug.hpp>

appl::TextViewerPlugin::TextViewerPlugin() :
  m_isEnable(true),
  m_activateOnEventEntry(false),
  m_activateOnEventInput(false),
  m_activateOnWrite(false),
  m_activateOnReplace(false),
  m_activateOnRemove(false),
  m_activateOnReceiveShortCut(false),
  m_activateOnCursorMove(false) {
	addObjectType("appl::TextViewerPlugin");
	m_menuInterface = ememory::dynamicPointerCast<ewol::widget::Menu>(getObjectNamed("appl-menu-interface"));
	if (m_menuInterface.expired() == true) {
		APPL_ERROR("Can not acces to the Menu interface");
	}
	// get a reference on the plugin manager...
	m_pluginManager = appl::textPluginManager::create();
}

appl::TextViewerPlugin::~TextViewerPlugin() {
	if (m_isEnable == false) {
		return;
	}
	m_isEnable = false;
	onPluginGlobalDisable();
}

void appl::TextViewerPlugin::setEnableStatus(bool _status) {
	if (_status == m_isEnable) {
		return;
	}
	m_isEnable = _status;
	if (m_isEnable == true) {
		onPluginGlobalEnable();
	} else {
		onPluginGlobalDisable();
	}
}

