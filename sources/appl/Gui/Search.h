/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <appl/Debug.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/Entry.h>

class Search : public widget::SizerHori
{
	public:
		// Constructeur
		Search(void);
		~Search(void);
		/**
		 * @brief Get the current Object type of the EObject
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type description
		 * @return true if the object is compatible, otherwise false
		 */
		const char * const GetObjectType(void) { return "ApplSearch"; };
		/**
		 * @brief Receive a message from an other EObject with a specific eventId and data
		 * @param[in] CallerObject Pointer on the EObject that information came from
		 * @param[in] eventId Message registered by this class
		 * @param[in] data Data registered by this class
		 * @return ---
		 */
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		/**
		 * @brief Inform object that an other object is removed ...
		 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
		 * @note : Sub classes must call this class
		 * @return ---
		 */
		virtual void OnObjectRemove(ewol::EObject * removeObject);
	private:
		bool    m_forward;
		widget::Entry * m_searchEntry;
		widget::Entry * m_replaceEntry;
};

#endif


