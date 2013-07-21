#ifndef VERTEX_H
#define VERTEX_H
///////////////////////////////////////////////////////////////////////
// Vertex.h - Vertex class support bassic operation on vertex        //
// ver 1.0 3/6/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
///////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * =================================
 * This module is the class of vertex, which will be used by class graph
 * This vertex class support basic operations on vertex including promotion
 * constructor, copy constructor, destructor and a collection of get and
 * set functions, it also provides simple display function to display info
 * about vertex.
 *
 * Public Interface:
 * ==========================
 * Constructor and Assignment Function:
 * ------------------------------------
 * Vertex(int id, VertexType value);          
 * Vertex(int id, VertexType value, VertexList &vectroList); 
 * Vertex(const Vertex &v);                 
 * const Vertex& operator=(const Vertex &v); 
 * Get and Set Functions:
 * ----------------------
 * int getID() const;                       // get vertex id
 * VertexType getValue() const;             // get vertex value
 * int getNum()const;                       // get vertex num
 * int getLowlink()const;                   // get vertex lowlink
 * std::vector<int> getChildIdCollection(); // get Child vertex id collection
 * void setID(int id);                      // set vertex id
 * void setValue(VertexType value);         // set vertex Value
 * void setNum(int n);                      // set vertex num
 * void setLowlink(int n);                  // set vertex lowlink
 * Basic TOOL Functions:
 * ---------------------
 * bool isSingleVertex()const;                      
 * void addEdge(std::pair<int,EdgeType>& edgepair); 
 * void addEdge(int childid, EdgeType edgeValue);           
 * Display Facilities:
 * ---------------------
 * void display() const;                            
 * void displayWithoutType() const;                 
 *
 * Private Function Catalog:
 * ==========================
 * void destroy();           
 * void copyElems(const Vertex& v); 
 */

#include <iostream>
#include <exception>
#include <vector>
#include <string>
// declare class Graph:
template <typename VertexType, typename EdgeType> class Graph;
// ----< class Vertex >-------------------------
template<typename VertexType, typename EdgeType> class Vertex
{
  // typedef VertexList
  friend class Graph<VertexType, EdgeType>;
  typedef std::vector<std::pair<int, EdgeType>> VertexList;

public:

  /* void constructor: declear void constructor Vertex() in private 
     to prevent user from defining a void constructor Vertex(void) */
   
  Vertex(int id, VertexType value);          // promotion constructor Vertex(int, VertexType)
  Vertex(int id, VertexType value, VertexList &vectroList); // promotion constructor Vertex(int VertexType VertexList)
  Vertex(const Vertex &v);                  // copy constructor Vertex(const Vertex &v)
  const Vertex& operator=(const Vertex &v); // assignment function

  // get and set functions:
  int getID() const;              // get vertex id
  VertexType getValue() const;    // get vertex value
  int getNum()const;              // get vertex num
  int getLowlink()const;          // get vertex lowlink
  void setID(int id);             // set vertex id
  void setValue(VertexType value);// set vertex Value
  void setNum(int n);             // set vertex num
  void setLowlink(int n);         // set vertex lowlink
  
  bool isSingleVertex()const;                      // true if vertex has no edges
  void addEdge(std::pair<int,EdgeType>& edgepair); // add edge pair to Vertex 
  void addEdge(int childid, EdgeType edgeValue);   // add edge to Vertex by (int, EdgeType)
  std::vector<int> getChildIdCollection();         // get Child vertex id collection
  void display() const;                            // display vertex infomation
  void displayWithoutType() const;                 // display vertex infomation without display value infomation

  ~Vertex();  // destructor

private:
  int id;                   // id of Vertex
  VertexType value;         // value of Vertex
  VertexList vertexList;    // contains index of children vertex
  int num;                  // used in DFS algorithm
  int lowlink;              // used in Strong Component algorithm
  bool inStack;             // used in Strong Component algotithm as tag
  // private functions:
  Vertex();                         // void constructor: prevent user from defining it
  void destroy();                   // delete all the edgeInfo in vertexList
  void copyElems(const Vertex& v);  // copy elements from parameter
};

// ----< promotion constructor: Vertex(int, VertexType) >----------------
template <typename VertexType, typename EdgeType>
Vertex<VertexType, EdgeType>::Vertex(int i, VertexType v)
  :id(i),value(v),num(0),lowlink(0),inStack(false)
{}

// ----< promotion constructor: Vertex(int, VertexType, VertexList) >----
template <typename VertexType, typename EdgeType>
Vertex<VertexType, EdgeType>::Vertex(int i, VertexType v, VertexList &list):
id(i),value(v),vertexList(list),num(0),lowlink(0),inStack(false)
{}

// ----< copy constructor >----------------------------------------------
template <typename VertexType, typename EdgeType>
Vertex<VertexType, EdgeType>::Vertex(const Vertex<VertexType,EdgeType> &v):
id(v.id),value(v.value),num(v.num),lowlink(v.lowlink),inStack(v.inStack)
{
  // copy elements from vertex v's vertexList
  copyElems(v);
}

// ----< assignment function >------------------------------------------ 
template <typename VertexType, typename EdgeType>
const Vertex<VertexType, EdgeType>& Vertex<VertexType, EdgeType>::operator=(const Vertex<VertexType,EdgeType>& v)
{
  if(this!=&v)
  {
    destroy();  // delete all the element in vertexList
    id=v.id;
    value=v.value;
    num=v.num;
    lowlink=v.lowlink;
    inStack=v.inStack;
    if(!v.isSingleVertex())
      copyElems(v); // copy elements from vertex v's vertexList
  }
  return *this;
}

// ----< get vertex id >------------------------------------------------
template <typename VertexType, typename EdgeType>
int Vertex<VertexType, EdgeType>::getID() const
{
  return id;
}

// ----< get vertex value >---------------------------------------------
template <typename VertexType, typename EdgeType>
VertexType Vertex<VertexType, EdgeType>::getValue() const
{
  return value;
}

// ----< get vertex num >-----------------------------------------
template <typename VertexType, typename EdgeType>
int Vertex<VertexType, EdgeType>::getNum() const
{
  return num;
}

// ----< get vertex lowlink >-------------------------------------
template <typename VertexType, typename EdgeType>
int Vertex<VertexType, EdgeType>::getLowlink() const
{
  return lowlink;
}

// ----< set vertex ID >------------------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::setID(int i)
{
  id=i;
}

// ----< set vertex Value >----------------------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::setValue(VertexType v)
{
  value=v;
}

// ----< set vertex num >------------------------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::setNum(int n)
{
  num=n;
}

// ----< set vertex lowlink >-------------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::setLowlink(int l)
{
  lowlink=l;
}

// ----< return true if vertex has no edges >-----------------------
template <typename VertexType, typename EdgeType>
bool Vertex<VertexType, EdgeType>::isSingleVertex()const
{
  return vertexList.empty();
}

// ----< add edgeinfo pair to Vertex >------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::addEdge(std::pair<int, EdgeType>& edgeInfo)
{
  vertexList.push_back(edgeInfo);
}

// ----< add edgeinfo to Vertex by (int, EdgeType) >-------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::addEdge(int childid,EdgeType edgeType)
{
  std::pair<int, EdgeType> edgeP(childid, edgeType);
  vertexList.push_back(edgeP);
}

// ----< get Child vertex id collection >----------------------------------
template <typename VertexType, typename EdgeType>
std::vector<int> Vertex<VertexType,EdgeType>::getChildIdCollection()
{
  std::vector<int> vec; // vec is temp to store return result
  for(size_t i=0;i<vertexList.size();++i)
  {
    vec.push_back(vertexList[i].first);
  }
  return vec;
}

// ----< display vertex information >-------------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::display() const
{
  std::cout<<"Vertex "<<id<<" value: "<<getValue()<<std::endl;
  // if the vertex has no edge display information
  if(isSingleVertex())
    std::cout<<"   Vertex "<<id<<" has no edges to other vertice."<<std::endl;
  // else display information about other connected vertice
  else
    for(size_t i=0; i<vertexList.size();++i)
    {
      std::cout<<"   From vertex "<<id<<" to "<<vertexList[i].first
        <<",    edge value: "<<vertexList[i].second<<std::endl;
    }
}

// ----< display vertex information without vertex and edge value information >----
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::displayWithoutType() const
{
  std::cout<<"Vertex id: "<<id<<std::endl;
  // if the vertex has no edge display information
  if(isSingleVertex())
    std::cout<<"   Vertex "<<id<<" has no edges to other vertice."<<std::endl;
  // else display information about other connected vertice
  else
    for(size_t i=0; i<vertexList.size();++i)
    {
      std::cout<<"   From vertex "<<id<<" to "<<vertexList[i].first<<std::endl;
    }
}

// ----< destructor >-------------------------------------
template <typename VertexType, typename EdgeType>
Vertex<VertexType, EdgeType>::~Vertex()
{ 
  // delete all the element in vertexList
  destroy();
}


// private functions:
// ----< delete all the element in vertexList >--------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::destroy()
{
  while(!isSingleVertex())
    vertexList.pop_back();
}

// ----< copy elements from parameter >------------------------------
template <typename VertexType, typename EdgeType>
void Vertex<VertexType, EdgeType>::copyElems(const Vertex<VertexType,EdgeType>& v)
{ 
  for(size_t i=0;i<v.vertexList.size();++i)
  { // push back elements into vertexList
    vertexList.push_back(v.vertexList[i]);
  }
}

#endif