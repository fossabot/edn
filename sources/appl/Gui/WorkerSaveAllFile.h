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
	class WorkerSaveAllFile : public ewol::EObject {
		public:
			WorkerSaveAllFile(void);
			virtual ~WorkerSaveAllFile(void);
		private:
			std::vector<std::string> m_bufferNameList;
			appl::WorkerSaveFile* m_worker; //! pop-up element that is open...
			appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual void onObjectRemove(ewol::EObject * _removeObject);
	};
};

#endif
