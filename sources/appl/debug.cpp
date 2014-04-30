/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>

int32_t appl::getLogId(void) {
	static int32_t g_val = etk::log::registerInstance("edn");
	return g_val;
}

