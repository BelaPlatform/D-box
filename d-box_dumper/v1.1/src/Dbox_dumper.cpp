/*
 * Dboxdumper.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: Victor Zappi
 */

#include "../headers/Dbox_dumper.h"

#define DATAPAR dataIn->partials

Dbox_dumper::Dbox_dumper()
{

}

Dbox_dumper::~Dbox_dumper()
{

}

int Dbox_dumper::dumpFile(string file, Spear_parser *dataIn)
{
	return dumper((char *)file.c_str(), dataIn);
}

int Dbox_dumper::dumpFile(char *file, Spear_parser *dataIn)
{
	return dumper(file, dataIn);
}

int Dbox_dumper::dumper(char *file, Spear_parser *dataIn)
{
	fstream fileOut;

	fileOut.open(file, ios::out | ios::binary);

	// general data
	int parNum	= DATAPAR.getPartialNum();
	int hopNum	= DATAPAR.getHopNum();
	int hopSize	= dataIn->getHopSize();
	int maxAct	= DATAPAR.getMaxActivePartialNum();
	fileOut.write((char *)&(parNum), sizeof(int));								// total number of partials
	fileOut.write((char *)&(hopNum), sizeof(int));								// total number of hops [jumps between frames]
	fileOut.write((char *)&(hopSize), sizeof(int));								// hop size
	fileOut.write((char *)&(maxAct), sizeof(int));								// hop size

	// partial data
	fileOut.write((char *)DATAPAR.partialStartFrame, sizeof(int)*parNum);		// start frame of each partial
	fileOut.write((char *)DATAPAR.partialNumFrames, sizeof(int)*parNum);		// num of frames of each partial
	fileOut.write((char *)DATAPAR.partialFreqMean, sizeof(float)*parNum);		// frequency mean of each partial


	for(int par=0; par<parNum; par++)
	{
		fileOut.write((char *)DATAPAR.partialAmplitudes[par], sizeof(float)*DATAPAR.partialNumFrames[par]);			// amplitude of each partial in each frame
		fileOut.write((char *)DATAPAR.partialFrequencies[par], sizeof(float)*DATAPAR.partialNumFrames[par]);		// frequency of each partial in each frame
		fileOut.write((char *)DATAPAR.partialAmpDelta[par], sizeof(float)*DATAPAR.partialNumFrames[par]);			// amplitude delta of each partial in each frame
		fileOut.write((char *)DATAPAR.partialFreqDelta[par], sizeof(float)*DATAPAR.partialNumFrames[par]);			// frequency delta of each partial in each frame
	}

	// frame data
	fileOut.write((char *)DATAPAR.activePartialNum, sizeof(short)*(hopNum+1));	// number of active partial per each frame

	for(int frame=0; frame<hopNum+1; frame++)
		fileOut.write((char *)DATAPAR.activePartials[frame], sizeof(int)*DATAPAR.activePartialNum[frame]);			// active partials per each frame

	fileOut.close();

	return 0;
}

