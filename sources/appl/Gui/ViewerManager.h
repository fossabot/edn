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
		public:
			virtual ~ViewerManager();
		private:
			ewol::object::Shared<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
			ewol::object::Shared<appl::TextViewer> m_viewer;
		public:
			/**
			 * @brief Set the current buffer selected
			 * @param[in] _viewer Pointer on the viewer selected
			 */
			void setViewerSelected(const ewol::object::Shared<appl::TextViewer>& _viewer, const ewol::object::Shared<appl::Buffer>& _buffer);
			/**
			 * @brief Get the current buffer selected
			 * @return Pointer on the buffer selected
			 */
			ewol::object::Shared<appl::TextViewer> getViewerSelected() {
				return m_viewer;
			};
			/**
			 * @breif Check if the element is the last request selection
			 * @param[in] _viewer element selected.
			 * @return true if the element is selected
			 */
			bool isLastSelected(const ewol::object::Shared<appl::TextViewer>& _viewer);
		public: // herited function
			void onReceiveMessage(const ewol::object::Message& _msg);
			void onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject);
		public: // resource manager
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::object::Shared<appl::ViewerManager> keep();
	};
};

#endif

