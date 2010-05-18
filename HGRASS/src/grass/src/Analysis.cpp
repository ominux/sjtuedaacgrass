/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* Analysis.cpp
* For AC analysis based on TPDD (Tree Pair Decision Diagram).
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/

#include	"Analysis.h"
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

#define		USE_HASHER	1

Analysis::Analysis(tpdd *tpe)
{
	if (!tpe)
	{
		cerr << "\nNeed an tree pair decision diagram object first before analysis.";
		exit(1);
	}
	this->TPE = tpe;
}

E_value Analysis::Evaluate_complex(tpddnode * pRoot, double freq)
{
	tpddnode *num = pRoot->GetPShort();
	tpddnode *den = pRoot->GetPOpen();
	
	pRoot->Mark();

	complex_t val_den, val_num;

	TPE->SetSymbValue(freq);

	TPE->UnReferenceAll(TPE->GetRootNode(), 1);
	TPE->UnMarkAllNodes(TPE->GetRootNode(), 1);

	val_num = EvaluateR(num, freq, (bool) 1);
	val_den = EvaluateR(den, freq, (bool) 1);

	E_value value_all;
	value_all.num = val_num;
	value_all.den = val_den;
	//cout << "val_num is " << val_num << endl;	
	return value_all;	
}
E_value Analysis::Evaluate_complex_diff_factor_conj(tpddnode *pRoot, double symbol_value, char *symb_name,int tag, double freq)
{
        tpddnode *num = pRoot->GetPShort();
        tpddnode *den = pRoot->GetPOpen();
        pRoot->Mark();


        complex_t diff_num;
        complex_t diff_den;
        int list_index = tag;
        double imag = 0;
        double real = 0;
        double trans_value;
        //cout << "&&&&&&&&&&&&&" << endl;
        trans_value = TPE->Search_value(tag, symb_name);
        //cout << "************" << endl;
        //cout << "tran_value is : " << trans_value << endl;
        TPE->Element_conj(freq);
	if(symb_name[0] == 'R')
        {
                real = (trans_value == 0? DBL_MAX: (-1)/(trans_value*trans_value));
          //      cout << "real is : " << real << endl;
                TPE->SetValue(list_index, real);
                TPE->SetImaginary(list_index, 0.0);
        }
        else if(symb_name[0] == 'L')
        {
        //      TPE->SetValue(list_index, 1);
                imag = (trans_value == 0? DBL_MAX : 1/(2*PI*freq*trans_value));
                TPE->SetValue(list_index,1/(imag*imag));
                TPE->SetImaginary(list_index,0.0);
        }
        else if(symb_name[0] == 'C')
        {
            //    cout << "not R or L" << endl;
                TPE->SetValue(list_index,-1);
                TPE->SetImaginary(list_index, 0.0);
        }
	else
	{
	//	cout << "not R or L or C" << endl;
                TPE->SetValue(list_index,1);
                TPE->SetImaginary(list_index, 0.0);
	}
	if(num->GetSymbIndex() > list_index)
        {
                cout << "the numerator of differential is 0." << endl;
                num = TPE->GetpZero();
        }
        if(den->GetSymbIndex() > list_index)
        {
                cout << "the differential  of D(s) is 0." << endl;
                den = TPE->GetpZero();
        }

        TPE->UnReferenceAll(TPE->GetRootNode(), 1);
        TPE->UnMarkAllNodes(TPE->GetRootNode(), 1);

        diff_num = EvaluateR_diff_factor(num, freq,1,list_index, symb_name);
        diff_den = EvaluateR_diff_factor(den, freq,1, list_index, symb_name);
        E_value diff;
        diff.num = diff_num;
//      cout << "diff_num is : " << diff_num << endl;
        diff.den = diff_den;
//      cout << "diff_den is : " << diff_den << endl;
        return diff;
}
E_value Analysis::Evaluate_complex_diff_factor(tpddnode *pRoot, double symbol_value, char *symb_name,int tag, double freq)
{
        tpddnode *num = pRoot->GetPShort();
        tpddnode *den = pRoot->GetPOpen();
        pRoot->Mark();


        complex_t diff_num;
        complex_t diff_den;
        int list_index = tag;
	double imag = 0;
	double real = 0;
	double trans_value;
	
	complex_t oValue = complex_t(TPE->GetValue(tag),TPE->GetImaginary(tag));
	trans_value = symbol_value;
	TPE->SetSymbValue(freq);
	if(symb_name[0] == 'R')
	{
		real = (trans_value == 0? DBL_MAX: (-1)/(trans_value*trans_value));
		TPE->SetValue(list_index, real);
		TPE->SetImaginary(list_index, 0.0);
	}
	else if(symb_name[0] == 'L')
	{
		imag = (trans_value == 0? DBL_MAX : 1/(2*PI*freq*trans_value*trans_value)); 
		TPE->SetValue(list_index,0.0);
		//ToDo: Invest whether is 1/imag or imag
		TPE->SetImaginary(list_index,1/(imag));
	}
	else if(symb_name[0] == 'C')
	{
		TPE->SetValue(list_index,0.0);
		TPE->SetImaginary(list_index,2*PI*freq);
	}
	else
	{
        	TPE->SetValue(list_index, 1);
       		TPE->SetImaginary(list_index, 0.0);
	}

        if(num->GetSymbIndex() > list_index)
        {
                num = TPE->GetpZero();
        }
        if(den->GetSymbIndex() > list_index)
        {
                den = TPE->GetpZero();
        }

        TPE->UnReferenceAll(TPE->GetRootNode(), 1);
        TPE->UnMarkAllNodes(TPE->GetRootNode(), 1);

        diff_num = EvaluateR_diff_factor(num, freq,1,list_index, symb_name);
        diff_den = EvaluateR_diff_factor(den, freq,1, list_index, symb_name);
        E_value diff;
        diff.num = diff_num;
	diff.den = diff_den;
	TPE->SetValue(list_index, oValue.real());
	TPE->SetImaginary(list_index, oValue.imag());
        return diff;
}
	
E_value Analysis::Evaluate_complex_diff(tpddnode *pRoot, double symbol_value, char *symb_name,int tag, double freq)
{
	
	tpddnode *num = pRoot->GetPShort();
	tpddnode *den = pRoot->GetPOpen();
	pRoot->Mark();
	
	
	complex_t diff_num;
	complex_t diff_den;
	int list_index = tag;
	
	TPE->SetSymbValue(freq);
 	TPE->SetValue(list_index, 1);
	TPE->SetImaginary(list_index, 0.0);

	if(num->GetSymbIndex() > list_index)
	{
		cout << "the numerator of differential is 0." << endl;
		num = TPE->GetpZero();
	}	
	if(den->GetSymbIndex() > list_index)
	{
		cout << "the differential  of D(s) is 0." << endl;
		den = TPE->GetpZero();
	}

	TPE->UnReferenceAll(TPE->GetRootNode(), 1);
	TPE->UnMarkAllNodes(TPE->GetRootNode(), 1);

	diff_num = EvaluateR_diff(num, freq,1,list_index);	
	diff_den = EvaluateR_diff(den, freq,1, list_index);
	E_value diff;
	diff.num = diff_num;
//	cout << "diff_num is : " << diff_num << endl;
	diff.den = diff_den;
//	cout << "diff_den is : " << diff_den << endl;	
	return diff;
}

complex_t Analysis::EvaluateR_diff_factor(tpddnode *P, double freq, bool visited, int sentag, char *name)
{
	//cout << "index is : " << P->GetSymbIndex() << endl;
	int PID = P->GetSymbIndex();
	
	//if(P->GetSymbIndex() == -2)
	if(PID == -2)

	{
                //if(!P->Marked())
                //{
                //        P->Mark();
                //}
                return complex_t(0.0);
        }

        //else if(P->GetSymbIndex() == -1)
        else if(PID == -1)
        {
                //if(!P->Marked())
                //{
                //        P->Mark();
                //}
                return complex_t(1.0);
        }
	if(P->Marked())
        {
                return P->GetValue();
        }
        else
                P->Mark();

        complex_t V,V1,V0;

	//ToDo: Simplize the call
        //double value = TPE->GetValue(P->GetSymbIndex());
        //double imag = TPE->GetImaginary(P->GetSymbIndex());
        double value = TPE->GetValue(PID);
        double imag = TPE->GetImaginary(PID);
	//if(sentag == P->GetSymbIndex()) 
	if(sentag == PID) 
	{
		/*if(name[0] == 'R')
		{
			V = complex_t(value,imag);
		}
		else if(name[0] == 'L')
		{
			V = complex_t(value,imag);
		}
		else if(name[0] == 'C')
		{
			V = complex_t(value,imag);
		}*/
		char t = name[0];
		if(t == 'R' || t == 'L' || t == 'C')
			V = complex_t(value,imag);
		else
		{
			V = complex_t(1);
		}
	}
        else
        {
		//ToDo: Simplize the call
                //switch (TPE->GetSymbName(P->GetSymbIndex())[0])
                switch (TPE->GetSymbName(PID)[0])
                {
                        case 'R':
                                if(value == 0)
                                V = complex_t(DBL_MAX);
                                else
                                V = complex_t(1/value);
                                break;
                        case 'L':
                                if(freq == 0)
                                V = complex_t(0, -DBL_MAX);
                                else
                                V = complex_t(0,-1/(2*PI*freq*value));
                                break;
                        case 'C':
                                V = complex_t(0,2*PI*freq*value);
                                break;
                        case '^':
                                V = complex_t(value, imag);
                                break;
                        default:
                                V = complex_t(value);
                                break;
                }
        }

	tpddnode *pZero = TPE->GetpZero();
	tpddnode *pNode = P->GetPShort();
	
        //if((P->GetSymbIndex() < sentag) && (pNode->GetSymbIndex() > sentag))
        if((PID < sentag) && (pNode->GetSymbIndex() > sentag))
        {
                //P->SetPShort(TPE->GetpZero());
                //pNode = P->GetPShort();
		pNode = pZero;
        }
        //else if((P->GetSymbIndex() < sentag) && (pNode == TPE->GetpOne()))
        else if((PID < sentag) && (pNode == TPE->GetpOne()))
        {
                //P->SetPShort(TPE->GetpZero());
                //pNode = P->GetPShort();
		pNode = pZero;
        }

        V1 = EvaluateR_diff_factor(pNode, freq, 1, sentag,name);

        pNode = P->GetPOpen();

        //if(P->GetSymbIndex() == sentag)
        if(PID == sentag)
        {
                //P->SetPOpen(TPE->GetpZero());
                //pNode = P->GetPOpen();
		pNode = pZero;
        }

        V0 = EvaluateR_diff_factor(pNode, freq, 1, sentag,name);

        V = V*V1*P->GetSignS() + V0 * P->GetSignO();
	if(P->GetValueP())
                P->SetValue(V);
        else
        {
                complex_t *val = new complex_t(V);
                P->SetValueP(val);
        }

        return V;

}
complex_t Analysis::EvaluateR_diff(tpddnode *P,double freq, bool visited, int sentag)
{
//	cout << "index diff is : " << P->GetSymbIndex() << endl;
	if(P->GetSymbIndex() == -2)
	{
		if(!P->Marked())
		{
			P->Mark();
		}
		return complex_t(0.0);
	}
	
	else if(P->GetSymbIndex() == -1)
	{
		if(!P->Marked())
		{
			P->Mark();
		}
		return complex_t(1.0);
	}
	
	if(P->Marked())	
	{
		return P->GetValue();
	}
	else
		P->Mark();
	
	complex_t V,V1,V0;
	double value;
	
	 value = TPE->GetValue(P->GetSymbIndex());
	
	double imag;

	if(sentag == P->GetSymbIndex()) V = complex_t(1);
	else
	{
		switch (TPE->GetSymbName(P->GetSymbIndex())[0])
		{
			case 'R':
				if(value == 0)
				V = complex_t(DBL_MAX);
				else
				V = complex_t(1/value);
				break;
			case 'L':
				if(freq == 0)
				V = complex_t(0, -DBL_MAX);
				else
				V = complex_t(0,-1/(2*PI*freq*value));
				break;
			case 'C':
				V = complex_t(0,2*PI*freq*value);
				break;
			case '^':
				imag = TPE->GetImaginary(P->GetSymbIndex());
				V = complex_t(value, imag);
				break;
			default:
				V = complex_t(value);
				break;	
		}
	}
	tpddnode *pNode;
	
	pNode = P->GetPShort();
	
	if((P->GetSymbIndex() < sentag) && (pNode->GetSymbIndex() > sentag))
	{
		P->SetPShort(TPE->GetpZero());
		pNode = P->GetPShort();

	}
	else if((P->GetSymbIndex() < sentag) && (pNode == TPE->GetpOne()))
	{
		P->SetPShort(TPE->GetpZero());
		pNode = P->GetPShort();
	}
	
	V1 = EvaluateR_diff(pNode, freq, 1, sentag);

	pNode = P->GetPOpen();

	if(P->GetSymbIndex() == sentag)
	{
		P->SetPOpen(TPE->GetpZero());
		pNode = P->GetPOpen();
	}
	
	V0 = EvaluateR_diff(pNode, freq, 1, sentag);

	V = V*V1*P->GetSignS() + V0 * P->GetSignO();

	if(P->GetValueP())
		P->SetValue(V);
	else
	{
		complex_t *val = new complex_t(V);
		P->SetValueP(val);
	}

	return V;

}

complex_t Analysis::Evaluate(tpddnode *pRoot, double freq)
{
	tpddnode *num = pRoot->GetPShort();
	tpddnode *den = pRoot->GetPOpen();
	pRoot->Mark();

	complex_t  val_den, val_num;

	// Set the composite symbol values.
	// This only can be done after a frequency is selected.
	TPE->SetSymbValue(freq);


#if 0
	HasherTree  *ht = new HasherTree();
	val_num = EvaluateH(num, freq, ht);
	val_den = EvaluateH(den, freq, ht);
	delete  (HasherTree *) ht;
	//delete sometimes causes problem.
#else

	TPE->UnMarkAllNodes(TPE->GetRootNode(), 1);
	val_num = EvaluateR(num, freq, (bool) 1);
	val_den = EvaluateR(den, freq, (bool) 1);
//	cout << "val_num is : " << val_num << endl;
//	cout << "val_den is : " << val_den << endl;
#endif

	if (val_den == complex_t(0))
	{
		cout << "\nDivide by zero.";
		return  -val_num * DBL_MAX;
	}
	else
		return  -val_num * pRoot->GetSignS() / val_den * pRoot->GetSignO();
}
	
complex_t	Analysis::EvaluateR(tpddnode *P, double freq, bool visited)
{
	int PID = P->GetSymbIndex();
	//if ( P->GetSymbIndex() == -2 )		// 'zero' node
	if ( PID == -2 )		// 'zero' node
	{
	/*	if (P->GetVisitFlag() != visited)
			P->SetVisitFlag(visited);
		return complex_t(0.0);*/
		//if(!P->Marked())
		//	P->Mark();
	//	P->SetValue(complex_t(0.0));
		return complex_t(0.0);
	}
	//else if (P->GetSymbIndex() == -1)	// 'one' node
	else if (PID == -1)	// 'one' node
	{
	/*	if (P->GetVisitFlag() != visited)
			P->SetVisitFlag(visited);
		return complex_t(1.0);*/
		//if(!P->Marked())
		//	P->Mark();
	//	P->SetValue(complex_t(1.0));
		return complex_t(1.0);
	}

/*	if (P->GetVisitFlag() == visited)
		return P->GetValue();
	else
		P->SetVisitFlag(visited);*/
	if(P->Marked())
		return P->GetValue();
	else
		P->Mark();



	complex_t	V, V1, V0;

//	V = 1.0;
//	return V;


	double	value = TPE->GetValue(P->GetSymbIndex());
	double	imag;

	//switch ( TPE->GetSymbName(P->GetSymbIndex())[0] )
	switch ( TPE->GetSymbName(PID)[0] )
	{
	case 'R':
		if (value == 0)
			V = complex_t(DBL_MAX);
		else
			V = complex_t(1/value);
		//	V = 1.0;
		break;
	case 'L':
		if (freq == 0)
			V = complex_t(0, -DBL_MAX);
		else
			V = complex_t(0, -1/(2*PI*freq*value));
		//	V = 1.0;
		break;
	case 'C':
		//V = complex_t(0, 1.0);
		V = complex_t(0, 2*PI*freq*value);
		break;
	case '^':	// Composite symbol
		imag = TPE->GetImaginary(P->GetSymbIndex());
		V = complex_t(value, imag);
		//cerr << "\n  V = " << V;
		break;
	default:		// Gain, or +/-: "+" = 1, "-" = -1.
		V = complex_t(value);
		break;
	}

	V1 = EvaluateR(P->GetPShort(), freq, visited);
	V0 = EvaluateR(P->GetPOpen(), freq, visited);
	V  = V * V1 * P->GetSignS() + V0 * P->GetSignO();

	if ( P->GetValueP() )
		P->SetValue(V);
	else
	{	// Must allocate memory for the first time use.
		complex_t *val = new complex_t(V);
		P->SetValueP(val); // allocate memory for the node value
	}

	return V;
}

complex_t	Analysis::EvaluateH(tpddnode *P, double freq, HasherTree *ht)
{
	if ( P->GetSymbIndex() == -2 )		// 'zero' node
		return complex_t(0.0);
	else if (P->GetSymbIndex() == -1)	// 'one' node
		return complex_t(1.0);

	if ( !ht->Put(*P, P) )	// Already computed sub-term value.
	{
		/*
		cout << "\nHit hasher.";
		cout << "\nValue = " << P->GetValue();
		*/
		return P->GetValue();
	}

	complex_t	V, V1, V0;
	// Get the symbol value.
	double	value = TPE->GetValue(P->GetSymbIndex());
	double 	imag;
	
//cout << "\nvalue = " << value;

//cout << "\nFile: " << __FILE__ << ", Line: " << __LINE__;
	// Get the element value: R/L/C/Gain.
	switch ( TPE->GetSymbName(P->GetSymbIndex())[0] )
	{
	case 'R':
		if (value == 0)
			V = complex_t(DBL_MAX);
		else
			V = complex_t(1/value);
		break;
	case 'L':
		if (freq == 0)
			V = complex_t(0, -DBL_MAX);
		else
			V = complex_t(0, -1/(2*PI*freq*value));
		break;
	case 'C':
		V = complex_t(0, 2*PI*freq*value);
		break;
	case '^':	// Composite symbol
		imag = TPE->GetImaginary(P->GetSymbIndex());
		V = complex_t(value, imag);
		//cerr << "\n  V = " << V;
		break;
	default:		// Gain or "+" or "-".
		V = complex_t(value);
		break;
	}

	V1 = EvaluateH(P->GetPShort(), freq, ht);
	V0 = EvaluateH(P->GetPOpen(), freq, ht);
	V  = V * V1 * P->GetSignS() + V0 * P->GetSignO();
#if DEBUG
	//cerr << "\nP->index = " << P->Index() << ", V = " << V;
#endif

	if ( P->GetValueP() )
		P->SetValue(V);
	else
	{	// Must allocate memory for the first time use.
		complex_t *val = new complex_t(V);
		P->SetValueP(val); // allocate memory for the node value
	}

	return V;
}

// freq: Hz
complex_t Analysis::EvaluateSCoeffFreq(double freq)
{	
	// unsigned i, j;

	complex_t  val_den, val_num, val_den_tmp, val_num_tmp;


	vector<scoeffNode*> *psCoeff	= TPE->pExpTpddRoot->pShort->sCoeffList;
	vector<int>         *psPower	= TPE->pExpTpddRoot->pShort->sPowerList;

	val_num = complex_t(0.0);

	for(unsigned i = 0; i < psCoeff->size(); i ++)
	{
		val_num_tmp = (*psCoeff)[i]->GetValue();
		for(int j = 0; j < abs((*psPower)[i]); j ++)
		{
			val_num_tmp = val_num_tmp * complex_t(0, 2 * PI * freq);
		}
		if((*psPower)[i] < 0)
			val_num_tmp = complex_t(1.0) / val_num_tmp;
		val_num += val_num_tmp;
	}

	psCoeff	= TPE->pExpTpddRoot->pOpen->sCoeffList;
	psPower	= TPE->pExpTpddRoot->pOpen->sPowerList;

	val_den = complex_t(0.0);

	for(unsigned i = 0; i < psCoeff->size(); i ++)
	{
		val_den_tmp = (*psCoeff)[i]->GetValue();
		for(int j = 0; j < abs((*psPower)[i]); j ++)
		{
			val_den_tmp = val_den_tmp * complex_t(0, 2 * PI * freq);
		}
		if((*psPower)[i] < 0)
			val_den_tmp = complex_t(1.0) / val_den_tmp;
		val_den += val_den_tmp;
	}

	if (val_den == complex_t(0))
	{
		cout << "\nDivide by zero.";
		return  -val_num * DBL_MAX;
	}
	else
		return  -val_num * TPE->pExpTpddRoot->signS / val_den * TPE->pExpTpddRoot->signO;
}
	
void Analysis::EvaluateSCoeffR(sexpTpddNode *P, bool visited)
{
	// Post order
#if 0
	if ( P->symbIndex == -2 )		// 'zero' node
	{
		if(!P->Marked())
			P->Mark();
		return;
	}
	else if (P->symbIndex == -1)	// 'one' node
	{
		if(!P->Marked())
			P->Mark();
		return;
	}

	if(P->Marked())
		return;
	else
		P->Mark();
#else
	// return when meeting a marked, zero or one node
	// Note: never mark the 0 node and the 1 node
	if(P->Marked() || P->symbIndex == -1 || P->symbIndex == -2)
		return;
	else
		P->Mark();
#endif
	EvaluateSCoeffR(P->pShort, visited);
	EvaluateSCoeffR(P->pOpen, visited);

	// int i;
	complex_t	V, V1, V0, Vr;
	SymbNode* pSymbN;

	char eType = TPE->GetSlist()[P->symbIndex]->name[0];
	
	vector<scoeffNode*> *psCoeff = P->sCoeffList;
	if(eType == '^')
	{
		eType = TPE->GetSlist()[P->symbIndex]->name[1];
		pSymbN = TPE->GetSlist()[P->symbIndex]->nextP;
	}
	else
		pSymbN = TPE->GetSlist()[P->symbIndex];

	double value = pSymbN->value;

	// Debug: Li Ji
	// if(eType == 'G' && !P->Marked())
		// cout << pSymbN->name << "=" << value << " Debugging: Li Ji" << endl;

	switch ( eType)
	{
	case 'R':
		if (value == 0)
			V = complex_t(DBL_MAX);
		else
			V = complex_t(1/value);
		break;
	case 'L':
		if (value == 0)
			V = complex_t(DBL_MAX);
		else
			V = complex_t(1/value);
		break;
	case 'C':
		V = complex_t(value);
		break;
	default:		// Gain, or +/-: "+" = 1, "-" = -1.
		V = complex_t(value);
		break;
	}

	for(unsigned i = 0; i < psCoeff->size(); i ++)
	{
		V1 = (*psCoeff)[i]->pShort->GetValue();
		V0 = (*psCoeff)[i]->pOpen->GetValue();
		Vr = V * V1 * (*psCoeff)[i]->pNode->signS + V0 * (*psCoeff)[i]->pNode->signO;

#if 0
		complex_t *temp_val_ptr = new complex_t(Vr);
		delete (*psCoeff)[i]->val_ptr;
		(*psCoeff)[i]->val_ptr = temp_val_ptr;
#endif
		*((*psCoeff)[i]->val_ptr) = Vr;
	}
}

void Analysis::EvaluateSCoeff()
{
	UnMarkSExpTree(TPE->pExpTpddRoot->pShort);
	UnMarkSExpTree(TPE->pExpTpddRoot->pOpen);

	EvaluateSCoeffR(TPE->pExpTpddRoot->pShort, true);
	EvaluateSCoeffR(TPE->pExpTpddRoot->pOpen, true);
}

// Li Ji
void Analysis::UnMarkSExpTree(sexpTpddNode *P)
{
	// return when meeting a unmarked, 0 or 1 node
	// Note: never mark the 0 node and the 1 node
	// Pre-order
	if(!P->Marked() || P->symbIndex == -2 || P->symbIndex == -1)
		return;
	else
		P->UnMark();

	UnMarkSExpTree(P->pShort);
	UnMarkSExpTree(P->pOpen);
}

void Analysis::UnMarkSCoefNodeTree(scoeffNode* P)
{
	if(!P->Marked() || (P->pNode && (P->pNode->symbIndex == -1 || P->pNode->symbIndex == -2)))
		return;
	else
		P->UnMark();
	UnMarkSCoefNodeTree(P->pShort);
	UnMarkSCoefNodeTree(P->pOpen);
}

complex_t Analysis::EvaluateSCoeffSensitivity(scoeffNode* P, char* symbName, int sensID)
{
	UnMarkSCoefNodeTree(P);
	return EvaluateSCoeffSensitivityR(P,sensID);
}

complex_t Analysis::EvaluateSCoeffSensitivityR(scoeffNode* P, int sensID)
{
	sexpTpddNode* mpNode = P->pNode;
	int symbID = mpNode->symbIndex;
	if(symbID == -2)
		return complex_t(0);
	else if(symbID == -1)
		return complex_t(1);
	else if(P->Marked())
		return *P->val_ptr;
	else
		P->Mark();

	SymbNode* pSymbN = NULL;
	char eType = TPE->GetSlist()[symbID]->name[0];
	if(eType == '^')
	{
	        eType = TPE->GetSlist()[symbID]->name[1];
	        pSymbN = TPE->GetSlist()[symbID]->nextP;
	}
	else
	        pSymbN = TPE->GetSlist()[symbID];
	double value = pSymbN->value;

	complex_t       V, V1, V0, Vr;
	if(symbID == sensID)
	{
		if(eType == 'R' || eType == 'L')
			V = value == 0 ? complex_t(DBL_MAX*DBL_MAX*-1) : complex_t(-1/(value*value));
		else
			V = complex_t(1);
	}
	else
	{
		if(eType == 'R' || eType == 'L')
			V = value == 0 ? complex_t(DBL_MAX) : complex_t(1/value);
		else
			V = complex_t(value);
	}

	scoeffNode* mpScieffOne = TPE->GetpSCoeffOne();
	scoeffNode* mpScieffZero = TPE->GetpSCoeffZero();
	scoeffNode* mpScoeffNode = NULL;

	mpScoeffNode = P->pShort;
	if((symbID < sensID) && (mpScoeffNode->pNode->symbIndex > sensID))
		mpScoeffNode = mpScieffZero;
	else if((symbID < sensID) && (mpScoeffNode == mpScieffOne))
		mpScoeffNode = mpScieffZero;
	V1 = EvaluateSCoeffSensitivityR(mpScoeffNode,sensID);

	mpScoeffNode = P->pOpen;
	if(symbID == sensID)
		mpScoeffNode = mpScieffZero;
        V0 = EvaluateSCoeffSensitivityR(mpScoeffNode,sensID);

	Vr = V * V1 * mpNode->signS + V0 * mpNode->signO;
	*P->val_ptr = Vr;
	return Vr;
}
