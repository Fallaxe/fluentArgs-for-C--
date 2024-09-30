# What is it?
This is a simple library written in cpp to handle arguments with a better design pattern.
# Namespace and classes
All the classes are declared in the header inside **FluentArgs namespace**.
- Flag
- FlagBuilder
- Argparser
- ArgParserBuilder
# How to use
include "ArgParser.hpp" in your project.
`#include "ArgParser.hpp"`
In main.cpp there are some examples of usage.
Usually you need to do the following steps:

1. create a Flag(or some flags) using those methods:
	`//myFun is a void function`
	`Flag flag1 = fluentArgs::createFlag()`
	`.withName("-a")`
	`.withAlias("--arg")`
	`.withDescription("first exemple of a flag in a program")`
	`.withOperation(myFun)`
	`.build();`
	
	`//you can also use`
	`Flag flag1 = Flag::create()`
	`.withName("-a")`
	`.withAlias("--arg")`
	`.withDescription("first exemple of a flag in a program")`
	`.withOperation(myFun)`
	`.build();`
2. create an argParser using those methods:
	`ArgParser argparser = FluentArgs::createArgParser()`
	`.withFlag(flag1,otherFlag)`
	`.withArgs(argc,argv)`
	`.build();`
	
	`//you can also use:`
	`ArgParser argparser = ArgParser::create()
	.`withFlag(flag1,otherFlag)`
	`.withArgs(argc,argv)`
	`.build();`
	
	`//then you can check all the arguments with`
	`argparser.checkArguments();`
	
	`//and you can also resume all the flags you assigned with`
	`cout<<argparser.resume()<<endl;`
# To do
- [ ] For now Flag supports only void function as operation to invoke,  i want to make this more abstact.
- [ ] explain in this MD file all the classes
- [ ] better implementation of terminateOnFailure