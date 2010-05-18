#include "tpdd.h"
#include "Analysis.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
using std::cout;
using std::endl;
using std::ofstream;
using std::setw;
using std::setprecision;
using std::ios;
using std::map;

extern ofstream sensFileMag;
extern ofstream sensFilePhase;

int compare (const void * a, const void * b)
{
	// return ( (*(SymbNd**)a)->isSrc - (*(SymbNd**)b)->isSrc );
	return ( (*(SymbNode**)b)->isSrc - (*(SymbNode**)a)->isSrc );
}

int compare_g (const void *a, const void *b)
{
	return (((stag*)a)->rand - ((stag*)b)->rand);
}

int compare_l (const void *a, const void *b)
{
	return (((stag*)b)->rand - ((stag*)a)->rand);
}

void Analysis_tpdd(tpdd * tpe)
{
	int i, j;
	Analysis* analysis = new Analysis(tpe);

	double freq = 1000;
	complex_t H;
	double start = 1, step = 1;

	// int count_number = 0;
	//	cout<<"hello"<<endl;	
	double start_time,finish_time;
	double duration;
	start_time = clock();
	// Curve Output: traverse the whole GRDD directly
	for(j = 0; j < 10; j ++)
	{
		for (i = 0; i < 10; i++)
		{
			//	freq = 1000 + i*500;
			freq = start + i * step;
			//	freq = i * step;
			//		cout << setw(10) << setprecision(2) << freq;
			//			sensFileMag << setw(10) << setprecision(0) << freq;
			//                  cout << "i is :" << i << " j is : " << j << endl;
			H = analysis->Evaluate(tpe->GetRootNode(), freq);
		}
		start *= 10;
		step *= 10;
	}
	finish_time = clock();
	duration =(finish_time - start_time )/1000000;
	cout << "duration is : " << duration << "s" << endl;
	delete analysis;
}

void Analysis_scoeff(tpdd * tpe)
{
	// evaluate s-expanded H(s) at each frequency point
	Analysis* analysis = new Analysis(tpe);
	double time_1 = clock();
	analysis->EvaluateSCoeff(); // evaluate all coefficients of s-expanded H(s)
	double time_2 = clock();
	cout << "EvaluateSCoeff()1 time is  : " << (time_2-time_1)/1000000 << endl;

	double freq;
	complex_t H; // cache every H(frequency)

	// generate the curve
	double start = 1, step = 1;
	int i, j;
	for(int k = 0; k < 10; k++)
	{
		start = 1;
		step = 1;
		for(int j = 0; j < 10; j ++)
		{
			for (int i = 0; i < 10; i++)
			{
				freq = start + i * step;
				H = analysis->EvaluateSCoeffFreq(freq); // core
#ifdef PRINT_TO_FILE
				sensFileMag << H << "\n";
#endif // PRINT_TO_FILE
			}
			start *= 10;
			step *= 10;
		}
	}

	// change component value and re-calculate by s-expand: Li Ji
	// mytpdd1->change_value("GM1", 0.007);
	// mytpdd1->PrintSymbols(); // print new graph info
	time_1 = clock();
	analysis->EvaluateSCoeff(); // evaluate all coefficients of s-expanded H(s)
	time_2 = clock();
	cout << "EvaluateSCoeff()2 time is  : " << (time_2-time_1)/1000000 << endl;

	// generate the curve
	start = 1, step = 1;
	i = j = 0;
	for(int k = 0; k < 10; k++)
	{
		start = 1;
		step = 1;
		for(int j = 0; j < 10; j ++)
		{
			for (int i = 0; i < 10; i++)
			{
				freq = start + i * step;
				H = analysis->EvaluateSCoeffFreq(freq); // core
#ifdef PRINT_TO_FILE
				sensFilePhase << H << "\n";
#endif // PRINT_TO_FILE
			}
			start *= 10;
			step *= 10;
		}
	}
	delete analysis;
}

void Analysis_tpdd_mul(tpdd * tpe, char * symbol_name)
{
	Analysis *analysis = new Analysis(tpe);

	complex_t H_s[10][100];
	double start_freq = 1, step_freq = 1;

	double start_para = 2e-2;
	double step_para = 2e-3 ;
	double symbol_value = 3e-11;
	double freq = 1000;

	double start_time;
	double finish_time;
	double total_start;
	double total_finish;

	int j,k;
	int m = 0;
	int i = 0;
	int count = 0;
	int sen_order = 0;
	sen_order = tpe->Determine_tag(symbol_name);
	total_start = clock();	
	for(k = 0; k < 10; k++)
	{

		m = 0;
		count = 0;
		start_freq = 1;
		step_freq = 1;
		symbol_value = start_para + k*step_para;
		//	int l = tpe->Determine_tag(symbol_name);
		tpe->Modify_value(symbol_value, sen_order, symbol_name);
		start_time = clock();
		for(j = 0; j < 7; j++)
		{
			for(i = 0; i < 10; i++)
			{
				freq = start_freq + i * step_freq;
				H_s[k][count] = analysis->Evaluate(tpe->GetRootNode(), freq);
				//cout << "K is : " << k << "count is : " << count << endl;
				count++;

			}
			start_freq *= 10;
			step_freq *= 10;
		}
		finish_time = clock();
		cout << "calculation time is : " << (finish_time - start_time)/1000000 << endl;
	}
	total_finish = clock();
	cout << "total time is : " << (total_finish - total_start)/1000000 << endl;
	start_freq = 1;
	step_freq = 1;
	m = 0;
	for(j = 0; j < 7; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start_freq + i * step_freq;
#ifdef PRINT_TO_FILE
			sensFileMag << setw(10) << setprecision(4) << log10(freq);
			sensFileMag << setw(20) << setprecision(6) << db20(H_s[0][m])
				<< setw(20) << setprecision(6) << db20(H_s[1][m])
				<< setw(20) << setprecision(6) << db20(H_s[2][m])
				<< setw(20) << setprecision(6) << db20(H_s[3][m])
				<< setw(20) << setprecision(6) << db20(H_s[4][m])
				<< setw(20) << setprecision(6) << db20(H_s[5][m])
				<< setw(20) << setprecision(6) << db20(H_s[6][m])
				<< setw(20) << setprecision(6) << db20(H_s[7][m])
				<< setw(20) << setprecision(6) << db20(H_s[8][m])
				<< setw(20) << setprecision(6) << db20(H_s[9][m]) << endl;
#endif // PRINT_TO_FILE
			m++;
		}
		start_freq *= 10;
		step_freq *= 10;
	}
	delete analysis;

}
void Analysis_tpdd_differential(tpdd * tpe, char * symbol_name)
{
	Analysis *analysis = new Analysis(tpe);

	double freq = 5000;

	int sen_order = 0;
	double start;
	double finish;
	start = clock();
	sen_order = tpe->Determine_tag(symbol_name);
	finish = clock();
	cout << "find time is : " << (finish - start)/1000000 << endl;
	double symbol_value = 1.59e-7;
	start = clock();
	tpe->Modify_value(symbol_value, sen_order, symbol_name);	
	finish = clock();
	cout << "modify time is : " << (finish - start)/1000000 << endl;
	//complex_t H = analysis->Evaluate(tpe->GetRootNode(), freq);
	E_value data;
	data = analysis->Evaluate_complex(tpe->GetRootNode(), freq);	
	cout << "data.num is : " << data.num << "data.den is : " << data.den << endl;

	E_value diff;
	start = clock();
	diff = analysis->Evaluate_complex_diff(tpe->GetRootNode(),symbol_value, symbol_name,sen_order,freq);
	cout << "diff.num is : " << diff.num << " diff.den is : " << diff.den << endl;
	finish = clock();
	cout << "diff computing time is : " << (finish-start)/1000000 << endl;
}

void    Analysis_tpdd_element(tpdd *tpe, char * symbol_name)
{
	int N = SAMPLE_NUM_SENS;
	Analysis *analysis = new Analysis(tpe);
	E_value H_s[N];
	E_value H_d[N];
	complex_t H_diff[N];
	complex_t Hs[N];

	int sen_order = tpe->Determine_tag(symbol_name);
	double symbol_value = tpe->Search_value(sen_order, symbol_name);
	int shortsign = tpe->GetRootNode()->GetSignS();
	int opensign = tpe->GetRootNode()->GetSignO();	

	double freq[N];
	double step = 1;
	for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
			freq[count] = step+j*step;
			count++;
		}
		step *= 10;
	}

	//time_begin = clock();
	for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
			H_s[count] = analysis->Evaluate_complex(tpe->GetRootNode(), freq[count]);
			if(H_s[count].den == complex_t(0))
				Hs[count] = (-1)*H_s[count].num*shortsign*DBL_MAX*opensign;
			else
				Hs[count] = (-1)*H_s[count].num*shortsign/(H_s[count].den*opensign);

			H_d[count] = analysis->Evaluate_complex_diff_factor(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq[count]);
			if(H_s[count].num == complex_t(0))
				H_d[count].num = H_d[count].num * symbol_value * DBL_MAX;
			else
				H_d[count].num = H_d[count].num * symbol_value / H_s[count].num;
			if(H_s[count].den == complex_t(0))
				H_d[count].den = H_d[count].den * symbol_value * DBL_MAX;
			else
				H_d[count].den = H_d[count].den * symbol_value / H_s[count].den;
			H_diff[count] = (H_d[count].num-H_d[count].den);
			count++;			
		}
	}
	//time_end = clock();
	//cout << "time_diff is : "<< (time_end - time_begin) << endl;

	double *mag_diff = new double[N];
	double *phase_diff = new double[N];
	//complex_t a  = complex_t(0,1);
	for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
			mag_diff[count] = H_diff[count].real();
		        phase_diff[count] = H_diff[count].imag()/arg(Hs[count]);
#ifdef PRINT_TO_FILE
			sensFileMag << setw(10) << setprecision(10) << freq[count];
			sensFileMag << setw(20) << setprecision (10) 
				<< setiosflags(ios::fixed) << mag_diff[count] << endl;
			sensFilePhase << setw(10) << setprecision(10) << freq[count];
			sensFilePhase << setw(20) << setprecision (10)
				<< setiosflags(ios::fixed) << phase_diff[count] << endl;
#endif // PRINT_TO_FILE
			count++;
		}		
	}
	delete analysis;
	delete [] mag_diff;
	delete [] phase_diff;
	//system("matlab -nosplash -r SizingSensitivity &");
	//system("matlab -r SizingSensitivity &");
}
void    Analysis_tpdd_freq_constant(tpdd *tpe, char *symbol_name)
{
	cout << "********************************" << endl;
	Analysis *analysis = new Analysis(tpe);
	int i,j;
	//	double start_freq = 10;
	//	double muti_freq = 10;
	E_value H[6][100];
	E_value H_d[6][100];
	// E_value H_d_conj[6][100];
	complex_t H_diff[6][100];
	complex_t H_diff_conj[6][100];
	complex_t Hs[6][100];
	int sen_order = 0;
	sen_order = tpe->Determine_tag(symbol_name);
	double start_element = 500;
	double step_element = 10;
	double symbol_value = 1000;
	double freq = 10;
	int k;
	//	start_freq = 10;
	for(k = 0; k < 6; k++)
	{
		for(j = 0; j < 100; j++)
		{
			symbol_value = start_element + j*step_element;
			tpe->Modify_value(symbol_value, sen_order, symbol_name);
			H[k][j] = analysis->Evaluate_complex(tpe->GetRootNode(),freq);
		}
		freq *=10;

	}
	freq = 10;
	for(k = 0;k <6;k++ )
	{
		for(j = 0; j < 100; j++)
		{
			symbol_value = start_element +j*step_element;
			tpe->Modify_value(symbol_value,sen_order, symbol_name);
			H_d[k][j] = analysis->Evaluate_complex_diff_factor(tpe->GetRootNode(),symbol_value,symbol_name,sen_order,freq);
		}
		freq *= 10;
	}

	int shortsign = tpe->GetRootNode()->GetSignS();
	int opensign = tpe->GetRootNode()->GetSignO();
	for(k = 0; k < 6;k++)
	{
		for(j = 0; j < 100; j++)
		{
			if(H[k][j].den == complex_t(0))
			{
				cout << "\n Divide by zero. " << endl;
				Hs[k][j] = -H[k][j].num * DBL_MAX;
				H_diff[k][j] = (-1)*(H_d[k][j].num*H[k][j].den - H_d[k][j].den*H[k][j].num) * DBL_MAX;
				H_diff_conj[k][j] = (-1)*((-1)*conj(H_d[k][j].num)*conj(H[k][j].den) +conj( H_d[k][j].den)*conj(H[k][j].num))
					* DBL_MAX;
			}
			else
			{
				Hs[k][j] = (-1)*H[k][j].num*shortsign/(H[k][j].den*opensign);
				// cout << "Hs[i] is : " << Hs[i] << endl;
				// cout << "db is : " << db20(Hs[i]) << endl;
				H_diff_conj[k][j]=(-1)*shortsign*opensign*((1)*conj(H_d[k][j].num)*conj(H[k][j].den)- conj( H_d[k][j].den)*conj(H[k][j].num))/(conj(H[k][j].den)*conj(H[k][j].den));
				H_diff[k][j] = (-1)*shortsign*opensign*((H_d[k][j].num)*(H[k][j].den) -( H_d[k][j].den)*(H[k][j].num))/((H[k][j].den)*(H[k][j].den));
				// cout << "diff is : " << H_diff[i] << endl;
			}

		}
	}

	complex_t a = complex_t(0,1);
	complex_t mag_diff[6][100];
	for(k = 0; k < 6; k++)
	{
		for(j = 0; j < 100; j++)
		{
			mag_diff[k][j]=((H_diff[k][j]*conj(Hs[k][j])+(H_diff_conj[k][j])*Hs[k][j])/(2*magn(Hs[k][j])));				
		}	
	}
	freq = 10;
	for(i = 0; i < 100; i++)
	{
		freq = 10;
		symbol_value = start_element + i*step_element;
#ifdef PRINT_TO_FILE
		sensFileMag << setw(10) << setprecision(4) << symbol_value;
		sensFileMag << setw(20) << setprecision(4)<<symbol_value*mag_diff[0][i]/magn(Hs[0][i])
			<< setw(20) << setprecision(4)<<symbol_value*mag_diff[1][i]/magn(Hs[1][i])
			<< setw(20) << setprecision(4)<<symbol_value*mag_diff[2][i]/magn(Hs[2][i])
			<< setw(20) << setprecision(4)<<symbol_value*mag_diff[3][i]/magn(Hs[3][i])
			<< setw(20) << setprecision(4)<<symbol_value*mag_diff[4][i]/magn(Hs[4][i])
			<< setw(20) << setprecision(4)<<symbol_value*mag_diff[5][i]/magn(Hs[5][i])<<endl;
#endif // PRINT_TO_FILE
	}
}

void Tree_modify(tpdd *tpe, char * symbol_name)
{
	cout << "tree modify" << endl;
	int sen_order = 0;
	cout << "symbol_name is : " << symbol_name << endl;

	sen_order = tpe->Determine_tag(symbol_name);
	cout << "sen_order is : " << sen_order  << endl;	

	double symbol_value = 0;
	symbol_value = tpe->Search_value(sen_order,symbol_name);
	cout << "symbol_value is : " << symbol_value << endl;	
	double time1 = clock();
	tpe->ModifyStructure(sen_order, symbol_name);
	double time2 = clock();
	cout << "modify time is : " << (time2 - time1)/1000000 << endl;
	cout << "1" << endl;
	tpe->sexpand();
	cout << "2" << endl;
	tpe->scoeffNew();
	cout << "3" << endl;
	time1 = clock();
	Analysis_scoeff(tpe);
	time2 = clock();
	cout  << "time is : " << (time2-time1)/1000000 << endl;
	cout << "4" << endl;
}


//Added By Ma Diming for Mosfet Senstivity
void	FindSensitivity2DR(tpdd* tpe, Analysis* analysis, char* symbol_name, double symbol_coef, double* mag_diff, double* phase_diff, double* freq)
{
	int N = SAMPLE_NUM_SENS;
	E_value H_s[N];
	E_value H_d[N];
	complex_t H_diff[N];
	complex_t Hs[N];

	double tStart = 0, tEnd = 0;
	tStart = clock();
	int sen_order = tpe->Determine_tag(symbol_name);
	double symbol_value = tpe->Search_value(sen_order, symbol_name);
	int shortsign = tpe->GetRootNode()->GetSignS();
	int opensign = tpe->GetRootNode()->GetSignO();	
	tEnd = clock();
	cout << "Time Reparation = " << 1.0*(tEnd-tStart)/1e6 << endl;

	//double freq[N];
	/*double step = 1;
	for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
			freq[count] = step+j*step;
			count++;
		}
		step *= 10;
	}*/

	//double step = 1;
	double t1Start =0, t2Start=0, t1End=0,t2End=0, t3Start=0,t3End=0, t1=0, t2=0, t3=0;
	for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
//			freq[count] = step + j * step;
			t3Start = clock();
			H_s[count] = analysis->Evaluate_complex(tpe->GetRootNode(), freq[count]);
			t3End = clock();
			t3 += t3End-t3Start;
			t1Start = clock();
			if(H_s[count].den == complex_t(0))
				Hs[count] = (-1) * H_s[count].num * shortsign * DBL_MAX * opensign;
			else
				Hs[count] = (-1) * H_s[count].num * shortsign / (H_s[count].den*opensign);
			t1End = clock();
			t1 += t1End-t1Start;
			t2Start = clock();
			H_d[count] = analysis->Evaluate_complex_diff_factor(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq[count]);
			t2End = clock();
			t2 += t2End-t2Start;
			//cout << "Time Sens = " << 1.0*(tEnd-tStart)/1e6 << endl;
			t1Start = clock();
			if(H_s[count].num == complex_t(0))
				H_d[count].num = H_d[count].num * symbol_value * DBL_MAX;
			else
				H_d[count].num = H_d[count].num * symbol_value / H_s[count].num;
			if(H_s[count].den == complex_t(0))
				H_d[count].den = H_d[count].den * symbol_value * DBL_MAX;
			else
				H_d[count].den = H_d[count].den * symbol_value / H_s[count].den;
			H_diff[count] = (H_d[count].num-H_d[count].den);
			//ToDo: Normalize the final result
			mag_diff[count] += symbol_coef * H_diff[count].real();
		        phase_diff[count] += symbol_coef * H_diff[count].imag()/arg(Hs[count]);
			count++;	
			t1End = clock();
			t1 += t1End-t1Start;		
		}
//		step *= 10;
	}
	cout << "Time Sens Other = "<<t1*1.0/1e6<<" Sens = "<<t2*1.0/1e6<<" Evalute = "<<t3*1.0/1e6<<endl;

	/*for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
			mag_diff[count] = H_diff[count].real();
		        phase_diff[count] = H_diff[count].imag()/arg(Hs[count]);
			count++;
		}		
	}*/
}

void    FindSensitivity2D(tpdd* tpe, Analysis* analysis)
{
	Mosfet* pMosfetHead = tpe->GetMosfetList();
	Mosfet* pMosfet = pMosfetHead;

	int N = SAMPLE_NUM_SENS;
	double freq[N];
	double step = 1;
	for(int count=0, i=0; i<7; i++)
	{
		for(int j=0; j<9; j++)
		{
			freq[count] = step+j*step;
			count++;
		}
		step *= 10;
	}
	double* magDiff = new double [N];
	double* phaseDiff = new double [N];
	//while(pMosfet)
	{
		for(int i=0; i<N ;i++)
			magDiff[i] = 0, phaseDiff[i] = 0;
	//	map<char*, double>* modelDevices = pMosfet->getModelDevices();
	//	if(modelDevices == NULL)
	//		pMosfet = pMosfet->next;
		double tStart = clock();
	//	for(map<char*, double>::const_iterator it = modelDevices->begin();
	//	    it != modelDevices->end(); ++it)
		{
	//		char* symbolName = (char*)it->first;
	//		if(symbolName == NULL)
	//			continue;
        //		double value = (double)it->second;
	//		FindSensitivity2DR(tpe,analysis,symbolName,value,magDiff,phaseDiff,freq);
			FindSensitivity2DR(tpe,analysis,"CC",1,magDiff,phaseDiff,freq);
			//free(symbolName);
		}
		double tEnd = clock();
	//	cout << "Mosfet #" << pMosfet->name << " W = " << pMosfet->w << " L = " << pMosfet->l << endl;
		cout << "Time / Mosfet = " << (tEnd-tStart)*1.0/1e6 <<endl;
		#ifdef PRINT_TO_FILE
		for(int count=0, i=0; i<7; i++)
		{
			for(int j=0; j<9; j++)
			{
				sensFileMag << setw(10) << setprecision(10) << freq[count];
				sensFileMag << setw(20) << setprecision (10) 
					<< setiosflags(ios::fixed) << magDiff[count] << endl;
				sensFilePhase << setw(10) << setprecision(10) << freq[count];
				sensFilePhase << setw(20) << setprecision (10)
					<< setiosflags(ios::fixed) << phaseDiff[count] << endl;
				count++;
			}		
		}
		#endif
	//	delete modelDevices;
	//	pMosfet = pMosfet->next;
	}
	delete [] magDiff;
	delete [] phaseDiff;
	//system("matlab -nosplash -r SizingSensitivity &");
	//system("matlab -r SizingSensitivity &");
}
#ifdef CONSIDER_MATCHED
Mosfet* FindMosfet(tpdd* tpe, char* name)
{
	Mosfet* pMosfetHead = tpe->GetMosfetList();
	Mosfet* pMosfet = pMosfetHead;
	while(pMosfet)
	{
		if(!strcmp(pMosfet->name,name))
			return pMosfet;
		pMosfet = pMosfet->next;
	}
	return NULL;
}

void    MakeMosfetMatch(tpdd* tpe)
{
	Mosfet* pMosfetHead = tpe->GetMosfetList();
	Mosfet* pMosfet = pMosfetHead;
	//Hard Code
	while(pMosfet)
	{
		if(!strcmp(pMosfet->name,"M1"))
		  pMosfet->pMatch = FindMosfet(tpe,"M2");
		else if(!strcmp(pMosfet->name,"M1"))
		  pMosfet->pMatch = FindMosfet(tpe,"M1");
		else if(!strcmp(pMosfet->name,"M3"))
		  pMosfet->pMatch = FindMosfet(tpe,"M4");
		else if(!strcmp(pMosfet->name,"M4"))
		  pMosfet->pMatch = FindMosfet(tpe,"M3");
		pMosfet = pMosfet->next;
	}
}
#endif
