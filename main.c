#include <stdio.h>
#include <io.h>
#include <windows.h>

char* CombineCharArrays(char *destination, char *source){
    destination = malloc(strlen(destination) + strlen(source) + 1 );
    strcat(destination, source);
    return destination;
}

const char* const cutSlash(char* s)
{
    static char buf[BUFSIZ];
    memset(buf, '\0', BUFSIZ);
    if (s[strlen(s)-1] == "\\"){
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

char* AddSlash(char* pt){
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
    FILE *input;
    FILE *output;
    input = fopen(aFrom, "rb");
    output = fopen(aTo, "wb");
    while (fgets (buf, 512, input) != NULL){
        fprintf(output ,"%s", buf);
    }
    /*while ((n = read(fileno(stdin), buf, sizeof buf)) > 0){
        write(fileno(stdout), buf, n);
    }*/
}

void ShowFiles(char aFrom[], char aTo[])
{
    HANDLE hSearch;
    WIN32_FIND_DATA pFileData;
    hSearch = FindFirstFile(AddStar(aFrom), &pFileData);
    if (hSearch != INVALID_HANDLE_VALUE)
    do{
        if ((pFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(pFileData.cFileName,"..") != 0) && (strcmp(pFileData.cFileName,".") != 0)){
            CreateDirectory(CombineCharArrays(aTo, pFileData.cFileName), NULL);
            ShowFiles(AddSlash(CombineCharArrays(aFrom, pFileData.cFileName)),
                AddSlash(CombineCharArrays(aTo, pFileData.cFileName)));
        }
        else if ((strcmp(pFileData.cFileName,"..") != 0) && (strcmp(pFileData.cFileName,".") != 0)){
                printf("\n");
            copyF(CombineCharArrays(aFrom, pFileData.cFileName),CombineCharArrays(aTo, pFileData.cFileName));
        }
    }while (FindNextFile(hSearch, &pFileData));
    FindClose(hSearch);
}

int main()
{
    char *from;
    char *to;
    from = malloc(10000);
    to = malloc(10000);
    printf("Write what to copy\n");
    gets(from);
    from = cutSlash(from);
    printf("Write where to copy\n");
    gets(to);
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
            copyF(from, CombineCharArrays(AddSlash(to), pFileData.cFileName));
        }
    }
    else{
        ShowFiles(AddSlash(from), AddSlash(to));
    }
    return 0;
}
