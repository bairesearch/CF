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
 * File Name: CFglobalDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1a3c 27-January-2015
 *
 *******************************************************************************/


#ifndef HEADER_CF_GLOBAL_DEFS
#define HEADER_CF_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

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
