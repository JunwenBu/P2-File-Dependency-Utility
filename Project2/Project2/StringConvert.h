#ifndef STRINGCONVERT_H
#define STRINGCONVERT_H
///////////////////////////////////////////////////////////////////////
// XmlStringConvert.h - convert from std::string in xml to type T    //
// ver 1.0 3/9/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
// Reference:   Jim Fawcett, Proj2HelpS12                            //
///////////////////////////////////////////////////////////////////////
/*
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
#include <sstream>    // use istringstream
#include <string>     
#include <exception>
#include <iostream>

//----< convert from std::string to type T >----------------
template <typename T>
T convert(const std::string& ToConvert)
{
  // add space to keep stream from reaching end on first read
  std::string test = ToConvert + " ";
  // in memory stream that holds a string
  std::istringstream from(test);
  // now, convert ToConvert string to T type if possible
  T t;
  from >> t;
  if(from.good())
    return t;
  else{
    std::cout<<std::endl<<"WARNING:";
    std::string msg = std::string("can't convert ") + ToConvert 
      + std::string(" to ") + typeid(t).name();
    throw std::runtime_error(msg);}
}

// ----< convert std::string to VertexType >--------------------
template <typename VertexType>
void convertStringToVertexType(const std::string& ToConvert, VertexType &v)
{
  try{ 
    // do convertion: std::string->VertexType
    v = convert<VertexType>(ToConvert);
  }
  catch(std::runtime_error& e){
    std::cout << "\n  " << e.what()<<std::endl;
    std::cout<<"Please check the contents of your XML file!"<<std::endl<<std::endl;
    exit(1);
  }
}

// ----< convert std::string to EdgeType >----------------------
template <typename EdgeType>
void convertStringToEdgeType(const std::string& ToConvert, EdgeType &edge)
{
  try{
    // do convertion: std::string->EdgeType
    edge = convert<EdgeType>(ToConvert);
  }
  catch(std::runtime_error& e){
    std::cout << "\n" << e.what()<<std::endl; 
    std::cout<< "Please check the contents of you XML file!"<<std::endl<<std::endl;
    exit(1);
  }
}

#endif