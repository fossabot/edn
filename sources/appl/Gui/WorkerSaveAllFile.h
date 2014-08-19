/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __WORKER_SAVE_ALL_FILE_H__
#define __WORKER_SAVE_ALL_FILE_H__

#include <appl/BufferManager.h>
#include <appl/Gui/WorkerSaveFile.h>

namespace appl {
	class WorkerSaveAllFile : public ewol::Object {
		protected:
			WorkerSaveAllFile();
			void init();
		public:
			DECLARE_FACTORY(WorkerSaveAllFile);
			virtual ~WorkerSaveAllFile();
		private:
			std::vector<std::string> m_bufferNameList;
			std::shared_ptr<appl::WorkerSaveFile> m_worker; //! pop-up element that is open...
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::object::Message& _msg);
	};
};

#endif
