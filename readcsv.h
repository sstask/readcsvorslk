#ifndef _H_READCSV_H_
#define _H_READCSV_H_

#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

string loadcsvfile(const char* filename)
{
    ifstream csvfile(filename);
    if (!csvfile)
    {
        return string("");
    }
    csvfile.seekg(0, ios::end);
    long m = csvfile.tellg();
    csvfile.seekg(0, ios::beg);
    char * pbuf = (char*)malloc(m + 1);
    memset(pbuf, 0, m + 1);
    csvfile.read(pbuf, m);
    csvfile.close();
    string con = pbuf;
    free(pbuf);
    return con;
}

#define CHAR_ESCAPE ('"')
#define CHAR_PARSER (',')
#define IS_LINE_CHAR(CharL) (CharL == '\r' || CharL == '\n') //on window '\r\n'; on linux '\n'; on mac '\r'

enum EM_CSVResult
{
	ECR_Success,
	ECR_Empty,
	ECR_NoMemory,
	ECR_NoMatchEscapeC,
};

EM_CSVResult ReadCSV(const string&	strCSVContent/*input*/, vector<vector<string> >& vecCSVParsed/*output*/)
{
	if (strCSVContent.empty())
	{
		return ECR_Empty;
	}
	const size_t uiContentLen = strCSVContent.length();				//csv content length
	size_t uiContentPos = 0;									//csv content pointer index
	size_t uiParsedPos = 0;										//parsed string index
	vector<string> vecParsedStr;								//csv line
	char* ptempParsed = (char*)malloc(uiContentLen + 1);			//csv gird content
	if (NULL == ptempParsed)
	{
		return ECR_NoMemory;
	}
	memset(ptempParsed, 0, uiContentLen + 1);

	while (uiContentPos < uiContentLen)
	{
		if (strCSVContent[uiContentPos] == CHAR_ESCAPE)					//if the grid content begin with '"'.it should have a matched '"'
		{
			bool bNextEscapeChar = false;
			++uiContentPos;
			while (uiContentPos < uiContentLen)
			{
				if (strCSVContent[uiContentPos] == CHAR_ESCAPE)
				{
					if (uiContentPos + 1 >= uiContentLen || strCSVContent[uiContentPos + 1] != CHAR_ESCAPE)
					{
						bNextEscapeChar = true;
						++uiContentPos;
						break;
					}
					ptempParsed[uiParsedPos] = CHAR_ESCAPE;
					++uiParsedPos;
					uiContentPos += 2;
				}
				else
				{
					ptempParsed[uiParsedPos] = strCSVContent[uiContentPos];
					++uiParsedPos;
					++uiContentPos;
				}
			}
			if (uiContentPos + 1 >= uiContentLen)					//over
			{
				ptempParsed[uiParsedPos] = '\0';
				vecParsedStr.push_back(ptempParsed);
				vecCSVParsed.push_back(vecParsedStr);
				free(ptempParsed);
				if (bNextEscapeChar)
				{
					return ECR_Success;
				}
				else
				{
					return ECR_NoMatchEscapeC;
				}
			}
		}
		while (uiContentPos < uiContentLen)
		{
			if (strCSVContent[uiContentPos] == CHAR_PARSER)						//char ',', next grid
			{
				ptempParsed[uiParsedPos] = '\0';
				vecParsedStr.push_back(ptempParsed);
				uiParsedPos = 0;
				++uiContentPos;
				break;
			}
			else if (IS_LINE_CHAR(strCSVContent[uiContentPos]))					//next csv line, next vector
			{
				ptempParsed[uiParsedPos] = '\0';
				vecParsedStr.push_back(ptempParsed);
				vecCSVParsed.push_back(vecParsedStr);
				vecParsedStr.clear();
				uiParsedPos = 0;
				++uiContentPos;
				if (uiContentPos < uiContentLen && strCSVContent[uiContentPos - 1] == '\r' && strCSVContent[uiContentPos] == '\n')
					//on the platform of windows the characters of next line are '\r\n'
				{
					++uiContentPos;
				}
				break;
			}
			else
			{
				ptempParsed[uiParsedPos] = strCSVContent[uiContentPos];
				++uiParsedPos;
				++uiContentPos;
			}
		}
	}

	ptempParsed[uiParsedPos] = '\0';
	vecParsedStr.push_back(ptempParsed);
	vecCSVParsed.push_back(vecParsedStr);
	free(ptempParsed);
	return ECR_Success;
}

#endif // !_H_READCSV_H_
