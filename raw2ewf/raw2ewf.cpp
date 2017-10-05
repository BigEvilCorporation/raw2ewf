#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 2)
	{
		std::wstring sourceFilename = argv[1];
		std::wstring destFilename = argv[2];

		std::ifstream inFile;
		inFile.open(sourceFilename, std::ios::in | std::ios::binary | std::ios::ate);
		if(inFile.is_open())
		{
			size_t fileSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);
			unsigned char* inData = new unsigned char[fileSize + 1];
			inFile.read((char*)inData, fileSize);
			inFile.close();

			for(int i = 0; i < fileSize; i++)
			{
				//Nudge 0xFF bytes to 0xFE
				if(inData[i] == 0xFF)
				{
					inData[i] = 0xFE;
				}
			}

			//End of data
			inData[fileSize] = 0xFF;

			std::ofstream outFile;
			outFile.open(destFilename, std::ios::out | std::ios::binary);
			if(outFile.is_open())
			{
				outFile.write((const char*)inData, fileSize + 1);
				outFile.close();
			}

			//Output delay size
			const int sampleRate = 10653;
			const float lengthSeconds = (float)fileSize / (float)sampleRate;
			const int ticksPerSecond = 50;
			const int lengthTicks = ceil((float)lengthSeconds * (float)ticksPerSecond) + 1;
			std::cout << "Converted: " << sourceFilename.c_str() << " Length: " << fileSize << " samples, " << lengthSeconds << " seconds, " << lengthTicks << " ticks" << std::endl;
		}
	}

	return 0;
}
