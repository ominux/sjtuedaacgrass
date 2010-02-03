#include "tpdd.h"
#include "Analysis.h"
#include <iostream>
using std::cout;
using std::endl;
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
			//			dataFile << setw(10) << setprecision(0) << freq;
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
				dataFile << H << "\n";
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
				dataFile2 << H << "\n";
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
			dataFile << setw(10) << setprecision(4) << log10(freq);
			dataFile << setw(20) << setprecision(6) << db20(H_s[0][m])
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

void Analysis_tpdd_differential_freq(tpdd * tpe, char * symbol_name)
{
	Analysis * analysis = new Analysis(tpe);
	int i, j;

	double freq = 1000;
	E_value H_s[100];
	E_value H[100];
	complex_t H_diff[100];
	complex_t Hs[100];

	int sen_order = 0;
	sen_order = tpe->Determine_tag(symbol_name);	
	double symbol_value = 3e-12;
	double start = 1, step = 1;
	double start_time, finish;
	int count = 0;
	// complex_t test[100];
	start_time = clock();
	for(j = 0; j < 10; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start + i * step;
			H_s[count] = analysis->Evaluate_complex(tpe->GetRootNode(),freq);
			//test[count] = analysis->Evaluate(tpe->GetRootNode(), freq);	
			count++;
		}
		start *=10;
		step *=10;
	}
	finish = clock();
	cout << "time is : " << (finish-start_time)/1000000 << endl;
	start = 1;
	step = 1;
	count = 0;
	start_time = clock();
	for(j = 0; j < 10; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start + i * step;
			H[count]= analysis->Evaluate_complex_diff(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq);
			count++;
		}
		start *= 10;
		step *= 10;	
	}
	finish = clock();
	cout << "time_diff is " << (finish - start_time)/1000000 << endl;
	int shortsign = tpe->GetRootNode()->GetSignS();
	int opensign = tpe->GetRootNode()->GetSignO();
	// int test_count = 0;
	for(i = 0; i < 100; i++)
	{
		if(H_s[i].den == complex_t(0))
		{
			cout << "\n Divide by zero. " << endl;
			Hs[i] = (-1)*H_s[i].num*DBL_MAX;
			H_diff[i] = (-1)*(H[i].num*H_s[i].den - H[i].den*H_s[i].num) * DBL_MAX;
		}
		else
		{

			Hs[i] = (-1)*H_s[i].num*shortsign/(H_s[i].den*opensign);
			H_diff[i]=(-1)*shortsign*opensign*(H[i].num*H_s[i].den - H[i].den*H_s[i].num)/(H_s[i].den*H_s[i].den);
			//cout << "test_count is : " << test_count << endl;
			//test_count++;
		}

	}
	complex_t *mag_diff;
	mag_diff = new complex_t[100];
	for(i = 0; i < 100; i++)
	{

		mag_diff[i]=(H_diff[i]*conj(Hs[i])+conj(H_diff[i])*Hs[i])/(2*magn(Hs[i]));
		cout << "mag_diff is " << mag_diff[i] << endl;
	}

	int m = 0;
	start = 1;
	step = 1;
	for(j = 0; j < 10; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start + i*step;
#ifdef PRINT_TO_FILE
			dataFile << setw(10) << setprecision(10) << log10(freq);
			dataFile << setw(20) << setprecision(4)
				<< setiosflags(ios::fixed) <<3e-12*2*PI*freq* mag_diff[m]/magn(Hs[m]) << endl;
#endif // PRINT_TO_FILE
			m++;
		}
		start *=10;
		step *=10;
	}

}
void test(tpdd *tpe, char * symbol_name)
{
	Analysis *analysis = new Analysis(tpe);

	double freq = 5000;

	int sen_order = 0;
	double start;
	double finish;
	start = clock();
	sen_order = tpe->Determine_tag(symbol_name);
	finish = clock();
	cout << "find time is : " << (finish - start)/1000 << endl;	
	cout << "***********sensitivity tag information is : " << sen_order << "********** " << endl; 
	E_value data;
	data = analysis->Evaluate_complex(tpe->GetRootNode(),freq);
	cout << "**************the information of H(s)********" << endl;
	cout << "data.num is : " << data.num << "data.den is  : " << data.den << endl;
	E_value diff;
	double symbol_value = 1e+3;
	//	diff = analysis->Evaluate_complex_diff(tpe->GetRootNode(), symbol_value, symbol_name, sen_order, freq);
	//	cout << "diff.num is : " << diff.num << "diff.den is : " << diff.den
	//<< endl;
	//	symbol_value = 400000;
	//	tpe->Modify_value(symbol_value, sen_order, symbol_name);
	// double initial_value = tpe->Search_value(sen_order, symbol_name);
	E_value diff_factor;
	diff_factor = analysis->Evaluate_complex_diff_factor(tpe->GetRootNode(), symbol_value, symbol_name, sen_order, freq);
	cout << "diff_factor.num is : " <<diff_factor.num << "diff_factor.den is : " << diff_factor.den << endl;
}
void    Analysis_tpdd_element(tpdd *tpe, char * symbol_name)
{
	Analysis *analysis = new Analysis(tpe);

	int i,j;

	double freq = 1000;
	E_value H_s[100];

	E_value H_d[100];
	// E_value H_d_conj[100];
	complex_t H_diff[100];
	complex_t H_diff_conj[100];
	complex_t Hs[100];

	int sen_order = 0;
	sen_order = tpe->Determine_tag(symbol_name);
	double symbol_value = 0;
	symbol_value = tpe->Search_value(sen_order, symbol_name);
	//	cout << "symbol_value is : " << symbol_value << endl;
	double start = 1;
	double step = 1	;
	int count = 0;
	// complex_t test[100];
	//	tpe->Element_conj(freq);
	double time_begin = clock();
	for(j = 0; j < 7; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start + i*step;
			H_s[count] = analysis->Evaluate_complex(tpe->GetRootNode(), freq);
			//			cout << "num is : " << H_s[count].num << endl;
			//			cout << "den is : " << H_s[count].den << endl;
			count++;
		}
		start *=10;
		step *=10;
	}
	double time_end = clock();
	cout << "time is : "<< (time_end - time_begin) << endl;

	time_begin = clock();

	start = 1;
	step = 1;
	count = 0;
	E_value test1;

	for(j = 0; j < 7; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start + i*step;
			//test1 = analysis->Evaluate_complex_diff(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq);			
			H_d[count] = analysis->Evaluate_complex_diff_factor(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq);
			//	test1 = analysis->Evaluate_complex_diff(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq);
			//			cout << "H_d " << H_d[count].num << endl;
			//			cout << "E_value is : " << test1.num << endl;
			count++;			
		}
		start *=10;
		step *=10;
	}
	time_end = clock();
	cout << "time_diff is : "<< (time_end - time_begin) << endl;
	start = 1;
	step = 1;
	count = 0;

	//count = 0;
	/*	for(j = 0; j < 7; j++)
		{
		for(i = 0; i < 10; i ++)
		{
		freq = start + i*step;
		H_d_conj[count] = analysis->Evaluate_complex_diff_factor(tpe->GetRootNode(),symbol_value, symbol_name, sen_order, freq);
		}
		}
		*/

	int shortsign = tpe->GetRootNode()->GetSignS();
	int opensign = tpe->GetRootNode()->GetSignO();	
	//	cout << "shotr sign is : " << shortsign << endl;
	//	cout << "open sign is : " << opensign << endl;

	for(i = 0; i < 70; i++)
	{
		if(H_s[i].den == complex_t(0))
		{
			cout << "\n Divide by zero. " << endl;
			Hs[i] = -H_s[i].num * DBL_MAX;
			H_diff[i] = (-1)*(H_d[i].num*H_s[i].den - H_d[i].den*H_s[i].num) * DBL_MAX;
			H_diff_conj[i] = (-1)*((-1)*conj(H_d[i].num)*conj(H_s[i].den) +conj( H_d[i].den)*conj(H_s[i].num)) * DBL_MAX;
		}
		else
		{
			Hs[i] = (-1)*H_s[i].num*shortsign/(H_s[i].den*opensign);
			//			cout << "Hs[i] is : " << Hs[i] << endl;
			//			cout << "db is : " << db20(Hs[i]) << endl;			
			H_diff_conj[i]=(-1)*shortsign*opensign*((1)*conj(H_d[i].num)*conj(H_s[i].den)- conj( H_d[i].den)*conj(H_s[i].num))/(conj(H_s[i].den)*conj(H_s[i].den));
			H_diff[i] = (-1)*shortsign*opensign*((H_d[i].num)*(H_s[i].den) -( H_d[i].den)*(H_s[i].num))/((H_s[i].den)*(H_s[i].den));
			//			cout << "diff is : " << H_diff[i] << endl;
		}
	}
	complex_t *mag_diff;
	mag_diff = new complex_t[100];
	complex_t *phase_diff;
	phase_diff = new complex_t[100];
	complex_t a  = complex_t(0,1);
	//	cout << "a is  " << a << endl;
	for(i = 0; i < 70; i++)
	{
		//		cout << "result" << endl;
		mag_diff[i]=((H_diff[i]*conj(Hs[i])+(H_diff_conj[i])*Hs[i])/(2*magn(Hs[i])));
		// phase_diff[i] = (H_diff[i]*a-mag_diff[i]*Hs[i]/magn(Hs[i]))/(Hs[i]*a);
		cout << "phase is : " << phase_diff[i] << endl;
		cout << "mag_diff is " << mag_diff[i] << endl;
	}
	start = 1;
	step = 1;
	count = 0;
	for(j = 0; j < 7; j++)
	{
		for(i = 0; i < 10; i++)
		{
			freq = start + i*step;
#ifdef PRINT_TO_FILE
			dataFile << setw(10) << setprecision(10) << freq;
			dataFile << setw(20) << setprecision (10) 
				<< setiosflags(ios::fixed) << (3.09e-2/magn(Hs[count]))*mag_diff[count] << endl;			
#endif // PRINT_TO_FILE
			count++;
		}		
		start *=10;
		step *= 10;
	}

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
		dataFile << setw(10) << setprecision(4) << symbol_value;
		dataFile << setw(20) << setprecision(4)<<symbol_value*mag_diff[0][i]/magn(Hs[0][i])
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
