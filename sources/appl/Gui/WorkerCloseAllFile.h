/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <ewol/object/Worker.h>
#include <appl/BufferManager.h>
#include <appl/Gui/WorkerCloseFile.h>

namespace appl {
	class WorkerCloseAllFile : public ewol::object::Worker {
		protected:
			WorkerCloseAllFile();
			void init();
		public:
			DECLARE_FACTORY(WorkerCloseAllFile);
			virtual ~WorkerCloseAllFile();
		private:
			std::vector<std::string> m_bufferNameList;
			std::shared_ptr<appl::WorkerCloseFile> m_worker; //! pop-up element that is open...
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // callback function
			void onCallbackCloseDone();
	};
}
