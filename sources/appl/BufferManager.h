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
			void BufferManager(void);
			void ~BufferManager(void);
		public:
			appl::Buffer* get(const etk::UString& _filename);
			appl::Buffer* get(esize_t _bufferID);
			esize_t size(void):
			
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

