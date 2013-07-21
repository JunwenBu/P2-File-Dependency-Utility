:testComplexGraph.bat
  cl /DTEST_COMPLEX_GRAPH /EHsc Exec.cpp XmlWriter.cpp StringConvert.cpp XmlReader.cpp Vertex.cpp Graph.cpp FileSystem.cpp FileMgr.cpp FileDependencyBuilder.cpp /link setargv.obj
..\Project2\Exec.exe ../Test ComplexGraphRep.xml