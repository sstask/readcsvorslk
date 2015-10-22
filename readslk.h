#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>

#define IsValidX(XX) (XX>0&&XX<=mxsize)
#define IsValidY(YY) (YY>0&&YY<=mysize)

class slkreader
{
public:
	slkreader():mbuffer(NULL),mitems(NULL),mxsize(0),mysize(0){}
	~slkreader(){if(mbuffer) free(mbuffer);if(mitems) free(mitems);}

	int getx(){return mxsize;}
	int gety(){return mysize;}
	const char* getitem(int x, int y){
		if(!IsValidX(x) || !IsValidY(y))
			return NULL;
		return mitems[y*mxsize+x] ;
	}

	template<typename T>
	std::string tostr(const T &t)
	{
		std::ostringstream sBuffer;
		sBuffer << t;
		return sBuffer.str();
	}

	template<typename T>
	void strto(char* p, T &t)
	{
		std::string intStr(p);
		std::istringstream sBuffer(intStr);
		sBuffer >> t;
	}

	bool readslk(char* pfile){
		FILE *fp = fopen(pfile, "rb");
		if (!fp)
			return false;
		int fsize = 0;
		fseek(fp,0,SEEK_END);
		fsize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		mbuffer = (unsigned char*)malloc(fsize + 3);
		if (!mbuffer){
			fclose(fp);
			return false;
		}
		memset(mbuffer, 0, fsize + 3);
		if(0 == fread(mbuffer, 1, fsize, fp))
			return false;
		fclose(fp);

		int p = 0;
		int x = 0;
		int y = 0;
		char* val = 0;
		while(p < fsize){
			if (mbuffer[p++] == '\n'){
				unsigned char bcf = mbuffer[p];
				while(p < fsize && mbuffer[p] != '\n'){
					if (mbuffer[p++] == ';'){
						unsigned char xyk = mbuffer[p++];
						if (xyk == 'X') x = 0;
						if (xyk == 'Y') y = 0;
						if (xyk == 'K') val = 0;
						while(p < fsize && mbuffer[p] != ';' && mbuffer[p] != '\n'){
							unsigned char c = mbuffer[p];
							if (xyk == 'X'){
								if (c >= '0' && c <= '9'){
										x = x * 10 + (c - '0');
								}
							}
							else if (xyk == 'Y'){
								if (c >= '0' && c <= '9'){
									y = y * 10 + (c - '0');
								}
							}
							else if (xyk == 'K'){
									val = (char*)mbuffer + p;
									if(val[0] == '"')
										val++;
									while(p < fsize && mbuffer[p] != '\r'){
										if(mbuffer[p] == ';'){
											if(mbuffer[p+1] == ';'){
												memmove(val+1, val, (char*)mbuffer+p-val);
												val++;
												p++;
											}
											else{
												mbuffer[p] = 0;
												if(mbuffer[p-1]=='"')
													mbuffer[p-1] = 0;
												while(p < fsize && mbuffer[p] != '\r') p++;
												p--;
											}
										}
										else if(mbuffer[p] == '\x1b' && mbuffer[p+1] == '\x20' && mbuffer[p+2] == '\x3a'){
											mbuffer[p] = '\n';
											memmove(val+2, val, (char*)mbuffer+p-val+1);
											val = val + 2;
											p = p + 2;
										}
										p++;
									}
									mbuffer[p] = 0;
									if(mbuffer[p-1]=='"')
										mbuffer[p-1] = 0;
							}
							p++;
						}
					}
				}

				if (bcf == 'B'){
					mxsize = x;
					mysize = y;
					if (mxsize == 0 || mysize == 0)
						return false;
					mitems = (char**)malloc((mxsize+1)*(mysize+1)*sizeof(char*));
					if (mitems == NULL){
						free(mbuffer);
						return false;
					}
					memset(mitems, 0, (mxsize+1)*(mysize+1)*sizeof(char*));
				}
				else if(bcf == 'C'){
					if(val){
						if(!IsValidX(x) || !IsValidY(y))
							return false;
						mitems[y*mxsize+x] = val;
						val = NULL;
					}
				}
				else{
					val = NULL;
				}
			}
		}
		return true;
	}

	void printslk()
	{
		for (int y = 1; y <= mysize; ++y)
		{
			printf("%d\n", y);
			for (int x = 1; x <= mxsize; ++x)
			{
				printf("%s ", mitems[y*mxsize+x]);
			}
			printf("\n");
		}
	}

private:
	unsigned char* mbuffer;
	char** mitems;
	int mxsize;
	int mysize;
};
