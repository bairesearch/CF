/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 * 
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 * 
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 * 
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CFcollapse.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1a2a 18-July-2014
 *
 *******************************************************************************/

#ifndef HEADER_CF_COLLAPSE
#define HEADER_CF_COLLAPSE

#include "CFclass.h"

bool collapseFile(CFblock * firstBlockInList, string outputFileName, bool foldInactive, bool foldComments, bool retainPPD);
	bool collapseBlockToFileObject(CFblock * firstBlockInLayer, CFpreprocessorDef * firstPPDinList, string * outputFileString, int level, bool foldInactive, bool foldComments, bool retainPPD);
		bool isPPDactive(CFpreprocessorDef * firstPPDinList, string PPD);
		void PPDdeactivate(CFpreprocessorDef * firstPPDinList, string PPD);
		void PPDadd(CFpreprocessorDef * firstPPDinList, string PPD);	
		string reworkCodeBlockHashTag(CFblock * currentBlockInLayer, bool foldComments, bool modifyHashTag, int newHashTagID);
			string extractPrependedWhiteSpace(string text);
		#ifdef CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF
		bool isSpecialCaseBlockAlwaysRetainTags(CFblock * currentBlockInLayer);
		#endif		
#endif
