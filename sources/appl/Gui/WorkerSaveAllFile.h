/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <ewol/object/Worker.h>
#include <appl/BufferManager.h>
#include <appl/Gui/WorkerSaveFile.h>

namespace appl {
	class WorkerSaveAllFile : public ewol::object::Worker {
		protected:
			WorkerSaveAllFile();
			void init();
		public:
			DECLARE_FACTORY(WorkerSaveAllFile);
			virtual ~WorkerSaveAllFile();
		private:
			std::vector<std::string> m_bufferNameList;
			ememory::SharedPtr<appl::WorkerSaveFile> m_worker; //! pop-up element that is open...
			ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // callback function
			void onCallbackSaveAsDone();
	};
}

