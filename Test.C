void Test(){
 
    std::cout << "Hello from Test.C" << std::endl;

    gROOT->ProcessLine("KUAnalysis->Process(\"Ana.C+\")");

}
