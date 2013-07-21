///////////////////////////////////////////////////////////////////////
// Exec.cpp - test graph facilities and display to demonstrate meet  //
//            all the requirements of the specification.             //
// ver 1.0 3/9/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
///////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==========================================
* This is a test executive package, it also displays the test results,
* use all graph facilities to demonstrates my program meets all the 
* requirement of the specification. It demonstrate building an XML file
* describing dependencies among files on a path; creating a graph from 
* that XML file; analyzing strong components and place them in a 
* topological order, then display this sequence.
*
* Required Files:
* ==========================================
* Graph.h, Graph.cpp, 
* FileDependencyBuiler.h,
* FileDependencyBuiler.cpp.
*
* Function Catalog:
* ==========================================
* testGraphAlgorithm(Graph &graph); // used by main function to do graph algorithms
* main(argc, argv[]);               // the main function 
*
*/
#include "Graph.h"
#include "FileDependencyBuilder.h"

// Adjacency List typedef
typedef Vertex<std::string, std::string> VertexUnit;
typedef std::vector<VertexUnit> AdjList;
// Edge List typedef
typedef std::pair< std::pair<int,int>, std::string > Edge;
typedef std::vector<Edge> EdgeList;
// Condensed Adjcency List typedef
typedef Vertex< std::vector<int>, Edge > CondensedVertex;
typedef std::vector<CondensedVertex> CondensedAdjList;
// Condensed Edge List typedef
typedef std::pair< std::pair<int,int>, Edge > CondensedEdge;
typedef std::vector<CondensedEdge> CondensedEdgeList;

// -----< Test Graph Algorithms on the graph built from File-Dependency XML file >-----
void testGraphAlgorithm( Graph<std::string,std::string> &graph )
{ // ----< display Graph info >-------------------------------------
	graph.displayAdjList();
	std::cout<<std::endl;
	graph.displayEdgeList();
	// -----< Test DFS Algorithm >-------------------------------------
	GraphSearch<DFS<std::string,std::string>,std::string,std::string> dfsSearch;
	std::vector<int> resultVec; // resultVec is used to store DFS result: veterx id sequence
	dfsSearch.search(graph,resultVec); // do DFS algorithm on graph, store result into resultVect 
	std::cout<<"Vertex id sequence:"<<std::endl;
	for(std::vector<int>::iterator iter=resultVec.begin();iter!=resultVec.end();++ iter)
		std::cout<<*iter<<" ";
	// ----< Test Strong Components Algorithm >-------------------------
	std::cout<<std::endl<<"\n========= < Strong Components > =========\n"<<
		"========================================="<<std::endl;
	std::vector<std::vector<int>> strongSet;
	strongSet=graph.findStrongComponents();
	for(size_t i=0;i<strongSet.size();++i){
		std::cout<<"Vertex id Set"<<i+1<<" :  ";
		for(size_t j=0;j<strongSet[i].size();++j)
			std::cout<<strongSet[i][j]<<" ";
		std::cout<<std::endl;}
	// ----< Test Condensation Algorithm >-----------------------------------------
	// displayCondensedGraph(graph) Function contains do doCondensation algorithm function
	Graph<std::vector<int>, Edge> condensedgraph;
	graph.displayCondensedGraph(condensedgraph);
	// ----< Test Topological Order Algorithm >------------------------------------
	std::cout<<std::endl<<"==<Topological Order on Condensed Graph>=\n"
		<<"========================================="<<std::endl;
	std::vector<int> t=condensedgraph.topologicalOrder(condensedgraph);
	for(size_t i=0;i<t.size();++i)
		std::cout<<t[i]<<" ";
	// ----< Test Find Partitions Algorithm >----------------------------------
	std::cout<<std::endl<<std::endl<<"======== < Partitions of Vertex id > ====\n"
		<<"========================================="<<std::endl;
	std::vector<std::vector<int>> partitionSet=graph.findPartitions();
	for(size_t i=0; i<partitionSet.size(); ++i){
		std::cout<<"Partition id Set"<<i+1<<":  ";
		for(size_t j=0; j<partitionSet[i].size(); ++j){
			std::cout<<partitionSet[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}

// --------------- < main function > --------------------------------------------
// assumes path may be named by argv[1], patterns may be named by argv[2], ...
int main(int argc, char* argv[]) 
{  
	// initialization: File Dependency will be generated in path directory and file name will be xmlGenerated
	std::string path = "../Test", xmlGenerated="FileDependency.xml"; 
	std::string complexRep="ComplexGraphRep.xml"; 
	if(argc>=2){
		path=argv[1];
		std::cout <<"Processing files in: " <<argv[1]<< std::endl;
		if(argc>=3) {xmlGenerated=complexRep=argv[2];}
	}
#ifdef TEST_COMPLEX_GRAPH // do complex graph Test:
	std::cout<< "\n  Start Complex Graph Test:\n"
		"=================================================="<<std::endl;
	Graph<std::string, std::string> graph=buildGraph<std::string,std::string>(path, complexRep);
	// -----< Test Graph Algorithms on the graph >---------
	testGraphAlgorithm(graph);
	std::string edgeValue="e4";
	std::cout<<std::endl;
#else // do regular File Dependency Test:
	FileDependencyBuilder fdBuilder;
	std::cout<< "\n  Start File Dependency Test:\n"
		"==================================================\n"
		<<"Build File Dependency XML File. \n"<<"... ... ... ... ... ... ... ..."<<std::endl;
	fdBuilder.scanFiles(path,xmlGenerated);
	std::cout<<std::endl<<"========= < Graph Information > ========="<<std::endl;
	// -----< Test build Graph from an xml facility >---------------------- 
	Graph<std::string,std::string> graph=buildGraph<std::string, std::string>(path, xmlGenerated);
	// -----< Test Graph Algorithms on the graph built from File-Dependency XML file >---------
	testGraphAlgorithm(graph);
	std::cout<<std::endl;
	std::string edgeValue="include:v4";
#endif
	// ----< test Global Function: getVertexIdByValue >-----------
	std::string value="v2";
	std::cout<<"========= < Test Global Function > ======="
		"\ngetVertexIdByValue -- Find vertex value: \"v2\""<<std::endl;
	std::vector<int> ivec=getVertexIdByValue(graph, value);
	for(size_t i=0; i<ivec.size();++i)
	{
		std::cout<<" vertexID="<<ivec[i]<<"; ";
	}
	// ----< test Global Function: getEdgeInfoByValue >-----------
	std::cout<<std::endl<<"getEdgeInfoByValue -- Find edge value: \"include:v4\""<<std::endl;
	std::vector<std::pair<int,int>> edgevec=getEdgeInfoByValue(graph,edgeValue);
	for(size_t i=0; i<edgevec.size();++i)
		std::cout<<" edge: <"<<edgevec[i].first<<","<<edgevec[i].second<<">; ";
	std::cout<<std::endl<<std::endl;
	return 0;
}

// end main function