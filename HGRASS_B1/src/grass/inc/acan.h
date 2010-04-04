#ifndef AC_ANALYSIS_H
#define AC_ANALYSIS_H

/*
 *    $RCSfile: acan.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include "unixstd.h"
#include "globals.h"
#include "dddbase.h"
#include "xdraw.h"
#include "coeff.h"
#include "fcoeff.h"

using namespace std;

class DCAnalysis;
class ACAnalysis;
class ACAnalysisManager;
class Cofactor;
class ExpList;
class Circuit;

typedef enum {
    oMEG,    // magnitude -- default output format
    oREAL,    // real part
    oIMAG,    // imagine part
    oPHASE,  // phase part
    oNOISE,  // noise spetral density (V^2 or A^2 volt^2/Hz)
    oDB    // decibel -- 20*log10(magnitude)
} OutFormat;

/* the variation type for frequency swapping */
typedef enum {
    vDEC, // decade variation
    vOCT, // octave variation
    vLIN // linear variation
} VariationType;

class DMatrix{
protected:
	ExpList*** elem_list;
	int size;
  friend class ACAnalysisManager;
public:
	DMatrix(int _size){
		size = _size;
		elem_list = new ExpList**[size+1];
		for(int i=1;i<=size;i++){
			elem_list[i] = new ExpList*[size+1];
			for(int j=1;j<=size;j++)
				elem_list[i][j]=NULL;
		}
	}
	~DMatrix(){
		for(int i = 1; i <= size; i++)
			delete [] elem_list[i];
		delete [] elem_list;
	}

	 void set_element(ExpList* _elem,int row,int col){
		elem_list[row][col] = _elem;
	 }
};

class QVector{
protected:
	ExpList** elem_list;
	int size;
  friend class ACAnalysisManager;
public:
	QVector(int _size){
		size = _size;
		elem_list = new ExpList*[size+1];
		for(int i=1;i<=size;i++){
			elem_list[i]=NULL;
		}
	}
	~QVector(){
		delete [] elem_list;
	}

	 void set_element(ExpList* _elem,int row){
		elem_list[row] = _elem;
	 }
};

// the input and output lists
class IOList {

  protected:
  	char				*keyword;
    int        	   node1; // the positive node
    int        	   node2; // the negative node
    int    	   dep_freq; // 0: independent, 1, yes, 2, special function
    DeviceType     type; // only VOL or CUR allowed
    OutFormat      format;
    double         value; // value of the independent source
    IOList         *next;

  friend class ExpList;
  friend class Cofactor;
  friend class ACAnalysis;
  friend class ACAnalysisManager;

  public:
    IOList( char*, int, int, DeviceType, double, 
		OutFormat format = oMEG, int dep_freq = 0);
    ~IOList();
    IOList  * Next() { return next;}
    void Next( IOList *_next) { next = _next;}
};
    
class DCAnalysis {

  private:
    int    index;     // the internal No of this DC analysis
    int    num_pts; 	// total dc sampling points
    double    dcstart; 	// start dc point
    double    dcstop; 	// stop dc point
    double    dcstep; 	// dc step
    double    *dc_pts; 	// calculated dc points

  friend class ACAnalysisManager;
  friend class ExpList;
  friend class Cofactor;

  public:

    DCAnalysis( double _dcstart, double _dcstop, double _dcstep);

    ~DCAnalysis();
    int     check_params();
    void    cal_dc_pts();
};

class ACAnalysis {

  private:
    int    index;     // the internal No of this AC analysis
    VariationType   var_type; 
    int    np; 		// number of points in each variation range
    int    num_pts; 	// total frequency sampling points
    double    fstart; 	// start frequency
    double    fstop; 	// stop frequency
    double    *freq_pts; 	// calculated frequency points

    /* approximation */
    double    mag_err; // relative error for magnitude
    double    phs_err; // phase error for magnitude

  friend class ACAnalysisManager;
  friend class ExpList;
  friend class Cofactor;

  public:

    ACAnalysis(VariationType _type, 
        int _np, double _fstart, double _fstop);

    ~ACAnalysis();
    int     check_params();
    void    cal_samp_pts();
    void    cal_freq_pts();
};

/* cofacor = (-1)^(row+col)*det(A) */
class Cofactor {

  protected:

    DDDmanager    	*ddd_mgr; // the ddd manager used
    DDDnode     	*cddd; // complex_t DDD
    DDDnode     	*sddd; // simplifed DDD.
    int        sign;  // the sign of the cofactor
    int        osign;  // the original sign of the cofactor
    int        row;   // row deleted (in terms of MNA index)
    int        col;   // column deleted
    double       	freq;  // previous freq used
    complex_t             value; // the value of previous calculation
    Cofactor    	*next;
    CoeffList    	*coeff_list; // coefficient list 
    FCoeffList    	*fcoeff_list; // fully expanded coefficient list 
    FCoeffList    	*sfcoeff_list; // simplified fcoefficient list 

  friend class ACAnalysis;
  friend class ACAnalysisManager;

  public:

    Cofactor(DDDmanager *,DDDnode *, int , int , int );
    ~Cofactor();
    void     initial_calc();
    void     sddd_init();
    complex_t evaluate( double, HasherTree *, bool); 
    int    get_row() { return row;}
    int    get_col() { return col;}
    int    get_sign() { return sign;}
    void     set_sign(int _sign) { sign = _sign;}
    DDDnode *get_cddd() { return cddd;}
    DDDnode *get_sddd() { return sddd;}
    void    set_cddd( DDDnode *_cddd ) { cddd = _cddd; }
    void    set_sddd( DDDnode *_sddd ) { sddd = _sddd; }
    DDDmanager    *get_ddd_mgr() { return ddd_mgr; }
    FCoeffList *get_fcoeff_list() { return fcoeff_list;}
    FCoeffList *get_sfcoeff_list() { return sfcoeff_list;}
    void set_sfcoeff_list() { sfcoeff_list = NULL;}

    Cofactor * Next() { return next;}
    void  Next(Cofactor *_next) { next = _next;}

    /* coefficient */
    complex_t coeff_evaluate(double ); 
    complex_t fcoeff_evaluate(double);


    /* approximation */
    void simp_cofactor( double freq, double mag_ref );
    void simp_fcofactor( double freq, double mag_ref );
    void fcoeff_supp(double freq, double meg_ref );  
	 /* Modified by Zhao Li, July 2001*/
    boolean elem_elim(double *_freq, complex_t *val,int _fnum, 
                int index, MNAMatrix* matrix, PolyType _type, bool _cancel_remove);
    int  find_diag_index(Branch *, int, IndexMatrix *);
    void  find_diag_index(Branch *, IndexMatrix *, int*);
	 /* Modified by Zhao Li, July 2001*/
	 void fcoeff_elem_elim(int index, PolyType _type);
	 void fcoeff_elem_elim(int index, MNAMatrix* matrix);

    void keep_fcoeff_ddd() { fcoeff_list->keep_fcoeff_ddd();}
    void unkeep_fcoeff_ddd() { fcoeff_list->unkeep_fcoeff_ddd();}

    void keep_sfcoeff_ddd() { sfcoeff_list->keep_fcoeff_ddd();}
    void unkeep_sfcoeff_ddd() { sfcoeff_list->unkeep_fcoeff_ddd();}

    void keep_cddd() { ddd_mgr->Keep(cddd);}
    void unkeep_cddd() { ddd_mgr->UnKeep(cddd);}
    void keep_sddd() { ddd_mgr->Keep(sddd);}
    void unkeep_sddd() { ddd_mgr->UnKeep(sddd);}

    void fcoeff_decancel(int, int, int, int);
    void whole_elem_elim(double *, complex_t *, int, PolyType, 
                char *, int, int, int, int);
    void print_fcoeff_list(ostream & out);
    void print_sfcoeff_list(ostream & out);

    // for pole/zero extraction
    int whole_elem_elim_polezero(char *, int, int, int, int, int);
    void factor_polezero(MNAMatrix *, int);

	// for noise analysis
	void maple_cmd( ostream& out );
};

/* Due to superposition, each independent input (voltage, current)
** has corresponding output contribution. Following class
** sperates each contribution of independent source and 
** combine all of them togather for final output variable 
** calculation.
*/

class ExpList { 

  protected:

    IOList   *input;   // the input nodes expression
	 RHSElement *rhs_elem;
    double   coeff;    // numerical coefficient of the expression
    Cofactor *num_list; /* the numerator cofactor list for input node 
                 		if it is not ground node */
    Cofactor *den_list; /* the denominator cofactor list for a input node 
					    if it is not ground node */
    int     num_pts; // number of sampling points.
    int	    index;	// index of the polynomail expression.
    ExpList    *next;

    /* approximation */
    complex_t *num_val;  // values for numerator at each freq point
    complex_t *den_val;  // values for denominator at each freq point

  friend class ACAnalysisManager;
  friend class Circuit;
  public:

    ExpList( IOList *, int , int,  double _coeff = 1.0, RHSElement* = NULL);
    ~ExpList();
    void     new_freq_number( int );
    void     add_num_cofactor( Cofactor *);
    void     add_den_cofactor( Cofactor *);
    complex_t evaluate( double, int, MNAMatrix *, HasherTree *, bool); 
    complex_t dc_evaluate(HasherTree *, bool); 

    complex_t noise_evaluate(double, int, HasherTree *, bool); 
	void maple_cmd( ostream& out );

    /* coefficient */
    complex_t coeff_evaluate( double); 
    complex_t coeff_noise_evaluate( double); 
    complex_t fcoeff_evaluate( double,int); 
	void calc_sfcoeff_list();

    ExpList *Next() { return next;}
    void Next(ExpList *_next) { next = _next;}

    /* approximation */
    void simp_expr( double , double , int );
    void simp_fcexpr(double , int );
    void fcoeff_supp_expr(double , double , int );
    void fcoeff_suppress_new(double *, int );
    double elem_elim_both_val(DDDmanager *, MNAMatrix*, int, double*, int, bool);
    int elem_elim_both(DDDmanager *,MNAMatrix*, int, double*, int, bool);
    double whole_elem_elim_both_val(DDDmanager *, double *, int , 
                    char *, int , int , int , int , bool);
    int whole_elem_elim_both(DDDmanager *, MNAMatrix*, double *, int , 
                    char *, int , int , int , int , bool);
    bool factor_ddd_node(DDDmanager *, MNAMatrix *, int, double *, int, bool);
    double factor_ddd_node_val(DDDmanager *, MNAMatrix *, int, double *, int, bool);
    int  find_diag_index(Branch *, int , IndexMatrix *);
    void  find_diag_index(Branch *, IndexMatrix *, int*);
    void  set_node_ground(Branch *, int , IndexMatrix *);

	 void statDDD(DDDmanager* ddd_mgr, HasherTree* ht);
};


class ACAnalysisManager {
 
 protected:
 
    DDDmanager     *ddd_mgr;// the DDD manager used
    DCAnalysis     *dc_command;
    ACAnalysis     *command;
    MNAMatrix      *matrix;
    RHSVector      *rhs;
    DMatrix        *d_matrix;
    QVector        *q_vector;
    int        	 num_pts; // number of frequency points
    int		   num_exprs; // number of polynomial expressions
    complex_t        *out_value; // output result vector 
    complex_t        *exact_value; // exact output result vector
    GraphOutput    *graph_out; // the vector structure for display
    ExpList        *poly; // the output linear polymonial expression

    static IOList    *inputs; // the input stimula
    static int       input_num; // number of input stimulas
    static IOList    *outputs; // the output nodes
    static int       output_num; // number of output stimulas

    /* simplification */
    int ddd_simplied;
    int fcoeff_simped;

  friend class Circuit;

  public:

    ACAnalysisManager(MNAMatrix *_matrix, RHSVector* _rhs, DDDmanager * _ddd_mgr);
    ~ACAnalysisManager();
    void         add_exprs( ExpList *expr); 
    DDDnode *    build_cofactor_ddd( int _row, int _col ); 
    DDDnode *    build_y_cofactor_ddd( int _row, int _col ); 
    void    	  build_d_matrix(); 
    void    	  build_q_vector(); 
    void         build_sys_ddd();
    void         build_y_ddd();
    complex_t      evaluate( double , int, HasherTree * , bool);
    complex_t      dc_evaluate( HasherTree * , bool);

    void         free_command();
    void         new_command( ACAnalysis *);
    void         add_command( ACAnalysis *);
    void         add_dc_command( DCAnalysis *);
    void         new_output( IOList *);


    void         dc_analysis(); 
    void         ac_analysis(); 
    void         build_dc_expressions(); 
    void         build_expressions(); 
    void         dc_sweep(); 
    void         freq_sweep(); 
    void         graph_display(char *);
    void         graph_compare(char *);
	void		 output(FILE*);
    //void         text_output(char *_filename);

    // coefficient construction
    void        coeff_build_expressions();    
    complex_t     coeff_evaluate( double);
    void        coeff_freq_sweep();
    void        coeff_ac_analysis();

    // fully expanded coefficient list construction
    void        fcoeff_build_expressions_from_ddd();    
    void        fcoeff_build_expressions_from_sddd();    
    complex_t     fcoeff_evaluate( double _freq, int findex);
    void        fcoeff_freq_sweep();
    void        build_fcoeff_from_ddd();
    void        build_fcoeff_from_sddd();

    // approximation
    void        simplify_ddd();
    void        simplify_coefflist();
    void        fcoeff_suppress();
    void        fcoeff_suppress_new();
    void        elem_elimination();
    void        elem_elimination_both();
    void        fcoeff_elem_elimination();
    void        fcoeff_decancel(int, int, int, int);
    void        whole_elem_elim(char *, int, int, int, int);
    int         whole_elem_elim_both(char *, int, int, int, int);
    double      whole_elem_elim_both_val(char *, int, int, int, int);
    void        factor_ddd_node(char *);

    /* garbage collection */
    void         fcoeff_GC();
    void         forceGC(const char *);
    void         fcoeff_forceGC();
    void         sfcoeff_forceGC();
    void         cddd_forceGC();
    void         sddd_forceGC();

	/* noise analysis */
    complex_t      noise_evaluate( double, int, HasherTree *, bool);
    void         noise_freq_sweep(); 
    void         noise_analysis(); 

    complex_t      coeff_noise_evaluate( double); 
    void         coeff_noise_freq_sweep(); 
    void         coeff_noise_analysis(); 
    void	 maple_cmd(ostream& );
    
    int          get_output_num() const { return output_num;}
    ACAnalysis   *get_command() { return command;}
    DCAnalysis   *get_dc_command() { return dc_command;}

    void         print_fcoeff_list(ostream & out);
    void         print_sfcoeff_list(ostream & out);
    static  	 void add_input( IOList *_inlist );
    static  	 void add_output( IOList *_output );

    void         new_freq_sweep(char *_spec);

    // for pole/zero extration
    int whole_elem_elim_polezero(char*,int, int, int, int, int);
    void factor_polezero(int);

	 void statDDD(HasherTree* ht);
};

// independant function
double noise_source_func(double);

#endif
