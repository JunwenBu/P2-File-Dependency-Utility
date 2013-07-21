#ifndef FILEDEPENDENCYBUILDER_H
#define FILEDEPENDENCYBUILDER_H
///////////////////////////////////////////////////////////////////////
// FileDependencyBuilder.h-generate an XML represent file dependency //
// ver 1.0 3/10/2012                                                 //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
///////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==========================================
 * This module generate an XML file describing dependency relationships 
 * between all the files residing in a directory on a specified path.
 *
 * Required Files:
 * ===============================================
 * FileMgr.h FileMgr.cpp XmlWriter.h XmlWriter.cpp
 *
 * Public Interface:
 * ==========================================
 * FileDependencyBuilder fdBuiler;
 * std::string filepath, filename;
 * fdBuiler.scanFiles(filepath, filename);
 *
 * Private Function Catalog:
 * ==========================================
 * define types:
 * ------------------------------------------
 * std::map<std::string,int> fileMap;
 * std::map<std::string,std::set<std::string>> includeMap;
 * std::string textline, packname, headname, cppname, filename;
 * FileMgr fm; int number;
 * ------------------------------------------
 * void generateXML(fileMap), includeMap, filename);
 * bool isInclude(textline);
 * std::string getHeadName(textline);
 * std::set<std::string> getIncludingFileSet(packname,fileMap,fm);
 * std::string removeDotH(headname);
 * std::string removeDotCPP(cppname);
 * std::string convertInt(number);
 */
#include <set>
#include <map>
#include "FileMgr.h"
#include "XmlWriter.h"

// ----< class FileDependencyBuilder >--------------------
class FileDependencyBuilder
{
  // type defefine: includeSet
  typedef std::vector<std::pair<std::string,std::vector<std::string>>> includeSet;

public:
  // scan file path and generate an XML file describing dependency 
  // relationships of .cpp and .h files under this directory
  void scanFiles(const std::string& filepath, const std::string& filename="FileDependency.xml");

  /* private functions:*/
private:
  // facility to generate XML file by using XMLWriter
  void generateXML(std::map<std::string,int> &fileMap, std::map<std::string,std::set<std::string>> includeMap, const std::string& filename);
  // return true if the textline contains "#include", else return false
  bool isInclude(const std::string& textline);
  // get name of the file included
  std::string getHeadName(const std::string& textline);
  // store included files into a std::set<std::string>
  std::set<std::string> getIncludingFileSet(std::string packname, std::map<std::string,int> fileMap, FileMgr &fm);
  // remove ".h" at the end of the headname string
  std::string removeDotH(std::string headname);
  //remover ".cpp" at the end of the headname string
  std::string removeDotCPP(std::string cppname);
  // convert int type to string type
  std::string convertInt(int number);
};

#endif