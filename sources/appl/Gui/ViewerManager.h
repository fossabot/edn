/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <appl/Buffer.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>
#include <gale/resource/Resource.h>
#include <appl/BufferManager.h>

namespace appl {
	class TextViewer;
	class ViewerManager;
	using ViewerManagerShared = ememory::SharedPtr<appl::ViewerManager>;
	using ViewerManagerWeak = ememory::WeakPtr<appl::ViewerManager>;
	class ViewerManager : public ewol::Object {
		protected:
			ViewerManager();
		public:
			DECLARE_SINGLE_FACTORY(ViewerManager, "???ViewerManager???");
			virtual ~ViewerManager();
		private:
			ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
			ememory::WeakPtr<appl::TextViewer> m_viewer;
		public:
			/**
			 * @brief Set the current buffer selected
			 * @param[in] _viewer Pointer on the viewer selected
			 */
			void setViewerSelected(const ememory::SharedPtr<appl::TextViewer>& _viewer, const ememory::SharedPtr<appl::Buffer>& _buffer);
			/**
			 * @brief Get the current buffer selected
			 * @return Pointer on the buffer selected
			 */
			ememory::SharedPtr<appl::TextViewer> getViewerSelected();
			/**
			 * @breif Check if the element is the last request selection
			 * @param[in] _viewer element selected.
			 * @return true if the element is selected
			 */
			bool isLastSelected(const ememory::SharedPtr<appl::TextViewer>& _viewer);
	};
}
