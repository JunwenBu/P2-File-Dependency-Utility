#include "Vertex.h"
///////////////////////////////////////////////////////////////////////
// Vertex.cpp - Vertex class support bassic operation on vertex      //
// ver 1.0 3/6/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
///////////////////////////////////////////////////////////////////////
/*
 * This cpp file includes "Vertex.h" to do test stub.
 *
 * Module Information:
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

// ----- < test stub > ----------------------------------------------
#ifdef TEST_VERTEX
int main()
{
  // test Constructor
  std::cout<<"Test void constructor:"<<std::endl;
  Vertex<int, std::string> v(0,0);
  v.display();
  v.setValue(99);
  v.setID(1);
  v.addEdge(std::pair<int,std::string>(2,"e1"));
  v.addEdge(std::pair<int,std::string>(4,"e3"));
  v.display();
  // test copy Constructor
  std::cout<<"Test copy Constructor:"<<std::endl;
  Vertex<int, std::string> copyV=v;
  copyV.setID(3);
  copyV.setValue(77);
  copyV.display(); std::cout<<"Set id to : "<<copyV.getID()<<std::endl;
  copyV.display();
  std::cout<<"Test assignment function:"<<std::endl;
  // create a VertexList
  std::vector<std::pair<int, std::string>> list;
  list.push_back(std::pair<int,std::string>(7,"e7"));
  list.push_back(std::pair<int,std::string>(8,"e8"));
  list.push_back(std::pair<int,std::string>(9,"e9"));
  // test promotion constructor
  Vertex<int, std::string> vSource(11,4,list);
  v=vSource;
  v.display();
  v.setValue(55); // test setValue and getValue
  std::cout<<"set Vertex Value then get this Value: vertex value "<<v.getValue()<<std::endl;
  return 0;
}
#endif
// end test stub