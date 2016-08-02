/*
 * Dbox_dumper.h
 *
 *  Created on: Jul 17, 2014
 *      Author: Victor Zappi
 */

#ifndef D_BOX_DUMPER_H_
#define D_BOX_DUMPER_H_

#include <iostream>
#include <fstream>

#include "../headers/spear_parser.h"


class Dbox_dumper
{

public:
	Dbox_dumper();
	~Dbox_dumper();

	int dumpFile(string file, Spear_parser *dataIn);
	int dumpFile(char *file, Spear_parser *dataIn);

private:
	int dumper(char *file, Spear_parser *dataIn);
};



#endif /* D_BOX_DUMPER_H_ */
