/*
 * netparser: P a r s e r  H e a d e r 
 *
 * Generated from: netgram.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * with AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#ifndef netparser_h
#define netparser_h

#ifndef ANTLR_VERSION
#define ANTLR_VERSION 13333
#endif

#include "AParser.h"


#include <iostream> 
#include "ToGraph.h"
class netparser : public ANTLRParser {
public:
	static  const ANTLRChar *tokenName(int tk);
	enum { SET_SIZE = 74 };
protected:
	static const ANTLRChar *_token_tbl[];
private:
protected:
	static SetWordType err1[12];
	static SetWordType err2[12];
	static SetWordType err3[12];
	static SetWordType err4[12];
	static SetWordType err5[12];
	static SetWordType setwd1[74];
	static SetWordType err6[12];
	static SetWordType err7[12];
	static SetWordType err8[12];
	static SetWordType err9[12];
	static SetWordType err10[12];
	static SetWordType err11[12];
	static SetWordType err12[12];
	static SetWordType err13[12];
	static SetWordType setwd2[74];
	static SetWordType err14[12];
	static SetWordType err15[12];
	static SetWordType err16[12];
	static SetWordType err17[12];
	static SetWordType err18[12];
	static SetWordType err19[12];
	static SetWordType err20[12];
	static SetWordType err21[12];
	static SetWordType setwd3[74];
	static SetWordType err22[12];
	static SetWordType err23[12];
	static SetWordType err24[12];
	static SetWordType err25[12];
	static SetWordType err26[12];
	static SetWordType err27[12];
	static SetWordType err28[12];
	static SetWordType err29[12];
	static SetWordType err30[12];
	static SetWordType err31[12];
	static SetWordType err32[12];
	static SetWordType err33[12];
	static SetWordType err34[12];
	static SetWordType err35[12];
	static SetWordType err36[12];
	static SetWordType setwd4[74];
	static SetWordType ONOFF_set[12];
	static SetWordType ONOFF_errset[12];
	static SetWordType err39[12];
	static SetWordType err40[12];
	static SetWordType err41[12];
	static SetWordType err42[12];
	static SetWordType err43[12];
	static SetWordType err44[12];
	static SetWordType err45[12];
	static SetWordType err46[12];
	static SetWordType err47[12];
	static SetWordType setwd5[74];
	static SetWordType err48[12];
	static SetWordType err49[12];
	static SetWordType err50[12];
	static SetWordType err51[12];
	static SetWordType err52[12];
	static SetWordType err53[12];
	static SetWordType err54[12];
	static SetWordType err55[12];
	static SetWordType err56[12];
	static SetWordType err57[12];
	static SetWordType err58[12];
	static SetWordType err59[12];
	static SetWordType err60[12];
	static SetWordType setwd6[74];
	static SetWordType err61[12];
	static SetWordType PULSE_set[12];
	static SetWordType PULSE_errset[12];
	static SetWordType SIN_set[12];
	static SetWordType SIN_errset[12];
	static SetWordType EXP_set[12];
	static SetWordType EXP_errset[12];
	static SetWordType PWL_set[12];
	static SetWordType PWL_errset[12];
	static SetWordType SFFM_set[12];
	static SetWordType SFFM_errset[12];
	static SetWordType err72[12];
	static SetWordType setwd7[74];
	static SetWordType err73[12];
	static SetWordType err74[12];
	static SetWordType err75[12];
	static SetWordType err76[12];
	static SetWordType err77[12];
	static SetWordType err78[12];
	static SetWordType setwd8[74];
	static SetWordType setwd9[74];
private:
	void zzdflthandlers( int _signal, int *_retsignal );

public:
	netparser(ANTLRTokenBuffer *input);
	void input(void);
	void title(void);
	void body(void);
	void dot_command(void);
	void analysis(void);
	void indep_source(void);
	void value_list(void);
	void variable_list(void);
	void parameter_list(void);
	void outvar(void);
	void mix_para_list(void);
};

#endif /* netparser_h */
