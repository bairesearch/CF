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
 * File Name: CFclass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1b2a 16-March-2017
 *
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


