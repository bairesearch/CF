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
 * File Name: CFmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1b2a 16-March-2017
 *
 *******************************************************************************/


#include "CFmain.h"
#include "CFparse.h"
#include "CFcollapse.h"
#include "CFclass.h"
#include "SHAREDvars.hpp"

#ifndef LINUX
	#include <windows.h>
#endif


static char errmessage[] = "Usage:  OpenCF.exe [options]"
"\n"
"\n\twhere options are any of the following"
"\n"
"\n\t-i [string]                     : list of input files to be folded (eg PROJECT*.cpp PROJECT*.hpp)"
"\n\t-foldComments                   : fold comments"
"\n\t-foldInactive                   : fold inactive code blocks based on preprocessor definitions"
"\n\t-foldSpecific [string]          : fold specific inactive code blocks based on preprocessor definitions"
"\n\t-retainPPD                      : retain code block preprocessor definitions when folding"
"\n"
"\n\t-inputfolder [string]     : input directory name for input files (def: same as exe)"
/*"\n\t-exefolder [string]     : exe directory name for executables; OpenCF.exe (def: same as exe)"*/
"\n\t-outputfolder [string]    : output directory name for temporary and output files (def: same as exe/output)"
"\n"
"\n\t-version                 : print version"
"\n"
"\n\tThe program permanently folds inactive code blocks based on active preprocessor definitions."
"\n"
"\n";



		
int main(const int argc, const char** argv)
{
	bool result = false;
	
	bool foldComments = false;
	bool foldInactive = false;
	bool foldSpecific = false;
	string foldSpecificBlockNameSubset = "";	//eg "DEBUG"
	bool retainPPD = false;
		
	bool passInputReq = false;
	vector<string> inputFileNamesVector;	
	
	#ifdef CF_DEBUG_ALLOW_SAME_OUTPUT_TEST
	if(SHAREDvarsClass().argumentExists(argc, argv, string("-i")))
	#else
	if(SHAREDvarsClass().argumentExists(argc, argv, string("-i")) && (SHAREDvarsClass().argumentExists(argc, argv, string("-foldComments")) || SHAREDvarsClass().argumentExists(argc, argv, string("-foldInactive"))))
	#endif
	{
		passInputReq = true;
		//getStringArgument(argc,argv,"-i");
		SHAREDvarsClass().getStringArrayArgument(argc, argv, "-i", &inputFileNamesVector);

		if(SHAREDvarsClass().argumentExists(argc, argv, string("-foldComments")))
		{
			foldComments = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, string("-foldInactive")))
		{
			foldInactive = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, string("-foldSpecific")))
		{
			foldSpecific = true;
			foldSpecificBlockNameSubset = SHAREDvarsClass().getStringArgument(argc, argv, string("-foldSpecific"));
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, string("-retainPPD")))
		{
			retainPPD = true;
		}
				
		string currentFolder = SHAREDvarsClass().getCurrentDirectory();	
		if(SHAREDvarsClass().argumentExists(argc, argv, string("-inputfolder")))
		{
			inputFolder = SHAREDvarsClass().getStringArgument(argc, argv, string("-inputfolder"));
		}
		else
		{
			inputFolder = currentFolder;
		}
		/*
		if(SHAREDvarsClass().argumentExists(argc, argv, string("-exefolder")))
		{
			exeFolder = SHAREDvarsClass().getStringArgument(argc, argv, string("-exefolder"));
		}
		else
		{
			exeFolder = currentFolder;
		}
		*/
		if(SHAREDvarsClass().argumentExists(argc, argv, string("-outputfolder")))
		{
			outputFolder = SHAREDvarsClass().getStringArgument(argc, argv, string("-outputfolder"));
		}
		else
		{
			outputFolder = currentFolder + CF_DEFAULT_OUTPUT_FOLDER_RELATIVE_PATH;
		}
				
		if(SHAREDvarsClass().argumentExists(argc, argv, "-version"))
		{
			cout << "OpenCF.exe - Project Version: 1b2a 16-March-2017" << endl;
			exit(EXIT_OK);
		}
	}

	if(passInputReq)
	{	
		#ifdef CF_DEBUG_PARSE
		cout << "inputFolder = " << inputFolder << endl;
		cout << "outputFolder = " << outputFolder << endl;
		#endif		
		for(vector<string>::iterator connectionIter = inputFileNamesVector.begin(); connectionIter != inputFileNamesVector.end(); connectionIter++)
		{
			CFblock* firstBlockInList = new CFblock();
			string fileName = *connectionIter;
			cout << "fileName = " << fileName << endl;
			SHAREDvarsClass().changeDirectory(inputFolder);
			#ifdef CF_DEBUG_PARSE
			cout << "main: parseBlocksFromFile()" << endl;			
			#endif
			if(parseBlocksFromFile(firstBlockInList, fileName, CF_INCLUDE_LEVEL_FIRST))
			{
				result = false;
			}
			#ifdef CF_DEBUG_PARSE
			cout << "main: collapseFile()" << endl;
			#endif
			SHAREDvarsClass().changeDirectory(outputFolder);			
			if(collapseFile(firstBlockInList, fileName, foldInactive, foldComments, retainPPD, foldSpecific, foldSpecificBlockNameSubset))
			{
				result = false;
			}
		}			
	}
	else
	{
		printf(errmessage);
		cout << "**** Known Limitations:* ****" << endl;
		cout << "all c/cpp and h/hpp files that wish to be parsed must be contained in the same directory" << endl;
		cout << "CF does not support #tags containing '\\' or '/' characters" << endl;
		cout << "CF does not parse #include files referenced by a relative path" << endl;
		cout << "CF does not support #tags proceeding comments on the same line" << endl;	
		cout << "CF only supports a single preprocessor statement per line" << endl;	
		/*cout << "Small (//) comments are ignored immediately after preprocessor definition statements (they are not folded)" << endl;*/
		cout << "Multiline comments are not supported immediately after preprocessor definition statements (on the same line)" << endl;			
		cout << "CF requires include/header files that wish to be parsed to to be delimited with " " rather than < >" << endl;
		cout << "CF requires a single space between #tags and preprocessor variables e.g. #include \"this.h\", #ifdef X, #define Y, etc" << endl;
		cout << "Ensure that the temp (output) folder is clear of all output files" << endl;
		cout << "CF does not support \"if defined\" logic (eg multiple if defined statements on a single line)" << endl;
		cout << "CF requires all files to be the operating system text format (eg UNIX/ASCII)" << endl;	
		cout << "CF requires all #tags to be properly defined (eg #endif; is illegal)" << endl;
		cout << "CF does not support external (library) preprocessor defs, eg #ifdef __FREEGLUT_EXT_H__" << endl;
		cout << "CF requires all relevant preprocessor defs to be defined within the scope (ie necessary preprocessor definitions cannot be inherited from files from which they are being included, they must be defined within their own include file scope)" << endl;
		cout << "****************************" << endl;
		exit(EXIT_ERROR);
	}
}

