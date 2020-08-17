/*******************************************************************************
 * 
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 * 
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CFparse.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1c2a 15-March-2018
 * /
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
