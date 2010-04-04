/* netgram.g
 * This parser is modified for parsing a standard netlist to 
 * a graph netlist.
 * G.Shi, 03-25-04
 */

/*
#header <<#include "globals.h">>
*/
#header << 
#include <iostream> 
#include "ToGraph.h"
>>

<<
#include "DLGLexer.h"		/* include definition of DLGLexer.
				* This cannot be generated automatically because
				* ANTLR has no idea what you will call this file
				* with the DLG command-line options.
				*/

using std::cout;
using std::endl;
typedef enum { 
	cAC,
	cDC,
	cPrint,
	cPlot,
	cSubCkt,
	cCktCall,
	cUnknown
} OutputCmd;

static OutputCmd cur_cmd = cUnknown;

/*
#include "globals.h"
#include "circuits.h"
#include "ckt_call.h"
#include "acan.h"
*/

extern class ToGraph *toGraph;

typedef ANTLRCommonToken ANTLRToken;

int
read_input(const char *filename )
{
	FILE *fp;
	fp = fopen(filename,"r");
	if(!fp){
		fprintf(stderr, "\nCan't open file %s", filename);
		return -1;
	}
	setbuf(stderr, 0);
	ANTLRToken aToken; 
	DLGFileInput in(fp);
	DLGLexer scan(&in);
	ANTLRTokenBuffer pipe(&scan);
	scan.setToken(&aToken);
	netparser parser(&pipe);
	parser.init();
	parser.input();
	return 0;
}

>>


#lexclass START
#token TITTLE	"~[\n]*" <<mode(NORMAL); newline();>>

#lexclass PAR_STATE
#token "[\ \t]+"	<<skip();>>
#token VALUE		"{(\+|\-)}[0-9]*{[\.]}[0-9]+{[eE]{(\+ | \-)}[0-9]+}{([Tt]|[Gg]|[Mm][Ee][Gg]|[Kk]|[Mm][Ii][Ll]|[Mm]|[Uu]|[Nn]|[Pp]|[Ff]|[Vv])[a-zA-Z]*}"
#token NEWLINE "\n" <<mode(NORMAL); newline();>>
#token COMMENT		"\*(~[\n])*" 
//#token DOT		"\."
#token IC		"[Ii][Cc]"
#token TEMP		"[Tt][Ee][Mm][Pp]"
#tokclass ONOFF		{"[Oo][Nn]" "[Oo][Ff][Ff]"}
#token AC		"[Aa][Cc]" // AC analysis
#token DC		"[Dd][Cc]" // DC analysis
#token TRAN		"[Tt][Rr][Aa][Nn]" // Transient analysis
#token EQUAL		"="
#token COMMA		","
#token LEFTB		"\("
#token RIGHTB		"\)"
#token VAR_NAME		"[VvIi]{([Rr] | [Ii] | [Mm] | [Pp] | [Dd][Bb])}"
#token IDENTIFIER	"[a-zA-Z][a-zA-Z0-9_\-\+\.]*"


/* general definition */


#lexclass NORMAL
#token "[\ \t]+"	<<skip();>>
#token COMMENT		"\*(~[\n])*" 
#token NEWLINE 		"\n" <<newline();>>
#token VALUE		"{(\+|\-)}[0-9]*{[\.]}[0-9]+{[eE]{(\+ | \-)}[0-9]+}{([Tt]|[Gg]|[Mm][Ee][Gg]|[Kk]|[Mm][Ii][Ll]|[Mm]|[Uu]|[Nn]|[Pp]|[Ff]|[Vv])[a-zA-Z]*}"
#token EQUAL		"="
#token COMMA		","
#token DOT		"\."
#token TEMP		"[Tt][Ee][Mm][Pp]"
/*
#tokclass ONOFF		{"[Oo][Nn]" "[Oo][Ff][Ff]"}
*/


/* analysis */
#token AC		"[Aa][Cc]" <<mode(PAR_STATE);>> // AC analysis
#token DC		"[Dd][Cc]" <<mode(PAR_STATE);>> // DC analysis
#token OP		"[Oo][Pp]" // operation point analysis
#token SENS		"[Ss][Ee][Nn][Ss]" <<mode(PAR_STATE);>> // Sensitivity analysis
#token TF		"[Tt][Ff]" // Transfer Function analysis
#token TRAN		"[Tt][Rr][Aa][Nn]" // Transient analysis
#token NOISE	"[Nn][Oo][Ii][Ss][Ee]" 	<<mode(PAR_STATE);>> // Noise analysis

/* Batch output */
#token SAVE		"[Ss][Aa][Vv][Ee]" // save lines
#token PRINT	"[Pp][Rr][Ii][Nn][Tt]" 	<<mode(PAR_STATE);>>// print lines
#token PLOT		"[Pp][Ll][Oo][Tt]" 		<<mode(PAR_STATE);>>// print lines

#token END		"[Ee][Nn][Dd]"
#token ENDS		"[Ee][Nn][Dd][Ss]"

/* output control */
#token MODEL	"[Mm][Oo][Dd][Ee][Ll]" 		<<mode(PAR_STATE);>> 
#token SUBCKT	"[Ss][Uu][Bb][Cc][Kk][Tt]" 	<<mode(PAR_STATE);>>
#token OPTION	"[Oo][Pp][Tt][Ii][Oo][Nn]{[Ss]}" 	<<mode(PAR_STATE);>>
#token NODESET	"[Nn][Oo][Dd][Ee][Ss][Ee][Tt]"		<<mode(PAR_STATE);>>
#token IC		"[Ii][Cc]" 					<<mode(PAR_STATE);>>

/* independent source functions */
#tokclass PULSE		{"PULSE" "pulse"}
#tokclass SIN		{"SIN" "sin"}
#tokclass EXP		{"EXP" "exp"}
#tokclass PWL		{"PWL" "pwl"}
#tokclass SFFM		{"SFFM" "sffm"}

/* basic passive elements */
#token RES		"[Rr][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// resistor
#token CAP		"[Cc][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// capacitor
#token IND		"[Ll][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// inductor

/* independent sources */
#token VOL		"[Vv][a-zA-Z0-9_\-\+]*" // independent voltage
#token CUR		"[Ii][a-zA-Z0-9_\-\+]*" // independent current


/* linear controlled source */
#token	VCCS 	"[Gg][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// VCCS
#token  VCVS	"[Ee][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// VCVS
#token 	CCCS	"[Ff][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// CCCS
#token 	CCVS	"[Hh][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// CCVS

/* basic nonlinear devices */
#token 	SWITCH	"[Ss][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Switches
#token 	DIODE	"[Dd][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Diodes
#token 	BJT		"[Qq][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Bipolar Junction Transistor
#token 	JFET	"[Jj][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Junction Field-Effect Transistor 
#token  MOSFET	"[Mm][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// MOSFET
#token  MESFET	"[Zz][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// MOSFET
#token  CKTCALL	"[Xx][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// MOSFET

#token IDENTIFIER	"[a-zA-Z][a-zA-Z0-9_\-\+]*" 

class netparser {		/* Define a grammar and action class */

input	: title (body)+ "@";

title	: tt:TITTLE 
		  <<
			#ifdef	TRACE
		    fprintf(stderr,"\n%s",$tt->getText());
			#endif
			toGraph->getTitle($tt->getText()); 
			/* theCurCkt->add_title($tt->getText());*/ 
			>>
		  NEWLINE
		  ;

body	: NEWLINE 
		| cm:COMMENT NEWLINE

		/* independent voltage source */
		| vk:VOL vn1:VALUE vn2:VALUE 
		  {(DC|TRAN) vv:VALUE} 
		  {AC {vm:VALUE {vp:VALUE}}}
		  << 
		  #ifdef	TRACE
		  printf("\nIVS: %s, %s, %s, %s, %s", $vk->getText(), $vn1->getText(), 
		  		$vn2->getText(), ($vv == NULL? NULL:$vv->getText()),
				($vm == NULL? NULL:$vm->getText()) ); 
		  #endif
		  toGraph->parseSRC($vk->getText(), 
				$vn1->getText(), $vn2->getText(), 
				($vv != NULL ?$vv->getText():NULL),
				($vm == NULL? NULL:$vm->getText()) );
		  /*
		  theCurCkt->parse_vi($vk->getText(), 
				$vn1->getText(), $vn2->getText(), 
				($vv != NULL ?$vv->getText():NULL));
		  */
		  >>
		  {("DISTOF1"|"distof1") {VALUE {VALUE}}}
		  {("DISTOF2"|"distof2") {VALUE {VALUE}}}
		  {indep_source}
		  NEWLINE 

		/* independent current source */
		| ik:CUR in1:VALUE in2:VALUE 
		  {(DC|TRAN) iv:VALUE} 
		  {AC {im:VALUE {ip:VALUE}}}
		  << 
		  #ifdef	TRACE
		  printf("\nICS: %s, %s, %s, %s, %s", $ik->getText(), $in1->getText(), 
		  		$in2->getText(), ($iv == NULL? NULL:$iv->getText()),
				($im == NULL? NULL:$im->getText()) ); 
		  #endif
		  toGraph->parseSRC($ik->getText(), 
				$in1->getText(), $in2->getText(), 
				($iv != NULL ? $iv->getText():NULL),
				($im == NULL? NULL:$im->getText()) ); 
		  /*
		  theCurCkt->parse_vi($ik->getText(), 
				$in1->getText(), $in2->getText(), 
				($iv != NULL ? $iv->getText():NULL)); 
		  */
		  >>
		  {("DISTOF1"|"distof1") {VALUE {VALUE}}}
		  {("DISTOF2"|"distof2") {VALUE {VALUE}}}
		  {indep_source}
		  NEWLINE 

		/* linear controlled source */
		| vccsk:VCCS vccsn1:VALUE vccsn2:VALUE vccscn1:VALUE 
			vccscn2:VALUE (vccsv:VALUE | vccsvar:IDENTIFIER) NEWLINE
		<<
		  #ifdef	TRACE
		  printf("\nVCCS: %s, %s, %s, %s, %s, %s, %s", 
			$vccsk->getText(), $vccsn1->getText(), 
			$vccsn2->getText(), $vccscn1->getText(), 
			$vccscn2->getText(), 
			($vccsv != NULL ?$vccsv->getText():NULL),
			($vccsvar != NULL ?$vccsvar->getText():NULL) );
		  #endif

		toGraph->parseVCXS($vccsk->getText(), $vccsn1->getText(), 
			$vccsn2->getText(), $vccscn1->getText(), 
			$vccscn2->getText(), 
			($vccsv != NULL ?$vccsv->getText():NULL),
			($vccsvar != NULL ?$vccsvar->getText():NULL));
		/*
		theCurCkt->parse_vcs($vccsk->getText(), $vccsn1->getText(), 
			$vccsn2->getText(), $vccscn1->getText(), 
			$vccscn2->getText(), 
			($vccsv != NULL ?$vccsv->getText():NULL),
			($vccsvar != NULL ?$vccsvar->getText():NULL));
		*/
		>>
				  
		| vcvsk:VCVS vcvsn1:VALUE vcvsn2:VALUE vcvscn1:VALUE 
			vcvscn2:VALUE (vcvsv:VALUE | vcvsvar:IDENTIFIER) NEWLINE
		<<
		  #ifdef	TRACE
		  printf("\nVCVS: %s, %s, %s, %s, %s, %s, %s", 
			$vcvsk->getText(), $vcvsn1->getText(), 
			$vcvsn2->getText(), $vcvscn1->getText(),
			$vcvscn2->getText(), 
			($vcvsv != NULL ?$vcvsv->getText():NULL),
			($vcvsvar != NULL ?$vcvsvar->getText():NULL) );
		  #endif

		  toGraph->parseVCXS($vcvsk->getText(), $vcvsn1->getText(), 
			$vcvsn2->getText(), $vcvscn1->getText(),
			$vcvscn2->getText(), 
			($vcvsv != NULL ?$vcvsv->getText():NULL),
			($vcvsvar != NULL ?$vcvsvar->getText():NULL));
		/*
		theCurCkt->parse_vcs($vcvsk->getText(), $vcvsn1->getText(), 
			$vcvsn2->getText(), $vcvscn1->getText(),
			$vcvscn2->getText(), 
			($vcvsv != NULL ?$vcvsv->getText():NULL),
			($vcvsvar != NULL ?$vcvsvar->getText():NULL));
		*/
		>>

		| cccsk:CCCS cccsn1:VALUE  cccsn2:VALUE  cccscid:IDENTIFIER 
			(cccsv:VALUE | cccsvar:IDENTIFIER) NEWLINE
		<<
		  #ifdef	TRACE
		  printf("\nCCCS: %s, %s, %s, %s, %s, %s", 
			$cccsk->getText(), $cccsn1->getText(), 
			$cccsn2->getText(), $cccscid->getText(), 
			($cccsv != NULL ?$cccsv->getText():NULL),
			($cccsvar != NULL ?$cccsvar->getText():NULL) );
		  #endif

		  toGraph->parseCCXS($cccsk->getText(), $cccsn1->getText(), 
			$cccsn2->getText(), $cccscid->getText(), 
			($cccsv != NULL ?$cccsv->getText():NULL),
			($cccsvar != NULL ?$cccsvar->getText():NULL));
		/*
		theCurCkt->parse_ccs($cccsk->getText(), $cccsn1->getText(), 
			$cccsn2->getText(), $cccscid->getText(), 
			($cccsv != NULL ?$cccsv->getText():NULL),
			($cccsvar != NULL ?$cccsvar->getText():NULL));
		*/
		>> 
		
		| ccvsk:CCVS ccvsn1:VALUE ccvsn2:VALUE ccvscid:IDENTIFIER 
			(ccvsv:VALUE | ccvsvar:IDENTIFIER) NEWLINE
		<<
		  #ifdef	TRACE
		  printf("\nCCVS: %s, %s, %s, %s, %s, %s", 
			$ccvsk->getText(), $ccvsn1->getText(), 
			$ccvsn2->getText(), $ccvscid->getText(), 
			($ccvsv != NULL ?$ccvsv->getText():NULL),
			($ccvsvar != NULL ?$ccvsvar->getText():NULL) );
		  #endif

		  toGraph->parseCCXS($ccvsk->getText(), $ccvsn1->getText(), 
			$ccvsn2->getText(), $ccvscid->getText(), 
			($ccvsv != NULL ?$ccvsv->getText():NULL),
			($ccvsvar != NULL ?$ccvsvar->getText():NULL));
		/*
		theCurCkt->parse_ccs($ccvsk->getText(), $ccvsn1->getText(), 
			$ccvsn2->getText(), $ccvscid->getText(), 
			($ccvsv != NULL ?$ccvsv->getText():NULL),
			($ccvsvar != NULL ?$ccvsvar->getText():NULL));
		*/
		>>

		/* basic linear device */
		| rk:RES rn1:VALUE rn2:VALUE (rv:VALUE | rvar:IDENTIFIER) NEWLINE
		  <<
		  #ifdef	TRACE
		  printf("\nRES: %s, %s, %s, %s, %s", 
		  	$rk->getText(), $rn1->getText(), $rn2->getText(), 
			($rv != NULL ?$rv->getText():NULL), 
			($rvar != NULL ?$rvar->getText():NULL) ); 
		  #endif

		  toGraph->parseRLC(
		  	$rk->getText(), 
		  	$rn1->getText(), $rn2->getText(), 
			($rv != NULL ?$rv->getText():NULL), 
			($rvar != NULL ?$rvar->getText():NULL)); 
		  /*
		  theCurCkt->parse_rlc($rk->getText(), $rn1->getText(), 
			$rn2->getText(), 
			($rv != NULL ?$rv->getText():NULL), 
			($rvar != NULL ?$rvar->getText():NULL)); 
		  */
		  >>

		| ck:CAP cn1:VALUE cn2:VALUE (cv:VALUE | cvar:IDENTIFIER) NEWLINE
		  <<
		  #ifdef	TRACE
		  printf("\nCAP: %s, %s, %s, %s, %s", 
		  	$ck->getText(), $cn1->getText(), $cn2->getText(), 
			($cv != NULL ?$cv->getText():NULL), 
			($cvar != NULL ?$cvar->getText():NULL) );
		  #endif
		  toGraph->parseRLC(
		  	$ck->getText(), $cn1->getText(), $cn2->getText(), 
			($cv != NULL ?$cv->getText():NULL), 
			($cvar != NULL ?$cvar->getText():NULL) );
		  /*
		  theCurCkt->parse_rlc($ck->getText(), $cn1->getText(), 
			$cn2->getText(), 
			($cv != NULL ?$cv->getText():NULL), 
			($cvar != NULL ?$cvar->getText():NULL));
		  */
		  >>

		| indk:IND indn1:VALUE indn2:VALUE (indv:VALUE | indvar:IDENTIFIER) 
			NEWLINE
		  <<
		  #ifdef	TRACE
		  printf("\nIND: %s, %s, %s, %s, %s", 
		    $indk->getText(), $indn1->getText(), $indn2->getText(), 
			($indv != NULL ?$indv->getText():NULL),
			($indvar != NULL ?$indvar->getText():NULL) );
		  #endif

		  toGraph->parseRLC(
		    $indk->getText(), $indn1->getText(), $indn2->getText(), 
			($indv != NULL ?$indv->getText():NULL),
			($indvar != NULL ?$indvar->getText():NULL) );
		  /*
		  theCurCkt->parse_rlc($indk->getText(), 
			$indn1->getText(), $indn2->getText(), 
			($indv != NULL ?$indv->getText():NULL),
			($indvar != NULL ?$indvar->getText():NULL));
			*/
			>>

		/* basic nonlinear device */
		| sk:SWITCH sn1:VALUE sn2:VALUE NEWLINE
		<<
		  printf("\nSWITCH: %s, %s, %s", 
			$sk->getText(), $sn1->getText(), $sn2->getText());
		/* 
		theCurCkt->parse_switch($sk->getText(),
			$sn1->getText(), $sn2->getText());
		*/
		>>

		| dk:DIODE dn1:VALUE dn2:VALUE dm:IDENTIFIER {VALUE} {ONOFF}
		{IC EQUAL VALUE} {TEMP EQUAL VALUE} NEWLINE
		<<
		  printf("\nDIODE: %s, %s, %s, %s", 
			$dk->getText(), $dn1->getText(), 
			$dn2->getText(), $dm->getText());
		/*
		theCurCkt->parse_diode($dk->getText(), 
			$dn1->getText(), $dn2->getText(), $dm->getText());
		*/
		>>

		| bjk:BJT bjn1:VALUE bjn2:VALUE bjn3:VALUE {bjn4:VALUE} bjm:IDENTIFIER
		  {VALUE} {ONOFF} {IC EQUAL VALUE COMMA VALUE} 
		  {TEMP EQUAL VALUE } NEWLINE
		<<
		  printf("\nBJT: %s, %s, %s, %s, %s, %s", 
			$bjk->getText(), $bjn1->getText(), 
			$bjn2->getText(), $bjn3->getText(), 
			($bjn4 != NULL ? $bjn4->getText():NULL), 
			$bjm->getText() );
		/*
		theCurCkt->parse_bjt($bjk->getText(), $bjn1->getText(), 
			$bjn2->getText(), $bjn3->getText(), 
			($bjn4 != NULL ? $bjn4->getText():NULL), 
			$bjm->getText());
		*/
		>>

		| jfk:JFET	jfn1:VALUE jfn2:VALUE jfn3:VALUE jfm:IDENTIFIER
		  {VALUE} {ONOFF} {IC EQUAL VALUE COMMA VALUE}
		  {TEMP EQUAL VALUE } NEWLINE
		<<
		  printf("\nJFET: %s, %s, %s, %s, %s", 
			$jfk->getText(), 
			$jfn1->getText(), $jfn2->getText(),
		  	$jfn3->getText(), $jfm->getText());
		/*
		theCurCkt->parse_jfet($jfk->getText(), 
			$jfn1->getText(), $jfn2->getText(),
		  	$jfn3->getText(), $jfm->getText());
			*/
			>>

		| mosk:MOSFET mosn1:VALUE mosn2:VALUE mosn3:VALUE mosn4:VALUE 
			mosm:IDENTIFIER mosp1:IDENTIFIER EQUAL mosv1:VALUE
			mosp2:IDENTIFIER EQUAL mosv2:VALUE (parameter_list)* NEWLINE
		<<
		#ifdef TRACE
		printf("\nMOSFET: %s, %s, %s, %s, %s, %s, %s=%s, %s=%s", 
				$mosk->getText(), 
				$mosn1->getText(), $mosn2->getText(),
				$mosn3->getText(), $mosn4->getText(), 
				$mosm->getText(),
				$mosp1->getText(), $mosv1->getText(),
				$mosp2->getText(), $mosv2->getText());
		#endif
		char* p1 = $mosp1->getText();
		char* p2 = $mosp2->getText();
		char* w = NULL;
		char* l = NULL;
		if(tolower(p1[0]) == 'w')
			w = strdup($mosv1->getText());
		else if(tolower(p1[0]) == 'l')
			l = strdup($mosv1->getText());
		if(tolower(p2[0]) == 'w')
		        w = strdup($mosv2->getText());
		else if(tolower(p2[0]) == 'l')
		        l = strdup($mosv2->getText());
		toGraph->parseMOSFET($mosk->getText(),$mosn1->getText(), $mosn2->getText(),
				     $mosn3->getText(), $mosn4->getText(),$mosm->getText(),
				     w,l);
			>>

		// subcircuit call
		| callid:CKTCALL 
		  << cout << "\nBegin cktcall -- " << endl;
			printf("%s", $callid->getText());
			/*
			theCurCkt->parse_ckt_call($callid->getText());
			*/
			cur_cmd = cCktCall;>>
		  (callnd:VALUE 
		  << 
		  printf(", %s", $callnd->getText());
		  /*
		  theCurCkt->parse_ckt_call_node($callnd->getText());
		  */
		  >>
		  )+
			callnm:IDENTIFIER 
		  << 
		  printf(", %s", $callnm->getText() );
		  /*
		  theCurCkt->cur_call->match_sub_ckt(
					$callnm->getText(), theCkt->sub_ckt);
					*/
					>>
			{(variable_list)+}
			NEWLINE
		  << cur_cmd = cUnknown;>>
		
		/* dot command */
		| DOT dot_command
		;

dot_command :	

		MODEL IDENTIFIER IDENTIFIER 
		  "\(" (parameter_list)+ "\)" NEWLINE

		| SUBCKT subid:IDENTIFIER 
		<<  cout <<"\nSubckt: "<<$subid->getText()<<endl; 
		/*
			theCurCkt = theCurCkt->parse_subckt(
						$subid->getText());
		*/
			cur_cmd = cSubCkt;>>
		(internd:VALUE 
		<< 
		printf(", %s", $internd->getText());
		/*
		theCurCkt->parse_inter_node($internd->getText());
		*/
		>>
		)+

		{(variable_list)+}
		NEWLINE
		<< cur_cmd = cUnknown; >>

		/* simulation variable */
		| OPTION (mix_para_list)+ NEWLINE

		/* initial condition */

		| AC << cur_cmd = cAC; >> 
			{outvar} acvar:IDENTIFIER acnp:VALUE acfb:VALUE acfe:VALUE
			<<
			#ifdef	TRACE
			printf("\nAC: %s, %s, %s, %s",
				$acvar->getText(),
				$acnp->getText(),
				$acfb->getText(),
				$acfe->getText());
			#endif
			/*	
			theCurCkt->parse_ac_cmd($acvar->getText(),
			$acnp->getText(),
			$acfb->getText(),
			$acfe->getText());
			*/
			>>
			NEWLINE
			<< cur_cmd = cUnknown; >>

		| NOISE ns:IDENTIFIER NEWLINE
		   <<
		   printf("\nNOISE: %s", $ns->getText());
		   /*
		   theCurCkt->read_nz_sources($ns->getText());
		   */
		   >>

		| DC << cur_cmd = cDC; >>
			dcvar:IDENTIFIER dcstart:VALUE dcstop:VALUE dcstep:VALUE
			//outvar dcstart:VALUE dcstop:VALUE dcstep:VALUE
			<<
			#ifdef	TRACE
			printf("\nDC: %s, %s, %s, %s",
				$dcvar->getText(),
				$dcstart->getText(),
				$dcstop->getText(),
				$dcstep->getText() );
			#endif
			/*
			theCurCkt->parse_dc_input($dcvar->getText());
			theCurCkt->parse_dc_cmd($dcstart->getText(),
			$dcstop->getText(),
			$dcstep->getText());
			*/
			>>
			NEWLINE
			<< cur_cmd = cUnknown; >>

		| OP NEWLINE

		| SENS outvar AC IDENTIFIER VALUE VALUE VALUE NEWLINE

		| TF outvar NEWLINE

		| TRAN VALUE VALUE {VALUE {VALUE}} NEWLINE

		/* batch output */
		| SAVE (IDENTIFIER)+ NEWLINE

		| PRINT 
		<< 
			cur_cmd = cPrint; 
			#ifdef	TRACE
			printf("\nPrint: ");
			#endif
		>>
			{analysis}  (outvar)+ NEWLINE
			<< cur_cmd = cUnknown; >>

		| PLOT 
		<< 
			cur_cmd = cPlot; 
			#ifdef	TRACE
			printf("\nPlot: ");
			#endif
		>> 
			{analysis} (outvar)+ NEWLINE
			<< cur_cmd = cUnknown; >>

		| ENDS {IDENTIFIER} NEWLINE
		  << cout << "\nChange current ckt"<<endl;
		  printf(": .END"); 
		  /*
			theCurCkt = theCurCkt->parent; 
			if(!theCurCkt)
				error_mesg(SYN_ERROR,"Invalid .ENDS command");
		  */
		  >>

		| END NEWLINE

		;
analysis	: AC
		| DC
		| TRAN
		;

indep_source	:
		PULSE "\(" VALUE VALUE VALUE VALUE VALUE VALUE VALUE "\)"
		|SIN "\(" VALUE VALUE VALUE VALUE VALUE "\)"
		|EXP "\(" VALUE VALUE VALUE VALUE VALUE VALUE"\)"
		|PWL "\(" (value_list)+ "\)"
		|SFFM "\(" VALUE VALUE VALUE VALUE VALUE "\)" 
		;

value_list	: VALUE	VALUE
		;

variable_list	:varnm:IDENTIFIER EQUAL varval:VALUE
		<< if(cur_cmd == cSubCkt)
			printf(" %s, %s ",
				$varnm->getText(), $varval->getText());
			/*
			theCurCkt->parse_deflt_value(
				$varnm->getText(), $varval->getText());
			*/
		    else if(cur_cmd == cCktCall)
			printf(" %s, %s ",
				$varnm->getText(), $varval->getText());
			/*
			theCurCkt->parse_ckt_call_param(
				$varnm->getText(), $varval->getText());
			*/
		>>
		;

/*parameter_list 	:IDENTIFIER EQUAL VALUE
		| IC EQUAL VALUEA {COMMA VALUEA {COMMA VALUEA}}
		| ONOFF
		| TEMP EQUAL	VALUE
		;*/
parameter_list 	: vark:IDENTIFIER EQUAL varv:VALUE
		<<
			//printf("\n<%s,%s>",$vark->getText(), $varv->getText());
			//toGraph->parseMosfetParameter($varnk->getText(),$varv->getText());
		>>
		| IC EQUAL VALUEA {COMMA VALUEA {COMMA VALUEA}}
		| ONOFF
		| TEMP EQUAL	VALUE
		;

outvar  : varnm:VAR_NAME LEFTB 
			(varn1:VALUE {COMMA varn2:VALUE} | var:IDENTIFIER) RIGHTB
		<< if(cur_cmd == cPrint || cur_cmd == cPlot){
		    // we only allow one output now
			#ifdef	TRACE
			printf("\n%s(%s, %s) %s", 
					$varnm->getText(), 
					($varn1 == NULL? NULL:$varn1->getText()), 
					($varn2 == NULL? NULL:$varn2->getText()),
					($var   == NULL? NULL:$var->getText()) );
			#endif
			toGraph->parseOUT($varnm->getText(), 
				($varn1 == NULL? NULL:$varn1->getText()), 
				($varn2 == NULL? NULL:$varn2->getText()),
				($var   == NULL? NULL:$var->getText()) );
			/*
		    if(!theCurCkt->ac_mgr->get_output_num()){
				theCurCkt->parse_print($varnm->getText(), 
				$varn1->getText(), 
				($varn2 != NULL? $varn2->getText():NULL));
		    } 
			*/
		}
		/* else if(cur_cmd == cDC){
			theCurCkt->parse_dc_input($varnm->getText(), 
				$varn1->getText(), ($varn2 != NULL? $varn2->getText():NULL));
		}*/ 
		else if(cur_cmd == cAC){
			#ifdef	TRACE
			printf("\n%s, %s, %s ", 
				$varnm->getText(), 
				$varn1->getText(), 
				($varn2 != NULL? $varn2->getText():NULL));
			#endif
			toGraph->parseSRC(
				$varnm->getText(), $varn1->getText(), 
				($varn2 != NULL? $varn2->getText():NULL), NULL, NULL);
			/*
			theCurCkt->parse_ac_input($varnm->getText(), 
				$varn1->getText(), ($varn2 != NULL? $varn2->getText():NULL));
			*/
		}
		>>
		;
					
mix_para_list	: IDENTIFIER {(EQUAL VALUE)}
				;

}	/* class net_parser */

