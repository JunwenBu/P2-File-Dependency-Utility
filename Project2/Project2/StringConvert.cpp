#include "StringConvert.h"
///////////////////////////////////////////////////////////////////////
// XmlStringConvert.cpp - convert from std::string in xml to type T  //
// ver 1.0 3/9/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
// Reference:   Jim Fawcett, Proj2HelpS12                            //
///////////////////////////////////////////////////////////////////////
/*
 * This .cpp file are used to do simple test on XmlStringConvert module.
 *
 * Module Operations:
 * ===================
 * This module provideds tool functions to convert from std::string in
 * xml file to specific Type, which will be used as VertexType or EdgeType
 * in class Graph. Throws exception if no conversion is possible.
 *
 * Function/Interface Catalog:
 * ==========================
 * std::string ToConvert;
 * VertexType vType;
 * EdgeType eType
 * T convert(ToConvert);
 * void convertStringToVertexType(ToConvert, vType);
 * void convertStringToEdgeType(ToConvert, eType);
 */

// ----< test stub >---------------------------------------------
#ifdef TEST_STRINGCONVERT
int main()
{
  std::cout << "\n  Demonstrating converstions needed for GraphReader<V,E>";
  std::cout << "\n ========================================================";
  // display Test information
  std::cout << "\n\n  calling PartOfGraphReader<std::string,double>(\"3.5\")";
  // test convertStringToVertexType function:
  std::string v;convertStringToVertexType<std::string>("3.5",v);
  std::cout << "\n\n  calling PartOfGraphReader<int,double>(\"3.5\")";
  std::cout << "\n\n  calling PartOfGraphReader<int,double>(\"a string\")";
  std::string i;
  // test convertStringToEdgeType function:
  convertStringToEdgeType<std::string>("a string",i);
  std::cout <<std::endl;
}
#endif
// end test stub