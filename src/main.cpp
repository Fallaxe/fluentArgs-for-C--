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
        -aggiustare nel caso ci siano argomenti nel mezzo che sporcano "./main -hw e poop -a" esegue uguale ma non deve se failetc dice di no
        -DONE: modificare operation in una funzione void con parametri a vettore/... string void operation(string.../vectorstring)
        -DONE: cambiare tutti i metodi di Flagbuilder in withSMTG() etc per avere un interfaccia più fluida
        -toglire reset?
        -togliere delim_? <---si facciamolo!
        -DONE: withArgs() che prende un vector string(?) ha senso per i sottoparametri che fanno altri flag a sua volta
        -rendere tipizzata la classe flag per aggiungere functional tipizzate meglio
    */

   
    Flag flag1 = Flag::create()
                    .withName("-a")
                    .withAlias("--arg")
                    .withDescription("first exemple of a flag in a program")
                    .withOperation(myFun)
                    .build();

    Flag flag2 = Flag::create()
                    .withName("-hw")
                    .withAlias("--helloworld")
                    .withNumberOfValues(2)
                    .withOperation(helloWorld)
                    .build();

    Flag flagWithoutAlias = Flag::create()
                                .withName("e")
                                .withOperation(exempleFun)
                                .build();

    // cout<<flagWithoutAlias.getAlias()<<endl;
    //i valori non sono per ora passati all operation, il che rende un po inutile la cosa TODO!
    
    ArgParser argparser = ArgParser::create()
                            .withFlag(flag1)
                            .withFlag(flag2,flagWithoutAlias)
                            .withArgs(argc,argv)
                            .build();

    cout<<argparser.resume()<<endl;
    argparser.checkArguments();

    return 0;
}
