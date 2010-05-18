#include "Mosfet.h"
#include <math.h>


/*
static double GAMMAn	=	0.4;
static double GAMMAp	=	-0.4;
static double VT0n	=	0.43;
static double VT0p	=	-0.4;
static double LAMBDAn	=	0.06;
static double LAMBDAp	=	-0.1;
static double KPn	=	115e-6;
static double KPp	=	-30e-6;
static double COXn	=	6e-3;
static double COXp	=	6e-3;
static double COn	=	0.31e-9;
static double COp	=	0.27e-9;
static double FAIBn	=	0.9;
static double FAIBp	=	0.9;
static double FAIBSWn	=	0.9;
static double FAIBSWp	=	0.9;
static double CJn	=	2e-3;
static double CJp	=	0.9e-3;
static double CJSWn	=	0.28e-9;
static double CJSWp	=	0.22e-9;
static double MJn	=	0.5;
static double MJp	=	0.48;
static double MJSWn	=	0.44;
static double MJSWp	=	0.32;
static double VOVn	=	0.152;
static double VOVp	=	-0.185;
*/

double MosfetModelBsim3V3::GAMMAn  =       0;
double MosfetModelBsim3V3::GAMMAp  =       0;
double MosfetModelBsim3V3::VT0n    =       0;
double MosfetModelBsim3V3::VT0p    =       0;
double MosfetModelBsim3V3::LAMBDAn =       0;
double MosfetModelBsim3V3::LAMBDAp =       0;
double MosfetModelBsim3V3::KPn     =       0;
double MosfetModelBsim3V3::KPp     =       0;
double MosfetModelBsim3V3::COXn    =       0;
double MosfetModelBsim3V3::COXp    =       0;
double MosfetModelBsim3V3::COn     =       0;
double MosfetModelBsim3V3::COp     =       0;
double MosfetModelBsim3V3::FAIBn   =       0;
double MosfetModelBsim3V3::FAIBp   =       0;
double MosfetModelBsim3V3::FAIBSWn =       0;
double MosfetModelBsim3V3::FAIBSWp =       0;
double MosfetModelBsim3V3::CJn     =       0;
double MosfetModelBsim3V3::CJp     =       0;
double MosfetModelBsim3V3::CJSWn   =       0;
double MosfetModelBsim3V3::CJSWp   =       0;
double MosfetModelBsim3V3::MJn     =       0;
double MosfetModelBsim3V3::MJp     =       0;
double MosfetModelBsim3V3::MJSWn   =       0;
double MosfetModelBsim3V3::MJSWp   =       0;
double MosfetModelBsim3V3::VOVn    =       0;
double MosfetModelBsim3V3::VOVp    =       0;
//double MosfetModelBsim3V3::KPVOVn  =       0;      //KPn * VOVn
//double MosfetModelBsim3V3::KPVOVp  =       0;      //KPp * VOVp
//double MosfetModelBsim3V3::LAMVOVn =       0;      //2 / (LAMBDAn * VOVn)
//double MosfetModelBsim3V3::LAMVOVp =       0;      //2 / (LAMBDAp * VOVp)
double MosfetModelBsim3V3::CAPBSDKn=	   0;      //CJn * LSn + CJSWn
double MosfetModelBsim3V3::CAPBSDKp=	   0;      //CJp * LSp + CJSWp

double MosfetModel::updateValueOfConst(double val)	{return val;}

void MosfetModelL1::updateValues(double val, double w, double l)
{
  updateValueOfGm(val,w,l);
  updateValueOfRds(val,w,l);
}

void MosfetModelL1::setModelDevicePtr(SymbNode* ptr)
{
  if(!ptr) return;
  char* deviceName = ptr->name;
  #ifdef DEBUG
  printf("[Debug] Name OF Device is %s\n",deviceName);
  #endif
  if(!strncmp(deviceName,"GM",2))
    gm = ptr;
  else if(!strncmp(deviceName,"RDS",3))
    rds = ptr;
}

void MosfetModelL1::calBasicModelParameters(MosfetModelL1* pModel, double val, double w, double l){}
map<char*,double>* MosfetModelL1::getDeviceMap(Mosfet* pMosfet){return NULL;}

void MosfetModelL1::updateValueOfGm(double val, double w, double l)
{
  if(!gm)
    return;
}

void MosfetModelL1::updateValueOfRds(double val, double w, double l)
{
  if(!rds)
    return;
}

void MosfetModelL1::calLAMBDA(MosfetModelL1* pModel, double val, double w, double l){}

void MosfetModelL1::calVOV(MosfetModelL1* pModel, double val, double w, double l){}

void MosfetModelL1::calKP(MosfetModelL1* pModel, double val, double w, double l){}

void MosfetModelL2::updateValues(double val, double w, double l)
{
  updateValueOfGm(val,w,l);
  updateValueOfRds(val,w,l);
  updateValueOfGmb(val,w,l);
}

void MosfetModelL2::setModelDevicePtr(SymbNode* ptr)
{
  if(!ptr) return;
  char* deviceName = ptr->name;
  #ifdef DEBUG
  printf("[Debug] Name OF Device is %s\n",deviceName);
  #endif
  if(!strncmp(deviceName,"GM",2))
    gm = ptr;
  else if(!strncmp(deviceName,"RDS",3))
    rds = ptr;
  else if(!strncmp(deviceName,"GMB",3))
    gmb = ptr;
}

void MosfetModelL2::calBasicModelParameters(MosfetModelL2* pModel, double val, double w, double l){}
map<char*,double>* MosfetModelL2::getDeviceMap(Mosfet* pMosfet){return NULL;}

void MosfetModelL2::updateValueOfGm(double val, double w, double l)
{
  if(!gm)
    return;
}

void MosfetModelL2::updateValueOfRds(double val, double w, double l)
{
  if(!rds)
    return;
}

void MosfetModelL2::updateValueOfGmb(double val, double w, double l)
{
  if(!gmb)
    return;
}

void MosfetModelL2::calGAMMA(MosfetModelL2* pModel, double val, double w, double l){}

void MosfetModelL2::calLAMBDA(MosfetModelL2* pModel, double val, double w, double l){}

void MosfetModelL2::calVOV(MosfetModelL2* pModel, double val, double w, double l){}

void MosfetModelL2::calKP(MosfetModelL2* pModel, double val, double w, double l){}

void MosfetModelL2::calFAIB(MosfetModelL2* pModel, double val, double w, double l){}

void MosfetModelL3::updateValues(double val, double w, double l)
{
  updateValueOfGm(val,w,l);
  updateValueOfRds(val,w,l);
  updateValueOfGmb(val,w,l);
  updateValueOfCgd(val,w,l);
  updateValueOfCgs(val,w,l);
  updateValueOfCdb(val,w,l);
  updateValueOfCsb(val,w,l);
}

void MosfetModelL3::setModelDevicePtr(SymbNode* ptr)
{
  if(!ptr) return;
  char* deviceName = ptr->name;
  #ifdef DEBUG
  printf("[Debug] Name OF Device is %s\n",deviceName);
  #endif
  if(!strncmp(deviceName,"GM",2))
    gm = ptr;
  else if(!strncmp(deviceName,"RDS",3))
    rds = ptr;
  else if(!strncmp(deviceName,"GMB",3))
    gmb = ptr;
  else if(!strncmp(deviceName,"CGD",3))
    cgd = ptr;
  else if(!strncmp(deviceName,"CGS",3))
    cgs = ptr;
  //else if(!strncmp(deviceName,"CDB",3))
  else if(!strncmp(deviceName,"CAPBD",5))
    cdb = ptr;
  //else if(!strncmp(deviceName,"CSB",3))
  else if(!strncmp(deviceName,"CAPBS",5))
    csb = ptr;
}

void MosfetModelL3::calBasicModelParameters(MosfetModelL3* pModel, double val, double w, double l){}
map<char*,double>* MosfetModelL3::getDeviceMap(Mosfet* pMosfet){return NULL;}

void MosfetModelL3::updateValueOfGm(double val, double w, double l)
{
  if(!gm)
    return;
}

void MosfetModelL3::updateValueOfRds(double val, double w, double l)
{
  if(!rds)
    return;
}

void MosfetModelL3::updateValueOfGmb(double val, double w, double l)
{
  if(!gmb)
    return;
}

void MosfetModelL3::updateValueOfCgd(double val, double w, double l)
{
  if(!cgd)
    return;
}

void MosfetModelL3::updateValueOfCgs(double val, double w, double l)
{
  if(!cgs)
    return;
}

void MosfetModelL3::updateValueOfCdb(double val, double w, double l)
{
  if(!cdb)
    return;
}

void MosfetModelL3::updateValueOfCsb(double val, double w, double l)
{
  if(!csb)
    return;
}

void MosfetModelL3::calGAMMA(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calLAMBDA(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calVOV(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calKP(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calFAIB(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calCJ(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calCJSW(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL3::calLS(MosfetModelL3* pModel, double val, double w, double l){}

void MosfetModelL4::updateValues(double val, double w, double l)
{
  updateValueOfGm(val,w,l);
  updateValueOfRds(val,w,l);
  updateValueOfGmb(val,w,l);
  updateValueOfCgd(val,w,l);
  updateValueOfCgs(val,w,l);
  updateValueOfCdb(val,w,l);
  updateValueOfCsb(val,w,l);
  updateValueOfCgb(val,w,l);
}

void MosfetModelL4::setModelDevicePtr(SymbNode* ptr)
{
  if(!ptr) return;
  char* deviceName = ptr->name;
  #ifdef DEBUG
  printf("[Debug] Name OF Device is %s\n",deviceName);
  #endif
  if(!strncmp(deviceName,"GM",2))
    gm = ptr;
  else if(!strncmp(deviceName,"RDS",3))
    rds = ptr;
  else if(!strncmp(deviceName,"GMB",3))
    gmb = ptr;
  else if(!strncmp(deviceName,"CGD",3))
    cgd = ptr;
  else if(!strncmp(deviceName,"CGS",3))
    cgs = ptr;
  //else if(!strncmp(deviceName,"CDB",3))
  else if(!strncmp(deviceName,"CAPBD",5))
    cdb = ptr;
  //else if(!strncmp(deviceName,"CSB",3))
  else if(!strncmp(deviceName,"CAPBS",5))
    csb = ptr;
  else if(!strncmp(deviceName,"CGB",3))
    cgb = ptr;
}

void MosfetModelL4::calBasicModelParameters(MosfetModelL4* pModel, double val, double w, double l){}
map<char*,double>* MosfetModelL4::getDeviceMap(Mosfet* pMosfet){return NULL;}

void MosfetModelL4::updateValueOfGm(double val, double w, double l)
{
  if(!gm)
    return;
}

void MosfetModelL4::updateValueOfRds(double val, double w, double l)
{
  if(!rds)
    return;
}

void MosfetModelL4::updateValueOfGmb(double val, double w, double l)
{
  if(!gmb)
    return;
}

void MosfetModelL4::updateValueOfCgd(double val, double w, double l)
{
  if(!cgd)
    return;
}

void MosfetModelL4::updateValueOfCgs(double val, double w, double l)
{
  if(!cgs)
    return;
}

void MosfetModelL4::updateValueOfCdb(double val, double w, double l)
{
  if(!cdb)
    return;
}

void MosfetModelL4::updateValueOfCsb(double val, double w, double l)
{
  if(!csb)
    return;
}

void MosfetModelL4::updateValueOfCgb(double val, double w, double l)
{
  if(!cgb)
    return;
}

void MosfetModelL4::calGAMMA(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calLAMBDA(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calVOV(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calKP(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calFAIB(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calCJ(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calCJSW(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelL4::calLS(MosfetModelL4* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::updateValues(double val, double w, double l)
{
  updateValueOfGm(val,w,l);
  updateValueOfRds(val,w,l);
  updateValueOfGmb(val,w,l);
  updateValueOfCgd(val,w,l);
  updateValueOfCgs(val,w,l);
  updateValueOfCdb(val,w,l);
  updateValueOfCsb(val,w,l);
  updateValueOfCgb(val,w,l);
  updateValueOfRd(val,w,l);
  updateValueOfRs(val,w,l);
  updateValueOfRbd(val,w,l);
  updateValueOfRbs(val,w,l);
}

void MosfetModelBsim3V3::setModelDevicePtr(SymbNode* ptr)
{
  if(!ptr) return;
  char* deviceName = ptr->name;
  #ifdef DEBUG
  printf("[Debug] Name OF Device Inside Mosfet %s is %s\n",ptr->edge->pMosfet->name,deviceName);
  #endif
  if(!strncmp(deviceName,"GM",2))
    gm = ptr;
  else if(!strncmp(deviceName,"RDS",3))
    rds = ptr;
  else if(!strncmp(deviceName,"GMB",3))
    gmb = ptr;
  else if(!strncmp(deviceName,"CGD",3))
    cgd = ptr;
  else if(!strncmp(deviceName,"CGS",3))
    cgs = ptr;
  //else if(!strncmp(deviceName,"CDB",3))
  else if(!strncmp(deviceName,"CAPBD",5))
    cdb = ptr;
  //else if(!strncmp(deviceName,"CSB",3))
  else if(!strncmp(deviceName,"CAPBS",5))
    csb = ptr;
  else if(!strncmp(deviceName,"CGB",3))
    cgb = ptr;
  else if(!strncmp(deviceName,"RD",2))
    rd = ptr;
  else if(!strncmp(deviceName,"RS",2))
    rs = ptr;
  else if(!strncmp(deviceName,"RBD",3))
    rbd = ptr;
  else if(!strncmp(deviceName,"RBS",3))
    rbs = ptr;
}

void MosfetModelBsim3V3::calBasicModelParameters(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(!pModel)
    return;
  calCO(pModel,val,w,l);
  calCOX(pModel,val,w,l);
  calGAMMA(pModel,val,w,l);
  calLAMBDA(pModel,val,w,l);
  calVOV(pModel,val,w,l);
  calKP(pModel,val,w,l);
  calFAIB(pModel,val,w,l);
  calCJ(pModel,val,w,l);
  calCJSW(pModel,val,w,l);
  calLS(pModel,val,w,l);
  pModel->calKPVOV(pModel,val,w,l);
  pModel->calLAMVOV(pModel,val,w,l);
  calCAPBSDK(pModel,val,w,l);
  pModel->calKPID(pModel,val,w,l);
  pModel->calLAMID(pModel,val,w,l);
}

map<char*,double>* MosfetModelBsim3V3::getDeviceMap(Mosfet* pMosfet)
{
  map<char*,double>* p = new map<char*,double>;
  //Normalized sens value for each device over w/l
  p->insert(make_pair(gm->name,0.5));
  //double CAPBSDK = isNMOS ? CAPBSDKn : CAPBSDKp;
  //p->insert(make_pair(csb->name,CAPBSDK));
  //p->insert(make_pair(cdb->name,CAPBSDK));
  //double COX = isNMOS ? COXn : COXp;
  //double CO = isNMOS ? COn : COp;
  //p->insert(make_pair(cgs->name,COX*(pMosfet->l)*2/3+CO));
  //p->insert(make_pair(cgd->name,CO));

#ifdef CONSIDER_MATCHED
  if(pMosfet->pMatch)
    p->insert(make_pair(pMosfet->pMatch->gm->name,0.5));
#endif
  return p;
}
void MosfetModelBsim3V3::updateValueOfGm(double val, double w, double l)
{
  if(!gm)
    return;
  /*double KP = isNMOS ? KPn : KPp;
  double VOV = isNMOS ? VOVn : VOVp;
  gm->value = KP*VOV*val;*/
  //double KPVOV = isNMOS ? KPVOVn : KPVOVp;
#ifdef DEBUG
  if(isNMOS)
    printf("[DEBUG] The Original Value OF GM is %g, KPVOVn is %g",gm->value,KPVOV);
  else
    printf("[DEBUG] The Original Value OF GM is %g, KPVOVp is %g",gm->value,KPVOV);
#endif
#ifdef VBIAS
  gm->value = KPVOV*val;
#else
  gm->value = sqrt(KPID*val);
#endif
#ifdef DEBUG
  printf(" The New Value OF GM is %g\n",gm->value);
#endif
}

void MosfetModelBsim3V3::updateValueOfRds(double val, double w, double l)
{
  if(!rds)
    return;
  /*double KP = isNMOS ? KPn : KPp;
  double LAMBDA = isNMOS ? LAMBDAn : LAMBDAp;
  double VOV = isNMOS ? VOVn : VOVp;
  rds->value = 2/(LAMBDA*KP*val*VOV^2)*/
  //double KPVOV = isNMOS ? KPVOVn : KPVOVp;
  //double LAMVOV = isNMOS ? LAMVOVn : LAMVOVp;
#ifdef DEBUG
  if(isNMOS)
    printf("[DEBUG] The Original Value OF RDS is %g, KPVOVn is %g LAMVOVn is %g",rds->value,KPVOV,LAMVOV);
  else
    printf("[DEBUG] The Original Value OF RDS is %g, KPVOVp is %g LAMVOVp is %g",rds->value,KPVOV,LAMVOV);
#endif
#ifdef VBIAS
  rds->value = LAMVOV/(KPVOV*val);
#else
  rds->value = 1/LAMID;
#endif
#ifdef DEBUG
  printf(" The New Value OF RDS is %g\n",rds->value);
#endif
}

void MosfetModelBsim3V3::updateValueOfGmb(double val, double w, double l)
{
  if(!gmb)
    return;
}

void MosfetModelBsim3V3::updateValueOfCgd(double val, double w, double l)
{
  if(!cgd)
    return;
  double CO = isNMOS ? COn : COp;
#ifdef DEBUG
  if(isNMOS)
    printf("[DEBUG] The Original Value OF CGD is %g, COn is %g",cgd->value,COn);
  else
    printf("[DEBUG] The Original Value OF CGD is %g, COp is %g",cgd->value,COp);
#endif
  cgd->value = CO * w;
#ifdef DEBUG
  printf(" The New Value OF CGD is %g\n",cgd->value);
#endif
}

void MosfetModelBsim3V3::updateValueOfCgs(double val, double w, double l)
{
  if(!cgs)
    return;
  double CO = isNMOS ? COn : COp;
  double COX = isNMOS ? COXn : COXp;
#ifdef DEBUG
  if(isNMOS)
    printf("[DEBUG] The Original Value OF CGS is %g, COn is %g, COXn is %g",cgs->value,COn,COXn);
  else
    printf("[DEBUG] The Original Value OF CGS is %g, COp is %g, COXp is %g",cgs->value,COp,COXp);
#endif
  cgs->value = CO * w + 2 * COX * w * l / 3;
#ifdef DEBUG
  printf(" The New Value OF CGS is %g\n",cgs->value);
#endif
}

void MosfetModelBsim3V3::updateValueOfCdb(double val, double w, double l)
{
  if(!cdb)
    return;
  double CAPBSDK = isNMOS ? CAPBSDKn : CAPBSDKp;
#ifdef DEBUG
  if(isNMOS)
    printf("[DEBUG] The Original Value OF CAPBD is %g, CAPBSDKn is %g",cdb->value,CAPBSDKn);
  else
    printf("[DEBUG] The Original Value OF CAPBD is %g, CAPBSDKp is %g",cdb->value,CAPBSDKp);
#endif
  cdb->value = CAPBSDK * w;
#ifdef DEBUG
  printf(" The New Value OF CAPBD is %g\n",cdb->value);
#endif
}

void MosfetModelBsim3V3::updateValueOfCsb(double val, double w, double l)
{
  if(!csb)
    return;
  double CAPBSDK = isNMOS ? CAPBSDKn : CAPBSDKp;
#ifdef DEBUG
  if(isNMOS)
    printf("[DEBUG] The Original Value OF CAPBS is %g, CAPBSDKn is %g",csb->value,CAPBSDKn);
  else
    printf("[DEBUG] The Original Value OF CAPBS is %g, CAPBSDKp is %g",csb->value,CAPBSDKp);
#endif
  csb->value = CAPBSDK * w;
#ifdef DEBUG
  printf(" The New Value OF CAPBD is %g\n",csb->value);
#endif
}

void MosfetModelBsim3V3::updateValueOfCgb(double val, double w, double l)
{
  if(!cgb)
    return;
}

void MosfetModelBsim3V3::updateValueOfRd(double val, double w, double l)
{
  if(!rd)
    return;
}

void MosfetModelBsim3V3::updateValueOfRs(double val, double w, double l)
{
  if(!rs)
    return;
}

void MosfetModelBsim3V3::updateValueOfRbd(double val, double w, double l)
{
  if(!rbd)
    return;
}

void MosfetModelBsim3V3::updateValueOfRbs(double val, double w, double l)
{
  if(!rbs)
    return;
}

void MosfetModelBsim3V3::calGAMMA(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calLAMBDA(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calVOV(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calKP(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calFAIB(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calCJ(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calCJSW(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calLS(MosfetModelBsim3V3* pModel, double val, double w, double l){}

void MosfetModelBsim3V3::calKPVOV(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  if(pModel->gm == NULL)
    return;
  pModel->KPVOV = pModel->gm->value / val;
  if(pModel->isNMOS)
  { 
    //KPVOVn = KPVOVn == 0 ? KPVOV : (KPVOV + KPVOVn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF KPVOVn is %g\n", pModel->KPVOV);
#endif
  }
  else
  {
    //KPVOVp = KPVOVp == 0 ? KPVOV : (KPVOV + KPVOVp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF KPVOVp is %g\n", pModel->KPVOV);
#endif
  }
}

void MosfetModelBsim3V3::calLAMVOV(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  if(pModel->gm == NULL || pModel->rds == NULL)
    return;
  pModel->LAMVOV = pModel->gm->value * pModel->rds->value;
  if(pModel->isNMOS)
  {
    //LAMVOVn = LAMVOVn == 0 ? LAMVOV : (LAMVOV + LAMVOVn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF LAMVOVn is %g\n", pModel->LAMVOV);
#endif
  }
  else
  {
    //LAMVOVp = LAMVOVp == 0 ? LAMVOV : (LAMVOV + LAMVOVp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF LAMVOVp is %g\n", pModel->LAMVOV);
#endif
  }
}

void MosfetModelBsim3V3::calKPID(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  if(pModel->gm == NULL)
    return;
  pModel->KPID = (pModel->gm->value*pModel->gm->value) / val;
  if(pModel->isNMOS)
  { 
    //KPVOVn = KPVOVn == 0 ? KPVOV : (KPVOV + KPVOVn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF KPIDn is %g\n", pModel->KPID);
#endif
  }
  else
  {
    //KPVOVp = KPVOVp == 0 ? KPVOV : (KPVOV + KPVOVp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF KPIDp is %g\n", pModel->KPID);
#endif
  }
}

void MosfetModelBsim3V3::calLAMID(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  if(pModel->rds == NULL)
    return;
  pModel->LAMID = 1/pModel->rds->value;
  if(pModel->isNMOS)
  { 
    //KPVOVn = KPVOVn == 0 ? KPVOV : (KPVOV + KPVOVn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF LAMIDn is %g\n", pModel->LAMID);
#endif
  }
  else
  {
    //KPVOVp = KPVOVp == 0 ? KPVOV : (KPVOV + KPVOVp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF LAMIDp is %g\n", pModel->LAMID);
#endif
  }
}

void MosfetModelBsim3V3::calCAPBSDK(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  double CAPBSDK = 0;
  if(pModel->csb == NULL && pModel->cdb != NULL)
    CAPBSDK = pModel->cdb->value / w;
  else if(pModel->csb != NULL && pModel->cdb == NULL)
    CAPBSDK = pModel->csb->value / w;
  else if(pModel->csb != NULL && pModel->cdb != NULL)
    CAPBSDK = (pModel->cdb->value / w + pModel->csb->value / w) / 2;
  else
    return;
  if(pModel->isNMOS)
  {
    CAPBSDKn = CAPBSDKn == 0 ? CAPBSDK : (CAPBSDK + CAPBSDKn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF CAPBSDKn is %g\n", CAPBSDKn);
#endif
  }
  else
  {
    CAPBSDKp = CAPBSDKp == 0 ? CAPBSDK : (CAPBSDK + CAPBSDKp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF CAPBSDKp is %g\n", CAPBSDKp);
#endif
  }
}

void MosfetModelBsim3V3::calCO(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  if(pModel->cgd == NULL)
    return;
  double CO = pModel->cgd->value / w;
  if(pModel->isNMOS)
  {
    COn = COn == 0 ? CO : (CO + COn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF COn is %g\n", COn);
#endif
  }
  else
  {
    COp = COp == 0 ? CO : (CO + COp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF COp is %g\n", COp);
#endif
  }
}

void MosfetModelBsim3V3::calCOX(MosfetModelBsim3V3* pModel, double val, double w, double l)
{
  if(pModel == NULL)
    return;
  if(pModel->cgd == NULL || pModel->cgs == NULL)
    return;
  double COX = (pModel->cgs->value - pModel->cgd->value) / (w * l * 2.0/3.0);
  if(pModel->isNMOS)
  {
    COXn = COXn == 0 ? COX : (COX + COXn)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF COXn is %g\n", COXn);
#endif
  }
  else
  {
    COXp = COXp == 0 ? COX : (COX + COXp)/2;
#ifdef DEBUG
    printf("[DEBUG] The New Value OF COXp is %g\n", COXp);
#endif
  }
}

void Mosfet::updateValue(double val, int flag)
{
  if(pModel == NULL)
    return;
  
  switch(flag)
  {
  case 0: //W/L
    value = val; w = l*val; pModel->updateValues(value,w,l); break;
  case 1: //W
    value = val/l; w = val; pModel->updateValues(value,w,l); break;
  case 2: //L
    value = w/val; l = val; pModel->updateValues(value,w,l); break;
  default:
    break;
  }
}

void Mosfet::calBasicModelParameters(Mosfet* pMosfet)
{
  if(pMosfet == NULL)
    return;
  MosfetModel* pMModel = pMosfet->pModel;
  if(pMModel == NULL)
    return;
  ModelLevel mModelLevel = pMModel->getModelLevel();
  if(mModelLevel == MOSFET_MODEL_L1)
  {
    MosfetModelL1* pMMModel = (MosfetModelL1*)pMModel;
    pMMModel->calBasicModelParameters(pMMModel,pMosfet->value,pMosfet->w,pMosfet->l);
  }
  else if(mModelLevel == MOSFET_MODEL_L2)
  {
    MosfetModelL2* pMMModel = (MosfetModelL2*)pMModel;
    pMMModel->calBasicModelParameters(pMMModel,pMosfet->value,pMosfet->w,pMosfet->l);
  }
  else if(mModelLevel == MOSFET_MODEL_L3)
  {
    MosfetModelL3* pMMModel = (MosfetModelL3*)pMModel;
    pMMModel->calBasicModelParameters(pMMModel,pMosfet->value,pMosfet->w,pMosfet->l);
  }
  else if(mModelLevel == MOSFET_MODEL_L4)
  {
    MosfetModelL4* pMMModel = (MosfetModelL4*)pMModel;
    pMMModel->calBasicModelParameters(pMMModel,pMosfet->value,pMosfet->w,pMosfet->l);
  }
  else if(mModelLevel == MOSFET_MODEL_BSIM3V3)
  {
    MosfetModelBsim3V3* pMMModel = (MosfetModelBsim3V3*)pMModel;
    pMMModel->calBasicModelParameters(pMMModel,pMosfet->value,pMosfet->w,pMosfet->l);
  }
}

map<char*,double>* Mosfet::getModelDevices()
{
  if(pModel == NULL)
    return NULL;
  return pModel->getDeviceMap(this);
}
