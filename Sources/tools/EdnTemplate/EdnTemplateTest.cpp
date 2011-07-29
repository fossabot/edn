
#include "tools_debug.h"
#include "tools_globals.h"

#include "VectorType.h"

void TestTemplate(void)
{
	
	EDN_WARNING("Start Template Test ...");
	Edn::VectorType<int32_t> plop;
	Edn::VectorType<int8_t> plop2;
	
	plop.PushBack(15365);
	plop.PushBack(1);
	plop.PushBack(2);
	plop.PushBack(3);
	plop.PushBack(4);
	
	EDN_INFO("data is : " << plop[0]);
	EDN_INFO("data is : " << plop[1]);
	EDN_INFO("data is : " << plop[2]);
	EDN_INFO("data is : " << plop[3]);
	
	plop2.PushBack(65);
	
	EDN_INFO("data is : " << plop2[0]);
	
}
