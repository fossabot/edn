/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <etk/types.h>
#include <ewol/object/Object.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	template <typename TYPE> class TextViewerPluginData : public appl::TextViewerPlugin {
		protected:
			TextViewerPluginData() {
				// nothing to do ...
				addObjectType("appl::TextViewerPluginData");
			}
		public:
			DECLARE_FACTORY(TextViewerPluginData);
			virtual ~TextViewerPluginData() {
				for (size_t iii = 0; iii < m_specificData.size() ; ++iii) {
					if (m_specificData[iii].second != nullptr) {
						remove(*m_specificData[iii].second);
					}
				}
				m_specificData.clear();
			}
		private:
			std::vector<std::pair<std::weak_ptr<appl::Buffer> ,std::unique_ptr<TYPE>>> m_specificData;
		protected:
			TYPE* getDataRef(appl::TextViewer& _textDrawer) {
				auto it = m_specificData.begin();
				while(it != m_specificData.end()) {
					std::shared_ptr<appl::Buffer> buf = it->first.lock();
					if (buf == nullptr) {
						it = m_specificData.erase(it);
						continue;
					}
					if (buf == _textDrawer.internalGetBuffer()) {
						return it->second.get();
					}
					++it;
				}
				std::unique_ptr<TYPE> data(new TYPE());
				if (data == nullptr) {
					return nullptr;
				}
				m_specificData.push_back(std::make_pair(_textDrawer.internalGetBuffer(), std::move(data)));
				// create a new one ...
				return data.get();
			}
		protected: // Wrap all element with their internal data: (do not use theses function)
			bool onReceiveShortCut(appl::TextViewer& _textDrawer,
			                       const std::string& _shortCutName) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == nullptr) {
					return false;
				}
				return onDataReceiveShortCut(_textDrawer, _shortCutName, *data);
			}
			bool onWrite(appl::TextViewer& _textDrawer,
			             const appl::Buffer::Iterator& _pos,
			             const std::string& _data) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == nullptr) {
					return false;
				}
				return onDataWrite(_textDrawer, _pos, _data, *data);
			}
			bool onReplace(appl::TextViewer& _textDrawer,
			               const appl::Buffer::Iterator& _pos,
			               const std::string& _data,
			               const appl::Buffer::Iterator& _posEnd) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == nullptr) {
					return false;
				}
				return onDataReplace(_textDrawer, _pos, _data, _posEnd, *data);
			}
			bool onRemove(appl::TextViewer& _textDrawer,
			              const appl::Buffer::Iterator& _pos,
			              const appl::Buffer::Iterator& _posEnd) {
				TYPE* data = getDataRef(_textDrawer);
				if (data == nullptr) {
					return false;
				}
				return onDataRemove(_textDrawer, _pos, _posEnd, *data);
			}
			
		public:
			virtual bool onDataReceiveShortCut(appl::TextViewer& _textDrawer,
			                                   const std::string& _shortCutName,
			                                   TYPE& _data) {
				return false;
			}
			virtual bool onDataWrite(appl::TextViewer& _textDrawer,
			                         const appl::Buffer::Iterator& _pos,
			                         const std::string& _strData,
			                         TYPE& _data) {
				return false;
			}
			virtual bool onDataReplace(appl::TextViewer& _textDrawer,
			                           const appl::Buffer::Iterator& _pos,
			                           const std::string& _strData,
			                           const appl::Buffer::Iterator& _posEnd,
			                           TYPE& _data) {
				return false;
			}
			virtual bool onDataRemove(appl::TextViewer& _textDrawer,
			                          const appl::Buffer::Iterator& _pos,
			                          const appl::Buffer::Iterator& _posEnd,
			                          TYPE& _data) {
				return false;
			}
			virtual void remove(TYPE& _data) {
				return;
			};
	};
}

