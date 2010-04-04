#ifndef MOSFET_H
#define MOSFET_H

#include        <fstream>
#include        "utility.h"
#include	"tpdd_struct.h"

typedef enum
{
  NMOS_DEF,
  NMOS_L3,
  PMOS_DEF,
  PMOS_L3,
  MODEL_TYPE_UNKNOWN,
}ModelType;

typedef enum
{
  MOSFET_MODEL_L1,
  MOSFET_MODEL_L2,
  MOSFET_MODEL_L3,
  MOSFET_MODEL_L4,
  MOSFET_MODEL_BSIM3V3,
  MOSFET_MODEL_UNKOWN,
}ModelLevel;

class MosfetModel
{
public:
  bool isNMOS;

  MosfetModel()
  {isNMOS = true;}
  MosfetModel(ModelType t)
  {if(t == PMOS_DEF || t == PMOS_L3) isNMOS = false; else isNMOS = true;}

  virtual void updateValues(double val, double w, double l) = 0;
  virtual void setModelDevicePtr(SymbNode* ptr) = 0;
  virtual ModelLevel getModelLevel()	{return MOSFET_MODEL_UNKOWN;}
protected:
  double updateValueOfConst(double val);
};

class MosfetModelL1 : public MosfetModel
{
public:
  SymbNode *gm;
  SymbNode *rds;

  MosfetModelL1() : MosfetModel()
  {gm = NULL; rds = NULL;}
  MosfetModelL1(ModelType t) : MosfetModel(t)
  {MosfetModelL1();}

  void updateValues(double val, double w, double l);
  void setModelDevicePtr(SymbNode* ptr);
  static void calBasicModelParameters(MosfetModelL1* pModel, double val, double w, double l);
  ModelLevel getModelLevel()	{return MOSFET_MODEL_L1;}
protected:
  void updateValueOfGm(double val, double w, double l);
  void updateValueOfRds(double val, double w, double l);
  static void calLAMBDA(MosfetModelL1* pModel, double val, double w, double l);
  static void calVOV(MosfetModelL1* pModel, double val, double w, double l);
  static void calKP(MosfetModelL1* pModel, double val, double w, double l);
};


class MosfetModelL2 : public MosfetModelL1
{
public:
  static double GAMMAn;
  static double GAMMAp;
  static double VT0n;
  static double VT0p;
  static double LAMBDAn;
  static double LAMBDAp;
  static double KPn;
  static double KPp;
  static double COXn;
  static double COXp;
  static double COn;
  static double COp;
  static double FAIBn;
  static double FAIBp;
  static double FAIBSWn;
  static double FAIBSWp;
  static double CJn;
  static double CJp;
  static double CJSWn;
  static double CJSWp;
  static double MJn;
  static double MJp;
  static double MJSWn;
  static double MJSWp;
  static double VOVn;
  static double VOVp;
  //Some Middle Constant
  static double KPVOVn;	//KPn * VOVn
  static double KPVOVp;	//KPp * VOVp
  static double LAMVOVn;	//2 / (LAMBDAn * VOVn)
  static double LAMVOVp;	//2 / (LAMBDAp * VOVp)
public:
  SymbNode *gmb;

  MosfetModelL2() : MosfetModelL1()
  {gmb = NULL;}
  MosfetModelL2(ModelType t) : MosfetModelL1(t)
  {MosfetModelL2();}

  void updateValues(double val, double w, double l);
  void setModelDevicePtr(SymbNode* ptr);
  static void calBasicModelParameters(MosfetModelL2* pModel, double val, double w, double l);
  ModelLevel getModelLevel()	{return MOSFET_MODEL_L2;}
protected:
  void updateValueOfGm(double val, double w, double l);
  void updateValueOfRds(double val, double w, double l);
  void updateValueOfGmb(double val, double w, double l);
  static void calGAMMA(MosfetModelL2* pModel, double val, double w, double l);
  static void calLAMBDA(MosfetModelL2* pModel, double val, double w, double l);
  static void calVOV(MosfetModelL2* pModel, double val, double w, double l);
  static void calKP(MosfetModelL2* pModel, double val, double w, double l);
  static void calFAIB(MosfetModelL2* pModel, double val, double w, double l);
};

class MosfetModelL3 : public MosfetModelL2
{
public:
  static double GAMMAn;
  static double GAMMAp;
  static double VT0n;
  static double VT0p;
  static double LAMBDAn;
  static double LAMBDAp;
  static double KPn;
  static double KPp;
  static double COXn;
  static double COXp;
  static double COn;
  static double COp;
  static double FAIBn;
  static double FAIBp;
  static double FAIBSWn;
  static double FAIBSWp;
  static double CJn;
  static double CJp;
  static double CJSWn;
  static double CJSWp;
  static double MJn;
  static double MJp;
  static double MJSWn;
  static double MJSWp;
  static double VOVn;
  static double VOVp;
  //Some Middle Constant
  static double KPVOVn;	//KPn * VOVn
  static double KPVOVp;	//KPp * VOVp
  static double LAMVOVn;	//2 / (LAMBDAn * VOVn)
  static double LAMVOVp;	//2 / (LAMBDAp * VOVp)
public:
  SymbNode *cgd;
  SymbNode *cgs;
  SymbNode *cdb;
  SymbNode *csb;

  MosfetModelL3() : MosfetModelL2()
  {cgd = NULL; cgs = NULL; cdb = NULL; csb = NULL;}
  MosfetModelL3(ModelType t) : MosfetModelL2(t)
  {MosfetModelL3();}

  void updateValues(double val, double w, double l);
  void setModelDevicePtr(SymbNode* ptr);
  static void calBasicModelParameters(MosfetModelL3* pModel, double val, double w, double l);
  ModelLevel getModelLevel()	{return MOSFET_MODEL_L3;}
protected:
  void updateValueOfGm(double val, double w, double l);
  void updateValueOfRds(double val, double w, double l);
  void updateValueOfGmb(double val, double w, double l);
  void updateValueOfCgd(double val, double w, double l);
  void updateValueOfCgs(double val, double w, double l);
  void updateValueOfCdb(double val, double w, double l);
  void updateValueOfCsb(double val, double w, double l);
  static void calGAMMA(MosfetModelL3* pModel, double val, double w, double l);
  static void calLAMBDA(MosfetModelL3* pModel, double val, double w, double l);
  static void calVOV(MosfetModelL3* pModel, double val, double w, double l);
  static void calKP(MosfetModelL3* pModel, double val, double w, double l);
  static void calFAIB(MosfetModelL3* pModel, double val, double w, double l);
  static void calCOX(MosfetModelL3* pModel, double val, double w, double l);
  static void calCO(MosfetModelL3* pModel, double val, double w, double l);
  static void calCJ(MosfetModelL3* pModel, double val, double w, double l);
  static void calCJSW(MosfetModelL3* pModel, double val, double w, double l);
  static void calLS(MosfetModelL3* pModel, double val, double w, double l);
};

class MosfetModelL4 : public MosfetModelL3
{
public:
  static double GAMMAn;
  static double GAMMAp;
  static double VT0n;
  static double VT0p;
  static double LAMBDAn;
  static double LAMBDAp;
  static double KPn;
  static double KPp;
  static double COXn;
  static double COXp;
  static double COn;
  static double COp;
  static double FAIBn;
  static double FAIBp;
  static double FAIBSWn;
  static double FAIBSWp;
  static double CJn;
  static double CJp;
  static double CJSWn;
  static double CJSWp;
  static double MJn;
  static double MJp;
  static double MJSWn;
  static double MJSWp;
  static double VOVn;
  static double VOVp;
  //Some Middle Constant
  static double KPVOVn;	//KPn * VOVn
  static double KPVOVp;	//KPp * VOVp
  static double LAMVOVn;	//2 / (LAMBDAn * VOVn)
  static double LAMVOVp;	//2 / (LAMBDAp * VOVp)
public:
  SymbNode *cgb;

  MosfetModelL4() : MosfetModelL3()
  {cgb = NULL;}
  MosfetModelL4(ModelType t) : MosfetModelL3(t)
  {MosfetModelL4();}

  void updateValues(double val, double w, double l);
  void setModelDevicePtr(SymbNode* ptr);
  static void calBasicModelParameters(MosfetModelL4* pModel, double val, double w, double l);
  ModelLevel getModelLevel()	{return MOSFET_MODEL_L4;}
protected:
  void updateValueOfGm(double val, double w, double l);
  void updateValueOfRds(double val, double w, double l);
  void updateValueOfGmb(double val, double w, double l);
  void updateValueOfCgd(double val, double w, double l);
  void updateValueOfCgs(double val, double w, double l);
  void updateValueOfCdb(double val, double w, double l);
  void updateValueOfCsb(double val, double w, double l);
  void updateValueOfCgb(double val, double w, double l);
  static void calGAMMA(MosfetModelL4* pModel, double val, double w, double l);
  static void calLAMBDA(MosfetModelL4* pModel, double val, double w, double l);
  static void calVOV(MosfetModelL4* pModel, double val, double w, double l);
  static void calKP(MosfetModelL4* pModel, double val, double w, double l);
  static void calFAIB(MosfetModelL4* pModel, double val, double w, double l);
  static void calCJ(MosfetModelL4* pModel, double val, double w, double l);
  static void calCJSW(MosfetModelL4* pModel, double val, double w, double l);
  static void calLS(MosfetModelL4* pModel, double val, double w, double l);
};

class MosfetModelBsim3V3 : public MosfetModelL4
{
public:
  static double GAMMAn;
  static double GAMMAp;
  static double VT0n;
  static double VT0p;
  static double LAMBDAn;
  static double LAMBDAp;
  static double KPn;
  static double KPp;
  static double COXn;
  static double COXp;
  static double COn;
  static double COp;
  static double FAIBn;
  static double FAIBp;
  static double FAIBSWn;
  static double FAIBSWp;
  static double CJn;
  static double CJp;
  static double CJSWn;
  static double CJSWp;
  static double MJn;
  static double MJp;
  static double MJSWn;
  static double MJSWp;
  static double VOVn;
  static double VOVp;
  //Some Middle Constant
  static double KPVOVn;	//KPn * VOVn
  static double KPVOVp;	//KPp * VOVp
  static double LAMVOVn;	//2 / (LAMBDAn * VOVn)
  static double LAMVOVp;	//2 / (LAMBDAp * VOVp)
  static double CAPBSDKn;	//CJn * LSn + CJSWn
  static double CAPBSDKp;	//CJp * LSp + CJSWp
public:
  SymbNode *rd;
  SymbNode *rs;
  SymbNode *rbd;
  SymbNode *rbs;

  MosfetModelBsim3V3() : MosfetModelL4()
  {rd = NULL; rs = NULL; rbd = NULL; rbs = NULL;}
  MosfetModelBsim3V3(ModelType t) : MosfetModelL4(t)
  {MosfetModelBsim3V3();}

  void updateValues(double val, double w, double l);
  void setModelDevicePtr(SymbNode* ptr);
  static void calBasicModelParameters(MosfetModelBsim3V3* pModel, double val, double w, double l);
  ModelLevel getModelLevel()	{return MOSFET_MODEL_BSIM3V3;}
protected:
  void updateValueOfGm(double val, double w, double l);
  void updateValueOfRds(double val, double w, double l);
  void updateValueOfGmb(double val, double w, double l);
  void updateValueOfCgd(double val, double w, double l);
  void updateValueOfCgs(double val, double w, double l);
  void updateValueOfCdb(double val, double w, double l);
  void updateValueOfCsb(double val, double w, double l);
  void updateValueOfCgb(double val, double w, double l);
  void updateValueOfRd(double val, double w, double l);
  void updateValueOfRs(double val, double w, double l);
  void updateValueOfRbd(double val, double w, double l);
  void updateValueOfRbs(double val, double w, double l);
  static void calCO(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calCOX(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calGAMMA(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calLAMBDA(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calVOV(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calKP(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calFAIB(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calCJ(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calCJSW(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calLS(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calKPVOV(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calLAMVOV(MosfetModelBsim3V3* pModel, double val, double w, double l);
  static void calCAPBSDK(MosfetModelBsim3V3* pModel, double val, double w, double l);
};

class Mosfet
{
public:
  char* name;
  Node_g *noded;
  Node_g *nodeg;
  Node_g *nodes;
  Node_g *nodeb;
  ModelType type; //nmos | pmos
  double w;
  double l;
  double value;
  MosfetModel* pModel;
  Mosfet* next;
  //Small Signal Devices Value Update Function
  Mosfet()
  {
    name = NULL;
    noded = NULL;
    nodeg = NULL;
    nodes = NULL;
    nodeb = NULL;
    type = MODEL_TYPE_UNKNOWN;
    w = 1u;
    l = 1u;
    value = 1;
    pModel = NULL;
    next = NULL;
  }
  ~Mosfet()
  {
    if(name)
      delete [] name;
  }

  void updateValue(double val, int flag);
  static void calBasicModelParameters(Mosfet* pMosfet);
};

#endif
