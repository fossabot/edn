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
			std::vector<appl::Buffer*> m_list; // list of all buffer curently open
		public:
			/**
			 * @brief Get a specific buffer with his name (can create a new buffer).
			 * @param[in] _fileName Name of the file to open.
			 * @param[in] _createIfNeeded Create the buffer if not existed.
			 * @return a pointer on the buffer
			 */
			appl::Buffer* get(const std::string& _fileName, bool _createIfNeeded=false);
			/**
			 * @brief Load a specific file, event if it not existed:
			 * @param[in] _fileName Name of the file to open or create.
			 */
			void open(const std::string& _fileName);
			/**
			 * @brief Check if a buffer is already open.
			 * @param[in] _fileName name of the file.
			 * @return true if the buffer is already open.
			 */
			bool exist(const std::string& _fileName);
			/**
			 * @brief Get count of all buffer availlable.
			 * @return Number of buffer
			 */
			esize_t size(void) const {
				return m_list.size();
			}
			/**
			 * @brief Get a pointer on a buffer Id (never remember this ID!!!).
			 * @param[in] _id Number of buffer
			 * @return pointer on the buffer
			 */
			appl::Buffer* get(esize_t _id) {
				return m_list[_id];
			}
			/**
			 * @brief Create a new buffer empty.
			 * @return Created buffer or NULL.
			 */
			appl::Buffer* createNewBuffer(void);
		private:
			appl::Buffer* m_bufferSelected;
		public:
			/**
			 * @brief Set the current buffer selected
			 * @param[in] _bufferSelected Pointer on the buffer selected
			 */
			void setBufferSelected(appl::Buffer* _bufferSelected);
			/**
			 * @brief Get the current buffer selected
			 * @return Pointer on the buffer selected
			 */
			appl::Buffer* getBufferSelected(void) {
				return m_bufferSelected;
			};
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

