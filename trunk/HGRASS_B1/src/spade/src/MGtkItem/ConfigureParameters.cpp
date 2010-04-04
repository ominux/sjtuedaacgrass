#include "ConfigureParameters.hpp"

ConfigureParameters confParas;
void ConfigureParameters::writeToFile(FILE* fp)
{
    if(confPath)
      fprintf(fp,"ConfPath = %s\n",confPath);
    else
      fprintf(fp,"ConfPath = NULL\n");
    if(cirPath)
      fprintf(fp,"CirPath = %s\n",cirPath);
    else
      fprintf(fp,"CirPath = NULL\n");
    if(libPath)
      fprintf(fp,"LibPath = %s\n",libPath);
    else
      fprintf(fp,"LibPath = NULL\n");
    if(nModels > 0)
    {
      fprintf(fp,"NModels = %d\n",nModels);
      fprintf(fp,"Models = %s",models[0]);
      for(int i=1;i<nModels;i++)
	fprintf(fp,",%s",models[i]);
      fprintf(fp,"\n");
      fprintf(fp,"ModelIndex = %d\n",modelIndex);
    }
    else
      fprintf(fp,"NModels = 0\n");
    if(nViews > 0)
    {
      fprintf(fp,"NViews = %d\n",nViews);
      fprintf(fp,"Views = %s",views[0]);
      for(int i=1;i<nViews;i++)
	fprintf(fp,",%s",views[i]);
      fprintf(fp,"\n");
      fprintf(fp,"ViewIndex = %d\n",viewIndex);
    }
    else
      fprintf(fp,"NViews = 0\n");
    fprintf(fp,"FreqFrom = %g\n",freqFrom);
    fprintf(fp,"FreqTo = %g\n",freqTo);
    fprintf(fp,"FreqN = %d\n",freqN);
    if(p1Name)
      fprintf(fp,"P1Name = %s\n",p1Name);
    else
      fprintf(fp,"P1Name = NULL\n");
    if(p1Alias)
      fprintf(fp,"P1Alias = %s\n",p1Alias);
    else
      fprintf(fp,"P1Alias = NULL\n");
    fprintf(fp,"P1ValFrom = %g\n",p1ValFrom);
    fprintf(fp,"P1ValTo = %g\n",p1ValTo);
    fprintf(fp,"P1ValN = %d\n",p1ValN);
    fprintf(fp,"P1ValSigma = %g\n",p1ValSigma);
    if(p2Name)
      fprintf(fp,"P2Name = %s\n",p2Name);
    else
      fprintf(fp,"P2Name = NULL\n");
    if(p2Alias)
      fprintf(fp,"P2Alias = %s\n",p2Alias);
    else
      fprintf(fp,"P2Alias = NULL\n");
    fprintf(fp,"P2ValFrom = %g\n",p2ValFrom);
    fprintf(fp,"P2ValTo = %g\n",p2ValTo);
    fprintf(fp,"P2ValN = %d\n",p2ValN);
    fprintf(fp,"P2ValSigma = %g\n",p2ValSigma);
    fprintf(fp,"Correlation = %g\n",correlation);
}

void ConfigureParameters::readFromFile(FILE* fp)
{
    char* key = new char [MAX_CHARS];
    char* value = new char [MAX_CHARS];
    while(!feof(fp))
    {
      fscanf(fp,"%s = %s\n",key,value);
      //fscanf(fp,"%[^=]%*c=%s\n",key,value);
      if(value == NULL || key == NULL)
	continue;
      if(!strcmp(key,"ConfPath"))
      {
	if(confPath)
	  delete [] confPath;
	if(!strcmp(value,"NULL") || !strcmp(value,"(NONE)") || !strcmp(value,""))
	  confPath = NULL;
	else
	  confPath = strdup(value);
      }
      else if(!strcmp(key,"CirPath"))
      {
	if(cirPath)
	  delete [] cirPath;
	if(!strcmp(value,"NULL") || !strcmp(value,"(NONE)") || !strcmp(value,""))
	  cirPath = NULL;
	else
	  cirPath = strdup(value);
      }
      else if(!strcmp(key,"LibPath"))
      {
	if(libPath)
	  delete [] libPath;
	if(!strcmp(value,"NULL") || !strcmp(value,"(NONE)") || !strcmp(value,""))
	  libPath = NULL;
	else
	  libPath = strdup(value);
      }
      else if(!strcmp(key,"NModels"))
      {
	int n = atoi(value);
	if(n == 0)
	{
	  if(models)
	  {
	    for(int i=0;i<nModels;i++)
	      delete [] models[i];
	    delete [] models;
	    models = NULL;
	  }
	  modelIndex = 0;
	}
	nModels = n;
      }
      else if(!strcmp(key,"Models"))
      {
	models = new char* [nModels];
	char* tmpValue = value;
	for(int i=0;i<nModels;)
	{
	  char* tmp = strsep(&tmpValue," ,");
	  if(strcmp(tmp,""))
	  {
	    models[i] = strdup(tmp);
	    i++;
	  }
	}
      }
      else if(!strcmp(key,"ModelIndex"))
      {
	modelIndex = atoi(value);
      }
      else if(!strcmp(key,"NViews"))
      {
	int n = atoi(value);
	if(n == 0)
	{
	  if(views)
	  {
	    for(int i=0;i<nViews;i++)
	      delete [] views[i];
	    delete [] views;
	    views = NULL;
	  }
	  viewIndex = 0;
	}
	nViews = n;
      }
      else if(!strcmp(key,"Views"))
      {
	views = new char* [nViews];
	char* tmpValue = value;
	for(int i=0;i<nViews;)
	{
	  char* tmp = strsep(&tmpValue," ,");
	  if(strcmp(tmp,""))
	  {
	    views[i] = strdup(tmp);
	    i++;
	  }
	}
      }
      else if(!strcmp(key,"ViewIndex"))
      {
	viewIndex = atoi(value);
      }
      else if(!strcmp(key,"FreqFrom"))
      {
	freqFrom = atof(value);
      }
      else if(!strcmp(key,"FreqTo"))
      {
	freqTo = atof(value);
      }
      else if(!strcmp(key,"FreqN"))
      {
	freqN = atoi(value);
      }
      else if(!strcmp(key,"P1Name"))
      {
	if(p1Name)
	  delete [] p1Name;
	if(!strcmp(value,"NULL"))
	  p1Name = NULL;
	else
  	  p1Name = strdup(value);
      }
      else if(!strcmp(key,"P1Alias"))
      {
	if(p1Alias)
	  delete [] p1Alias;
	if(!strcmp(value,"NULL"))
	  p1Alias = NULL;
	else
	  p1Alias = strdup(value);
      }
      else if(!strcmp(key,"P1ValFrom"))
      {
	p1ValFrom = atof(value);
      }
      else if(!strcmp(key,"P1ValTo"))
      {
	p1ValTo = atof(value);
      }
      else if(!strcmp(key,"P1ValN"))
      {
	p1ValN = atoi(value);
      }
      else if(!strcmp(key,"P1ValSigma"))
      {
	p1ValSigma = atof(value);
      }
      else if(!strcmp(key,"P2Name"))
      {
	if(p2Name)
	  delete [] p2Name;
	if(!strcmp(value,"NULL"))
	  p2Name = NULL;
	else
	  p2Name = strdup(value);
      }
      else if(!strcmp(key,"P2Alias"))
      {
	if(p2Alias)
	  delete [] p2Alias;
	if(!strcmp(value,"NULL"))
	  p2Alias = NULL;
	else
	  p2Alias = strdup(value);
      }
      else if(!strcmp(key,"P2ValFrom"))
      {
	p2ValFrom = atof(value);
      }
      else if(!strcmp(key,"P2ValTo"))
      {
	p2ValTo = atof(value);
      }
      else if(!strcmp(key,"P2ValN"))
      {
	p2ValN = atoi(value);
      }
      else if(!strcmp(key,"P2ValSigma"))
      {
	p2ValSigma = atof(value);
      }
      else if(!strcmp(key,"Correlation"))
      {
	correlation = atof(value);
      }
    }
    delete [] key;
    delete [] value;
}
