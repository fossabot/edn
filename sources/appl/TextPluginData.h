/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_DATA_H__
#define __APPL_TEXT_PLUGIN_DATA_H__

#include <etk/types.h>
#include <ewol/renderer/EObject.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	template <typename TYPE> class TextViewerPluginData : public appl::TextViewerPlugin {
		public:
			TextViewerPluginData(void) {
				// nothing to do ...
			}
			virtual ~TextViewerPluginData(void) {
				for (size_t iii = 0; iii < m_specificData.size() ; ++iii) {
					if (m_specificData[iii].second != NULL) {
						remove(*m_specificData[iii].second);
						delete(m_specificData[iii].second);
						m_specificData[iii].second = NULL;
					}
				}
				m_specificData.clear();
			}
		private:
			std::vector<std::pair<appl::Buffer* ,TYPE* >> m_specificData;
		protected:
			TYPE* getDataRef(appl::TextViewer& _textDrawer) {
				for (size_t iii = 0; iii < m_specificData.size() ; ++iii) {
					APPL_DEBUG("compare " << (int64_t)m_specificData[iii].first << " && " << (int64_t)&_textDrawer);
					if (m_specificData[iii].first == _textDrawer.m_buffer) {
						APPL_DEBUG("find data : " << iii);
						return m_specificData[iii].second;
					}
				}
				TYPE* data = new TYPE();
				if (data == NULL) {
					return NULL;
				}
				m_specificData.push_back(std::make_pair(_textDrawer.m_buffer, data));
				// create a new one ...
				return data;
			}
		protected: // Wrap all element with their internal data: (do not use theses function)
			void onPluginEnable(appl::TextViewer& _textDrawer) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == NULL) {
					return;
				}
				return onPluginEnable(_textDrawer, *data);
			}
			void onPluginDisable(appl::TextViewer& _textDrawer) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == NULL) {
					return;
				}
				return onPluginDisable(_textDrawer, *data);
			}
			bool onReceiveMessage(appl::TextViewer& _textDrawer,
			                      const ewol::EMessage& _msg) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == NULL) {
					return false;
				}
				return onReceiveMessage(_textDrawer, _msg, *data);
			}
			bool onWrite(appl::TextViewer& _textDrawer,
			             const appl::Buffer::Iterator& _pos,
			             const std::string& _data) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == NULL) {
					return false;
				}
				return onWrite(_textDrawer, _pos, _data, *data);
			}
			bool onReplace(appl::TextViewer& _textDrawer,
			               const appl::Buffer::Iterator& _pos,
			               const std::string& _data,
			               const appl::Buffer::Iterator& _posEnd) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == NULL) {
					return false;
				}
				return onReplace(_textDrawer, _pos, _data, _posEnd, *data);
			}
			bool onRemove(appl::TextViewer& _textDrawer,
			              const appl::Buffer::Iterator& _pos,
			              const appl::Buffer::Iterator& _posEnd) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == NULL) {
					return false;
				}
				return onRemove(_textDrawer, _pos, _posEnd, *data);
			}
			
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer,
			                            TYPE& _data) {
				return;
			}
			virtual void onPluginDisable(appl::TextViewer& _textDrawer,
			                             TYPE& _data) {
				return;
			}
			virtual bool onReceiveMessage(appl::TextViewer& _textDrawer,
			                              const ewol::EMessage& _msg,
			                              TYPE& _data) {
				return false;
			}
			virtual bool onWrite(appl::TextViewer& _textDrawer,
			                     const appl::Buffer::Iterator& _pos,
			                     const std::string& _strData,
			                     TYPE& _data) {
				return false;
			}
			virtual bool onReplace(appl::TextViewer& _textDrawer,
			                       const appl::Buffer::Iterator& _pos,
			                       const std::string& _strData,
			                       const appl::Buffer::Iterator& _posEnd,
			                       TYPE& _data) {
				return false;
			}
			virtual bool onRemove(appl::TextViewer& _textDrawer,
			                      const appl::Buffer::Iterator& _pos,
			                      const appl::Buffer::Iterator& _posEnd,
			                      TYPE& _data) {
				return false;
			}
			virtual void remove(TYPE& _data) {
				return;
			};
		public:
			virtual void onObjectRemove(ewol::EObject* _removeObject) {
				// TODO : plop
			};
	};
};


#endif