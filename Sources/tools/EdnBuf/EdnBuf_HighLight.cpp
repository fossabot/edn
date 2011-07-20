/**
 *******************************************************************************
 * @file EdnBuf_HighLight.cpp
 * @brief Editeur De N'ours : Buffer for internal Data - section highlight (Sources)
 * @author Edouard DUPIN
 * @date 23/03/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include "tools_debug.h"
#include "tools_globals.h"
#include "EdnBuf.h"


#undef __class__
#define __class__	"EdnBuf{HighLight}"

void EdnBuf::SetHLSystem(Highlight * newHLSystem)
{
	if (m_Highlight != newHLSystem) {
		m_Highlight = newHLSystem;
		m_HLDataPass1.clear();
		RegenerateHighLightAt(0, 0, m_data.Size());
	}
}


void EdnBuf::RegenerateHighLightAt(int32_t pos, int32_t nbDeleted, int32_t nbAdded)
{
	GTimeVal timeStart;
	g_get_current_time(&timeStart);
	
	// remove display HL...
	m_HLDataSequence++;
	
	// prevent ERROR...
	if (NULL == m_Highlight) {
		return;
	}
	// prevent No data Call
	if(		0 == nbDeleted
		&&	0 == nbAdded)
	{
		return;
	}
	// normal case
	EDN_INFO("(pos="<<pos<<", nbDeleted="<<nbDeleted<<", nbAdded=" << nbAdded << "\");");
	int32_t i;
	int32_t posEnd = pos + nbDeleted;
	// search position of the old element to reparse IT...
	int32_t startId;
	int32_t stopId;
	// clean data if needed
	if (0 != m_HLDataPass1.size()) {
		// find element previous
		FindMainHighLightPosition(pos, posEnd, startId, stopId, true);

		// Remove deprecated element
		if(		-1 == startId
			&&	-1 == stopId)
		{
			m_HLDataPass1.clear();
		} else if(-1 == startId) {
			if (0 == stopId){
				m_HLDataPass1.erase(m_HLDataPass1.begin());
			} else {
				m_HLDataPass1.erase(m_HLDataPass1.begin(),m_HLDataPass1.begin()+stopId);
			}
		} else if(-1 == stopId) {
			m_HLDataPass1.erase(m_HLDataPass1.begin()+startId+1,m_HLDataPass1.end());
		} else {
			m_HLDataPass1.erase(m_HLDataPass1.begin()+startId+1,m_HLDataPass1.begin()+stopId);
		}
		EDN_DEBUG("new size=" << (int32_t)m_HLDataPass1.size()-1);
		// update position after the range position : 
		int32_t elemStart;
		if(-1 == startId) {
			elemStart = 0;
		} else {
			elemStart = startId+1;
		}
		for (i=elemStart; i< (int32_t)m_HLDataPass1.size(); i++) {
			//EDN_DEBUG("move element=" << i);
			m_HLDataPass1[i].beginStart += nbAdded - nbDeleted;
			m_HLDataPass1[i].beginStop  += nbAdded - nbDeleted;
			m_HLDataPass1[i].endStart   += nbAdded - nbDeleted;
			m_HLDataPass1[i].endStop    += nbAdded - nbDeleted;
		}
		//Regenerate Element inside range
		if(		-1 == startId
			&&	-1 == stopId)
		{
			//EDN_DEBUG("*******  Regenerate ALL");
			GenerateHighLightAt(0, m_data.Size());
		} else if(-1 == startId) {
			//EDN_DEBUG("*******  Regenerate START");
			GenerateHighLightAt(0, m_HLDataPass1[0].beginStart, 0);
		} else if(-1 == stopId) {
			//EDN_DEBUG("*******  Regenerate STOP");
			GenerateHighLightAt(m_HLDataPass1[m_HLDataPass1.size() -1].endStop, m_data.Size(), m_HLDataPass1.size());
		} else {
			//EDN_DEBUG("*******  Regenerate RANGE");
			GenerateHighLightAt(m_HLDataPass1[startId].endStop, m_HLDataPass1[startId+1].beginStart, startId+1);
		}
	} else {
		// Parse the new element ...
		//GenerateHighLightAt(pos, nbAdded);
		GenerateHighLightAt(0, m_data.Size());
	}
	for (i=0; i< (int32_t)m_HLDataPass1.size(); i++) {
		Edn::String ploppp;
		if (NULL != m_HLDataPass1[i].patern ) {
			ploppp = ((HighlightPattern*)m_HLDataPass1[i].patern)->GetName();
		}
		//EDN_DEBUG("HighLight element id=" << i << " S=" << m_HLDataPass1[i].beginStart << " E=" << m_HLDataPass1[i].endStop << " patern name=" << ploppp.c_str() );
	}
	GTimeVal timeStop;
	g_get_current_time(&timeStop);
	EDN_DEBUG("HL General = " << timeStop.tv_usec - timeStart.tv_usec << " micro-s");
}

void EdnBuf::FindMainHighLightPosition(int32_t startPos, int32_t endPos, int32_t &startId, int32_t &stopId, bool backPreviousNotEnded)
{
	startId = -1;
	stopId = -1;
	/* rules to start stop:
		HighLight data ----
		Remove area    ****
		Start pos      S
		End pos        E

	Some Case :
		-----------          ------------          -------------            ----------
			      S              ****              E                                  

		-----------          ------------          -------------            ----------
			      S                **********      E                                  

		-----------          ------------          -------------            ----------
			                            S   ****   E                                  

		-----------          ------------          -------------            ----------
			      S     *********                  E                                  

		-----------          ------------          -------------            ----------
			      S     *********************      E                                  

		-----------          ------------          -------------            ----------
			      S               ************************                  E         

		-----------          ------------          -------------            ----------
			      S     *****************          E                                  

		-----------          ------------          -------------            ----------
			      S          ***************       E                                  

		-----------          ------------       
			      S          ***************       E=-1

			                 ------------          -------------            ----------
			      S=-1      ***************        E                                  
	*/
	int32_t i;
	for (i=0; i< (int32_t)m_HLDataPass1.size(); i++) {
		if (m_HLDataPass1[i].endStop > startPos) {
			break;
		}
		startId = i;
	}
	// go back while the previous element is not eneded
	if (true == backPreviousNotEnded) {
		for (i=startId; i>=0; i--) {
			if (m_HLDataPass1[i].notEnded == false) {
				break;
			}
			startId = i-1;
		}
	}
	int32_t elemStart;
	if(-1 == startId) {
		elemStart = 0;
	} else {
		elemStart = startId+1;
	}
	for (i=elemStart; i< (int32_t)m_HLDataPass1.size(); i++) {
		if (m_HLDataPass1[i].beginStart > endPos)
		{
			stopId = i;
			break;
		}
	}
	/*
	if (-1 != startId && startId < (int32_t)m_HLDataPass1.size()) {
		EDN_DEBUG("==> BEGIN : start="<<m_HLDataPass1[startId].beginStart<<", stop="<<m_HLDataPass1[startId].endStop<<" id=" << startId << "/" << (int32_t)m_HLDataPass1.size()-1);
	} else {
		EDN_DEBUG("==> BEGIN : start=???, stop=??? id=" << startId);
	}
	if (-1 != stopId && stopId < (int32_t)m_HLDataPass1.size()) {
		EDN_DEBUG("==> END   : start="<<m_HLDataPass1[stopId].beginStart<<", stop="<<m_HLDataPass1[stopId].endStop<<" id=" << stopId<< "/" << (int32_t)m_HLDataPass1.size()-1);
	} else {
		EDN_DEBUG("==> END   : start=???, stop=??? id=" << stopId);
	}
	*/
}




void EdnBuf::GenerateHighLightAt(int32_t pos, int32_t endPos, int32_t addinPos)
{
	if (NULL == m_Highlight) {
		return;
	}
	//EDN_DEBUG("area : ("<<pos<<","<<endPos<<") insert at : " << addinPos);
	m_Highlight->Parse(pos, endPos, m_HLDataPass1, addinPos, m_data);
}



void EdnBuf::CleanHighLight(void)
{
	// Remove all element in the list...
	m_HLDataPass1.clear();
}


colorInformation_ts *EdnBuf::GetElementColorAtPosition(int32_t pos, int32_t &starPos)
{
	int32_t i;
	int32_t start = edn_max(0, starPos-1);
	for (i=start; i<(int32_t)m_HLDataPass1.size(); i++) {
		starPos = i;
		if(		m_HLDataPass1[i].beginStart <= pos
			&&	m_HLDataPass1[i].endStop    > pos)
		{
			return &m_HLDataPass1[i];
		}
		if(m_HLDataPass1[i].beginStart > pos) {
			return NULL;
		}
	}
	return NULL;
}



void EdnBuf::HightlightGenerateLines(displayHLData_ts & MData, int32_t HLStart, int32_t nbLines)
{
	MData.posHLPass1 = 0;
	MData.posHLPass2 = 0;
	if (NULL == m_Highlight) {
		return;
	}
	if (MData.idSequence != m_HLDataSequence) {
		GTimeVal timeStart;
		g_get_current_time(&timeStart);
		MData.idSequence = m_HLDataSequence;
		HLStart = StartOfLine(HLStart);
		MData.HLData.clear();
		int32_t HLStop = CountForwardNLines(HLStart, nbLines);
		int32_t startId, stopId;
		// find element previous
		FindMainHighLightPosition(HLStart, HLStop, startId, stopId, true);

		int32_t k;
		//EDN_DEBUG("List of section between : "<< startId << " & " << stopId);
		int32_t endSearch = stopId+1;
		if (-1 == stopId) {
			endSearch = m_HLDataPass1.size();
		}
		for (k=edn_max(startId, 0); k<endSearch; k++) {
			// empty section :
			if (0==k) {
				if (HLStart < m_HLDataPass1[k].beginStart) {
					//EDN_DEBUG("  ==> (empty section 1 ) k="<<k<<" start="<<HLStart<<" stop="<<m_HLDataPass1[k].beginStart );
					m_Highlight->Parse2(HLStart,
										m_HLDataPass1[k].beginStart,
										MData.HLData,
										m_data);
				} // else : nothing to do ...
			} else {
				//EDN_DEBUG("  ==> (empty section 2 ) k="<<k<<" start="<<m_HLDataPass1[k-1].endStop<<" stop="<<m_HLDataPass1[k].beginStart );
				m_Highlight->Parse2(m_HLDataPass1[k-1].endStop,
									m_HLDataPass1[k].beginStart,
									MData.HLData,
									m_data);
			}
			// under section :
			//EDN_DEBUG("  ==> (under section   ) k="<<k<<" start="<<m_HLDataPass1[k].beginStart<<" stop="<<m_HLDataPass1[k].endStop << "subSectionOfID=" << 99999999);
			// TODO ...
		}
		if (endSearch == (int32_t)m_HLDataPass1.size() ){
			if(		k < (int32_t)m_HLDataPass1.size()) {
				//EDN_DEBUG("  ==> (empty section 3 ) k="<<k<<" start="<<m_HLDataPass1[k-1].endStop<<" stop="<<HLStop );
				m_Highlight->Parse2(m_HLDataPass1[k-1].endStop,
									HLStop,
									MData.HLData,
									m_data);
			} else {
				//EDN_DEBUG("  ==> (empty section 4 ) k="<<k<<" start=0 stop="<<HLStop );
				m_Highlight->Parse2(0,
									HLStop,
									MData.HLData,
									m_data);
			}
		}
		
		GTimeVal timeStop;
		g_get_current_time(&timeStop);
		EDN_DEBUG("Display reAnnalyse = " << timeStop.tv_usec - timeStart.tv_usec << " micro-s");
	}

}


colorInformation_ts * EdnBuf::GetElementColorAtPosition(displayHLData_ts & MData, int32_t pos)
{
	int32_t i;
	int32_t start = edn_max(0, MData.posHLPass2-1);
	for (i=start; i<(int32_t)MData.HLData.size(); i++) {
		MData.posHLPass2 = i;
		if(		MData.HLData[i].beginStart <= pos
			&&	MData.HLData[i].endStop    > pos)
		{
			return &MData.HLData[i];
		}
		if(MData.HLData[i].beginStart > pos) {
			return GetElementColorAtPosition(pos, MData.posHLPass1);
		}
	}
	return GetElementColorAtPosition(pos, MData.posHLPass1);
}