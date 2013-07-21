#ifndef FILEMGR_H
#define FILEMGR_H
///////////////////////////////////////////////////////////////////////
// FileMgr.h - Support file and directory operations                 //
// ver 1.1 3/9/2012                                                  //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// Application: CSE 687 Project2                                     //
// Author:      Junwen Bu                                            //
///////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ===================
 * This module provideds class FileMgr used to do File System Management
 * Use some facilities of FileSystem Package to do file management 
 * operations: get or put lines of file; store contents of file into 
 * vector<string>; copy file from a directory to another directory; rename
 * files.
 *
 * Required Files:
 * ===============
 * FileSystem.h, FileSystem.cpp
 *
 * Public Interface:
 * ==========================
 * FileMgr f;               
 * FileMgr f2(argc, argv);  
 * FileMgr fmgr;
 * std::vector<std::string> strVec = fmgr.findFiles(path, pattern);
 * std::string textline = fmgr.getLine(filename);
 * fmgr.putLine(filename, textline);
 * std::vector<std::string> textlineVec = fmgr.storeFile(filename);
 * copyAndRenameFile(sourcefile, destfile)
 *
 * Private Function Catalog:
 * ==========================
 * NULL
 */

#include <vector>
#include <string>
#include <iostream>
#include "FileSystem.h"

class FileMgr
{
public:
  // typedef fileSet
  typedef std::vector<std::string> fileSet;

  FileMgr::FileMgr();               // void constructor 
  FileMgr(int argc, char* argv[]);  // promotion constructor (int, char*)
  ~ FileMgr(){delete []_argv;}      // destructor

  // get files which file pattern is "pattern" under directory "path",
  // store names of this files into a fileSet(Vector<string>)
  fileSet findFiles(const std::string& path, const std::string& pattern);
  
  // get a line of the content of file
  std::string getLine(const std::string& filename); 

  // write a line of content stored in 'textline' into file named 'filename'
  void putLine(const std::string& filename,const std::string& textline);

  // store content of a file line by line into a vector<sting> and
  // return a vector<string> object
  fileSet storeFile(const std::string& filename);

  // copy file form directory fsource, copy it to fdest directory and rename it
  void static copyAndRenameFile(const std::string& sourcename, const std::string& destname);

private:
  // designed to meet main function parametres
  int _argc;
  char** _argv;
};
#endif
