/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: CFclass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1b2a 16-March-2017
 *
 *******************************************************************************/


#ifndef HEADER_CF_CLASS
#define HEADER_CF_CLASS

#include "CFglobalDefs.h"
#include "SHAREDvars.hpp"	//required for isWhiteSpace and convertStringToLowerCase

#define CF_BLOCK_TYPE_UNDEFINED (0)
#define CF_BLOCK_TYPE_COMMENT (1)
#define CF_BLOCK_TYPE_INCLUDE (2)
#define CF_BLOCK_TYPE_CASE (3)
#define CF_BLOCK_TYPE_CODE (4)	//normal
#define CF_BLOCK_NUMBER_OF_TYPES (5)
static string blockTypeArray[CF_BLOCK_NUMBER_OF_TYPES] = {"undefined", "comment", "include", "case", "code"};

#define CF_HASH_TAG_INCLUDE "#include"
#define CF_HASH_TAG_DEFINE "#define"
#define CF_HASH_TAG_IFDEF "#ifdef"
#define CF_HASH_TAG_IFNDEF "#ifndef"
#define CF_HASH_TAG_ELIF_DEFINED "#elif defined"
#define CF_HASH_TAG_ELIF_NDEFINED "#elif !defined"
#define CF_HASH_TAG_ELSE "#else"
#define CF_HASH_TAG_ENDIF "#endif"
#define CF_HASH_TAG_UNDEF "#undef"
#define CF_BLOCK_CASE_TYPE_UNDEFINED (-1)
#define CF_BLOCK_CASE_TYPE_INCLUDE (0)
#define CF_BLOCK_CASE_TYPE_DEFINE (1)
#define CF_BLOCK_CASE_TYPE_IFDEF (2)
#define CF_BLOCK_CASE_TYPE_IFNDEF (3)
#define CF_BLOCK_CASE_TYPE_ELIF_DEFINED (4)
#define CF_BLOCK_CASE_TYPE_ELIF_NDEFINED (5)
#define CF_BLOCK_CASE_TYPE_ELSE (6)
#define CF_BLOCK_CASE_TYPE_ENDIF (7)
#define CF_BLOCK_CASE_TYPE_UNDEF (8)
#define CF_HASH_TAG_NUMBER_OF_TYPES (9)
static string hashTagArrayAll[CF_HASH_TAG_NUMBER_OF_TYPES] = {CF_HASH_TAG_INCLUDE, CF_HASH_TAG_DEFINE, CF_HASH_TAG_IFDEF, CF_HASH_TAG_IFNDEF, CF_HASH_TAG_ELIF_DEFINED, CF_HASH_TAG_ELIF_NDEFINED, CF_HASH_TAG_ELSE, CF_HASH_TAG_ENDIF, CF_HASH_TAG_UNDEF};
#define CF_HASH_TAG_WITH_VARIABLES_NUMBER_OF_TYPES (7)	//#else and #endif do not have a proceeding #tag variable name
static string hashTagArrayWithVariables[CF_HASH_TAG_WITH_VARIABLES_NUMBER_OF_TYPES] = {CF_HASH_TAG_INCLUDE, CF_HASH_TAG_DEFINE, CF_HASH_TAG_IFDEF, CF_HASH_TAG_IFNDEF, CF_HASH_TAG_ELIF_DEFINED, CF_HASH_TAG_ELIF_NDEFINED, CF_HASH_TAG_UNDEF};
#define CF_HASH_TAG_WITHOUT_VARIABLES_NUMBER_OF_TYPES (2)	//#else and #endif do not have a proceeding #tag variable name
static string hashTagArrayWithoutVariables[CF_HASH_TAG_WITHOUT_VARIABLES_NUMBER_OF_TYPES] = {CF_HASH_TAG_ELSE, CF_HASH_TAG_ENDIF};
#define CF_HASH_TAG_CASE_DEFINITIONS_NUMBER_OF_TYPES (2)
static string hashTagArrayDefinitions[CF_HASH_TAG_CASE_DEFINITIONS_NUMBER_OF_TYPES] = {CF_HASH_TAG_DEFINE, CF_HASH_TAG_UNDEF};
#define CF_HASH_TAG_CASE_IFDEF_IFNDEF_NUMBER_OF_TYPES (2)
static string hashTagArrayIfdefIfndef[CF_HASH_TAG_CASE_IFDEF_IFNDEF_NUMBER_OF_TYPES] = {CF_HASH_TAG_IFDEF, CF_HASH_TAG_IFNDEF};
#define CF_HASH_TAG_CASE_IFDEF_ELIF_DEFINED_ELIF_NDEFINED_NUMBER_OF_TYPES (2)
static string hashTagArrayElifDefinedElifNdefined[CF_HASH_TAG_CASE_IFDEF_ELIF_DEFINED_ELIF_NDEFINED_NUMBER_OF_TYPES] = {CF_HASH_TAG_IFDEF, CF_HASH_TAG_IFNDEF};
#define CF_HASH_TAG_CASE_EXIT_NUMBER_OF_TYPES (4)
static string hashTagArrayExit[CF_HASH_TAG_CASE_EXIT_NUMBER_OF_TYPES] = {CF_HASH_TAG_ELIF_DEFINED, CF_HASH_TAG_ELIF_NDEFINED, CF_HASH_TAG_ELSE, CF_HASH_TAG_ENDIF};
#define CF_HASH_TAG_CASE_ENTER_NUMBER_OF_TYPES (2)
static string hashTagArrayEnter[CF_HASH_TAG_CASE_ENTER_NUMBER_OF_TYPES] = {CF_HASH_TAG_IFDEF, CF_HASH_TAG_IFNDEF};


#define CF_HASH_TAG_SPECIAL_CASE_ELIF "#elif"

#define CF_BLOCK_CASE_TYPE_COMMENT_UNDEFINED (0)
#define CF_BLOCK_CASE_TYPE_COMMENT_SMALL (1)
#define CF_BLOCK_CASE_TYPE_COMMENT_LARGE (2)


class CFblock{
private:
	/*There are currently no private attributes of this class*/
public:

	CFblock(void); // constructor declaration
	~CFblock();	//  and destructor.

	int type;
	string text;
	int hashTagID;			//only used if type == CF_BLOCK_TYPE_CASE / CF_BLOCK_TYPE_INCLUDE
	string hashTag;			//only used if type == CF_BLOCK_TYPE_CASE (for case sensitive operations only; otherwise use hashTagID)
	string hashTagVariableName;	//only used if type == CF_BLOCK_TYPE_CASE / CF_BLOCK_TYPE_INCLUDE
	int commentType;
	 
	CFblock* next;
	CFblock* lower;
};

class CFpreprocessorDef{
private:
	/*There are currently no private attributes of this class*/
public:

	CFpreprocessorDef(void); // constructor declaration
	~CFpreprocessorDef();	//  and destructor.

	string name;
	bool active;
	 
	CFpreprocessorDef* next;
};

int getSupportedHashTagID(string hashTag, string* hashTagArray, int hashTagArraySize);

#endif
