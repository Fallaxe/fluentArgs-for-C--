#include <string>
#include <functional>
#include <stdexcept> // For std::runtime_error

using namespace std;

namespace fluentArgs{
    
    class Argument{
        friend class ArgParserBuilder;
        private:
            string arg_;
            Argument(string arg) : arg_(arg){};
        public:
            string getArg();
    };
    
    class Flag{
        
        friend class FlagBuilder;
        friend class ArgParser;

        private:
            string name_;
            string alias_;
            int numValues_;
            function<void(std::vector<string>)> operation_;

            string delim_;

            Flag(string name, string alias, int numValues, function<void(std::vector<string>)> operation, string delim)
            :name_(name),alias_(alias),operation_(operation),numValues_(numValues),delim_(delim){}; //init

        public:
            string getName();
            string getAlias();
            int getNumValues();
            string getDelim();

        protected:
            void setName(string name);
            void setDouble_dash(string name);
            void setValue(string numValues);
            void setOperation(function <void(std::vector<string>)> operation);
            void executeOperation(std::vector<string> subParam);

        };
        
        
    class FlagBuilder{
        
        private:
            string name_;
            string alias_ = "";
            int numValues_ = 0;
            function<void(std::vector<string>)> operation_;
            void reset();


            string delim_ = " ";

        public:
            FlagBuilder& setName(const string name);
            FlagBuilder& setAlias(const string alias);
            FlagBuilder& setOperation(function<void(std::vector<string>)> operation);
            FlagBuilder& setNumValues(int numValues);
            FlagBuilder& withDelim(const string delim);

            Flag build();
    };
    

    class ArgParser{
        friend class ArgParserBuilder;

    public:
        void checkArguments();
        //bool checkOnly(//flags);

    private:
        ArgParser(vector<Flag> flags,vector<Argument> arguments, bool terminateOnFailure) : flags_(flags),arguments_(arguments),terminateOnFailure_(terminateOnFailure){};
        bool compare(Flag flag);
        bool compare(Flag flag, Argument arg);
        vector<Flag> flags_;
        vector<Argument> arguments_;
        bool terminateOnFailure_ = true;
    };


    class ArgParserBuilder{
    
    public:
        ArgParserBuilder& addFlag(Flag flag);
        ArgParserBuilder& addArgs(int argc, char const *argv[]);
        ArgParserBuilder& withoutTerminateOnFailure();
        ArgParser build();

    private:
        vector<Flag> flags_;
        vector<Argument> arguments_;
        bool terminateOnFailure_ = true;
    };


}