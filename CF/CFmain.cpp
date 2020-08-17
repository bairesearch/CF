/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: Code Folder
 * Project Version: 1a1a 20-July-2013
 *
 *******************************************************************************/

#include "CFmain.h"
#include "CFparse.h"
#include "CFcollapse.h"
#include "CFclass.h"
#include "SHAREDvars.h"

#ifndef LINUX
	#include <windows.h>
#endif


static char errmessage[] = "Usage:  OpenCF.exe [options]"
"\n"
"\n\twhere options are any of the following"
"\n"
"\n\t-i [string]             : list of input files to be folded (eg PROJECT*.cpp PROJECT*.h main.cpp main.h operations.cpp operations.h)"
"\n\t-foldComments           : fold comments"
"\n\t-foldInactive           : fold inactive code blocks based on preprocessor definitions"
"\n\t-retainPPD              : retain code block preprocessor definitions when folding"
"\n"
"\n\t-workingfolder [string] : working directory name for input files (def: same as exe)"
/*"\n\t-exefolder [string]     : exe directory name for executables OpenCF.exe and (def: same as exe)"*/
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe/output)"
"\n"
"\n\t-version                : print version"
"\n"
"\n\tThe program permanently folds inactive code blocks based on active preprocessor definitions."
"\n"
"\n";



		
int main(int argc,char **argv)
{
	bool result = false;
	
	bool foldComments = false;
	bool foldInactive = false;
	bool retainPPD = false;
		
	bool passInputReq = false;
	vector<string> inputFileNamesVector;	
	
	#ifdef CF_DEBUG
	if(argumentExists(argc, argv, string("-i")))
	#else
	if(argumentExists(argc, argv, string("-i")) && (argumentExists(argc, argv, string("-foldComments")) || argumentExists(argc, argv, string("-foldInactive"))))
	#endif
	{
		passInputReq = true;
		//getStringArgument(argc,argv,"-i");
		getStringArrayArgument(argc, argv, "-i", &inputFileNamesVector);

		if(argumentExists(argc, argv, string("-foldComments")))
		{
			foldComments = true;
		}
		if(argumentExists(argc, argv, string("-foldInactive")))
		{
			foldInactive = true;
		}
		if(argumentExists(argc, argv, string("-retainPPD")))
		{
			retainPPD = true;
		}
				
		string currentFolder = getCurrentDirectory();	
		if(argumentExists(argc, argv, string("-workingfolder")))
		{
			workingFolder = getStringArgument(argc, argv, string("-workingfolder"));
		}
		else
		{
			workingFolder = currentFolder;
		}
		/*
		if(argumentExists(argc, argv, string("-exefolder")))
		{
			exeFolder = getStringArgument(argc, argv, string("-exefolder"));
		}
		else
		{
			exeFolder = currentFolder;
		}
		*/
		if(argumentExists(argc, argv, string("-tempfolder")))
		{
			tempFolder = getStringArgument(argc, argv, string("-tempfolder"));
		}
		else
		{
			tempFolder = currentFolder + CF_DEFAULT_OUTPUT_FOLDER_RELATIVE_PATH;
		}
				
		if(argumentExists(argc,argv,"-version"))
		{
			cout << "OpenCF.exe - Project Version: 1a1a 20-July-2013" << endl;
			exit(1);
		}
	}

	if(passInputReq)
	{	
		#ifdef CF_DEBUG
		cout << "workingFolder = " << workingFolder << endl;
		cout << "tempFolder = " << tempFolder << endl;
		#endif		
		for(vector<string>::iterator connectionIter = inputFileNamesVector.begin(); connectionIter != inputFileNamesVector.end(); connectionIter++)
		{
			CFblock * firstBlockInList = new CFblock();
			string fileName = *connectionIter;
			#ifdef CF_DEBUG
			cout << "fileName = " << fileName << endl;
			#endif			
			changeDirectory(workingFolder);
			#ifdef CF_DEBUG
			cout << "main: parseBlocksFromFile()" << endl;			
			#endif
			if(parseBlocksFromFile(firstBlockInList, fileName, CF_INCLUDE_LEVEL_FIRST))
			{
				result = false;
			}
			#ifdef CF_DEBUG
			cout << "main: collapseFile()" << endl;
			#endif
			changeDirectory(tempFolder);			
			if(collapseFile(firstBlockInList, fileName, foldInactive, foldComments, retainPPD))
			{
				result = false;
			}
		}			
	}
	else
	{
		printf(errmessage);
		cout << "**** Known Limitations: *****" << endl;
		cout << "all c/cpp and h/hpp files that wish to be parsed must be contained in the same directory" << endl;
		cout << "CF does not support #tags containing '\\' or '/' characters" << endl;
		cout << "CF does not parse #include files referenced by a relative path" << endl;
		cout << "CF does not support #tags proceeding comments on the same line" << endl;	
		cout << "CF only supports a single preprocessor statement per line" << endl;	
		/*cout << "Small (//) comments are ignored immediately after preprocessor definition statements (they are not folded)" << endl;*/
		cout << "Multiline comments are not supported immediately after preprocessor definition statements (on the same line)" << endl;			
		cout << "CF requires include/header files that wish to be parsed to to be delimited with " " rather than < >" << endl;
		cout << "CF requires a single space between #tags and preprocessor variables e.g. #include \"this.h\"" << endl;
		cout << "Ensure that the temp (output) folder is clear of all output files" << endl;
		cout << "****************************" << endl;
		exit(0);
	}
}

