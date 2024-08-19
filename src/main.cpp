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

int main(int argc, char const *argv[])
{
    /*
        -numValues_ cambia nome(?) e deve rappresentare un intero di quanti parametri ha il flag dopo
            es: -a 1 2 3
            (aggiungere delimitatore al flag per i parametri? oppure si usa sempre ' ')
        -rendere static i builder
        -aggiustare nel caso ci siano argomenti nel mezzo che sporcano "./main -hw e poop -a" esegue uguale ma non deve se failetc dice di no
        -modificare operation in una funzione void con parametri a vettore/... string void operation(string.../vectorstring)
    */
    FlagBuilder flagbuilder;
    Flag flag1 = flagbuilder
                    .setName("-a")
                    .setAlias("--arg")
                    .withDescription("first exemple of a flag in a program")
                    .setOperation(myFun)
                    .build();
    
    Flag flag2 = flagbuilder
                    .setName("-hw")
                    .setAlias("--helloworld")
                    .setNumValues(2)
                    .setOperation(helloWorld)
                    .build();
    Flag flagWithoutAlias = flagbuilder
                                .setName("e")
                                .setOperation(exempleFun)
                                .build();

    // cout<<flagWithoutAlias.getAlias()<<endl;
    //i valori non sono per ora passati all operation, il che rende un po inutile la cosa TODO!
    
    ArgParserBuilder argparserbuilder;
    ArgParser argparser = argparserbuilder
                            .addFlag(flag1)
                            .addFlag(flag2)
                            .addArgs(argc,argv)
                            .addFlag(flagWithoutAlias)
                            .build();

    cout<<argparser.resume()<<endl;
    argparser.checkArguments();

    return 0;
}
