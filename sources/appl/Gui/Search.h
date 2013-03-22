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
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Entry.h>

class Search : public widget::Sizer
{
	public:
		// Constructeur
		Search(void);
		~Search(void);
		// herited function
		const char * const GetObjectType(void) { return "ApplSearch"; };
		// herited function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data);
		// herited function
		virtual void OnObjectRemove(ewol::EObject * removeObject);
	private:
		bool    m_forward;
		widget::Entry * m_searchEntry;
		widget::Entry * m_replaceEntry;
};

#endif


