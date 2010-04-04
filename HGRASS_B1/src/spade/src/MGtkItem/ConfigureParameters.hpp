#ifndef CONFIGURE_PARAMETERS_H
#define CONFIGURE_PARAMETERS_H

extern "C"
{
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <assert.h>
}

#include "../defs.h"

class ConfigureParameters
{
public:
        char* confPath;
        char* cirPath;
        char* libPath;
        int nModels;
        char** models;
        int modelIndex;
        int nViews;
        char** views;
        int viewIndex;
        double freqFrom;
        double freqTo;
        int freqN;
        char* p1Name;
        char* p1Alias;
        double p1ValFrom;
        double p1ValTo;
        int p1ValN;
        double p1ValSigma;
        char* p2Name;
        char* p2Alias;
        double p2ValFrom;
        double p2ValTo;
        int p2ValN;
        double p2ValSigma;
        double correlation;

        ConfigureParameters()
        {
          confPath = NULL;
          cirPath = NULL;
          libPath = NULL;
          nModels = 0;
          models = NULL;
          modelIndex = 0;
          nViews = 0;
          views = NULL;
          viewIndex = 0;
          freqFrom = 0;
          freqTo = 0;
          freqN = 0;
          p1Name = NULL;
          p1Alias = NULL;
          p1ValFrom = 0;
          p1ValTo = 0;
          p1ValN = 0;
          p1ValSigma = 0;
          p2Name = NULL;
          p2Alias = NULL;
          p2ValFrom = 0;
          p2ValTo = 0;
          p2ValN = 0;
          p2ValSigma = 0;
          correlation = 0;
        }

        ~ConfigureParameters()
        {
          if(confPath)
            delete [] confPath;
          if(cirPath)
            delete [] cirPath;
          if(libPath)
            delete [] libPath;
          if(models)
          {
            for(int i=0;i<nModels;i++)
              delete [] models[i];
            delete [] models;
          }
          if(views)
          {
            for(int i=0;i<nViews;i++)
              delete [] views[i];
            delete [] views;
          }
          if(p1Name)
            delete [] p1Name;
          if(p1Alias)
            delete [] p1Alias;
          if(p2Name)
            delete [] p2Name;
          if(p2Alias)
            delete [] p2Alias;
        }

        void writeToFile(FILE* fp);
        void readFromFile(FILE* fp);
};

#endif
