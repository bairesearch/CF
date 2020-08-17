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
 * File Name: CFclass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1c2a 15-March-2018
 * /
 *******************************************************************************/


#include "CFclass.h"

CFblock::CFblock(void)
{
	type = CF_BLOCK_TYPE_CODE;				//default case
	text = "";
	hashTagID = CF_BLOCK_CASE_TYPE_UNDEFINED;	//only used if type == CF_BLOCK_TYPE_CASE / CF_BLOCK_TYPE_INCLUDE
	hashTag = "";					//only used if type == CF_BLOCK_TYPE_CASE (for case sensitive operations only; otherwise use hashTagID)
	hashTagVariableName = "NA";			//only used if type == CF_BLOCK_TYPE_CASE / CF_BLOCK_TYPE_INCLUDE
	commentType = CF_BLOCK_CASE_TYPE_COMMENT_UNDEFINED;	//only used if type == CF_BLOCK_TYPE_COMMENT
	
	next = NULL;
	lower = NULL;
}

CFblock::~CFblock(void)
{
}

CFpreprocessorDef::CFpreprocessorDef(void)
{
	name = "";		//default case
	active = false;
	 
	next = NULL;
}

CFpreprocessorDef::~CFpreprocessorDef(void)
{
}


int getSupportedHashTagID(string hashTag, string* hashTagArray, int hashTagArraySize)
{
	int hashTagID = CF_BLOCK_CASE_TYPE_UNDEFINED;
	for(int i=0; i< hashTagArraySize; i++)
	{
		if(SHAREDvarsClass().convertStringToLowerCase(&hashTag) == hashTagArray[i])	//convertStringToLowerCase: allow preprocessor defs of arbitrary case
		{	
			hashTagID = i;
		}
	}
	
	return hashTagID;
}


