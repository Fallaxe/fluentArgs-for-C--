#include "ArgParser.hpp"
#include <iostream>

using namespace std;
using namespace fluentArgs;

void myFun(){
    cout<<"ciao"<<endl;
}

int main(int argc, char const *argv[])
{
    /*
        -numValues_ cambia nome(?) e deve rappresentare un intero di quanti parametri ha il flag dopo
            es: -a 1 2 3
            (aggiungere delimitatore al flag per i parametri? oppure si usa sempre ' ')
        -rendere static i builder
    */
    FlagBuilder flagbuilder;
    Flag flag1 = flagbuilder
                    .setName("-a")
                    .setAlias("--arg")
                    .setOperation(myFun)
                    .build();
    
    // Flag flag2 = flagbuilder
    //                 .setName("-hw")
    //                 .setAlias("--helloworld")
    //                 .setNumValues()///!!!!
    // mettere value come booleano/intero(numero prametri per opzione) per capire se riceve un parametro e giocare con l'iteratore per andare avanti;
    
    ArgParserBuilder argparserbuilder;
    ArgParser argparser = argparserbuilder
                            .addFlag(flag1)
                            .addArgs(argc,argv)
                            .build();

    argparser.checkArguments();

    return 0;
}
