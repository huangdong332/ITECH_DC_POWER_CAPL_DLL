#include "minilogger.h"


const char * logFileName;



void FileLoggerInit(const char *fileName)
{
    logFileName=fileName;
}

void Logger(const char* tag, const char* message,...) {
   time_t now;
   time(&now);
   va_list args;
   va_start(args,message);
   printf("%.24s [%s]: ", ctime(&now), tag);
   vprintf(message,args);
   printf("\n");
   va_end(args);
}

void FileLogger(const char* tag, const char* message,...) {
   time_t now;
   time(&now);
   va_list args;
   va_start(args,message);
   FILE * pFile=fopen(logFileName,"a");
   fprintf(pFile,"%.24s [%s]: ", ctime(&now), tag);
   vfprintf(pFile,message,args);
   fprintf(pFile,"\n");
   fclose(pFile);
   va_end(args);
}
