/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __VIEWER_MANAGER_H__
#define __VIEWER_MANAGER_H__

#include <appl/Buffer.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>
#include <ewol/resource/Resource.h>
#include <appl/BufferManager.h>

namespace appl {
	class TextViewer;
	class ViewerManager : public ewol::Resource {
		protected:
			ViewerManager();
			void init(const std::string& _uniqueName);
		public:
			DECLARE_RESOURCE_SINGLE_FACTORY(ViewerManager, "???ViewerManager???");
			virtual ~ViewerManager();
		private:
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
			std::shared_ptr<appl::TextViewer> m_viewer;
		public:
			/**
			 * @brief Set the current buffer selected
			 * @param[in] _viewer Pointer on the viewer selected
			 */
			void setViewerSelected(const std::shared_ptr<appl::TextViewer>& _viewer, const std::shared_ptr<appl::Buffer>& _buffer);
			/**
			 * @brief Get the current buffer selected
			 * @return Pointer on the buffer selected
			 */
			std::shared_ptr<appl::TextViewer> getViewerSelected() {
				return m_viewer;
			};
			/**
			 * @breif Check if the element is the last request selection
			 * @param[in] _viewer element selected.
			 * @return true if the element is selected
			 */
			bool isLastSelected(const std::shared_ptr<appl::TextViewer>& _viewer);
		public: // herited function
			void onReceiveMessage(const ewol::object::Message& _msg);
			void onObjectRemove(const std::shared_ptr<ewol::Object>& _removeObject);
	};
};

#endif

