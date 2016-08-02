// Name        : SPEAR_parser.cpp
// Author      : Victor Zappi
// Version     : 1.1
// Copyright   : Your copyright notice
// Description : static copy of latest version
//============================================================================


#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>					// only for getopt
#include <dirent.h> 				// files in dir
#else
#include "../headers/XGetopt.h"		// getopt wrapper for win
#include "../headers/direntwin.h"	// dirent wrapper for win
#include <direct.h>
#endif
#include "../headers/spear_parser.h"
#include "../headers/Dbox_dumper.h"


#define PARTIALS parser->partials

// from path to name
string getExeName(char *path)
{
	// From http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	string s = string(path);
	string delimiter = "/";

	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos)	// split in token delimited by "/"
	{
	    token = s.substr(0, pos);
	    s.erase(0, pos + delimiter.length());			// move until we get the last token, which is the name
	}

	return s;
}

int setDumpfileName(string &file)
{
	int len  = file.length();

	if(file[len-4] != '.')
	{
		cout << "File name not ending with \".txt\", please provide a valid .txt file." << endl;
		return 1;
	}

	file[len-3] = 'd';
	file[len-2] = 'b';
	file[len-1] = 'x';

	return 0;
}


void parseAndDump(string parsefile)
{
	Spear_parser *parser;
	Dbox_dumper dumper;

	parser = new Spear_parser();

	cout << "Parsing file: " << parsefile << endl;

	if(!parser->parseFile(parsefile))
		return;

	int maxSamples = PARTIALS.getHopNum()*parser->getHopSize();

	cout << endl;
	cout << "Hopsize: "		<< parser->getHopSize()				<< endl;
	cout << "Sample rate: " << parser->getFileSampleRate()		<< endl;
	cout << "Partial num: " << parser->partials.getPartialNum()	<< endl;
	cout << "Hop num: " 	<< parser->partials.getHopNum()		<< endl;
	cout << "Tot Frames: " 	<< parser->partials.getHopNum()+1	<< endl;
	cout << "Last sample: " << maxSamples						<< endl;

	//---------------------------------------------------------------------------------------

	string dumpfile = string(parsefile);

	if(setDumpfileName(dumpfile)>0)
		return;

	cout << endl << "Dumping file: " << dumpfile << endl;

	dumper.dumpFile(dumpfile, parser);

	delete parser;
}



int lookForFiles(string exeName, char *directoryName)
{
	string s = string(directoryName);	// string is handy to find length
	int nameLen = exeName.length();
	int dirLen  = s.length();

	// directory, without name of exe...inelegant but effective
	directoryName[dirLen-nameLen-1] = '\0';

	DIR *dir;
	struct dirent *ent;
	int cnt = 0;

	// From http://stackoverflow.com/questions/612097/how-can-i-get-a-list-of-files-in-a-directory-using-c-or-c
	if ((dir = opendir (directoryName)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			// Ignore dotfiles and . and .. paths
			if(!strncmp(ent->d_name, ".", 1))
				continue;
			printf ("%s\n", ent->d_name);

			string file = string(ent->d_name);

			if(file.substr(file.find_last_of(".") + 1) == "txt")
			{
				cnt++;
				file = string(directoryName) + "/" + file;
				parseAndDump(file);
			}
		}
		closedir (dir);
		if(cnt==0)
			printf ("No .txt Spear files to parse in dir %s\n", directoryName);
		return 0;
	} else {
		/* could not open directory */
		printf("Could not open directory %s\n", directoryName);
		return -1;
	}
}



void printHelp()
{
	printf("Usage:\n");
	printf("Simply run the executable in a directory with .txt Spear file to convert them all in .dbx files\n");
	printf("or\n");
	printf("Run with argument -f [filename] to convert only the speficied .txt Spear file\n");
}


int main(int argc, char **argv)
{
	string parsefile;

	while (1)
	{
		int c;
		if ((c = getopt(argc, argv, "f:h")) < 0)
			break;
		switch (c)
		{
			case 'f':
				parsefile = string((char *)optarg);
				break;
			case 'h':
				printHelp();
				break;
			default:
				printf("Argument %c not valid /:\n", c);
				printHelp();
				break;
		}
	}

	// if no file has been passed...
	if(parsefile.empty())
	{
		//...check no other arguments have been passed
		if(argc == 1)
		{
			// get the name of the executable
			string exeName = getExeName(argv[0]);
			char *dir;
#ifndef _WIN32
			dir = argv[0];
#else
			dir = new char[500];
			_fullpath(dir, argv[0], 500);
#endif
			// look for files in dir and try to parse all .txt's
			if(lookForFiles(exeName, dir)<0)
			{
#ifdef _WIN32
				delete[] dir;
#endif
				return -1;
			}
#ifdef _WIN32
			delete[] dir;
#endif
		}
	}
	else	//FIXME this still does not work in windows
		parseAndDump(parsefile);	// ...otherwise simply parse the passed file

	cout << endl << "C Ya_" << endl;
	return 0;
}
