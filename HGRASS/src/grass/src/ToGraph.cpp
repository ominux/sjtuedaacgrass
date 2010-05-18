/** ToGraph.cpp
 * Parsing a standard netlist to a graph file.
 * G.Shi, 03-25-04, 03-31-04
 *
 * Cautions of this code:
 * (1) Multiple dependent sources:
 * 	   - The case of one edge controlled by multiple edges can be
 * 		 treated by splitting the controlled edge into multiple ones
 * 		 (in series or parallel depending on CS/VS).
 * 	   - The case of one edge controlling multiple edges can be
 * 	     handeled, but for internal representation during parsing
 * 	     a list is needed for such a controlling edge. This is NOT
 * 	     implemented at the moment.
 * (2) 
 * 		
 */

#include	<iostream>
#include	<fstream>
#include	<string>
#include	<stdlib.h>
#include <cstdio>
#include	"ToGraph.h"
using std::cout;
using std::endl;
using std::cerr;
using std::printf;

extern int read_input(const char *filename);
// Defined in "netgram.g" and duplicated in "netgram.cpp".

//using namespace std;

ToGraph::ToGraph()
{
	printf("ToGraph(): this = %p\n", this);
	E = 0;
	M = 0;
	N = 0;
	Nsrc = Nout = 0;
	extra_nn = 0;
	edge_list = cur_edge = NULL;
	mos_list = cur_mos = NULL;
	src_edge  = out_edge = NULL;
	node_list = node_tail = NULL;
}

ToGraph::~ToGraph()
{
	printf("~ToGraph(): this = %p\n", this);
	// Release the list of edges created.
	while (edge_list)
	{
		cur_edge = edge_list;
		edge_list = edge_list->next;
		delete cur_edge;
	}

	// Release the list of mosfets created
	/*while (mos_list)
	{
		cur_mos = mos_list;
		mos_list = mos_list->next;
		delete cur_mos;
	}*/
	// Release the list of nodes created.
	while (node_list)
	{
		Node_g *node = node_list;
		node_list = node_list->next;
		delete node;
	}
}

void
ToGraph::readNetlist(const char *filename)
{
#ifdef	TRACE
	cout << "\nIn readNetlist() ";
#endif
	cout << "\n->Start parsing the netlist ... \n";
	// Call the parser.
	if (read_input(filename) == -1)
	{
		cerr << "\nMake sure the netlist file exists.";
		exit(1);
	}
	// read_input() defined in the grammar file "netgram.g".
	cout << "\n->Finished parsing the netlist." << endl;
	
	// Check the number of input/output.
	if (Nsrc != 1 || Nout != 1)
	{
		cout << "\nWarning: Something wrong with input and output."
		     << "\nMake sure there are only one input and one output " 
			 << "in the original netlist."
			 << endl;
		exit(1);
	}

	//calMosfetModelParameters();
	// IMPORTANT!
	// Must renumber the node_list. It also counts the # of nodes.
	renumber_nodes();
}

// Write the edge list to a graph file.
void
ToGraph::writeGraph(const char *filename)
{
#ifdef	TRACE
	cout << "\nIn writeGraph() ";
#endif
	// Before writing the graph file, check whether the edge_list
	// and the node_list have been created.
	if (!edge_list || !node_list)
	{
		cerr << "\nNo edges or nodes. writeGraph() aborted.";
		return;
	}

	outFile.open(filename);
	if (outFile.fail())
	{
		cerr << "\nCan't open file: " << filename << endl;
		exit(1);
	}

	outFile << title << endl;
	outFile << "E = " << E << ", N = " << N << endl;
	// Nodes counted from 0.

	Edge_g 	*edge;
	for(edge = edge_list; edge; edge = edge->next)
	{
		outFile << ToUpper(edge->name) << " " 
				<< edge->node1->num << " " << edge->node2->num << " ";
		// Make sure that the pairing edge names are all set.
		if ( (edge->type >= CC) && (edge->pname == NULL) )
		{
			cerr << "\nThe name for one pairing edge is missing.";
		}
		switch (edge->type)
		{
		case Z:
			outFile << "Z  " << edge->value;
			break;
		case Y:
			outFile << "Y  " << edge->value;
			break;
		case VS:
			outFile << "VS  " << ToUpper(edge->pname) 
					<< "  "   << edge->value;
			break;
		case CS:
			outFile << "CS  " << ToUpper(edge->pname)
					<< "  "   << edge->value;
			break;
		case VC:
			outFile << "VC" << " " << ToUpper(edge->pname);
			break;
		case CC:
			outFile << "CC" << " " << ToUpper(edge->pname);
			break;
		default:
			break;
		}
		outFile << endl;
	}
	outFile.close();
	cout << "\n->A graph file has been created: \"" << filename << "\"";
}

void
ToGraph::parseRLC(char *name, char *n1, char *n2,
				char *value, char *var_name)
{
#ifdef	TRACE
	cout << "\nIn parseRLC()" << flush;
#endif

	// Check whether the branch is a duplicate.
	if ( query_edge(name) )
	{
		cout << "\nBranch [" << name << "] is a duplicate, skipped.";
		return;
	}

	// Process the nodes
	int 	nn1 = atoi(n1);
	int 	nn2 = atoi(n2);
	Node_g	*node1, *node2;

	//cout << "\n nn1 = " << nn1 << ", nn2 = " << nn2;
	node1 = query_node(nn1);
	if ( !node1 )
	{
		node1 = new Node_g;
		node1->num = nn1;
		node1->next = NULL;
		add_node(node1);
	}
	node2 = query_node(nn2);
	if ( !node2 )
	{
		node2 = new Node_g;
		node2->num = nn2;
		node2->next = NULL;
		add_node(node2);
	}

	Edge_g *edge = new Edge_g;

	if (!edge)
	{
		cerr << "\nMemory allocation error for an RLC edge.\n";
		exit(1);
	}

	edge->name  = CopyStr(name);
	edge->node1 = node1;
	edge->node2 = node2;
	edge->value = TransValue(value);
	edge->pname = NULL;
	edge->var_name = CopyStr(var_name);
	edge->next = NULL;

	// Check the element type.
	switch ( tolower(edge->name[0]) )
	{
	case 'r':
	case 'l':
		edge->type = Z;
		break;
	case 'c':
		edge->type = Y;
		break;
	default:
		break;
	}

	char* tmpName = strdup(edge->name);
	char* tmp = NULL;
	while(tmpName)
	  tmp = strsep(&tmpName,"_");
	if(tmp && strcmp(tmp,edge->name))
	{
		Mosfet* pMos = query_mosfet(tmp);
		if(pMos)	edge->pMosfet = pMos; 
        }
	free(tmpName);
	add_edge( edge );
} // parseRLC()

// parseSRC()
// parses an independent input source.
// Note: 
// (1) By the standard netlist format, a Voltage Source (Vname) is used for
//     a CC branch.
// (2) We assume an independent V-SRC will not be used for CC.
// (3) We assume a V-SRC used for CC always has a value of 0 volt.
// (4) The two edges representing input and output are specifically
//     added to the beginning of the edge list so that they are sorted
//     with the highest order (index) for using DDD.
void
ToGraph::parseSRC(char *name, char *n1, char *n2, 
				char *dc_val, char *ac_val)
{
#ifdef	TRACE
	cout << "\nIn parseSRC()" << flush;
#endif

	int		CC_edge = 0;	// Flag for a CC_edge.
	
	// Check whether the branch is a duplicate.
	if ( query_edge(name) )
	{
		cout << "\nBranch [" << name << "] already parsed, hence skipped.";
		return;
	}

	if (Nsrc == 1)
	{
		cout << "\nWarning: ";
		cout << "Only one indep source allowed; this one is ignored.";
		return;
	}

	// Process the nodes
	int 	nn1 = atoi(n1);
	int 	nn2 = atoi(n2);
	Node_g	*node1, *node2;

	node1 = query_node(nn1);
	if ( !node1 )
	{
		node1 = new Node_g;
		node1->num = nn1;
		node1->next = NULL;
		add_node(node1);
	}
	node2 = query_node(nn2);
	if ( !node2 )
	{
		node2 = new Node_g;
		node2->num = nn2;
		node2->next = NULL;
		add_node(node2);
	}

	Edge_g *edge = new Edge_g;
	if (!edge)
	{
		cerr << "\nMemory allocation error for a source edge.\n";
		exit(1);
	}

	edge->node1 = node1;
	edge->node2 = node2;
	// Check whether it is a Current Controlling (CC) edge.
	if (dc_val)
	{
		edge->value = TransValue(dc_val);
		if (edge->value == 0)	CC_edge = 1;
	}
	else if (ac_val)
	{
		edge->value = TransValue(ac_val);
		if (edge->value == 0)	CC_edge = 1;
	}
	else
		edge->value = 0.0;

	edge->pname = NULL;
	edge->var_name = NULL;
	edge->next = NULL;

	// Check the edge type.
	if (!CC_edge)
	{
		edge->name  = CopyStr(name);

		switch ( tolower(edge->name[0]) )
		{
		case 'v':
			edge->type = VS;
			break;
		case 'i':
			edge->type = CS;
			break;
		default:
			break;
		}
		// Let src_edge point to this edge.
		src_edge = edge;   // Only one source allowed now.
		// Check whether the out_edge is available.
		if ( out_edge )
		{
			// Establish the pairing of edges.
			src_edge->pname = out_edge->name;
			out_edge->pname = src_edge->name;
		}
		++Nsrc;
	}
	else  // A CC edge
	{
		// Modify the edge name with a preceding "I".
		char* cc_name = new char[strlen(name)+2];
		sprintf(cc_name, "I%s", name);
		//cout << "\n name = " << name;
		edge->name = CopyStr(cc_name);
		//cout << "\n edge->name = " << edge->name;
		edge->type = CC;

		// Query another edge controlled by this edge.
		Edge_g *xs_edge = query_pair(edge->name);

		// If the XS edge exists, assign a pairing name to it.
		if (xs_edge)
		{
			xs_edge->pname = edge->name;
			edge->pname = xs_edge->name;
		}
		else
			edge->pname = NULL;		// the pairing edge not parsed yet.

		delete[]	cc_name;
	}

	// Add the indep src edge at the head of edge list.
	add_edge_at_head( edge );
} // parseSRC()


// parseOUT()
// parses the output control meanwhile creates an output edge.
// Note:
// (1) If the output is a voltage, then add a new edge IN PARALLEL to
//     the output element.
// (2) If the output is a current, then add a new edge IN SERIES to
//     the output element. In this case, an extra node has to be added
//     (extra_nn, a negative number in this implementation).
//     Also, in this case the string <br_name> is the name of the branch
//     from which the output current is measured.
void
ToGraph::parseOUT(char *name, char *n1, char *n2, char *br_name)
{
#ifdef	TRACE
	cout << "\nIn parseOUT()" << flush;
#endif
	
	// Make a name for output edge.
	char out_name[10];
	sprintf(out_name, "%co", name[0]);

	// Check whether the name is alreay used.
	if ( query_edge(out_name) )
	{
		cout << "\nBranch [" << name << "] already parsed -- skipped.";
		return;
	}
	if (Nout == 1)
	{
		cout << "\nWarning: ";
		cout << "-- Outputs except the first one are ignored.";
		return;
	}
	
	// Make an extra edge.
	Edge_g *edge = new Edge_g;
	if (!edge)
	{
		cerr << "\nMemory allocation error for an output edge.\n";
		exit(1);
	}

	if (tolower(name[0]) == 'v') 
	{
		// Process an output of voltage.
		int 	nn1 = atoi(n1);
		int 	nn2 = (n2 == NULL? 0 : atoi(n2));
		Node_g	*node1, *node2;
	
		node1 = query_node(nn1);
		if ( !node1 )
		{
			node1 = new Node_g;
			node1->num = nn1;
			node1->next = NULL;
			add_node(node1);
		}
		node2 = query_node(nn2);
		if ( !node2 )
		{
			node2 = new Node_g;
			node2->num = nn2;
			node2->next = NULL;
			add_node(node2);
		}
	
		// Assign to the extra edge for the voltage output.	
		edge->name  = CopyStr(out_name);
		edge->node1 = node1;
		edge->node2 = node2;
		edge->value = 0.0;
		edge->pname = NULL;
		edge->var_name = NULL;
		edge->next = NULL;
		edge->type = VC;
	}
	else if (tolower(name[0]) == 'i')
	{
		// Process an output of current.
		// First get the branch name that the current flows.
		//cout << "\nOutput branch : " << br_name;
		Edge_g *edge0 = query_edge(br_name);
		if (!edge0)
		{
			cerr << "\nThe branch for current (I) output is unknown."
				 << "\nCheck the netlist.";	
			exit(1);
		}
		// Get the node nums of the output branch.
		Node_g 	*node2 = edge0->node2;
		// Make a new node for an extra current edge in series.
		Node_g	*node0 = new Node_g;
	
		node0->num = --extra_nn;	// -1, -2, ...
		node0->next = NULL;
		add_node(node0);

		// Modify edge0->node2 to node0
		// (node1) ------ (node0) ------ (node2)
		edge0->node2 = node0;
	
		// Assign to the extra edge for the current output.
		edge->name  = CopyStr(out_name);
		edge->node1 = node0;
		edge->node2 = node2;
		edge->value = 0.0;
		edge->pname = NULL;
		edge->var_name = NULL;
		edge->next = NULL;
		edge->type = CC;
	}

	// Let out_edge point to this edge.
	out_edge = edge;   // Only one output allowed now.
	// Check whether a src_edge is already parsed.
	if ( src_edge )
	{
		// Establish the pairing of edges.
		out_edge->pname = src_edge->name;
		src_edge->pname = out_edge->name;
	}

	// Add the output edge at the head of edge list.
	add_edge_at_head( edge );
	++Nout;
}  // parseOUT()


// parseVCXS()
// parses a VCCS/VCVS branch. Because parsing a VCCS is very similar to
// parsing a VCVS branch, they can be parsed by the same function.
// Note: 
// (1) For tree enumeration analysis, an extra edge is added to the
//     graph for the controlling 'VC' branch, although in the circuit 
//     an element branch can be used as a VC branch. 
// (2) Since we assume only one independent source, the VC edge should
//     not be an independent voltage source. An independe source is
//     parsed separately by parseSRC().
// (3) Two edges (CS/VS, VC) will be added to the graph after parsing 
//     a VCXS branch.
//
void 
ToGraph::parseVCXS(char *name, char *n1, char *n2, char *nc1, char *nc2,
					char *value, char *var_name)
{
#ifdef	TRACE0
	cout << "\nIn parseVCXS()" << flush;
#endif
	
	// Check whether the branch is a duplicate.
	if ( query_edge(name) )
	{
		cout << "\nBranch [" << name << "] already parsed -- skipped.";
		return;
	}

	int		nn[4];
	Node_g	*node[4];
	nn[0] = atoi(n1); 	nn[1] = atoi(n2);
	nn[2] = atoi(nc1); 	nn[3] = atoi(nc2);
	
	// Spice netlist allows the controlling and controlled edges are the
	// same edge. This is normally used for a conductance/admittance (G).
	if ( (nn[0] == nn[2] && nn[1] == nn[3]) ||
	     (nn[0] == nn[3] && nn[1] == nn[2]) )
	{
		cout << "\nA G-edge in VCCS form.";

		for (int i = 0; i < 2; i++)
		{
			node[i] = query_node(nn[i]);
			if ( !node[i] )
			{
				node[i] = new Node_g;
				node[i]->num = nn[i];
				node[i]->next = NULL;
				add_node(node[i]);
			}
		}
		Edge_g *edge = new Edge_g;
		if (!edge)
		{
			cerr << "\nMemory allocation error for a G-edge.\n";
			exit(1);
		}
	
		edge->name  = CopyStr(name);
		edge->node1 = node[0];
		edge->node2 = node[1];
		if (nn[0] == nn[2])
			edge->value = TransValue(value);
		else
			edge->value = -TransValue(value);
		edge->pname = NULL;
		edge->var_name = CopyStr(var_name);
		edge->next = NULL;
		edge->type = Y;

		add_edge( edge );
		return;
	}

	// Process the nodes of two distinct edges.

	for (int i = 0; i < 4; i++)
	{
		node[i] = query_node(nn[i]);
		if ( !node[i] )
		{
			node[i] = new Node_g;
			node[i]->num = nn[i];
			node[i]->next = NULL;
			add_node(node[i]);
		}
	}

	Edge_g *edge1 = new Edge_g;	// for the CS/VS edge
	if (!edge1)
	{
		cerr << "\nMemory allocation error for edge in parseVCCS().\n";
		exit(1);
	}
	edge1->name  = CopyStr(name);
	edge1->node1 = node[0];
	edge1->node2 = node[1];
	edge1->value = TransValue(value);
	edge1->pname = NULL;
	edge1->var_name = NULL;
	edge1->next = NULL;
	// Check the element type.
	switch ( tolower(edge1->name[0]) )
	{
	case 'e':
		edge1->type = VS;	// VCVS
		break;
	case 'g':
		edge1->type = CS;	// VCCS
		break;
	default:
		break;
	}
	
	// Make a name for the (extra) VC edge 
	char* vc_name = new char[strlen(name)+2];
	sprintf(vc_name, "V%s", name);
	edge1->pname = CopyStr(vc_name); // name of the controlling edge

	char* tmpName = strdup(edge1->name);
	char* tmp = NULL;
	while(tmpName)
	  tmp = strsep(&tmpName,"_");
	if(tmp && strcmp(tmp,edge1->name))
	{
		Mosfet* pMos = query_mosfet(tmp);
		if(pMos)	edge1->pMosfet = pMos; 
        }
	free(tmpName);

	add_edge( edge1 );

	Edge_g *edge2 = new Edge_g;	// Allocate another edge for VC.
	if (!edge2)
	{
		cerr << "\nMemory allocation error for edge in parseVCCS().\n";
		exit(1);
	}

	edge2->name  = edge1->pname; // name of the controlling edge
	edge2->node1 = node[2];
	edge2->node2 = node[3];
	edge2->value = 0.0;
	edge2->pname = edge1->name;	 // name of the controlled edge
	edge2->var_name = NULL;
	edge2->next = NULL;
	edge2->type = VC;

	add_edge( edge2 );


	delete[] vc_name;
	return;
}  // parseVCXS()


// parseCCXS()
// parses a CCCS/CCVS branch. 
// Note: 
// (1) According to standard netlist format, the Controlling Current (CC) 
//     is always a current flowing through a Voltage Source (noted by Vname).
// (2) It might be easier for coding by requiring that a CC branch
//     always precede a branch controlled by it in the netlist.
//     But I think by relaxing this requirement it should give more freedom
//     to the netlist. This implementation does not require such precedence.
void 
ToGraph::parseCCXS(char *name, char *n1, char *n2, char *Vname,
					char *value, char *var_name)
{
#ifdef	TRACE0
	cout << "\nIn parseCCXS()" << flush;
#endif
	
	// Check whether the branch is a duplicate.
	if ( query_edge(name) )
	{
		cout << "\nBranch [" << name << "] already parsed -- skipped.";
		return;
	}

	int		nn[2];
	Node_g	*node[2];
	nn[0] = atoi(n1); 	nn[1] = atoi(n2);

	for (int i = 0; i < 2; i++)
	{
		node[i] = query_node(nn[i]);
		if ( !node[i] )
		{
			node[i] = new Node_g;
			node[i]->num = nn[i];
			node[i]->next = NULL;
			add_node(node[i]);
		}
	}
	// Make a name for the CC edge from "Vname".
	char *cc_name = new char[strlen(Vname)+2];
	sprintf(cc_name, "I%s", Vname);

	Edge_g *edge1 = new Edge_g;	// for CS edge
	if (!edge1)
	{
		cerr << "\nMemory allocation error for edge in parseCCXS().\n";
		exit(1);
	}
	edge1->name  = CopyStr(name);
	edge1->node1 = node[0];
	edge1->node2 = node[1];
	edge1->value = TransValue(value);
	edge1->pname = CopyStr(cc_name);
	edge1->var_name = NULL;
	edge1->next = NULL;
	// Check the controlled edge type.
	switch ( tolower(edge1->name[0]) )
	{
	case 'f':
		edge1->type = CS;	// CCCS
		break;
	case 'h':
		edge1->type = VS;	// CCVS
		break;
	default:
		break;
	}

	char* tmpName = strdup(edge1->name);
	char* tmp = NULL;
	while(tmpName)
	  tmp = strsep(&tmpName,"_");
	if(tmp && strcmp(tmp,edge1->name))
	{
		Mosfet* pMos = query_mosfet(tmp);
		if(pMos)	edge1->pMosfet = pMos; 
        }
	free(tmpName);

	add_edge( edge1 );

	// Now query whether the CC (Vname) edge is already parsed.
	// If it was parsed before this edge, then its pairing edge
	// (controlled) hasn't been assigned.
	// Warning: For the moment we assume one edge does not control
	// multiple edges (to be extended.)
	Edge_g *cc_edge = query_edge(edge1->pname);
	if (cc_edge && !cc_edge->pname)
		cc_edge->pname = edge1->name;
	// Do not create the CC edge if not parsed yet!
	// It will be created when it is parsed.

	delete[] cc_name;
	return;
} // parseCCXS()

void
ToGraph::parseMOSFET(char *name, char *nd, char *ng, char *ns, char *nb,
		                char *model, char *w, char *l)
{
	printf("\nM%s nd=%s, ng=%s, ns=%s, nb=%s, model =%s, w=%s, l=%s",name,nd,ng,ns,nb,model,w,l);
	/*int	nn[4];
	Node_g	*node[4];
	nn[0] = atoi(nd);	nn[1] = atoi(ng);
	nn[2] = atoi(ns);	nn[3] = atoi(nb);
	for (int i = 0; i < 4; i++)
	{
		node[i] = query_node(nn[i]);
		if ( !node[i] )
		{
			node[i] = new Node_g;
			node[i]->num = nn[i];
			node[i]->next = NULL;
			add_node(node[i]);
		}
	}*/
	
	Mosfet* pMosfet = new Mosfet();
	pMosfet->name = CopyStr(name);
	pMosfet->noded = NULL;//node[0];
	pMosfet->nodeg = NULL;//node[1];
	pMosfet->nodes = NULL;//node[2];
	pMosfet->nodeb = NULL;//node[3];
	pMosfet->w = TransValue(w);
	pMosfet->l = TransValue(l);
	pMosfet->value = pMosfet->w/pMosfet->l;
	bool isNMOS = true;
	if(!strcmp(model,"nmos"))
	  pMosfet->type = NMOS_DEF;
	else if(!strcmp(model,"nch3"))
	  pMosfet->type = NMOS_L3;
	else if(!strcmp(model,"pmos"))
	  pMosfet->type = PMOS_DEF;
	else if(!strcmp(model,"pch3"))
	  pMosfet->type = PMOS_L3;
	else
	  pMosfet->type = MODEL_TYPE_UNKNOWN;
	// Ma Diming ToDo:
	// Create Model According to the different ModelType
	pMosfet->pModel = new MosfetModelBsim3V3(pMosfet->type);
	// End of ToDo Ma Diming
	pMosfet->next = NULL;
	add_mosfet(pMosfet);
}

//-------- the following are private methods --------------------------

// query_node() 
// searches the node list to see whether
// the node is already there. If yes, it returns the pointer to this node.
Node_g *
ToGraph::query_node(int node_num)
{
	//cout << "\nIn query_node() ";
	Node_g  *node = node_list;
	for (; node; node = node->next)
		if (node->num == node_num)	return node;
	return NULL;
}

// add_node() 
// inserts a new node to the node list. 
// The nodes are sorted in increasing node number. However,
// negative node numbers are always attached to the end of the list.
// Note that negative node numbers are used for those auxiliary nodes.
void
ToGraph::add_node(Node_g *node)
{
	//cout << "\nIn add_node() ";
	
	if (!node)
	{
		cerr << "\nNo node to add by add_node().";
		return;
	}

	// Attach a negative-numbered node to the tail of the node_list.
	if (node->num < 0)
	{
		if (!node_list) 
			node_list = node_tail = node;
		else
			node_tail->next = node,  node_tail = node_tail->next;
		return;
	}

	Node_g *pre_nd = NULL, *this_nd = node_list;
	while (	this_nd && this_nd->num < node->num )
		pre_nd = this_nd,  this_nd = this_nd->next; 

	//cout << "\nAt " << __FILE__ << ": " << __LINE__;

	// Proof of no repeated node.
	if (this_nd && this_nd->num == node->num)
	{
		cerr << "\nAdding a duplicated node, ignored.";
		return;
	}
	
	if (!node_list)		// Empty list
		node_list = node_tail = node;
	else if (!pre_nd)  	// Add to the head
		node->next = node_list,  node_list = node;
	else	// Add to a place behind the head
		pre_nd->next = node,  node->next = this_nd;
	// Update the tail if another node attached to the tail.
	if (node_tail->next)
		node_tail = node_tail->next;
}

// renumber_nodes() 
// traverses the node_list, resets the node numbers continuously,
// and counts the nodes.
// After renumbering, the nodes are numbered continuously from 0,
// with node 0 the ground (datum).
//
// The reason to do this is that the netlist writer is allowed
// to use an arbitrary numbering scheme, except that the datum must
// be node 0 and no node is numbered negative.
void
ToGraph::renumber_nodes()
{
	//cout << "\nIn renumber_nodes() ";
	
	if (!node_list)		return;		// do nothing

	int		cnt = 0;
	Node_g 	*node = node_list;
	for (; node; node = node->next)
		node->num = cnt++;
	// Set the number of nodes
	N = cnt;
}

// query_edge() 
// searches the edge_list to see whether
// the edge is already there. If yes, it returns the pointer to this edge.
Edge_g *
ToGraph::query_edge(char *edge_name)
{
	//cout << "\nIn query_edge() ";
	if (!edge_name)		return NULL;

	Edge_g  *edge = edge_list;
	for (; edge; edge = edge->next)
		if ( strcmp(edge->name, edge_name) == 0)		
			return edge;
	return NULL;
}

// query_pair() 
// queries the edge with the given pair_name.
// If yes, it returns the pointer to this edge.
Edge_g *
ToGraph::query_pair(char *pair_name)
{
	//cout << "\nIn query_pair() for \"" << pair_name << "\"";
	if (!pair_name)		return NULL;

	Edge_g  *edge = edge_list;
	for (; edge; edge = edge->next)
	{
		if (!edge->pname)	continue;
		if ( strcmp(edge->pname, pair_name) == 0)		
			return edge;
	}
	return NULL;
}

// add_edge()
// adds an edge to the edge_list. Always add the edge to the end
// pointed by cur_edge.
void
ToGraph::add_edge(Edge_g *edge)
{
	//cout << "\nIn add_edge() ";
	
	// Insert to edge_list
	if ( !edge_list ) 	
		edge_list = cur_edge = edge;
	else
		cur_edge->next = edge,  cur_edge = cur_edge->next;
	++E;
}

// add_edge_at_head()
// adds an edge to the beginning of the edge_list. 
void
ToGraph::add_edge_at_head(Edge_g *edge)
{
	//cout << "\nIn add_edge_at_head() ";
	
	// Insert to edge_list
	if ( !edge_list ) 	
		edge_list = cur_edge = edge;
	else
		edge->next = edge_list,  edge_list = edge;
	++E;
}

void
ToGraph::add_mosfet(Mosfet* mosfet)
{
	//cout << "\nIn add_mosfet() ";
	// Insert to mos_list
	if ( !mos_list )
	        mos_list = cur_mos = mosfet;
	else
	        cur_mos->next = mosfet,  cur_mos = cur_mos->next;
	 ++M;
}

Mosfet *
ToGraph::query_mosfet(char* name)
{
	//cout << "\nIn query_mosfet() ";
	Mosfet* mList = mos_list;
	for (; mList; mList = mList->next)
		if (!strcmp((mList->name)+1,name))	return mList;
	return NULL;
}

void
ToGraph::calMosfetModelParameters()
{
	Mosfet* mList = mos_list;
	for (; mList; mList = mList->next)
		mList->calBasicModelParameters(mList);
}
