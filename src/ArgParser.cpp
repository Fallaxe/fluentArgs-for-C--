#include "ArgParser.hpp"

using namespace fluentArgs;
using namespace std;

// Flag implementation
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

bool fluentArgs::Flag::isDelim()
{
    return this->delim_;
}

string fluentArgs::Flag::getDescription()
{
    return this->description_;
}

FlagBuilder fluentArgs::Flag::create()
{
    return FlagBuilder();
}

string fluentArgs::Flag::resume()
{
    string str;
    str.append(this->name_+"\n\talias: "+this->alias_+"\n\tdescription: "+this->description_);
    return str;
}

// void fluentArgs::Flag::executeOperation()
// {
//     this->operation_();
// }
void fluentArgs::Flag::executeOperation(std::vector<string> subParam)
{
    this->operation_(subParam);
}

// FlagBuilder implementation
FlagBuilder &fluentArgs::FlagBuilder::withName(const string name)
{
    name_ = name;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::withAlias(const string alias)
{
    alias_ = alias;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::withOperation(function<void(std::vector<string>)> operation)
{
    operation_ = operation;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::withNumberOfValues(int numValues)
{
    numValues_ = numValues;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::withDelim()
{
    delim_ = true;
    return *this;
}

FlagBuilder &fluentArgs::FlagBuilder::withDescription(const string description)
{
    description_ = description;
    return *this;
}

void fluentArgs::FlagBuilder::reset()
{
    this->alias_ = "";
    this->name_ = "";
    this->operation_ = std::function<void(std::vector<string>)>();
    this->delim_ = " ";
    this->description_ = " ";
    this->numValues_ = 0;
}

Flag fluentArgs::FlagBuilder::build()
{
    if(name_.empty())
        throw runtime_error("name cannot be empty");
        
    // if(alias_.empty())
    //     throw runtime_error("alias cannot be empty");
    
    //numValues can be empty 'cause sometimes you need options without a value;

    if(!operation_)
        throw runtime_error("operation cannot be unsetted");
    
    Flag flag = Flag(name_,alias_,numValues_,operation_,delim_,description_);

    reset();

    return flag;
}

//argParserBuilder implementation

ArgParserBuilder &fluentArgs::ArgParserBuilder::withFlag(Flag arg)
{
    this->flags_.push_back(arg);
    return *this;
}

ArgParserBuilder &fluentArgs::ArgParserBuilder::withArgs(int argc, char const *argv[])
{
    
    for(int i = 1; i< argc; i++){

        arguments_.emplace_back(Argument(std::string(argv[i])));
    }
    return *this;
}

ArgParserBuilder &fluentArgs::ArgParserBuilder::withArgs(vector<Argument> args)
{
    arguments_ = args;
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
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));  // Aggiungi l'ultimo token
    return tokens;
}

void fluentArgs::ArgParser::checkArguments()
{
    for(std::vector<Argument>::iterator itArg = this->arguments_.begin(); itArg != this->arguments_.end(); ++itArg){
        for(std::vector<Flag>::iterator itFlag = this->flags_.begin(); itFlag != this->flags_.end(); ++itFlag){
            if(compare(*itFlag,*itArg)){
                vector<string> subArg;

                if(std::distance(arguments_.begin(),itArg) + itFlag->getNumValues() > arguments_.size()-1)
                    return;// posso contenere gli ipotetici argomenti o vado in overflow?
                
                if(itFlag->getNumValues() > 0 || itFlag->isDelim()){ //ci sono argomenti?
                    // itArg++;
                    if(!itFlag->isDelim()){
                        for(int i = 0; i < itFlag->getNumValues(); i++){
                            if(i > arguments_.size() || itFlag->getNumValues() > arguments_.size()) return;
                    
                            itArg++;
                            subArg.emplace_back(itArg->getArg());
                        }
                    }
                    else{ //si e hanno un delimitatore per più comandi
                        itArg++;
                        subArg = splitString(itArg->getArg(),',');
                    }
                }
                
                itFlag->executeOperation(subArg);

            }
            // else if(this->terminateOnFailure_) showError(*itFlag); spostare dentro la verifica di compare
            
        }
    }
}

string fluentArgs::ArgParser::resume()
{
    string resumeStr;
    for(Flag flag : flags_){
        resumeStr.append(flag.resume()+"\n");
    }
    
    return resumeStr;
}

ArgParserBuilder fluentArgs::ArgParser::create()
{
    return ArgParserBuilder();
}

bool fluentArgs::ArgParser::compare(Flag flag, Argument arg)
{   
    bool check = (flag.getAlias() != " ");
    check = (check ? flag.getName() == arg.getArg() || flag.getAlias() == arg.getArg() : flag.getName() == arg.getArg());
    return (check);
}

void fluentArgs::ArgParser::showError(Flag flag)
{
    throw runtime_error(flag.getName()+" generate an error:\n remember:\n"+flag.resume());
    // cout<<flag.getName()+"generate an error:\n remember:\n"+flag.resume()<<endl;
}

string fluentArgs::Argument::getArg()
{
    return this->arg_;
}
