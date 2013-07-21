#ifndef GRAPH_H
#define GRAPH_H
///////////////////////////////////////////////////////////////////////
// Graph.h - Support basic operations and algorithma on Graph        //
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
* ====< CLASS GRAPH >==============
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

#include "StringConvert.h"
#include "FileMgr.h"
#include "XmlReader.h"
#include "Vertex.h"
#include <map>
using namespace WinTools_Extracts;

/* ////////////////////////////////////////////////////////////////////////
* ====< GLOBAL FUNCTIONS: >===============================================
* Function Description:
* =================================
*
* ==== class buildGraph:
* This is a global template function that builds a graph instance from 
* dependency relationships in an XML file.
* ---------------------------------------- 
* parameters: std::string& path, std::string& filename
*
* ==== class getVertexIdByValue:
* This global functions return collections of all vertices ids that match
* a specific value for the contained instance of the vertex parameterized 
* type.
* ----------------------------------------
* parameters: Graph<VertexType, EdgeType> &graph , const VertexType& value
*
* ==== class getEdgeInfoByValue:
* This global functions return collections of all edges info that match a 
* specific value of the contained instance of the edge parameterized type.
* -----------------------------------------
* parameters: Graph<VertexType, EdgeType> &graph , const EdgeType& value
*
*========================================================================*/
typedef std::vector<std::string> strVector;
// ----< Global function: buildGraph >-------------------------------------
template <typename VertexType, typename EdgeType>
Graph<VertexType, EdgeType> buildGraph(std::string& path, std::string& filename)
{ // create objects edgeList, adjList used by Graph
	std::vector< std::pair< std::pair<int,int>,EdgeType > > edgeList;
	std::vector< Vertex<VertexType,EdgeType> > adjList;
	std::string xml;
	std::string fileposition=path+"/"+filename;
	std::ifstream in;
	in.open(fileposition.c_str());
	while(in.good()){   // write contents to xml
		if(!in.eof())
			xml+=in.get();
	}
	in.close();
	// int vid used as vertex id 
	int vid=0;
	XmlReader rdr(xml); // create XmlReader object rdr
	while(rdr.next()){ // find keyword "vertex"
		if(rdr.tag().compare("vertex")==0){ // start create vertex
			XmlReader::attribElems vattribs = rdr.attributes();
			vid=atoi(vattribs[0].second.c_str());
			VertexType vtype; 
			convertStringToVertexType<VertexType>(vattribs[1].second.c_str(),vtype);
			Vertex<VertexType,EdgeType> vertex=Vertex<VertexType,EdgeType>(vid,vtype);
			adjList.push_back(vertex); // push vertex into adjList
		}
		if(rdr.tag().compare("edge")==0){ // start create edge
			XmlReader::attribElems eattribs = rdr.attributes();
			EdgeType etype;
			convertStringToEdgeType<EdgeType>(eattribs[1].second.c_str(),etype);
			int childid=atoi(rdr.body().c_str());
			std::pair<std::pair<int,int>, EdgeType> edge(std::pair<int,int>(vid,childid),etype);
			adjList.back().addEdge(childid,etype);
			edgeList.push_back(edge); // push edge into edgeList
		}
	} // use adjList and edgeList to build Graph
	Graph<VertexType, EdgeType> graph(adjList,edgeList);
	return graph;
}

// ----< Global function: getVertexIdByValue >-------------------
template <typename VertexType, typename EdgeType>
std::vector<int> getVertexIdByValue( Graph<VertexType, EdgeType> &graph , const VertexType& value)
{ // create temp vector to store id
	std::vector<int> idvec;
	for(size_t i=0;i<graph.getAdjList().size();++i)
	{ // compair value with the value of vertex in adjList
		if(graph.getAdjList()[i].getValue()==value)
			idvec.push_back(graph.getAdjList()[i].getID());
	}// if such vertex not found, display info to user
	if(idvec.empty())
		std::cout<<"No vertex matchs the search value!"<<std::endl;
	return idvec;
}

// ----< Global function: getEdgeInfoByValue >-------------------
template <typename VertexType, typename EdgeType>
std::vector<std::pair<int,int>> getEdgeInfoByValue( Graph<VertexType, EdgeType> &graph , const EdgeType& value)
{ // create temp vector to store pair describing <source_vertex_id, dest_vertex_id>
	std::vector<std::pair<int, int>> edgevec;
	for(size_t i=0;i<graph.getEdgeList().size();++i)
	{
		if(graph.getEdgeList()[i].second==value)
		{ // compiar value with the value of the edge in edgeList
			std::pair<int, int> temppair(graph.getEdgeList()[i].first.first, graph.getEdgeList()[i].first.second);
			edgevec.push_back(temppair);
		}
	} // if such edge not found, display info to user
	if(edgevec.empty())
		std::cout<<"No edge matchs the search value!"<<std::endl;
	return edgevec;
}
// <GLOBAL FUNCTIONS END>
///////////////////////////////////////////////////////////////////////////////


/* ====< GRAPH SEARCH MODULE >=================================================
* Module Operations 
* =================================
* ==== class GraphSearch, DFS: This module provide facilities for searching the 
* graph in several ways. class DFS is a functor operation, combined with class 
* GraphSearch implements Depth First Search (DFS) by using functor.
*==========================================================================*/
// ----< class GraphSearch >------------------------------ 
template <typename T, typename VertexType, typename EdgeType> 
class GraphSearch
{ // Functor class
public:
	GraphSearch(); // constructor
	void search(Graph<VertexType,EdgeType> &g,std::vector<int>&vec); // search function
private:
	T specificSearch;
};

template <typename T, typename VertexType, typename EdgeType>
GraphSearch<T, VertexType, EdgeType>::GraphSearch():specificSearch()
{
	//display information to show user that search begin
	std::cout<<"\n========= < Start Graph Search > ======== \n" 
		<<"========================================="<<std::endl;
}

// ----< search function >---------------------
template <typename T, typename VertexType, typename EdgeType>
void GraphSearch<T, VertexType, EdgeType>::search(Graph<VertexType,EdgeType> &g, std::vector<int>& vec)
{
	specificSearch(g,vec); 
}

// ----< class DFS >---------------------------
template<typename VertexType, typename EdgeType>
class DFS
{ // support Depth Firest Search Algorithm on the graph
	typedef std::vector<Vertex<VertexType,EdgeType>> AdjList ;
public:
	DFS(); // constructor
	// () operator: functor core technique
	void operator()(Graph<VertexType, EdgeType> &g, std::vector<int>& v);
private:
	// explore used by DFS algorithm
	void explore(Graph<VertexType,EdgeType> &g, Vertex<VertexType, EdgeType>& v, std::vector<int>& vec);
	int i;
};

// ----< void constructor >----------------------------
template<typename VertexType, typename EdgeType>
DFS<VertexType, EdgeType>::DFS():i(0)
{} 

// ----< ()operator: do DFS >-------------------------------
template<typename VertexType, typename EdgeType>
void DFS<VertexType,EdgeType>::operator()(Graph<VertexType,EdgeType> &g, std::vector<int> &vec)
{ // display info show that DFS begin
	std::cout<<"Do Depth-First Search in directed Graphs:"<<std::endl;
	// initialize all vertex num to ZERO
	for(AdjList::iterator iter=g.getAdjList().begin();iter!=g.getAdjList().end();++iter)
		iter->setNum(0); 
	for(AdjList::iterator iter=g.getAdjList().begin();iter!=g.getAdjList().end();++iter)
	{ // if vertex num is ZERO then do explore
		if(iter->getNum()==0) 
			explore(g,*iter,vec);
	}
}

// ----< explore function: used by DFS algorithm >---------------
template<typename VertexType, typename EdgeType>
void DFS<VertexType, EdgeType>::explore(Graph<VertexType,EdgeType> &g, Vertex<VertexType, EdgeType>& v, std::vector<int>& vec)
{
	i++; // update
	v.setNum(i); // set vertex num, if num is not zero then this vertex is visited
	vec.push_back(v.getID()); 
	std::vector<int> vect=v.getChildIdCollection();
	for(size_t j=0;j<vect.size();j++)
	{ // for all vertex belongs to adj(v)
		int id=vect[j];
		if(g.getVertexNum(id)==0)
		{ // this is a tree edge
			explore(g,g.getVertex(id),vec);
		}
	}
}
// end <GRAPH SEARCH MODULE>

/* ////////////////////////////////////////////////////////////////////////
* ====< CLASS GRAPH >=====================================================
* Module Operations 
* =================================
* ==== class Graph: provides assignment and copy construction, support
* analysys of strong components and creation of a condensed graph. Plus,
* implement a Topological Sorting algorithm on condensed graph. Class 
* Graph also provide algorithm to find partitions of vertices. Class Graph
* also provide basic operation support on a graph and a collection of get
* functions.
*========================================================================*/
template <typename VertexType, typename EdgeType> class Graph
{
	// Adjacent List typedef:
	typedef Vertex<VertexType, EdgeType> VertexUnit;
	typedef std::vector<VertexUnit> AdjList;
	// Edge List typedef:
	typedef std::pair<std::pair<int,int>, EdgeType> Edge;
	typedef std::vector<Edge> EdgeList;
	// Condensed Graph Vertex and Edge typedef:
	typedef Vertex< std::vector<int>, Edge > CondensedVertex;
	typedef std::vector<CondensedVertex> CondensedAdjList;
	typedef std::pair< std::pair<int,int>, Edge > CondensedEdge;
	typedef std::vector<CondensedEdge> CondensedEdgeList;

public:
	Graph();                                       // void constructor
	Graph(AdjList &adjacentList);                  // promotion constructor: Graph(AdjList&)
	Graph(AdjList &adjacentList, EdgeList &eList); // promotion constructor: Graph(AdjList&, EdgeList&)
	Graph(const Graph &g);                         // copy constructor
	const Graph& operator=(const Graph &g);        // assignment function
	~Graph();                                      // destructor

	// algorithms:
	std::vector< std::vector<int> > findStrongComponents();           // algorithm to find Strong Components 
	std::vector<int> topologicalOrder(Graph<VertexType,EdgeType> &g); // algorithm to find Topological Sorting order
	std::vector<std::vector<int>> findPartitions();                   // find Partitions of vertice store vertex id into vector
	void Graph<VertexType, EdgeType>::doCondensation(Graph<std::vector<int>, Edge> &graph); // algorithm to do condensation

	// get Functions:
	bool isGraphEmpty()const;                               // return true if Graph is empty
	int getNumberOfVertex()const;                           // get number of vertice
	int getNumberOfEdge()const;                             // get number of edges
	std::vector<int> getVertexID(VertexType value);         // get vertex id collection according to vertex value
	EdgeType& getEdgeValue(int vertexU, int vertexV);       // get edge value
	int getIndex(int id);                                   // get Vertex index in adjacent list by Vertex id
	VertexType& getVertexValue(int vertexID);               // get value of vertex
	int getVertexNum(int vertexID);                         // get num of vertex by vertex id
	Vertex<VertexType, EdgeType>& getVertex(int vertexID);  // get Vertex object by vertex id
	int getSourceVertexId(Edge& edge);                      // get Sourse VertexId of edge
	int getDestVertexId(Edge& edge);                        // get Dest VertexId of edge
	std::vector<Vertex<VertexType, EdgeType>>& getAdjList();// get reference of adjList
	std::vector< std::pair<std::pair<int,int>,EdgeType> >& getEdgeList(); // get reference of edgeList

	void addVertex(Vertex<VertexType, EdgeType> v);         // add new Vertex to adjList
	void addEdge(int fromID, int toID, EdgeType e);         // add new edge to edgeList

	void displayAdjList();                 // display adjList
	void displayAdjListWithoutValueType(); // display adjList without displaying value info
	void displayEdgeList();                // display edgeList
	void displayEdgeListWithoutValue();    // display edgeList without edge value info
	void displayCondensedGraph(Graph<std::vector<int>, Edge> &graph); // display condensed Graph

private:
	AdjList adjList;        // list to store vertice
	EdgeList edgeList;      // list to store edges

	/* private Functions: */
	// strong Function: findStrongComponents function use 
	// this private function to find strong components
	void strong(std::vector<std::vector<int>> & vect, Vertex<VertexType, EdgeType> &v, std::vector<int> &idStack, int& i);
	void destroy();                               // delete all the element in adjList and edgeList
	void copyElems(const Graph& g);               // copy elements in adjList and edgeList to Graph from g
	int findBaseVertexId(std::vector<int> idvec); // find base vertex id in StrongComponent
	// get Verctex's Base Vertex Id
	int getVertexBaseId(int vertexID, std::vector< std::pair<int, int> > &idAndBaseidVect);
	// build undirected grpah
	std::vector<std::vector<int>> buildUndirectedG();
	// explore Functoin will be used by findPartitions()
	void search(std::vector<std::vector<int>> & graph, std::vector<int> & visit, int index, int& setNum);

};

// ----< void constructor >-----------------------
template <typename VertexType, typename EdgeType>
Graph<VertexType, EdgeType>::Graph()
{}

// ----< promotion constructor: Graph(AdjList, EdgeList) >-----------------------
template <typename VertexType, typename EdgeType>
Graph<VertexType, EdgeType>::Graph(AdjList &adjacentList, EdgeList &eList):
adjList(adjacentList),edgeList(eList)
{}

// ----< promotion constructor: Graph(AdjList) >-----------------------
template <typename VertexType, typename EdgeType>
Graph<VertexType, EdgeType>::Graph(AdjList &adjacentList):
adjList(adjacentList)
{}

// ----< copy constructor >-----------------------
template <typename VertexType, typename EdgeType>
Graph<VertexType, EdgeType>::Graph(const Graph<VertexType, EdgeType> &g)
{
	// copy elements from g
	copyElems(g);
}

// ----< assignment function >-----------------------
template <typename VertexType, typename EdgeType>
const Graph<VertexType, EdgeType>& Graph<VertexType, EdgeType>::operator=(const Graph<VertexType, EdgeType> &g)
{
	if(this!=&g)
	{
		// delete all the elements in adjList and edgeList
		destroy();
		copyElems(g);
	}
	return *this;
}

// ----< return true if Graph is empty >-----------------------
template <typename VertexType, typename EdgeType>
bool Graph<VertexType, EdgeType>::isGraphEmpty()const
{
	return adjList.empty();
}

// ----< get number of vertice >-----------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType, EdgeType>::getNumberOfVertex()const
{
	return adjList.size();
}

// ----< get number of edges >-----------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType, EdgeType>::getNumberOfEdge()const
{
	return edgeList.size();
}

// ----< get id collection according to vertex value >-----------------------
template <typename VertexType, typename EdgeType>
std::vector<int> Graph<VertexType, EdgeType>::getVertexID(VertexType value)
{ // create temp vector to store id
	std::vector<int> idvec;
	for(size_t i=0;i<adjList.size();++i)
	{
		if(adjList[i].getValue()==value)
			idvec.push_back(adjList[i].getID());
	}
	return idvec;
}

// ----< get edge value by (u,v), 
// u is source vertex id and v is destinated vertex id >----------------------
template <typename VertexType, typename EdgeType>
EdgeType& Graph<VertexType, EdgeType>::getEdgeValue(int uid, int vid)
{ 
	// initialize iterator
	for(EdgeList::iterator iter=edgeList.begin();iter!=edgeList.end(); ++iter)
	{ 
		if(iter->first.first==uid && iter->first.second==vid )
			return iter->second;
	}throw NULL;
}


// ----< get Vertex index in adjacent list by Vertex id >---------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType,EdgeType>::getIndex(int id)
{
	int i=id-1; 
	// check if vertex is stored in position id-1
	if(id<=(int)adjList.size()&&0<id&&adjList[i].getID()==id)
		return i;
	// if vertices are stored not in order
	for(size_t j=0;j<adjList.size();++j)
	{
		if(adjList[j].getID()==id)
			return j;
	}
	return -1;
}

// ----< get value of vertex by vertex id>-----------------------
template <typename VertexType, typename EdgeType>
VertexType& Graph<VertexType, EdgeType>::getVertexValue(int vertexID)
{
	int i=vertexID-1;
	// in this case vertex stored in order
	if(vertexID<=adjList.size()&&vertexID>0&&adjList[i].getID()==vertexID)
		return adjList[i].getValue();
	else 
		// in this case vertex stored without order
		// need to search the index of vertex
		for(int j=0;j<adjList.size();++j)
		{
			if(adjList[j].getID()==vertexID)
				return adjList[j].getValue();
		}throw NULL;
}

// ----< get num of vertex by vertex id >-----------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType, EdgeType>::getVertexNum(int vertexID)
{
	int i=getIndex(vertexID);
	if (i>=0)
		return adjList[i].getNum();
	return -1;
}

// ----< get Vertex by id >-----------------------
template <typename VertexType, typename EdgeType>
Vertex<VertexType, EdgeType>& Graph<VertexType,EdgeType>::getVertex(int vertexID)
{
	int i=getIndex(vertexID);
	if(i>=0)
		return adjList[i];
	throw NULL;
}

// ----< Strong Components Algorithm >-----------------------
template <typename VertexType, typename EdgeType>
std::vector<std::vector<int>> Graph<VertexType,EdgeType>::findStrongComponents()
{ // store result in retVector
	std::vector< std::vector<int> > retVector;
	std::vector<int> idStack;
	int i=0;
	// initialize vertex number and lowlink to zero
	for(size_t n=0; n<adjList.size(); ++n)
	{ 
		adjList[n].setNum(0);
		adjList[n].setLowlink(0);
	}
	// invoke strong function 
	for(size_t s=0; s<adjList.size(); ++s)
	{
		if(adjList[s].getNum()==0)
			strong(retVector,adjList[s],idStack, i);
	}
	return retVector;
}

// ----< strong function >--------------------------
// strong Function: findStrongComponents function use 
// this private function to find strong components
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::strong(std::vector<std::vector<int>> & vect,
	Vertex<VertexType, EdgeType> &v, std::vector<int> &idStack, int& i)
{
	i++;   // i<-i+1
	v.setNum(i); 
	v.setLowlink(i);
	idStack.push_back(v.getID()); // push v onto the stack S
	v.inStack=true;  // set inStack to be true
	std::vector<int> vectid=v.getChildIdCollection();
	for(size_t j=0; j<vectid.size(); j++)
	{
		int id=vectid[j];
		Vertex<VertexType, EdgeType> &w=getVertex(id);
		int wNum=w.getNum();
		if(wNum==0){ // if num(w)==0 then strong on w
			strong(vect,w,idStack,i);
			int low=(v.getLowlink()<w.getLowlink()?v.getLowlink():w.getLowlink());
			if(v.getLowlink()!=low)
				v.setLowlink(low);
		}else if(wNum<v.getNum()){
			if(w.inStack){ // to check w is in the same strong component as v
				int low=(v.getLowlink()<w.getNum()?v.getLowlink():wNum);
				if(v.getLowlink()!=low)
					v.setLowlink(low);
			}
		}
	} // end for
	int vNum=v.getNum(); 
	if(v.getLowlink()==vNum) // v is a base vertex
	{ 
		std::vector<int> tempVec;
		while(!idStack.empty() && this->getVertex(idStack.back()).getNum()>=vNum )
		{ // pop vertece off the stack while num(stack top)>= num(v)
			getVertex(idStack.back()).inStack = false;
			tempVec.push_back(idStack.back());
			idStack.pop_back();
		} // store tempVec to vect
		if(!tempVec.empty())
			vect.push_back(tempVec);
	}
}

// ----< find Topological Sorting order >-------------------------
template <typename VertexType, typename EdgeType>
std::vector<int> Graph<VertexType, EdgeType>::topologicalOrder(Graph<VertexType,EdgeType> &g)
{ // create retVector to store vertex id info to show the result of the algorithm
	std::vector<int> retVector;
	std::vector<int> idStack;
	for(int n=0; n<g.getNumberOfVertex(); ++n)
		g.getAdjList()[n].setNum(0);
	// calculate in degree of each vertex
	for(int j=0; j<g.getNumberOfVertex(); ++j){
		std::vector<int> vectid=g.getAdjList()[j].getChildIdCollection();
		for(size_t m=0;m<vectid.size(); ++m){
			Vertex<VertexType, EdgeType> &w = g.getVertex(vectid[m]);
			w.setNum(w.getNum()+1);
		}
	}
	// if a vertex's indgree is zero, it is a candidate of next vertex in topological sort order
	for(int j=0; j<g.getNumberOfVertex(); ++j){
		int in=g.getAdjList()[j].getNum();
		if(g.getAdjList()[j].getNum()==0)
			idStack.push_back(g.getAdjList()[j].getID());
	}
	// choose candidate of next vertex in topological sort order 
	for(int j=0; j<g.getNumberOfVertex(); ++j){
		if(idStack.empty()){ // in this case there is a cycle in the graph
			std::cout<<"There is a cycle in the Graph."<<std::endl;
			return retVector;
		}
		else{
			retVector.push_back(idStack.back());
			Vertex<VertexType, EdgeType> &s = g.getVertex(idStack.back());
			idStack.pop_back();
			std::vector<int> vectid=s.getChildIdCollection();
			for(size_t m=0;m<vectid.size(); ++m){
				Vertex<VertexType, EdgeType> &w = g.getVertex(vectid[m]);
				w.setNum(w.getNum()-1); // decress the indegree of the vertex 
				if(w.getNum()==0)
					idStack.push_back(w.getID());
			}
		}
	}
	return retVector;
}


// ----< add new Vertex to adjList >---------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::addVertex(Vertex<VertexType, EdgeType> v)
{
	adjList.push_back(v);
}

// ----< add edge to edgeList >-------------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::addEdge(int fromID, int toID, EdgeType e)
{
	std::pair<int,int> ft(fromID, toID);
	std::pair<std::pair<int,int>,EdgeType> edge(ft, e);
	edgeList.push_back(edge);
}

// ----< display adjList >-------------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::displayAdjList()
{ 
	std::cout<<"Display adjacent list:\n"
		<<"========================================="<<std::endl;
	if(isGraphEmpty()) // check to see if the graph has no vertex
		std::cout<<"This is an empty Graph."<<std::endl;
	else // display vertex info 
		for(size_t i=0; i<adjList.size(); ++i)
			adjList[i].display();
}

// ----< display adjList without Value type >----------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::displayAdjListWithoutValueType()
{ 
	std::cout<<"Display adjacent list:\n"
		<<"========================================="<<std::endl;
	std::cout<<"(VertexValue OR EdgeValue cannot be printed)"<<std::endl;
	if(isGraphEmpty()) // check to see if graph has no vertex
		std::cout<<"This is an empty Graph."<<std::endl;
	else // display vertex info with out info about vertex value
		for(size_t i=0; i<adjList.size(); ++i)
			adjList[i].displayWithoutType(); 
}

// ----< display edgeList >-------------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::displayEdgeList()
{
	std::cout<<"Display edge list:\n"
		<<"========================================="<<std::endl;
	if(getNumberOfEdge()==0) // check edgeList to see if is empty
		std::cout<<"This Graph has no edges."<<std::endl;
	else
		for(size_t i=0; i<edgeList.size(); ++i)
		{ // display edge info: source vertex, dest vertex and edge value
			std::cout<<"edge ("<<edgeList[i].first.first<<", "
				<<edgeList[i].first.second<<")"<<" :   "
				<<edgeList[i].second<<std::endl;
		}
}

// ----< display edgeList without edge value info >---------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::displayEdgeListWithoutValue()
{
	std::cout<<"Display edge list:\n"
		<<"========================================="<<std::endl;
	if(getNumberOfEdge()==0) // check edgeList to see if is empty
		std::cout<<"This Graph has no edges."<<std::endl;
	else
		for(size_t i=0; i<edgeList.size(); ++i)
		{ // display edge info: source vertex, destination vertex without edge value info
			std::cout<<"edge ("<<edgeList[i].first.first<<", "
				<<edgeList[i].first.second<<")"<<" :  value cannot be printed out. "<<std::endl;
		}
}

// ----< do condensation, store result to parameter graph >-----------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::doCondensation(Graph<std::vector<int>, Edge> &graph)
{
	// find strong components of original graph
	std::vector<std::vector<int>> strongSet=findStrongComponents();
	std::vector< std::pair<int, int> > idAndBaseidVect;
	CondensedAdjList condensedAdjList;
	// condense vertice in a same strong component into a single vertex 
	// use baseid as the new vertex id
	for(size_t i=0; i<strongSet.size(); ++i){
		int baseid=this->findBaseVertexId(strongSet[i]);
		CondensedVertex condensedVertex(baseid,strongSet[i]);
		condensedAdjList.push_back(condensedVertex);
		for(size_t j=0; j<strongSet[i].size(); ++j)
			idAndBaseidVect.push_back(std::pair<int, int>(strongSet[i][j],baseid) );
	}
	EdgeList copyEdgeList=edgeList; // copyEdgeList stores original graph edge info
	std::map< std::pair<int,int> , Edge > condensedEdgeMap;
	// create condensed edges which is used in condensed graph
	for(size_t m=0; m<copyEdgeList.size(); ++m){
		int sourceBaseid=getVertexBaseId(getSourceVertexId(copyEdgeList[m]),idAndBaseidVect);
		int destBaseid=getVertexBaseId(getDestVertexId(copyEdgeList[m]), idAndBaseidVect);
		if(sourceBaseid!=destBaseid){
			std::pair<int, int> newSourceDestpair(sourceBaseid,destBaseid);
			CondensedEdge condensedpair(newSourceDestpair,copyEdgeList[m]);
			condensedEdgeMap.insert(condensedpair); // insert condensed edge into a map
		}
	}
	// create edgeList of the new condensed graph
	CondensedEdgeList condensedEdgeList(condensedEdgeMap.begin(),condensedEdgeMap.end());
	for(CondensedEdgeList::iterator eit=condensedEdgeList.begin(); eit!=condensedEdgeList.end(); ++eit){
		for(CondensedAdjList::iterator vit=condensedAdjList.begin(); vit!=condensedAdjList.end();++vit){
			if(eit->first.first==vit->getID() )
				vit->addEdge(eit->first.second, eit->second);
		}
	}
	// after do condensation, the return value is also a graph object 
	Graph<std::vector<int>, Edge> condensedGraph(condensedAdjList,condensedEdgeList);
	graph=condensedGraph;
}

// ----< display condensed Graph >-----------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::displayCondensedGraph(Graph<std::vector<int>, Edge> &graph)
{
	std::cout<<"\n========= < Result of Condensation > ====\n"
		<<"========================================="<<std::endl;
	std::cout<<"Display condensed adjacency list:\n"
		<<"-----------------------------------------"<<std::endl;
	doCondensation(graph);    // run doCondensation algorithm
	if(graph.isGraphEmpty())  // to check if the graph is empty
		std::cout<<"This is an empty Graph."<<std::endl;
	else                      // if graph is not empty
		for(size_t i=0; i<graph.getAdjList().size(); ++i){ // display information about condensed vertex
			std::cout<<"Id: "<<graph.getAdjList()[i].getID()<<" condensed from vertex:  ";
			for(size_t j=0; j<graph.getAdjList()[i].getValue().size();++j)
				std::cout<<graph.getAdjList()[i].getValue()[j]<<" ";
			std::cout<<std::endl;
		}
		// display condensed edge information
		std::cout<<"\nDisplay condensed edge list:\n"
			<<"-----------------------------------------"<<std::endl;
		if(graph.getNumberOfEdge()==0)
			std::cout<<"This Graph has no edges."<<std::endl;
		else
			for(size_t n=0; n<graph.getEdgeList().size(); ++n){
				int sourceid=graph.getSourceVertexId(graph.getEdgeList()[n]);
				int destid=graph.getDestVertexId(graph.getEdgeList()[n]);
				std::cout<<"edge from "<<sourceid<<" to " // display original edge information
					<<destid<<" :  original edge <"<<graph.getEdgeValue(sourceid,destid).first.first<<","
					<<graph.getEdgeValue(sourceid,destid).first.second
					<<") value: "<<graph.getEdgeValue(sourceid,destid).second<<std::endl;
			}
}

// ----< find Partitions of vertice store vertex id into vector >------------------
template <typename VertexType, typename EdgeType> 
std::vector<std::vector<int>> Graph<VertexType,EdgeType>::findPartitions(){
	std::vector<std::vector<int>> partitionSet;
	std::vector<std::vector<int>> graph = buildUndirectedG();
	int setNum=0;
	std::vector<int> visit; // store tags to see vertice are visited
	for(size_t i=0; i<adjList.size(); ++i)
		visit.push_back(0);
	for(size_t j=0; j<adjList.size(); ++j){
		if(visit[j]==0){
			setNum++;
			search(graph,visit,j,setNum);
		}
	}
	// insert vertex in every partitions into partitionSet
	for(int i=1; i<=setNum; i++){
		std::vector<int> temp;
		for(size_t j=0;j<visit.size();++j){
			if(visit[j]==i)
				temp.push_back(adjList[j].getID());
		}
		partitionSet.push_back(temp);
	}
	return partitionSet;
}

// ----< destructor >-----------------------
template <typename VertexType, typename EdgeType>
Graph<VertexType, EdgeType>::~Graph()
{
	destroy();
}

// private functions:

// ----< delete all the element in adjList and edgeList >-----------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::destroy()
{ // delete elements in edgeList
	while(!edgeList.empty())
		edgeList.pop_back();
	// delete elements in adjList
	while(!adjList.empty())
		adjList.pop_back(); 
}

// ----< copy elements in adjList and edgeList to Graph from g >---------
template <typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::copyElems(const Graph& g)
{
	// copy elements from g's adjList
	for(size_t i=0; i<g.adjList.size();++i)
		adjList.push_back(g.adjList[i]);
	// copy elements from g's edgeList
	for(size_t j=0; j<g.edgeList.size();++j)
		edgeList.push_back(g.edgeList[j]);
}

// ----< find base vertex id in StrongComponent >-------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType, EdgeType>::findBaseVertexId(std::vector<int> idvec)
{
	for(size_t i=0; i<idvec.size(); ++i)
	{
		if(getVertex(idvec[i]).getNum()==getVertex(idvec[i]).getLowlink())
			return idvec[i];
	}
	// if fail to find baseid, return -999
	return -999;
}

// ---< get Verctex's Base Vertex Id >-----------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType,EdgeType>::getVertexBaseId(int vertexID, std::vector< std::pair<int, int> > &idAndBaseidVect)
{
	for(size_t i=0; i<idAndBaseidVect.size(); ++i){
		if(idAndBaseidVect[i].first==vertexID)
			return idAndBaseidVect[i].second;
	}
	// if return -999 that means no such id
	return -999;
}

// ----< get & adjList >-----------------------
template <typename VertexType, typename EdgeType>
std::vector<Vertex<VertexType, EdgeType>>& Graph<VertexType, EdgeType>::getAdjList()
{
	return adjList;
}

// ----< get & edgeList >-----------------------
template <typename VertexType, typename EdgeType>
std::vector< std::pair<std::pair<int,int>,EdgeType> >& Graph<VertexType, EdgeType>::getEdgeList()
{
	return edgeList;
}

// ----< get Source VertexId of edge >-----------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType,EdgeType>::getSourceVertexId(Edge& edge)
{
	return edge.first.first;
}

// ----< get Dest VertexId of edge >---------------------
template <typename VertexType, typename EdgeType>
int Graph<VertexType,EdgeType>::getDestVertexId(Edge& edge)
{
	return edge.first.second;
}

// ----< build undirected graph >-----------------------
template <typename VertexType, typename EdgeType>
std::vector<std::vector<int>> Graph<VertexType, EdgeType>::buildUndirectedG()
{
	// create a 2-ary int vector to use as a matrix to store graph info
	std::vector<std::vector<int>> retVect;
	size_t size=adjList.size();
	// initialize retVect;
	for(size_t i=0;i<size;++i){
		std::vector<int> temp;
		for(size_t j=0;j<size; ++j){
			temp.push_back(0);
		}
		retVect.push_back(temp);
	}
	// if there is an edge between two vertex then set matrix representation
	for(size_t m=0; m<size; ++m){
		std::vector<int> childidVect=adjList[m].getChildIdCollection();
		for(size_t n=0; n<childidVect.size(); ++n){
			int index=getIndex(childidVect[n]);
			// change directed graph to indericted
			retVect[m][index]=1;
			retVect[index][m]=1;
		}
	}
	return retVect;
}

// ----< explore Functoin will be used by findPartitions() >-----------------------
template <typename VertexType, typename EdgeType>
void Graph<VertexType,EdgeType>::search(std::vector<std::vector<int>> & graph, std::vector<int> & visit, int index, int &setNum)
{
	// set vertex visit so that this vertex is visited
	visit[index]=setNum;
	for(size_t i=0; i<visit.size(); ++i)
	{ // if vertex not visited then search on this vertex
		if(graph[index][i]==1 && visit[i]==0)
			search(graph, visit, i, setNum);
	}
}

#endif