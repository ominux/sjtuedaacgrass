#ifndef DLGLexer_h
#define DLGLexer_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: parser.dlg
 *
 * 1989-2001 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR33
 */


#include "DLexerBase.h"

class DLGLexer : public DLGLexerBase {
public:
public:
	static const int MAX_MODE;
	static const int DfaStates;
	static const int START;
	static const int PAR_STATE;
	static const int NORMAL;
	typedef unsigned short DfaState;

	DLGLexer(DLGInputStream *in,
		unsigned bufsize=2000)
		: DLGLexerBase(in, bufsize, 1)
	{
	;
	}
	void	  mode(int);
	ANTLRTokenType nextTokenType(void);
	void     advance(void);
protected:
	ANTLRTokenType act1();
	ANTLRTokenType act2();
	ANTLRTokenType act3();
	ANTLRTokenType act4();
	ANTLRTokenType act5();
	ANTLRTokenType act6();
	ANTLRTokenType act7();
	ANTLRTokenType act8();
	ANTLRTokenType act9();
	ANTLRTokenType act10();
	ANTLRTokenType act11();
	ANTLRTokenType act12();
	ANTLRTokenType act13();
	ANTLRTokenType act14();
	ANTLRTokenType act15();
	ANTLRTokenType act16();
	ANTLRTokenType act17();
	ANTLRTokenType act18();
	ANTLRTokenType act19();
	ANTLRTokenType act20();
	ANTLRTokenType act21();
	ANTLRTokenType act22();
	ANTLRTokenType act23();
	ANTLRTokenType act24();
	ANTLRTokenType act25();
	ANTLRTokenType act26();
	ANTLRTokenType act27();
	ANTLRTokenType act28();
	ANTLRTokenType act29();
	ANTLRTokenType act30();
	ANTLRTokenType act31();
	ANTLRTokenType act32();
	ANTLRTokenType act33();
	ANTLRTokenType act34();
	ANTLRTokenType act35();
	ANTLRTokenType act36();
	ANTLRTokenType act37();
	ANTLRTokenType act38();
	ANTLRTokenType act39();
	ANTLRTokenType act40();
	ANTLRTokenType act41();
	ANTLRTokenType act42();
	ANTLRTokenType act43();
	ANTLRTokenType act44();
	ANTLRTokenType act45();
	ANTLRTokenType act46();
	ANTLRTokenType act47();
	ANTLRTokenType act48();
	ANTLRTokenType act49();
	ANTLRTokenType act50();
	ANTLRTokenType act51();
	ANTLRTokenType act52();
	ANTLRTokenType act53();
	ANTLRTokenType act54();
	ANTLRTokenType act55();
	ANTLRTokenType act56();
	ANTLRTokenType act57();
	ANTLRTokenType act58();
	ANTLRTokenType act59();
	ANTLRTokenType act60();
	ANTLRTokenType act61();
	ANTLRTokenType act62();
	ANTLRTokenType act63();
	ANTLRTokenType act64();
	ANTLRTokenType act65();
	ANTLRTokenType act66();
	ANTLRTokenType act67();
	ANTLRTokenType act68();
	ANTLRTokenType act69();
	ANTLRTokenType act70();
	ANTLRTokenType act71();
	ANTLRTokenType act72();
	ANTLRTokenType act73();
	ANTLRTokenType act74();
	ANTLRTokenType act75();
	ANTLRTokenType act76();
	ANTLRTokenType act77();
	ANTLRTokenType act78();
	ANTLRTokenType act79();
	static DfaState st0[3];
	static DfaState st1[3];
	static DfaState st2[3];
	static DfaState st3[34];
	static DfaState st4[34];
	static DfaState st5[34];
	static DfaState st6[34];
	static DfaState st7[34];
	static DfaState st8[34];
	static DfaState st9[34];
	static DfaState st10[34];
	static DfaState st11[34];
	static DfaState st12[34];
	static DfaState st13[34];
	static DfaState st14[34];
	static DfaState st15[34];
	static DfaState st16[34];
	static DfaState st17[34];
	static DfaState st18[34];
	static DfaState st19[34];
	static DfaState st20[34];
	static DfaState st21[34];
	static DfaState st22[34];
	static DfaState st23[34];
	static DfaState st24[34];
	static DfaState st25[34];
	static DfaState st26[34];
	static DfaState st27[34];
	static DfaState st28[34];
	static DfaState st29[34];
	static DfaState st30[34];
	static DfaState st31[34];
	static DfaState st32[34];
	static DfaState st33[34];
	static DfaState st34[34];
	static DfaState st35[34];
	static DfaState st36[34];
	static DfaState st37[34];
	static DfaState st38[34];
	static DfaState st39[34];
	static DfaState st40[34];
	static DfaState st41[34];
	static DfaState st42[34];
	static DfaState st43[34];
	static DfaState st44[34];
	static DfaState st45[34];
	static DfaState st46[34];
	static DfaState st47[34];
	static DfaState st48[34];
	static DfaState st49[34];
	static DfaState st50[34];
	static DfaState st51[34];
	static DfaState st52[34];
	static DfaState st53[34];
	static DfaState st54[34];
	static DfaState st55[34];
	static DfaState st56[34];
	static DfaState st57[34];
	static DfaState st58[34];
	static DfaState st59[34];
	static DfaState st60[34];
	static DfaState st61[34];
	static DfaState st62[57];
	static DfaState st63[57];
	static DfaState st64[57];
	static DfaState st65[57];
	static DfaState st66[57];
	static DfaState st67[57];
	static DfaState st68[57];
	static DfaState st69[57];
	static DfaState st70[57];
	static DfaState st71[57];
	static DfaState st72[57];
	static DfaState st73[57];
	static DfaState st74[57];
	static DfaState st75[57];
	static DfaState st76[57];
	static DfaState st77[57];
	static DfaState st78[57];
	static DfaState st79[57];
	static DfaState st80[57];
	static DfaState st81[57];
	static DfaState st82[57];
	static DfaState st83[57];
	static DfaState st84[57];
	static DfaState st85[57];
	static DfaState st86[57];
	static DfaState st87[57];
	static DfaState st88[57];
	static DfaState st89[57];
	static DfaState st90[57];
	static DfaState st91[57];
	static DfaState st92[57];
	static DfaState st93[57];
	static DfaState st94[57];
	static DfaState st95[57];
	static DfaState st96[57];
	static DfaState st97[57];
	static DfaState st98[57];
	static DfaState st99[57];
	static DfaState st100[57];
	static DfaState st101[57];
	static DfaState st102[57];
	static DfaState st103[57];
	static DfaState st104[57];
	static DfaState st105[57];
	static DfaState st106[57];
	static DfaState st107[57];
	static DfaState st108[57];
	static DfaState st109[57];
	static DfaState st110[57];
	static DfaState st111[57];
	static DfaState st112[57];
	static DfaState st113[57];
	static DfaState st114[57];
	static DfaState st115[57];
	static DfaState st116[57];
	static DfaState st117[57];
	static DfaState st118[57];
	static DfaState st119[57];
	static DfaState st120[57];
	static DfaState st121[57];
	static DfaState st122[57];
	static DfaState st123[57];
	static DfaState st124[57];
	static DfaState st125[57];
	static DfaState st126[57];
	static DfaState st127[57];
	static DfaState st128[57];
	static DfaState st129[57];
	static DfaState st130[57];
	static DfaState st131[57];
	static DfaState st132[57];
	static DfaState st133[57];
	static DfaState st134[57];
	static DfaState st135[57];
	static DfaState st136[57];
	static DfaState st137[57];
	static DfaState st138[57];
	static DfaState st139[57];
	static DfaState st140[57];
	static DfaState st141[57];
	static DfaState st142[57];
	static DfaState st143[57];
	static DfaState st144[57];
	static DfaState st145[57];
	static DfaState st146[57];
	static DfaState st147[57];
	static DfaState st148[57];
	static DfaState st149[57];
	static DfaState st150[57];
	static DfaState st151[57];
	static DfaState st152[57];
	static DfaState st153[57];
	static DfaState st154[57];
	static DfaState st155[57];
	static DfaState st156[57];
	static DfaState st157[57];
	static DfaState st158[57];
	static DfaState st159[57];
	static DfaState st160[57];
	static DfaState st161[57];
	static DfaState st162[57];
	static DfaState st163[57];
	static DfaState st164[57];
	static DfaState st165[57];
	static DfaState st166[57];
	static DfaState st167[57];
	static DfaState st168[57];
	static DfaState st169[57];
	static DfaState st170[57];
	static DfaState st171[57];
	static DfaState st172[57];
	static DfaState st173[57];
	static DfaState st174[57];
	static DfaState st175[57];
	static DfaState st176[57];
	static DfaState st177[57];
	static DfaState st178[57];
	static DfaState st179[57];
	static DfaState st180[57];
	static DfaState st181[57];
	static DfaState st182[57];
	static DfaState st183[57];
	static DfaState st184[57];
	static DfaState st185[57];
	static DfaState st186[57];
	static DfaState st187[57];
	static DfaState st188[57];
	static DfaState st189[57];
	static DfaState st190[57];
	static DfaState st191[57];
	static DfaState st192[57];
	static DfaState st193[57];
	static DfaState st194[57];
	static DfaState st195[57];
	static DfaState st196[57];
	static DfaState st197[57];
	static DfaState st198[57];
	static DfaState st199[57];
	static DfaState st200[57];
	static DfaState st201[57];
	static DfaState st202[57];
	static DfaState st203[57];
	static DfaState st204[57];
	static DfaState st205[57];
	static DfaState st206[57];
	static DfaState st207[57];
	static DfaState st208[57];
	static DfaState st209[57];
	static DfaState st210[57];
	static DfaState st211[57];
	static DfaState st212[57];
	static DfaState st213[57];
	static DfaState st214[57];
	static DfaState st215[57];
	static DfaState st216[57];
	static DfaState st217[57];
	static DfaState st218[57];
	static DfaState st219[57];
	static DfaState st220[57];
	static DfaState st221[57];
	static DfaState st222[57];
	static DfaState st223[57];
	static DfaState st224[57];
	static DfaState st225[57];
	static DfaState st226[57];
	static DfaState st227[57];
	static DfaState st228[57];
	static DfaState *dfa[229];
	static DfaState dfa_base[];
	static unsigned char *b_class_no[];
	static DfaState accepts[230];
	static DLGChar alternatives[230];
	static ANTLRTokenType (DLGLexer::*actions[80])();
	static unsigned char shift0[257];
	static unsigned char shift1[257];
	static unsigned char shift2[257];
	int ZZSHIFT(int c) { return b_class_no[automaton][1+c]; }
//
// 133MR1 Deprecated feature to allow inclusion of user-defined code in DLG class header
//
#ifdef DLGLexerIncludeFile
#include DLGLexerIncludeFile
#endif
};
typedef ANTLRTokenType (DLGLexer::*PtrDLGLexerMemberFunc)();
#endif
