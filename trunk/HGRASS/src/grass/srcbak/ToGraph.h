/* ToGraph.h
 * The ToGraph class reads the input netlist and create an internal edge list.
 * After finishing reading, it writes the edge list to a graph file.
 *
 * 03-25-03, G.Shi
 */

#ifndef	TOGRAPH_H
#define	TOGRAPH_H

#include	<fstream>
#include	"utility.h"
#include	"tpdd_struct.h"
#include	"Mosfet.h"

/*// For storing a node.
struct Node_g
{
	int	 	num;		// Node_g number
	Node_g	*next;		// To form a linked list.
};
// At the moment I only use this Node_g List to sort the node numbers,
// and if necessary to renumber the nodes from 0 consecutively.

// For storing an edge.
struct Edge_g
{
	char	*name;		// Name of the edge
	Node_g	*node1;
	Node_g	*node2;		// Node number is stored in the node list
	int		type;
	char	*pname;		// Name of the pairing edge
	double	value;		// Value of the element
	char	*var_name;	// Variable name for the value (not used now)
	Edge_g	*next;		// Make a linked list
};*/

		
class ToGraph 
{

private:
	int		E;		// Num of edges in the graph.
	int		N;		// Num of nodes in the graph.
	int		M;
	int		Nsrc;	// Num of indep srcs (only one allowed now)
	int		Nout;	// Num of outputs (only one allowed now)
	int		extra_nn;	// extra node number (-1, -2, ...)
	char	*title;	// netlist title
	Edge_g	*edge_list;		// Head of the edge list
	Edge_g	*cur_edge;		// Points to the current edge
	Edge_g	*src_edge;		// Points to the source edge
	Edge_g	*out_edge;		// Points to the output edge
	Node_g	*node_list;		// Head of the node list
	Node_g	*node_tail;		// Tail of the node list
	Mosfet  *mos_list;
	Mosfet  *cur_mos;
	std::ofstream	outFile;	// For output to a graph file.

public:
	ToGraph();
	~ToGraph();

	/* Methods for parsing the netlist. */
	void readNetlist(const char *filename);
	void writeGraph(const char *filename);  // Create a graph file.
	void calMosfetModelParameters();
	void getTitle(char *title) {this->title = CopyStr(title);}
	inline int		getE()  		{return E;}
	inline int		getN()  		{return N;}
	inline int		getM()  		{return M;}
	inline Edge_g 	*getEdgeList()	{ return edge_list; }
	inline Mosfet	*getMosfetList() { return mos_list;}
	
	void parseRLC(char *name, char *n1, char *n2, 
					char *value, char *var_name);
	// Parse an independent source.
	void parseSRC(char *name, char *n1, char *n2, 
					char *dc_val, char *ac_val);
	// Parse an output control. br_name is the branch from which
	// the output current is measured.
	void parseOUT(char *name, char *n1, char *n2, char *br_name);
	// Parse a VCCS/VCVS branch.
	void parseVCXS(char *name, char *n1, char *n2, char *nc1, char *nc2,
					char *value, char *var_name);
	// Parse a CCCS/CCVS branch.
	void parseCCXS(char *name, char *n1, char *n2, char *Vname,
					char *value, char *var_name);
	void parseMOSFET(char *name, char *nd, char *ng, char *ns, char *nb,
				char *model, char *w, char *l);
private:
	/* Methods for accessing the Node_g List */
	Node_g *query_node(int node_num);
	void add_node(Node_g *node);
	// Renumber and count the nodes in the node_list.
	void renumber_nodes();

	/* Methods for accessing the Edge_g List */
	Edge_g *query_edge(char *edge_name);
	Edge_g *query_pair(char *pair_name); // query an edge with the pair_name
	void add_edge(Edge_g *edge);
	void add_edge_at_head(Edge_g *edge);
	void add_mosfet(Mosfet* mosfet);
	Mosfet *query_mosfet(char* name);
};

#endif

