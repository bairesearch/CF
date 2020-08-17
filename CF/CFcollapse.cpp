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
 * File Name: CFcollapse.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1a4a 10-July-2015
 *
 *******************************************************************************/


#include "CFcollapse.h"

//retainPPD is assumed true as not all #define statements are related to code blocks (and as such it is unknown whether they can be deleted entirely from the code)
bool collapseFile(CFblock* firstBlockInList, string outputFileName, bool foldInactive, bool foldComments, bool retainPPD)
{
	bool result = true;
	
	CFpreprocessorDef* firstPPDinList = new CFpreprocessorDef();
	
	string outputFileString = "";
	if(!collapseBlockToFileObject(firstBlockInList, firstPPDinList, &outputFileString, 1, foldInactive, foldComments, retainPPD))
	{
		result = false;
	}
	
	ofstream writeFileObject(outputFileName.c_str());
	for(int i=0; i<outputFileString.length(); i++)
	{
		writeFileObject.put(outputFileString[i]);
	}
	//writeFileObject.put(CHAR_NEWLINE);	//required for Windows SW to re-read files?
	writeFileObject.close();
	
	return result;
}

bool collapseBlockToFileObject(CFblock* firstBlockInLayer, CFpreprocessorDef* firstPPDinList, string* outputFileString, int level, bool foldInactive, bool foldComments, bool retainPPD)
{	
	bool result = true;
	
	bool ifCaseStillTrying = true;					//e.g. if "IF" failed, still looking for "ELIF"/"ELSE"
	int retainPPDelseStatementTagID = CF_BLOCK_CASE_TYPE_UNDEFINED;	//used to modify #else statement when retainPPD is used
	string retainPPDelseStatementTagVariableName = "";		//used to modify #else statement when retainPPD is used
	bool specialCaseBlockAlwaysRetainTags = false;		//used in case of CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF
		
	CFblock* currentBlockInLayer = firstBlockInLayer;
	
	while(currentBlockInLayer->next != NULL)
	{
		#ifdef CF_DEBUG_COLLAPSE
		for(int i=0; i<level; i++)
		{
			cout << "\t";
		}
		cout << "\033[1;34m collapseBlockToFileObject: hashTag = " << currentBlockInLayer->hashTag << ", varName = " <<  currentBlockInLayer->hashTagVariableName << ", isActive = " << isPPDactive(firstPPDinList, currentBlockInLayer->hashTagVariableName) << " \033[0m" << endl;
		#endif	
		
		if(currentBlockInLayer->type == CF_BLOCK_TYPE_UNDEFINED)
		{
			cout << "collapseBlockToFileObject() error: (currentBlockInLayer->type == CF_BLOCK_TYPE_UNDEFINED)" << endl;
		}
		else if(currentBlockInLayer->type == CF_BLOCK_TYPE_COMMENT)
		{
			if(level == CF_INCLUDE_LEVEL_FIRST)
			{//only write code for 1st level (ie, output file)
				if(!foldComments)
				{
					*outputFileString = *outputFileString + currentBlockInLayer->text;
				}
				else
				{
					if(currentBlockInLayer->commentType == CF_BLOCK_CASE_TYPE_COMMENT_SMALL)
					{
						*outputFileString = *outputFileString + CHAR_NEWLINE;	//compensate for the fact char new lines are being recorded in small comments
					}
				}
			}
		}
		else if(currentBlockInLayer->type == CF_BLOCK_TYPE_INCLUDE)
		{
			if(level == CF_INCLUDE_LEVEL_FIRST)
			{//only write code for 1st level (ie, output file)
				if(foldComments)
				{									
					*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
				}
				else
				{
					*outputFileString = *outputFileString + currentBlockInLayer->text;	//place the "#include _" file inf
				}			
			}
			if(!collapseBlockToFileObject(currentBlockInLayer->lower, firstPPDinList, outputFileString, level+1, foldInactive, foldComments, retainPPD))
			{
				result = false;
			}
			
		}
		else if(currentBlockInLayer->type == CF_BLOCK_TYPE_CASE)
		{
			if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_INCLUDE)
			{
				cout << "collapseBlockToFileObject() error: (currentBlockInLayer->type == CF_BLOCK_TYPE_CASE) && (currentBlockInLayer->hashTagID == CF_HASH_TAG_INCLUDE)" << endl;
			}
			else if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_DEFINE)
			{
				if(level == CF_INCLUDE_LEVEL_FIRST)
				{//only write code for 1st level (ie, output file)
					*outputFileString = *outputFileString + currentBlockInLayer->text;
				}			
				PPDadd(firstPPDinList, currentBlockInLayer->hashTagVariableName);
			}	
			else if((currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_IFDEF) || (currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELIF_DEFINED))
			{
				if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_IFDEF)
				{
					retainPPDelseStatementTagID = CF_BLOCK_CASE_TYPE_IFNDEF;
					retainPPDelseStatementTagVariableName = currentBlockInLayer->hashTagVariableName;			
				}
				if(!foldInactive || (ifCaseStillTrying && isPPDactive(firstPPDinList, currentBlockInLayer->hashTagVariableName)))
				{
					if(!foldInactive || retainPPD)
					{
						if(level == CF_INCLUDE_LEVEL_FIRST)
						{//only write code for 1st level (ie, output file)
							if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELIF_DEFINED)
							{
								if(!foldInactive)
								{
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
								}
								else if(retainPPD)
								{//convert #elif defined/#elif !defined to #if/#ifndef
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, true, CF_BLOCK_CASE_TYPE_IFDEF);
								}
							}
							else 
							{
								if(foldComments)
								{									
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
								}
								else
								{
									*outputFileString = *outputFileString + currentBlockInLayer->text;
								}
							}
						}
					}				
					if(!collapseBlockToFileObject(currentBlockInLayer->lower, firstPPDinList, outputFileString, level, foldInactive, foldComments, retainPPD))
					{
						result = false;
					}
					ifCaseStillTrying = false;	
				}
			}	
			else if((currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_IFNDEF) || (currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELIF_NDEFINED))
			{
				#ifdef CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF
				specialCaseBlockAlwaysRetainTags = isSpecialCaseBlockAlwaysRetainTags(currentBlockInLayer);
				#endif
								
				if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_IFNDEF)
				{
					retainPPDelseStatementTagID = CF_BLOCK_CASE_TYPE_IFDEF;
					retainPPDelseStatementTagVariableName = currentBlockInLayer->hashTagVariableName;			
				}			
				if(!foldInactive || (ifCaseStillTrying && !isPPDactive(firstPPDinList, currentBlockInLayer->hashTagVariableName)) || specialCaseBlockAlwaysRetainTags)
				{
					if(!foldInactive || retainPPD || specialCaseBlockAlwaysRetainTags)
					{
						if(level == CF_INCLUDE_LEVEL_FIRST)
						{//only write code for 1st level (ie, output file)
							if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELIF_NDEFINED)
							{
								if(!foldInactive)
								{
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
								}
								else if(retainPPD)
								{//convert #elif defined/#elif !defined to #if/#ifndef
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, true, CF_BLOCK_CASE_TYPE_IFNDEF);
								}
							}
							else 
							{
								if(foldComments)
								{
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
								}
								else
								{
									*outputFileString = *outputFileString + currentBlockInLayer->text;
								}
							}
						}
					}
									
					if(!collapseBlockToFileObject(currentBlockInLayer->lower, firstPPDinList, outputFileString, level, foldInactive, foldComments, retainPPD))
					{
						result = false;
					}
					ifCaseStillTrying = false;	
				}			
			}	
			else if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELSE)
			{
				if(!foldInactive || ifCaseStillTrying)
				{
					if(!foldInactive || retainPPD)
					{
						if(level == CF_INCLUDE_LEVEL_FIRST)
						{//only write code for 1st level (ie, output file)
							if(retainPPD)
							{//convert #else to #if/#ifndef
								currentBlockInLayer->hashTagVariableName = retainPPDelseStatementTagVariableName;	//temporarily set this value
								*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, true, retainPPDelseStatementTagID);
							}
							else
							{											
								if(foldComments)
								{
									*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
								}
								else
								{
									*outputFileString = *outputFileString + currentBlockInLayer->text;
								}
							}
						}
					}			
					if(!collapseBlockToFileObject(currentBlockInLayer->lower, firstPPDinList, outputFileString, level, foldInactive, foldComments, retainPPD))
					{
						result = false;
					}
					ifCaseStillTrying = false;		
				}
			}	
			else if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ENDIF)
			{	
				if(!foldInactive || (retainPPD && !ifCaseStillTrying) || specialCaseBlockAlwaysRetainTags)
				{
					if(level == CF_INCLUDE_LEVEL_FIRST)
					{//only write code for 1st level (ie, output file)
						if(foldComments)
						{
							*outputFileString = *outputFileString + reworkCodeBlockHashTag(currentBlockInLayer, foldComments, false, NULL);
						}
						else
						{
							*outputFileString = *outputFileString + currentBlockInLayer->text;
						}
					}
				}
				
				ifCaseStillTrying = true;
				retainPPDelseStatementTagID = CF_BLOCK_CASE_TYPE_UNDEFINED;
				retainPPDelseStatementTagVariableName = "";	
				specialCaseBlockAlwaysRetainTags = false;								
				//assume at the end of the currentBlockInLayer 
			}	
			else if(currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_UNDEF)
			{
				if(level == CF_INCLUDE_LEVEL_FIRST)
				{//only write code for 1st level (ie, output file)
					*outputFileString = *outputFileString + currentBlockInLayer->text;
				}			
				PPDdeactivate(firstPPDinList, currentBlockInLayer->hashTagVariableName);
			}			
			else
			{
				cout << "collapseBlockToFileObject() error: (currentBlockInLayer->hashTagID == " << currentBlockInLayer->hashTagID << ")" << endl;
			}																									
		}
		else if(currentBlockInLayer->type == CF_BLOCK_TYPE_CODE)
		{
			if(level == CF_INCLUDE_LEVEL_FIRST)
			{//only write code for 1st level (ie, output file)
				*outputFileString = *outputFileString + currentBlockInLayer->text;
			}
		}
		else
		{
			cout << "collapseBlockToFileObject() error: (currentBlockInLayer->type == " << currentBlockInLayer->type << ")" << endl;
		}
		
		currentBlockInLayer = currentBlockInLayer->next;	
	}
	return result;
}

void PPDadd(CFpreprocessorDef* firstPPDinList, string PPD)
{
	CFpreprocessorDef* currentPPD = firstPPDinList;
	while(currentPPD->next != NULL)
	{
		currentPPD = currentPPD->next;
	}
	currentPPD->name = PPD;
	currentPPD->active = true;
	currentPPD->next = new CFpreprocessorDef();
}

void PPDdeactivate(CFpreprocessorDef* firstPPDinList, string PPD)
{
	CFpreprocessorDef* currentPPD = firstPPDinList;
	while(currentPPD->next != NULL)
	{
		if(currentPPD->name == PPD)
		{	
			currentPPD->active = false;
		}
		currentPPD = currentPPD->next;
	}
}

bool isPPDactive(CFpreprocessorDef* firstPPDinList, string PPD)
{
	bool isActive = false;
	CFpreprocessorDef* currentPPD = firstPPDinList;
	while(currentPPD->next != NULL)
	{
		if(currentPPD->name == PPD)
		{
			if(currentPPD->active)
			{
				isActive = true;
			}
		}
		currentPPD = currentPPD->next;
	}
	return isActive;	
}

string reworkCodeBlockHashTag(CFblock* currentBlockInLayer, bool foldComments, bool modifyHashTag, int newHashTagID)
{
	string hashTagLineWithAppendedCommentsRemoved = "";
	
	string prependedWhiteSpace = extractPrependedWhiteSpace(currentBlockInLayer->text);
	
	string hashTag = currentBlockInLayer->hashTag;	//OR hashTagArrayAll[currentBlockInLayer->hashTagID];
	if(modifyHashTag)
	{		
		hashTag = hashTagArrayAll[newHashTagID];
	}
	
	//cout << "2" << endl;
	string comments = "";
	if(foldComments)
	{
		if((currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELSE) || (currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ENDIF))
		{//if original hashTagID is #else/#endif, then expect no hashTagVariableName in text
			int indexOfComments = (currentBlockInLayer->text).find(currentBlockInLayer->hashTag) + (currentBlockInLayer->hashTag).length();
			int lengthOfComments = (currentBlockInLayer->text).length() - indexOfComments  - 1;	//-1 to ignore the CHAR_NEW
			comments = (currentBlockInLayer->text).substr(indexOfComments, lengthOfComments);
			/*
			cout << "hashTagLength = " << (currentBlockInLayer->hashTag).length() << endl;
			cout << "indexOfComments = " << indexOfComments << endl;	
			cout << "lengthOfComments = " << lengthOfComments << endl;							
			*/
		}
		else
		{
			int indexOfComments = (currentBlockInLayer->text).find(currentBlockInLayer->hashTagVariableName) + (currentBlockInLayer->hashTagVariableName).length();
			int lengthOfComments = (currentBlockInLayer->text).length() - indexOfComments  - 1;	//-1 to ignore the CHAR_NEW
			comments = (currentBlockInLayer->text).substr(indexOfComments, lengthOfComments);
		}	
	}

	/*for safety only
	if(!modifyHashTag)
	{
		if((currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ELSE) || (currentBlockInLayer->hashTagID == CF_BLOCK_CASE_TYPE_ENDIF))
		{
			currentBlockInLayer->hashTagVariableName = "";
		}
	}
	*/
			
	//cout << "\n reworkCodeBlockHashTag():" << endl;
	//cout << "\033[1;36m orig text: " << currentBlockInLayer->text << "\033[0m" << endl;
	//cout << "prependedWhiteSpace = " << prependedWhiteSpace << endl;
	//cout << "hashTag = " << hashTag << endl;
	//cout << "currentBlockInLayer->hashTagVariableName = " << currentBlockInLayer->hashTagVariableName << endl;
	//cout << "comments = " <<comments << endl;
	
	hashTagLineWithAppendedCommentsRemoved = hashTagLineWithAppendedCommentsRemoved + prependedWhiteSpace + hashTag + CHAR_SPACE + currentBlockInLayer->hashTagVariableName + comments + CHAR_NEWLINE;
	
	/*
	cout << "\n reworkCodeBlockHashTag():" << endl;
	cout << "\033[1;36m orig text: " << currentBlockInLayer->text << "\033[0m" << endl;
	cout << "prependedWhiteSpace = " << prependedWhiteSpace << endl;
	cout << "hashTagID = " << currentBlockInLayer->hashTagID << endl;
	cout << "hashTag = " << currentBlockInLayer->hashTag << endl;
	cout << "newHashTagID = " << newHashTagID << endl;	
	cout << "newHashTag = " << hashTag << endl;
	cout << "comments = " << comments << endl;
	cout << "currentBlockInLayer->hashTagVariableName = " << currentBlockInLayer->hashTagVariableName << endl;
	cout << "hashTagLineWithAppendedCommentsRemoved = " << hashTagLineWithAppendedCommentsRemoved << endl;	
	cout << "end reworkCodeBlockHashTag" << endl;
	*/
	
	return hashTagLineWithAppendedCommentsRemoved;
}

string extractPrependedWhiteSpace(string text)
{
	bool stillWhiteSpace = true;
	string whiteSpace = "";
	for(int i=0; i<text.length(); i++)
	{
		if(stillWhiteSpace)
		{
			if(isWhiteSpace(text[i]))
			{
				whiteSpace = whiteSpace + text[i];	
			}
			else
			{
				stillWhiteSpace = false;
			}
		}
	}
	return whiteSpace;
}

#ifdef CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF
bool isSpecialCaseBlockAlwaysRetainTags(CFblock* currentBlockInLayer)
{
	bool specialCaseBlockAlwaysRetainTags = false;
	int indexOfSpecial = (currentBlockInLayer->hashTagVariableName).find(CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF_TAG_NAME_PREPEND);
	if(indexOfSpecial == 0)
	{
		specialCaseBlockAlwaysRetainTags = true;
	}
	return specialCaseBlockAlwaysRetainTags;
}
#endif
