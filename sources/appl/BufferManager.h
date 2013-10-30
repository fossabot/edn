/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include <Buffer.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>
#include <ewol/resources/Resource.h>

namespace appl {
	class BufferManager : public ewol::Resource {
		protected:
			BufferManager(void);
			~BufferManager(void);
		private:
			etk::Vector<appl::Buffer*> m_list; // list of all buffer curently open
		public:
			/**
			 * @brief Get a specific buffer with his name (can create a new buffer).
			 * @param[in] _fileName Name of the file to open.
			 * @param[in] _createIfNeeded Create the buffer if not existed.
			 * @return a pointer on the buffer
			 */
			appl::Buffer* get(const etk::UString& _fileName, bool _createIfNeeded=false);
			/**
			 * @brief Load a specific file, event if it not existed:
			 * @param[in] _fileName Name of the file to open or create.
			 */
			void open(const etk::UString& _fileName);
			/**
			 * @brief Check if a buffer is already open.
			 * @param[in] _fileName name of the file.
			 * @return true if the buffer is already open.
			 */
			bool exist(const etk::UString& _fileName);
			/*
			appl::Buffer* get(esize_t _bufferID);
			esize_t size(void);
			*/
		public: // herited function
			void onReceiveMessage(const ewol::EMessage& _msg);
			void onObjectRemove(ewol::EObject * _removeObject);
		public: // resource manager
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static appl::BufferManager* keep(void);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(appl::BufferManager*& _object);
	};
};

#endif

