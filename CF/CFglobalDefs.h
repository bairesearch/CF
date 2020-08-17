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
 * File Name: CFglobalDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1c2a 15-March-2018
 * /
 *******************************************************************************/


#ifndef HEADER_CF_GLOBAL_DEFS
#define HEADER_CF_GLOBAL_DEFS

#include "SHAREDglobalDefs.hpp"

//#define CF_FOLDSPECIFIC_MATCH_EXACT_STRING	//1b1b

//#define CF_DEBUG_PARSE
//#define CF_DEBUG_COLLAPSE
#define CF_DEBUG_ALLOW_SAME_OUTPUT_TEST
#define CF_INCLUDE_LEVEL_FIRST (1)
#define CF_DEFAULT_OUTPUT_FOLDER_RELATIVE_PATH "/output"
#define CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF
#ifdef CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF
	#define CF_SPECIAL_CASE_BLOCK_ALWAYS_RETAIN_TAGS_HEADER_IFNDEF_TAG_NAME_PREPEND "HEADER_"
#endif

#endif
