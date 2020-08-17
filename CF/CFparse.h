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
 * File Name: CFparse.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1a3c 27-January-2015
 *
 *******************************************************************************/


#ifndef HEADER_CF_PARSE
#define HEADER_CF_PARSE

#include "CFclass.h"

bool parseBlocksFromFile(CFblock* firstBlockInFile, string parseFileName, int level);
	CFblock* parseTextBlock(ifstream* parseFileObject, CFblock* firstBlockInLayer, int level, int* lineCount, bool* returnUponHashTagExitStatement);
		CFblock* readLargeComment(ifstream* parseFileObject, CFblock* firstBlockInComment, int level, int* lineCount, bool* parsingTextBlock);
		CFblock* readSmallComment(ifstream* parseFileObject, CFblock* firstBlockInComment, int level, int* lineCount, bool* parsingTextBlock);
		void waitForNewLine(ifstream* parseFileObject, int level, int* lineCount, string* currentLine, string* currentTextBlock, bool writeToTextBlock, bool* parsingTextBlock);
		CFblock* processHashTagStatement(ifstream* parseFileObject, CFblock* currentBlockInLayer, int level, int* lineCount, string* currentLine, string* currentTextBlock, string* hashTag, string* hashTagVariableName, bool* returnUponHashTagExitStatement);

		string removeLastCharactersFromString(string str, int numberOfCharactersToRemove);
		string removeSpecificCharactersFromString(string str, char characterToRemove);

#endif
