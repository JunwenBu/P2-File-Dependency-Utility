/////////////////////////////////////////////////////////////////////////
// FileDependencyBuilder.cpp - generate XML represents file dependency //
// ver 1.0 3/10/2012                                                   //
// Language:    Visual C++, Visual Studio 2010, SP1                    //
// Application: CSE 687 Project2                                       //
// Author:      Junwen Bu                                              //
/////////////////////////////////////////////////////////////////////////
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

#include "FileDependencyBuilder.h"

// ----< scan file path and generate an XML file describing dependency 
// relationships of .cpp and .h files under this directory >---------
void FileDependencyBuilder::scanFiles(const std::string& path, const std::string& filename)
{
  FileMgr fm=FileMgr(); // create FileMgr object to support file operations
  std::map<std::string,int> fileMap;
  std::map<std::string, std::set<std::string>> includeMap;
  std::set<std::string> tempSet;
  FileMgr::fileSet hFileList=fm.findFiles(path,"*.h"); // find .h files
  FileMgr::fileSet cppFileList=fm.findFiles(path,"*.cpp"); //find .cpp files
  for(size_t i=0; i<hFileList.size(); ++i)    // store .h files into tempSet
    tempSet.insert(removeDotH(hFileList[i])); 
  for(size_t j=0; j<cppFileList.size(); ++j)  // store .cpp files into tempSet
    tempSet.insert(removeDotCPP(cppFileList[j])); // if name same then save only one name
  int id=1;  // create int id used as vertex id
  for(std::set<std::string>::iterator iter=tempSet.begin(); iter!=tempSet.end(); ++iter, ++id)
    fileMap.insert(std::pair<std::string,int>(*iter,id));
  for(std::map<std::string,int>::iterator iter=fileMap.begin(); iter!=fileMap.end(); ++iter){ 
    //store included files into includeMap
    includeMap[iter->first]=getIncludingFileSet(iter->first,fileMap,fm);
  }
  generateXML(fileMap,includeMap,filename);
}

/* ----< private functions: >-------------------------------------*/ 
// ----< facility to generate XML file by using XMLWriter >---------
void FileDependencyBuilder::generateXML(std::map<std::string,int> &fileMap,std::map<std::string,std::set<std::string>> includeMap, const std::string& filename)
{
  XmlWriter wrt, name; // create XmlWriter object
  wrt.addDeclaration();
  wrt.start("graph");  // start item <graph>
  // add comment into xml content
  wrt.addComment("Each vertex and edge have an id used as the target for same searches.");
  wrt.addComment("The vertex bodies are composed of edge elements.");
  wrt.addComment("Reprecent files as a graph for CSE687 - OOD, Project #2");
  name.start("name");  // start item <name>
  name.addBody("FileDependency");
  name.end();
  wrt.addBody(name.xml());
  int edgeid=0;   // create int edge id used as id of edges
  for(std::map<std::string,int>::iterator iter=fileMap.begin();iter!=fileMap.end();++iter){
    XmlWriter vertex;   // create <vertex> sub xml
    vertex.start("vertex"); // start <vertex>
    vertex.addAttribute("id", convertInt(iter->second)); // add vertex edge attribute
    vertex.addAttribute("value",iter->first); // add vertex value attribute
    std::map<std::string,std::set<std::string>>::iterator incMapIter=includeMap.find(iter->first);
    std::set<std::string> tempSet=incMapIter->second; 
    for(std::set<std::string>::iterator it3=tempSet.begin(); it3!=tempSet.end(); ++it3){
      XmlWriter edge;   // create <edge> sub xml
      edge.start("edge"); // start <edge>
      edge.addAttribute("id",convertInt(++edgeid)); // add edge id attribute
      edge.addAttribute("value","include:"+*it3); // add edge value attribute
      if(fileMap.find(*it3)!=fileMap.end())
        edge.addBody( convertInt(fileMap.find(*it3)->second) ); // add child vertex id to <edge> body
      else{ // if miis files then display info to reminder user.
        std::cout<<"Xml Generate process FAIL!\n"<<"Miss file : "<<*it3<<"!! Please put <"<<*it3<<"> into test path."<<std::endl;
        return;
      }
      edge.end();
      vertex.addBody(edge.xml()); // add <edge> to <vertex> body
    }
    vertex.end();
    wrt.addBody(vertex.xml());
  }
  wrt.end(); // end xml
  std::ofstream out(filename); // create xml file in the file path
  out.clear();     
  if(out.good()){   // write contents to "FileDependency.xml" file
    out<<wrt.xml().c_str();
    out.close();
  }
}

// ----< return true if the textline contains "#include", else return false >----
bool FileDependencyBuilder::isInclude(const std::string& textline)
{
  size_t locpos=textline.find("#include");
  // if find "#include"
  if(locpos!=textline.npos) 
  { 
    if(locpos==0)      // check if '#include'
      return true;     // is in notification
    else
      for(size_t i=0; i<locpos; ++i) 
      {
        if(textline[i]!=' ')
          return false;
      }
    return true;           
  }
  else              // if "#include" not be found
    return false;
}

// ----< get name of the file included >------------------------------------
// eg. if #include "name.h", get "name" and store it to a string
std::string FileDependencyBuilder::getHeadName(const std::string& textline)
{
  if(isInclude(textline))
  {
    size_t startpos=textline.find("\""); // find "
    if(startpos==textline.npos)
      return "";
    else{
      size_t endpos=textline.find(".h\""); // find .h"
      // return name of the headfile
      return textline.substr(startpos+1,endpos-startpos-1);
    }
  }
  // if not found return empty string
  else return "";
}

// ----< store included files into a std::set<std::string> >--------------
std::set<std::string> FileDependencyBuilder::getIncludingFileSet(std::string packname, std::map<std::string,int> fileMap, FileMgr &fm)
{
  std::set<std::string> fileSet; // used to store return resulet
  // scan the contents of .cpp file
  std::string packnameCpp=packname+".cpp"; 
  // store contents of file line by line
  FileMgr::fileSet temp=fm.storeFile(packnameCpp); 
  for(size_t i=0;i<temp.size(); ++i)
  { 
    std::string head=getHeadName(temp[i]);
    std::string headcpp=head+".cpp";
    // a "sameName.cpp" will include "sameName.h", 
    // consider such case, eliminate this "sameName.h"
    if(head.compare("")!=0 && headcpp.compare(packnameCpp)!=0)
      fileSet.insert(head);
  }
  // scan the contents of .h file
  std::string packnameH=packname+".h";
  temp.clear();
  temp=fm.storeFile(packnameH);
  for(size_t j=0; j<temp.size(); ++j)
  { // insert head info into fileSet
    std::string head=getHeadName(temp[j]);
    if(head.compare("")!=0)
      fileSet.insert(head);
  }
  return fileSet;
}

// ----< remove ".h" at the end of the headname string >----------
std::string FileDependencyBuilder::removeDotH(std::string headname)
{
  headname.erase(headname.length()-2);
  return headname;
}

// ----< remover ".cpp" at the end of the headname string >---------
std::string FileDependencyBuilder::removeDotCPP(std::string cppname)
{
  cppname.erase(cppname.length()-4);
  return cppname;
}

// ----< convert int type to string type >--------------
std::string FileDependencyBuilder::convertInt(int n)
{
  if (n == 0) // if ZERO return string vertion
      return "0";
  std::string temp="";
  std::string returnStr="";
  while (0<n)
  { // use modulum operation to convert single digit
    temp+=n%10+48;
    n/=10;
  } 
  // store int digit into a string
  for (size_t i=0;i<temp.length();i++)
    returnStr+=temp[temp.length()-i-1];
  return returnStr;
}


// ---- < test stub >------------------------------------
//NOTE:///////////////////////////////////////////////////
// This test stub only test scanFiles this function, for 
// this function use all the otehr facilities in this module
// and it is the only public interface in this module.
//////////////////////////////////////////////////////////
#ifdef TEST_FILEDEPENDENCYBUILDER
int main(int argc, char* argv[])
{
  std::string path="../Test";
  std::string filename="FileDependency.xml";
  int count=2;
  if(count<=argc)
  {
    path=argv[1];
    if(argc>=3)
      filename=argv[2];
  }
  FileDependencyBuilder fdBuilder;
  // test scanFiles function
  fdBuilder.scanFiles(path,filename);
}
#endif
// end < test stub >