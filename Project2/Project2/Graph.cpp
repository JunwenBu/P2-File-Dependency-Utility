#include "Graph.h"
///////////////////////////////////////////////////////////////////////
// Graph.cpp - Support basic operations and algorithma on Graph      //
// ver 1.0 3/3/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
///////////////////////////////////////////////////////////////////////
/*
 * Required Files:
 * ===============
 * Vertex.h, Vertex.cpp, StringConvert.h, StringConvert.cpp
 * FileMgr.h, FileMgr.cpp, XmlReader.h, XmlReader.cpp
 *
 * DESCRIPTION:
 * =========================================================
 * This module provides several classes and global functions:
 *
 *
 * GLOBAL FUNCTIONS description:
 * /////////////////////////////////////////////////////////////////////
 * ==== class buildGraph:
 * This is a global template function that builds a graph instance from 
 * dependency relationships in an XML file.
 * ---------------------------------------- 
 * ==== class getVertexIdByValue:
 * This global functions return collections of all vertices ids that match
 * a specific value for the contained instance of the vertex parameterized 
 * type.
 * ----------------------------------------
 * ==== class getEdgeInfoByValue:
 * This global functions return collections of all edges info that match a 
 * specific value of the contained instance of the edge parameterized type.
 * -----------------------------------------
 *
 * CORE CLASS description:
 * ///////////////////////////////////////////////////////////////////////
 * ====< CLASS GRAPH >=========
 * Module Operations 
 * =================================
 * ==== class Graph: provides assignment and copy construction, support
 * analysys of strong components and creation of a condensed graph. Plus,
 * implement a Topological Sorting algorithm on condensed graph. Class 
 * Graph also provide algorithm to find partitions of vertices. Class Graph
 * also provide basic operation support on a graph and a collection of get
 * functions.
 *
 * Public Interface:
 * ==========================
 * Constructor and Assignment Function:
 * ------------------------------------
 * Graph();                                       // void constructor
 * Graph(AdjList &adjacentList);                  // promotion constructor: Graph(AdjList&)
 * Graph(AdjList &adjacentList, EdgeList &eList); // promotion constructor: Graph(AdjList&, EdgeList&)
 * Graph(const Graph &g);                         // copy constructor
 *
 * Algorithms:
 * ------------------------------------
 * std::vector< std::vector<int> > findStrongComponents();           // algorithm to find Strong Components 
 * std::vector<int> topologicalOrder(Graph<VertexType,EdgeType> &g); // algorithm to find Topological Sorting order
 * std::vector<std::vector<int>> findPartitions();                   // find Partitions of vertice store vertex id into vector
 * void Graph<VertexType, EdgeType>::doCondensation(Graph<std::vector<int>, Edge> &graph); // algorithm to do condensation
 *
 * Get Functions:
 * ----------------------------
 * bool isGraphEmpty()const;                               // return true if Graph is empty
 * int getNumberOfVertex()const;                           // get number of vertice
 * int getNumberOfEdge()const;                             // get number of edges
 * std::vector<int> getVertexID(VertexType value);         // get vertex id collection according to vertex value
 * EdgeType& getEdgeValue(int vertexU, int vertexV);       // get edge value
 * int getIndex(int id);                                   // get Vertex index in adjacent list by Vertex id
 * VertexType& getVertexValue(int vertexID);               // get value of vertex
 * int getVertexNum(int vertexID);                         // get num of vertex by vertex id
 * Vertex<VertexType, EdgeType>& getVertex(int vertexID);  // get Vertex object by vertex id
 * int getSourceVertexId(Edge& edge);                      // get Sourse VertexId of edge
 * int getDestVertexId(Edge& edge);                        // get Dest VertexId of edge
 * std::vector<Vertex<VertexType, EdgeType>>& getAdjList();// get reference of adjList
 * std::vector< std::pair<std::pair<int,int>,EdgeType> >& getEdgeList(); // get reference of edgeList
 *
 * Basic TOOL Functions:
 * ---------------------
 * void addVertex(Vertex<VertexType, EdgeType> v);         // add new Vertex to adjList
 * void addEdge(int fromID, int toID, EdgeType e);         // add new edge to edgeList
 * void displayAdjList();                 // display adjList
 * void displayAdjListWithoutValueType(); // display adjList without displaying value info
 * void displayEdgeList();                // display edgeList
 * void displayEdgeListWithoutValue();    // display edgeList without edge value info
 * void displayCondensedGraph(Graph<std::vector<int>, Edge> &graph); // display condensed Graph  
 *
 * Private Function Catalog:
 * ==========================
 * strong              //used by findStrongComponents
 * destroy             // delete all the element in adjList and edgeList
 * copyElems           // copy elements in adjList and edgeList to Graph from g
 * findBaseVertexId    // find base vertex id in StrongComponent
 * int getVertexBaseId // get Verctex's Base Vertex Id
 * buildUndirectedG    // build undirected grpah
 * search              // explore Functoin will be used by findPartitions()
 *
 *
 * //////////////////////////////////////////////////////////////////////////////
 * ====< CLASS GRAPH_SEARCH >=================
 * ====< MODULE GRAPH SEARCH >=========
 * Module Operations 
 * =================================
 * ==== class GraphSearch, DFS: This module provide facilities for searching the 
 * graph in several ways. class DFS is a functor operation, combined with class 
 * GraphSearch implements Depth First Search (DFS) by using functor.
 *
 * HOW TO USE (interface:)
 * ================================================================
 * GraphSearch< T,VertexType, EdgeType >
 * DFS< VertexType, EdgeType >
 * GraphSearch< DFS<std::string,std::string>, std::string, std::string > dfsSearch;
 * T: DFS<std::string,std::string>; 
 * VertexType: std::string
 * EdgeType: std::string
 *
 */

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

// -------- < test stub > ------------------------------------------------
#ifdef TEST_GRAPH
int main(){
  // test build Graph facility:
  std::string path="../Test";
  std::string filename="GraphRep.xml";
  Graph<std::string,std::string> g=buildGraph<std::string,std::string>(path, filename);
  g.displayAdjList();  // display infomation:
  std::cout<<std::endl;
  g.displayEdgeList();
  GraphSearch<DFS<std::string,std::string>,std::string,std::string> dfsSearch;
  std::vector<int> vec;
  dfsSearch.search(g,vec);
  std::cout<<"Vertex id sequence:"<<std::endl;
  for(std::vector<int>::iterator iter=vec.begin();iter!=vec.end();++ iter)
    std::cout<<*iter<<" ";
  std::cout<<std::endl;  // display Strong Components Sets
  std::cout<<"\nStrong Components: \n"<<"========================================="<<std::endl;
  std::vector<std::vector<int>> strongSet;
  strongSet=g.findStrongComponents();
  for(size_t i=0;i<strongSet.size();++i){
    std::cout<<"Vertex id Set"<<i+1<<" :  ";
    for(size_t j=0;j<strongSet[i].size();++j)
      std::cout<<strongSet[i][j]<<" ";
    std::cout<<std::endl;
  }
  std::cout<<std::endl;
  Graph<std::vector<int>, Edge> graph;
  g.displayCondensedGraph(graph);
  std::cout<<std::endl;
  // display topological order on condensed graph
  std::cout<<"Topological Order on Condensed Graph:\n"<<"========================================="<<std::endl;
  std::vector<int> t=graph.topologicalOrder(graph);
  for(size_t i=0;i<t.size();++i)
    std::cout<<t[i]<<" ";
  std::cout<<std::endl<<std::endl;
  // display partitions
  std::cout<<"Display id of Vertex Partitions:\n"<<"========================================="<<std::endl;
  std::vector<std::vector<int>> partitionSet=g.findPartitions();
  for(size_t i=0; i<partitionSet.size(); ++i){
    std::cout<<"Partition id Set"<<i+1<<":  ";
    for(size_t j=0; j<partitionSet[i].size(); ++j)
      std::cout<<partitionSet[i][j]<<" ";
    std::cout<<std::endl;
  }
}

#endif
// end test stub