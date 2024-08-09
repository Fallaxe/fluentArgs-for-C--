#include "ArgParser.hpp"
using namespace fluentArgs;
using namespace std;

//Flag implementation
string fluentArgs::Flag::getName()
{
    return this->name_;
}

string fluentArgs::Flag::getAlias()
{
    return this->alias_;
}

int fluentArgs::Flag::getNumValues()
{
    return this->numValues_;
}

string fluentArgs::Flag::getDelim()
{
    return this->delim_;
}

void fluentArgs::Flag::executeOperation()
{
    this->operation_();
}

//FlagBuilder implementation
FlagBuilder &fluentArgs::FlagBuilder::setName(const string dash)
{
    name_ = dash;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::setAlias(const string doubleDash)
{
    alias_ = doubleDash;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::setOperation(function<void()> operation)
{
    operation_ = operation;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::setNumValues(const int value)
{
    numValues_ = value;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::withDelim(const string delim)
{
    delim_ = delim;
    return *this;
}

Flag fluentArgs::FlagBuilder::build()
{
    if(name_.empty())
        throw runtime_error("name cannot be empty");
        
    // if(alias_.empty())
    //     throw runtime_error("alias cannot be empty");
    
    //value can be empty 'cause sometimes you need options without a value;

    if(!operation_)
        throw runtime_error("operation cannot be unsetted");

    return Flag(name_,alias_,numValues_,operation_,delim_);
}

//argParserBuilder implementation

ArgParserBuilder &fluentArgs::ArgParserBuilder::addFlag(Flag arg)
{
    this->flags_.push_back(arg);
    return *this;
}

ArgParserBuilder &fluentArgs::ArgParserBuilder::addArgs(int argc, char const *argv[])
{
    
    for(int i = 1; i< argc; i++){

        arguments_.emplace_back(Argument(std::string(argv[i])));
    }
    return *this;
}

ArgParserBuilder& fluentArgs::ArgParserBuilder::withoutTerminateOnFailure()
{
    this->terminateOnFailure_ = false;

    return *this;
}

ArgParser fluentArgs::ArgParserBuilder::build()
{
    if(arguments_.size() == 0)
        throw runtime_error("arguments cannot be empty, pass them into the builder!");
    
    if(flags_.size() == 0)
        throw runtime_error("you need at least one flag to check, pass them into the builder!");
    
    return ArgParser(flags_,arguments_,terminateOnFailure_);
}


//argParser implementation

void fluentArgs::ArgParser::checkArguments()
{
    for(std::vector<Flag>::iterator it = this->flags_.begin(); it != this->flags_.end(); ++it){
        if(compare(*it)) it->executeOperation(); //execute the operation!
        else if(terminateOnFailure_) return;

        it += it->getNumValues(); // mando avanti di numvalues posizioni l'iteratore
    }
}


bool fluentArgs::ArgParser::compare(Flag flag)
{
    for(std::vector<Argument>::iterator it = this->arguments_.begin(); it != this->arguments_.end(); ++it){
        if((flag.getName() == it->getArg()) || (flag.getAlias() == it->getArg())){
            int pos = 0;
            for(int i = 0; i< flag.getNumValues(); i++){
                if(flag.getDelim() == " "){
                    if(i > arguments_.size() || flag.getNumValues() > arguments_.size()) return false;
                    it++;
                }
                else{
                    std::string token = it->getArg().substr(pos, it->getArg().find(flag.getDelim()));
                    pos = token.length()+flag.getDelim().length();
                    if(pos > it->getArg().length()) // se sono passati meno parametri...?
                        return false;
                }
            }
            return true;
        }
    }

    return false;
}

string fluentArgs::Argument::getArg()
{
    return this->arg_;
}
