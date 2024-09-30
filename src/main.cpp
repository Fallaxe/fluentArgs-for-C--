#include "ArgParser.hpp"
#include <iostream>

using namespace std;
using namespace fluentArgs;

void myFun(std::vector<string> vec){
    cout<<"ciao"<<endl;
}
void exempleFun(std::vector<string> vec){
    cout<<"exemple"<<endl;
}

void helloWorld(std::vector<string> vec){
    cout<<"Hello world!"+vec.at(0)<<endl;
}

void fantasticFun(std::vector<string> vec){
    cout<<"fantastic: "+vec.at(1)<<endl;
}

int main(int argc, char const *argv[])
{

   
    Flag flag1 = Flag::create()
                    .withName("-a")
                    .withAlias("--arg")
                    .withDescription("first exemple of a flag in a program")
                    .withOperation(myFun)
                    .build();

    Flag flag2 = fluentArgs::createFlag()
                    .withName("-hw")
                    .withAlias("--helloworld")
                    .withNumberOfValues(2)
                    .withOperation(helloWorld)
                    .build();

    Flag flagWithoutAlias = Flag::create()
                                .withName("e")
                                .withOperation(exempleFun)
                                .build();
    Flag flagWithMultipleArgs = Flag::create()
                                    .withName("-f")
                                    .withAlias("--fantastic")
                                    .withDelim()
                                    .withOperation(fantasticFun)
                                    .build();

    // cout<<flagWithoutAlias.getAlias()<<endl;
    //i valori non sono per ora passati all operation, il che rende un po inutile la cosa TODO!
    
    ArgParser argparser = ArgParser::create()
                            .withFlag(flag1)
                            .withFlag(flag2,flagWithoutAlias,flagWithMultipleArgs)
                            .withArgs(argc,argv)
                            .build();

    cout<<argparser.resume()<<endl;
    argparser.checkArguments();

    return 0;
}
