#include <string>
#include <functional>
#include <stdexcept> // For std::runtime_error

using namespace std;

namespace fluentArgs{

    //forward declaration
    class Flag;
    class FlagBuilder;
    class ArgParser;
    class ArgParserBuilder;

    class Argument{
        friend class ArgParserBuilder;
        private:
            string arg_;
            Argument(string arg) : arg_(arg){};
        public:
            string getArg();
    };
    
    class FlagBuilder{
        
        friend class Flag;
        private:
            string name_;
            string alias_ = "";
            int numValues_ = 0;
            function<void(std::vector<string>)> operation_;
            string delim_ = " ";
            string description_=" ";
            FlagBuilder() = default;

            void reset();


        public:
            FlagBuilder& withName(const string name);
            FlagBuilder& withAlias(const string alias);
            FlagBuilder& withOperation(function<void(std::vector<string>)> operation);
            FlagBuilder& withNumberOfValues(int numValues);
            FlagBuilder& withDelim(const string delim);
            FlagBuilder& withDescription(const string description);

            Flag build();
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
            string description_;

            Flag(string name, string alias, int numValues, function<void(std::vector<string>)> operation, string delim,string description)
            :name_(name),alias_(alias),operation_(operation),numValues_(numValues),delim_(delim),description_(description){}; //init

        public:
            string getName();
            string getAlias();
            int getNumValues();
            string getDelim();
            string getDescription();
            string resume();
            static FlagBuilder create();

        protected:
            void withName(string name);
            void setDouble_dash(string name);
            void setValue(string numValues);
            void withOperation(function <void(std::vector<string>)> operation);
            void executeOperation(std::vector<string> subParam);


        };
    
    class ArgParser{
        friend class ArgParserBuilder;

    public:
        void checkArguments();
        string resume();
        static ArgParserBuilder create();
        //bool checkOnly(//flags);

    private:
        ArgParser(vector<Flag> flags,vector<Argument> arguments, bool terminateOnFailure) : flags_(flags),arguments_(arguments),terminateOnFailure_(terminateOnFailure){};
        bool compare(Flag flag);
        bool compare(Flag flag, Argument arg);
        void showError(Flag flag);
        vector<Flag> flags_;
        vector<Argument> arguments_;
        bool terminateOnFailure_ = true;
    };


    class ArgParserBuilder{
    
    friend class ArgParser;
    public:
        ArgParserBuilder& withFlag(Flag flag);
        template<typename... Flags>
        ArgParserBuilder& withFlag(Flag flag, Flags...otherFlag);
        ArgParserBuilder& withArgs(int argc, char const *argv[]);
        ArgParserBuilder& withArgs(vector<Argument> args);
        ArgParserBuilder& withoutTerminateOnFailure();
        ArgParser build();

    private:
        vector<Flag> flags_;
        vector<Argument> arguments_;
        bool terminateOnFailure_ = true;
        ArgParserBuilder() = default;
    };

    template <typename... Flags>
    inline ArgParserBuilder &ArgParserBuilder::withFlag(Flag flag, Flags... otherFlag)
    {
        this->withFlag(flag);
        
        if constexpr (sizeof...(otherFlag) > 0) {
            this->withFlag(otherFlag...); // Chiamata ricorsiva
        }

        return *this;
    }

    static FlagBuilder createFlag(){
        return Flag::create();
    }

    static ArgParserBuilder createArgParser(){
        return ArgParser::create();
    }


}