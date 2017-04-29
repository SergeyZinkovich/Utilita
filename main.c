#include <stdio.h>
#include <io.h>
#include <windows.h>

char* CombineCharArrays(char *destination, char *source){
    char *destFile = malloc(strlen(destination) + strlen(source) + 1 );
    memcpy(destFile, destination, sizeof(char) * (strlen(destination)+1));
    strcat(destFile, source);
    return destFile;
    free(destFile);
}

const char* const cutSlash(char* s)
{
   static char buf[BUFSIZ];
   memset(buf, '\0', BUFSIZ);
   if (s[strlen(s)-1] = "\\"){
     return strncpy(buf, s , strlen(s) - 1);
   }
   else{
     return s;
   }
}

char* AddStar(char* pt){
    if (pt[strlen(pt)-1] != "*"){
            return CombineCharArrays(pt, "*");
    }
    else{
        return pt;
    }
}

char* AddSlesh(char* pt){
    if (pt[strlen(pt)-1] != "\\"){
            return CombineCharArrays(pt, "\\");
    }
    else{
        return pt;
    }
}

void copyF(char aFrom[], char aTo[]){
int  n;
char buf [512];
freopen(aFrom,"rb", stdin);
freopen(aTo,"wb",stdout);
while ((n = read(fileno(stdin), buf, sizeof buf)) > 0)
  {
    write(fileno(stdout), buf, n);
  }
}

void ShowFiles(char aFrom[], char aTo[])
{
		HANDLE hSearch;
		WIN32_FIND_DATA pFileData;

		hSearch = FindFirstFile(AddStar(aFrom), &pFileData);
		if (hSearch != INVALID_HANDLE_VALUE)
			do
			{
				if ((pFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(pFileData.cFileName,"..") != 0) && (strcmp(pFileData.cFileName,".") != 0)){
                    CreateDirectory(CombineCharArrays(aTo, pFileData.cFileName), NULL);
                    ShowFiles(AddSlesh(CombineCharArrays(aFrom, pFileData.cFileName)),
                               AddSlesh(CombineCharArrays(aTo, pFileData.cFileName)));
				}
                else if ((strcmp(pFileData.cFileName,"..") != 0) && (strcmp(pFileData.cFileName,".") != 0)){
                    copyF(CombineCharArrays(aFrom, pFileData.cFileName),CombineCharArrays(aTo, pFileData.cFileName));
                }
			}
			while (FindNextFile(hSearch, &pFileData));
			FindClose(hSearch);
}

int main()
{
    char *from;
    char *to;
    from = malloc(10000);
    to = malloc(10000);
    printf("Write what to copy\n");
    scanf("%s", from);
    from = cutSlash(from);
    printf("Write where to copy\n");
    scanf("%s", to);
    to = cutSlash(to);
    HANDLE hSearch;
    WIN32_FIND_DATA pFileData;
    hSearch = FindFirstFile(from, &pFileData);
    if (!((pFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(pFileData.cFileName,"..") != 0) && (strcmp(pFileData.cFileName,".") != 0))){
      hSearch = FindFirstFile(to, &pFileData);
      if (!((pFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(pFileData.cFileName,"..") != 0) && (strcmp(pFileData.cFileName,".") != 0))){
        copyF(from, to);
      }
      else{
        hSearch = FindFirstFile(from, &pFileData);
        copyF(from, CombineCharArrays(AddSlesh(to), pFileData.cFileName));
      }
    }
    else{
      ShowFiles(AddSlesh(from), AddSlesh(to));
    }
    return 0;
}
