#include <iostream>
#include <fstream>
#include <list>

using namespace std;

// Declaration of the different functions in order to be able to call them
// this because of the functions of the parsing process are in inverse order from the specification process;
void arithmeticExpression();
void factorPrime();
void termPrime();
void arithmeticExpressionPrime();
void statement();
void localDeclarations();
void statementList();
void returnStmt();
void varDeclaration();
void paramList();
void returnStmt();

// Definition for the object used to fill up the Symbol Table for Identifiers
class EntryIdSymbolTable {
public:
    int entry;
    string lexeme;
    string isVariable;
    string isFunction;
    string dataType;
    string isArray;
    string isLocal;
    string isGlobal;
};

// Definition for the object used to fill up the Symbol Table for Numbers
class EntryNumSymbolTable {
public:
    int entry;
    string lexeme;
};

// Definition for the object used to fill up the Scanners Output in form of a list of Tokens
class Token {
public:
    int tokenId;
    int entrySymbolTable;
    int codeLineFound;
};

//Class to see if a function is in the form void main (void)
class FunctionIsVoid {
public:
    string name;
    bool isVoid;
};

//Method to convert all the characters in a String to lower case
string toLowerCase(string word){
    //Iterates over the whole word and parses the element and at the same time is indexed again
    for (int i = 0; i < word.length(); i++){
        word[i] = tolower(word[i]);
    }
    return word;
}

//Boolean method to check if a character is a letter or not, returning true or false, respectively
bool isLetter(char character){
    //All of the valid letters are defined in an array
    char letter[54] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'ñ', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'Ñ', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    bool result = false;
    //For loop to iterate over the entire valid letters and if coincides with one it returns true
    //if it does not coincide, the default value is returned, which is false
    for (int i = 0; i < 54; i++ ){
        if (letter[i] == character){
            result = true;
        }
    }

    return result;
}

//Boolean method to check if a character is a number or not, returning true or false, respectively
bool isNumber(char character){
    //All of the valid numbers are defined in an array
    char number[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    bool result = false;
    //For loop to iterate over the entire valid numbers and if coincides with one it returns true
    //if it does not coincide, the default value is returned, which is false
    for (int i = 0; i < 10; i++ ){
        if (number[i] == character){
            result = true;
        }
    }
    return result;
}

//Boolean method to check if a string is a keyword of the language or not, returning true or false, respectively
bool isKeyword(string word){
    //All of the keywords are defined in an array
    string keywords[8] = {"else", "if", "int", "return", "void", "while", "input", "output"};
    bool result = false;

    //First it converts the word introduced to lowercase to be able to compare with the keyWords array
    string lowerCaseWord = toLowerCase(word);

    //For loop to iterate over all of the keywords and if coincides with one it returns true
    //if it does not coincide, the default value is returned, which is false
    for (int i = 0; i < 8; i++ ){
        if (keywords[i] == lowerCaseWord){
            result = true;
        }
    }
    return result;
}

//Boolean method to check if a string corresponds to an Identifier token or not, returning true or false, respectively
bool isId(string token){

    //To see if it is an id, it checks if the first character is a letter and if the
    //last character is a letter, if both are then is an Id
    if( isLetter(token.at(0)) && isLetter(token.at(token.length()-1))){
        return true;
    }else{
        return false;
    }
}

//Boolean method to check if a string corresponds to a number token or not, returning true or false, respectively
bool isNum(string token){

    //To see if it is a number, it checks if the first character is a number and if the
    //last character is a number, if both are then is a number
    if( isNumber(token.at(0)) && isNumber(token.at(token.length()-1))){
        return true;
    }
    return false;
}

//Boolean method to check if a state correspond to state of delimiter or composite state
bool isDelimOrComposite(int state){

    //All of the delimiter states and composite states
    int states[16] = {10, 11, 12, 24, 25, 26, 27, 28, 29, 30, 31, 14, 16, 18, 20, 21};
    bool result = false;

    //For loop to iterate over all of the states and if coincides with one it returns true
    //if it does not coincide, the default value is returned, which is false
    for (int i = 0; i < 16; i++ ){
        if (states[i] == state){
            result = true;
        }
    }
    return result;
}

//Global variables declarations
//Transition Table defined as a bi-dimensional array to represent all of the states and check whether
//It only contains the states that are not acceptors
int transitionTable [10][20] = {


                                    {10, 11, 12, 1, 4, 5, 6, 7,	24,	25,	26,	27,	28,	29,	30,	31,	8,	9,	0,	32},

                                    {13, 13, 2, 13, 13, 13, 13, 13, 13,	13,	13,	13,	13,	13,	13,	13,	13,	13,	13,	32},

                                    {2,	 2,	 3,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2},

                                    {2,	 2,	 2,	14,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2},

                                    {15, 15, 15, 15, 15, 15, 16, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 32},

                                    {17, 17, 17, 17, 17, 17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 32},

                                    {19, 19, 19, 19, 19, 19, 20, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 32},

                                    {32, 32, 32, 32, 32, 32, 21, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},

                                    {22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 8, 32, 22, 32},

                                    {23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 32, 9, 23, 32},

                                 };

//Advance transition table defined as a bi-dimensional array to represent all of the states taking into account the DFA definition
//they are advancing states or not
bool advanceTransitionTable [10][20] = {
                                    {false, false, false, true, true, true, true, true,	false, false, false, false, false, false, false, false,	true, true,	true, false},

                                    {false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

                                    {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},

                                    {true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},

                                    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

                                    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

                                    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

                                    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},

                                    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false},

                                    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false},

                                 };

//Array of accepting states that represent whether or not the state is acceptor
//To access the state it is the index of the array
bool acceptingStates [33] = {false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true,
                                true, true, true, true, true, true, true, true, true, true, false};

//Array of error states that represent whether or not the state is an error state
//To access the state it is the index of the array
bool errorStates [33] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                            false, false, false, false, false, false, false, false, false, false, false, true};

//Array of string storing all the valid tokens of the language
//The token id of each one is the index + 1
string validTokens[29] = {"else", "if", "int", "return", "void", "while", "input", "output", "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "=", ";", ",", "(", ")",
                            "[", "]", "{", "}", "ID", "NUM"};

//This variable represents the value modified in getNewState() to obtain the index to look for its value in
//the transition table and the advance transition table
int characterIndex;

//Variable used to control the number of new lines that the files has in order to specify the line when an error occurs
int codeLines = 1;

//List of objects of type Token to dynamically add them into it and a the end present it as the output
list <Token> tokenList;

//List of objects of type EntryIdSymbolTable to dynamically add them into
//it and a the end present it as the output
list <EntryIdSymbolTable> idSymbolTable;

//List of objects of type EntryIdSymbolTable to dynamically add them into
//it and a the end present it as the output
list <EntryIdSymbolTable> idSymbolTableParsed;

//List of objects of type EntryNumSymbolTable to dynamically add them into
//it and a the end present it as the output
list <EntryNumSymbolTable> numSymbolTable;

//List of strings to dynamically add them into and check the names of the functions used in the program
list <FunctionIsVoid> functionsUsed;

//Boolean method to check if a string exists as a lexeme in the id symbol table
bool existsInIdTable(string token, list<EntryIdSymbolTable> ids){

    bool result = false;

    //It iterates over the complete list checking the lexemes of each object element and if
    //one coincides then it exists in the list
    for (EntryIdSymbolTable entry : ids){
        if(entry.lexeme == token){
           result = true;
        }
    }

    return result;
}

//Boolean method to check if a variable's scope is global
bool isGlobal(string token, list<EntryIdSymbolTable> ids){

    bool result = false;

    //It iterates over the complete list checking the scopes of each object element and if
    //one coincides then it exists in the list
    for (EntryIdSymbolTable entry : ids){
        if(entry.lexeme == token){
            //If the scope is global returns true
            if(entry.isGlobal == "true"){
                result = true;
            }
        }
    }

    return result;
}

//Boolean method to check if a string exists as a lexeme in the number symbol table
bool existsInNumTable(string token){

    bool result = false;

    //It iterates over the complete list checking the lexemes of each object element and if
    //one coincides then it exists in the list
    for (EntryNumSymbolTable entry : numSymbolTable){
        if(entry.lexeme == token){
           result = true;
        }
    }

    return result;
}

//Method to get the entry of an specific lexeme in any of the symbol tables, based on the type received
int getEntry (string lexeme, string type) {

    //Checks if the type is Id to get the entry from that table
    if(type == "ID"){
        //It iterates over the id symbol table checking the lexemes, when it matches with
        //the parameter string then it returns the entry related with that lexeme
        for(EntryIdSymbolTable entry : idSymbolTable){
            if(entry.lexeme == lexeme){
                return entry.entry;
            }
        }
    //If the type is Num it gets the entry from that table
    } else{
        //It iterates over the num symbol table checking the lexemes, when it matches with
        //the parameter string then it returns the entry related with that lexeme
        for(EntryNumSymbolTable entry : numSymbolTable){
            if(entry.lexeme == lexeme){
                return entry.entry;
            }
        }
    }
}

//Method to calculate the new state of the DFA given the current state and the character that receives
int getNewState(int state, char nextCharacter){

    //In order to access the index of the column of the characters, it is checked based in the
    //transition table and the corresponding index is assigned depending in the character that receives
    if(nextCharacter == '+'){
       characterIndex = 0;
    }
    else if(nextCharacter == '-'){
       characterIndex = 1;
    }
    else if(nextCharacter == '*'){
       characterIndex = 2;
    }
    else if(nextCharacter == '/'){
       characterIndex = 3;
    }
    else if(nextCharacter == '<'){
       characterIndex = 4;
    }
    else if(nextCharacter == '>'){
       characterIndex = 5;
    }
    else if(nextCharacter == '='){
       characterIndex = 6;
    }
    else if(nextCharacter == '!'){
       characterIndex = 7;
    }
    else if(nextCharacter == ';'){
       characterIndex = 8;
    }
    else if(nextCharacter == ','){
       characterIndex = 9;
    }
    else if(nextCharacter == '('){
       characterIndex = 10;
    }
    else if(nextCharacter == ')'){
       characterIndex = 11;
    }
    else if(nextCharacter == '['){
       characterIndex = 12;
    }
    else if(nextCharacter == ']'){
       characterIndex = 13;
    }
    else if(nextCharacter == '{'){
       characterIndex = 14;
    }
    else if(nextCharacter == '}'){
       characterIndex = 15;
    }
    else if(isLetter(nextCharacter)){
       characterIndex = 16;
    }
    else if( isNumber(nextCharacter)){
       characterIndex = 17;
    }
    else if(nextCharacter == ' '||nextCharacter == '\n'||nextCharacter == '\t'){
       characterIndex = 18;
    }
    else{
       characterIndex = 19;
    }

    //When the index character is determined the value that returns is the value of the transition table
    //that represents the combination of the state and the index character
    return transitionTable[state][characterIndex];
}

//Method used to handle when an error occurs, depending on the characteristics of the erroneous token
//it can be identified
void handleErrors(string token){

    //If the first character of the token is a letter and the last is a number it means that the token
    //contains number next to a letter, which is not allowed, specifying the line
    if( isLetter(token.at(0)) && isNumber(token.at(token.length()-1))){
        cout << "ERROR -- It exists a number next to a letter, which is: " << token << " <<"<< token.at(token.length()-1)<< ">> In the line --- "<<codeLines<<endl;
    }
    //If the first character of the token is a number and the last is a letter it means that the token
    //contains letter next to a number, which is not allowed, specifying the line
    else if(isNumber(token.at(0)) && isLetter(token.at(token.length()-1))){
        cout << "ERROR -- It exists a letter next to a number, which is: " << token << " <<"<< token.at(token.length()-1)<< ">> In the line --- "<<codeLines<<endl;
    }
    //If the first character of the token is a '!' symbol, it is known that
    //the second character is not the '=' symbol, which is not allowed, specifying the line
    else if(token.at(0) == '!'){
        cout << "ERROR -- It exists an invalid character next to symbol \"!\" which is: " <<token << " <<"<< token.at(token.length()-1)<< ">> In the line --- "<<codeLines<<endl;
    }
    //If the first character is a '/' it means that comes from a comment
    else if(token.at(0) == '/'){
        //Checks if the token has the '*' at the end to show message of what is missing, specifying the line
        if(token.at(token.length()-1) == '*'){
            cout << "ERROR -- A comment was not correctly closed: "<<token << " is missing: << / >> In the line --- "<<codeLines<<endl;
        }else{
            cout << "ERROR -- A comment was not correctly closed: "<<token << " is missing: << */ >> In the line --- "<<codeLines<<endl;
        }
    }

    else{
        //It is identified as a strange character out of the language
        cout << "ERROR -- A strange character was found, and is: "<<token << " <<"<< token.at(token.length()-1)<< ">> In the line --- "<<codeLines<<endl;

    }
}

//Method to record the tokens in the token list and if its the case in its corresponding symbol table
void recordToken(string token){

    //String variable to define if the token is an ID or a NUM
    string itIs = "";

    //Int variable to store the entry of an already existing object in a symbol table
    int pastEntry = 0;

    //Checks if the token is an identifier
    if (isId(token)){
        //If it is then is checked if the identifier is a keyword
        if(isKeyword(token)){
            //If it is then is lower cased to be compared to the list of valid tokens and obtain its index
            //to be added to the token list
            string lowerCaseWord = toLowerCase(token);
            for (int i = 0; i < 29; i++ ){
                if (validTokens[i] == lowerCaseWord){

                    //Once is found in the valid tokens, the object is created
                    Token tokenObj;
                    //The token id as previously shown is the index + 1
                    tokenObj.tokenId = i + 1;
                    //And as the token is a keyword it does not need an entry to symbol table so it is set as 0
                    tokenObj.entrySymbolTable = 0;
                    //Set of code line of the token
                    tokenObj.codeLineFound = codeLines;
                    //The object is added to the token list
                    tokenList.push_back(tokenObj);
                }
            }
        }else{
            //If the identifier is not a keyword then is an ID
            //The itIs variable is set as ID to be used like one
            itIs = "ID";

            //It iterates over the entire validTokens array to check its index
            for (int i = 0; i < 29; i++ ){
                if (validTokens[i] == itIs){
                    //The Token object is created to store the information
                    Token tokenObj;

                    //It checks if the token exists in the identifier symbol table
                    if(!existsInIdTable(token, idSymbolTable)){
                        //The EntryIdSymbolTable object is created to store the information
                        EntryIdSymbolTable idObj;
                        //If it does not the entry value is obtained by getting the size of the list
                        //and adding 1
                        idObj.entry = idSymbolTable.size()+1;
                        //The lexeme is directly the token received
                        idObj.lexeme = token;
                        //The object is added to the symbol table
                        idSymbolTable.push_back(idObj);

                        //As is a token it has to be added to the token list too
                        //The token id as previously shown is the index + 1
                        tokenObj.tokenId = i + 1;
                        //The entry to symbol table is equals to the entry in the id symbol table
                        //in order to be related by it
                        tokenObj.entrySymbolTable = idObj.entry;
                        //Set of code line of the token
                        tokenObj.codeLineFound = codeLines;
                        //The object is added to the token list
                        tokenList.push_back(tokenObj);
                    }else{
                        //If the object already exists in the symbol table it is not
                        //registered in the symbol table but it has to be registered in the token list
                        //with the same entry that has in the symbol table

                        //It gets the int value of the entry, with the token and the type
                        pastEntry = getEntry(token, itIs);
                        //The token id as previously shown is the index + 1
                        tokenObj.tokenId = i + 1;
                        //Once the entry is obtained, that is the one assigned to the token list
                        //as the entry to symbol table
                        tokenObj.entrySymbolTable = pastEntry;
                        //Set of code line of the token
                        tokenObj.codeLineFound = codeLines;
                        //The object is added to the token list
                        tokenList.push_back(tokenObj);
                    }
                }
            }
        }
    }//If the token is a number
    else if (isNum(token)){
        //The itIs variable is set as NUM to be used like one
        itIs = "NUM";
            //It iterates over the entire validTokens array to check its index
            for (int i = 0; i < 29; i++ ){
                if (validTokens[i] == itIs){
                    //The Token object is created to store the information
                    Token tokenObj;

                    //It checks if the token exists in the number symbol table
                    if(!existsInNumTable(token)){

                        //The EntryNumSymbolTable object is created to store the information
                        EntryNumSymbolTable numObj;
                        //If it does not the entry value is obtained by getting the size of the list
                        //and adding 1
                        numObj.entry = numSymbolTable.size()+1;
                        //The lexeme is directly the token received
                        numObj.lexeme = token;
                        //The object is added to the symbol table
                        numSymbolTable.push_back(numObj);

                        //As is a token it has to be added to the token list too
                        //The token id as previously shown is the index + 1
                        tokenObj.tokenId = i + 1;
                        //The entry to symbol table is equals to the entry in the num symbol table
                        //in order to be related by it
                        tokenObj.entrySymbolTable = numObj.entry;
                        //Set of code line of the token
                        tokenObj.codeLineFound = codeLines;
                        //The object is added to the token list
                        tokenList.push_back(tokenObj);
                    }else{

                        //If the object already exists in the symbol table it is not
                        //registered in the symbol table but it has to be registered in the token list
                        //with the same entry that has in the symbol table

                        //It gets the int value of the entry, with the token and the type
                        pastEntry = getEntry(token, itIs);
                        //The token id as previously shown is the index + 1
                        tokenObj.tokenId = i + 1;
                        //Once the entry is obtained, that is the one assigned to the token list
                        //as the entry to symbol table
                        tokenObj.entrySymbolTable = pastEntry;
                        //Set of code line of the token
                        tokenObj.codeLineFound = codeLines;
                        //The object is added to the token list
                        tokenList.push_back(tokenObj);
                    }
                }
            }
    }else{
        //If the token is not a keyword, number or identifier
        //it means is a special character and is added to only the token list

        //It iterates over the entire validTokens array to check its index
        for (int i = 0; i < 29; i++ ){
            if (validTokens[i] == token){

                //Once is found in the valid tokens, the object is created
                Token tokenObj;
                //The token id as previously shown is the index + 1
                tokenObj.tokenId = i + 1;
                //And as the token is a special character it does not need an entry to
                //symbol table so it is set as 0
                tokenObj.entrySymbolTable = 0;
                //Set of code line of the token
                tokenObj.codeLineFound = codeLines;
                //The object is added to the token list
                tokenList.push_back(tokenObj);
            }
        }
    }
}

//Method to print the output of the scanner in the console and in an external file
void getOutput(list <Token> tokens, list<EntryIdSymbolTable> ids, list<EntryNumSymbolTable> nums){

    //The file is defined as an ofstream to be able to write on it
    ofstream outputFile;
    //Specification of the file used to write the output
    string exportFile = "Scanner_Output.txt";
    //Opening of the file
    outputFile.open(exportFile);

    //Printing format to present the output in the console and in the ofstream file
    cout << "--Token list --"<< endl;
    outputFile << "--Token list --"<< endl;

    //Iterates over the list of tokens to print each one of them
    for (Token token : tokens){
        //If the token has an entry value of 0 it is not presented
        //if it has is presented together with the token id
        if(token.entrySymbolTable == 0){
            cout << "<"<<token.tokenId<< ">"<< endl;
            outputFile << "<"<<token.tokenId<< ">"<<endl;
        }else{
            cout << "<"<<token.tokenId<< ", "<<token.entrySymbolTable<< ">"<<endl;
            outputFile << "<"<<token.tokenId<< ", "<<token.entrySymbolTable<< ">"<<endl;
        }
    }

    cout << "-- Identifier Symbol table --"<<endl;
    cout << "Entry | Contents"<<endl;
    outputFile << "-- Identifier Symbol table --"<<endl;
    outputFile << "Entry | Contents"<<endl;

    //Iterates over the identifier symbol table to print each one of them
    //the entry and the lexeme is printed
    for (EntryIdSymbolTable token : ids){
        cout << token.entry<< " | "<<token.lexeme <<endl;
        outputFile << token.entry<< " | "<<token.lexeme <<endl;

    }

    cout << "-- Number Symbol Table --"<< endl;
    cout << "Entry | Contents"<<endl;
    outputFile << "-- Number Symbol Table --"<< endl;
    outputFile << "Entry | Contents"<<endl;

    //Iterates over the number symbol table to print each one of them
    //the entry and the lexeme is printed
    for (EntryNumSymbolTable token : nums){
        cout << token.entry<< " | "<<token.lexeme<<endl;
        outputFile << token.entry<< " | "<<token.lexeme<<endl;

    }

    //Closing of the file used
    outputFile.close();
}


///Parser Functionality

//Token definition to store the current token processed
Token currentToken;

//Variable to control the number of the token which is being processed
int tokenCounter = 0;

//Method to print the output of the parser in the console
void getOutputSymbolTables(list<EntryIdSymbolTable> ids, list<EntryNumSymbolTable> nums){

    cout << "-- Identifier Symbol table --"<<endl;
    cout << "Entry | Contents |    Is var   | Is function  | Data type  |  Is Array   |  Is Global  | Is Local "<<endl;

    //Iterates over the identifier symbol table to print all of the properties of each one of them
    for (EntryIdSymbolTable token : ids){
        cout << token.entry<< "     |    "<<token.lexeme << "     |    "
        <<token.isVariable<< "     |    "<<token.isFunction << "     |    "
        <<token.dataType << "     |    "<<token.isArray  << "     |    "
        <<token.isGlobal<< "   |    "
        <<token.isLocal << endl;
    }

    cout << "-- Number Symbol Table --"<< endl;
    cout << "Entry | Contents"<<endl;

    //Iterates over the number symbol table to print each one of them
    //the entry and the lexeme is printed
    for (EntryNumSymbolTable token : nums){
        cout << token.entry<< " | "<<token.lexeme<<endl;
    }
}

//Method to get the next token in the list of tokens
Token getNextToken(list <Token> tokens){

    //The first thing done is to get the size of the list of tokens by iterating over the list and adding it to a counter
    int tokenListSize = 0;
    for (Token token : tokens){
        tokenListSize++;
    }

    //Declaration of the array of tokens with the size of the token list obtained
    Token tokenArray [tokenListSize] = {};

    //Filling the created array with all the elements of the list iterating over the list
    int counter = 0;
    for (Token token : tokens){
        tokenArray[counter] = token;
        counter++;
    }

    //Temporal variable to store th token counter
    int counterToGetNext = tokenCounter;

    //Increment the token counter by 1 in order to advance
    tokenCounter++;

    return tokenArray[counterToGetNext];

}

//Method to get the specific token in the list of tokens by an index
Token getSpecificToken(list <Token> tokens, int index){

    //The first thing done is to get the size of the list of tokens by iterating over the list and adding it to a counter
    int tokenListSize = 0;
    for (Token token : tokens){
        tokenListSize++;
    }

    //Declaration of the array of tokens with the size of the token list obtained
    Token tokenArray [tokenListSize] = {};

    //Filling the created array with all the elements of the list iterating over the list
    int counter = 0;
    for (Token token : tokens){
        tokenArray[counter] = token;
        counter++;
    }

    return tokenArray[index];
}

//Method to add the specific final token $ to the list
void addFinalToken(){

    //Declaration of the token object to be added
    Token tokenObj;
    //The token id is set to a Special token Id which is 90
    tokenObj.tokenId = 90;
    //As it is the final Token $ does not need a entry
    tokenObj.entrySymbolTable = 0;
    //The object is added to the token list
    tokenList.push_back(tokenObj);
}

//Method to handle the different syntactic error occurred, based on a token and a error
void raiseError(int token, string error){

    // If the error occurs an the current token is the last one then the EOF is reached
    if(currentToken.tokenId == 90){

        cout << "End of file is reached and it does not match with the current token" << endl;

    }else{
        //In order to get the correct line of code the token has to be 2 before the token counter
        Token specificToken = getSpecificToken(tokenList, tokenCounter - 2);

        ///Print the token where the error is found
        cout << "Error in the symbol: " << validTokens[token - 1] << endl;
        //Prints the error seen and the line of the error
        cout << error << " At line: " << specificToken.codeLineFound << endl;
    }

    //Stops the execution when an error arise
    exit(0);
}

//Method to modify the symbol table every time it is needed, given a token to process, an index, and a scope
void fillSymbolTable(Token token, int index, string scope){

    //Declaration of token to get the next token of the given token
    Token nextToken = getSpecificToken(tokenList, index - 2);

    //Declaration of token to get the next token plus 1 of the given token
    Token secondNextToken = getSpecificToken(tokenList, index - 1);

    //Declaration of token to get the past token of the given token
    Token previousToken = getSpecificToken(tokenList, index - 4);

    //If the token represent an id it can be either a variable or a function
    if(token.tokenId == 28){

        //Check if it is a variable by checking it the next token is an open bracket
        if(nextToken.tokenId == 24){

            //If the scope of the variable is global
            if(scope == "G"){

                //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
                for (EntryIdSymbolTable entry : idSymbolTable){
                    if(entry.entry  == token.entrySymbolTable){

                        entry.dataType = "int";
                        entry.isArray = "true";
                        entry.isFunction = "false";
                        entry.isVariable = "true";
                        entry.isGlobal = "true";
                        entry.isLocal = "false";

                        //If the lexeme does not exists in the symbol table then it is added to a new symbol table now parsed
                        if(!existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                            idSymbolTableParsed.push_back(entry);
                        }else{
                            //If the global variable is repeated then an error arises
                            cout<< "The global variable: " <<entry.lexeme<<" is already defined"<<endl;
                            exit(0);
                        }
                    }
                }
            }
            //If the scope of the variable is local
            else if(scope == "L"){

                //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
                for (EntryIdSymbolTable entry : idSymbolTable){
                    if(entry.entry  == token.entrySymbolTable){

                        entry.dataType = "int";
                        entry.isArray = "true";
                        entry.isFunction = "false";
                        entry.isVariable = "true";
                        entry.isGlobal = "false";
                        entry.isLocal = "true";

                        //The entry is going to be added independently of the usage or the name of the variables
                        //this is because if it is restricted to different names it could cause problems

                        idSymbolTableParsed.push_back(entry);
                    }
                }
            }
        }
        //Check if it is a variable by checking it the next token is a semicolon, coma or close parenthesis
        else if(nextToken.tokenId == 20 || nextToken.tokenId == 21 || nextToken.tokenId == 23){

            //If the scope of the variable is global
            if(scope == "G"){

                //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
                for (EntryIdSymbolTable entry : idSymbolTable){
                    if(entry.entry  == token.entrySymbolTable){

                        entry.dataType = "int";
                        entry.isArray = "false";
                        entry.isFunction = "false";
                        entry.isVariable = "true";
                        entry.isGlobal = "true";
                        entry.isLocal = "false";

                        //If the lexeme does not exists in the symbol table then it is added to a new symbol table now parsed
                        if(!existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                            idSymbolTableParsed.push_back(entry);
                        }else{
                            //If the global variable is repeated then an error arises
                            cout<< "The global variable: " <<entry.lexeme<<" is already defined"<<endl;
                            exit(0);
                        }
                    }
                }
            }
            //If the scope of the variable is local
            else if(scope == "L"){

                //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
                for (EntryIdSymbolTable entry : idSymbolTable){
                    if(entry.entry  == token.entrySymbolTable){

                        entry.dataType = "int";
                        entry.isArray = "false";
                        entry.isFunction = "false";
                        entry.isVariable = "true";
                        entry.isGlobal = "false";
                        entry.isLocal = "true";

                        //The entry is going to be added independently of the usage or the name of the variables
                        //this is because if it is restricted to different names it could cause problems

                        idSymbolTableParsed.push_back(entry);

                    }
                }
            }
        }

        //Check if it is a function by checking if the next token is a parenthesis and the previous is an int reserved word
        else if(nextToken.tokenId == 22 && previousToken.tokenId == 3){

            //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
            for (EntryIdSymbolTable entry : idSymbolTable){
                if(entry.entry  == token.entrySymbolTable){
                    entry.dataType = "int";
                    entry.isArray = "false";
                    entry.isFunction = "true";
                    entry.isVariable = "false";
                    entry.isGlobal = "true";
                    entry.isLocal = "false";

                    //If the lexeme does not exists in the symbol table then it is added to a new symbol table now parsed
                    if(!existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                        idSymbolTableParsed.push_back(entry);

                        //Object to store the name of the function and if the parameters are void
                        FunctionIsVoid func;
                        func.name = entry.lexeme;
                        func.isVoid = false;
                        //The name of the functions is stored to store all the functions of the program
                        functionsUsed.push_back(func);

                    }
                    //If the function is repeated then an error arises
                    else if(existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                        cout<< "Two or more functions are named the same <<" << entry.lexeme << ">> already is declared"<<endl;
                        exit(0);
                    }
                }
            }
        }
        //Check if it is a function by checking if the next token is a parenthesis and the previous is an void reserved word
        else if(nextToken.tokenId == 22 && previousToken.tokenId == 5){

            //It checks if the parameters of a void function are void
            if(secondNextToken.tokenId == 5){
                //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
                for (EntryIdSymbolTable entry : idSymbolTable){
                    if(entry.entry  == token.entrySymbolTable){
                        entry.dataType = "void";
                        entry.isArray = "false";
                        entry.isFunction = "true";
                        entry.isVariable = "false";
                        entry.isGlobal = "true";
                        entry.isLocal = "false";

                        //If the lexeme does not exists in the symbol table then it is added to a new symbol table now parsed
                        if(!existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                            idSymbolTableParsed.push_back(entry);

                            //Object to store the name of the function and if the parameters are void, here set to true
                            FunctionIsVoid func;
                            func.name = entry.lexeme;
                            func.isVoid = true;
                            //The name of the functions is stored to store all the functions of the program
                            functionsUsed.push_back(func);
                        }
                        //If the function is repeated then an error arises
                        else if(existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                            cout<< "Two or more functions are named the same <<" << entry.lexeme << ">> already is declared"<<endl;
                            exit(0);
                        }
                    }
                }
            }else{
                //It iterates  over the original symbol table and if the entry is found then it is modified with the new information
                for (EntryIdSymbolTable entry : idSymbolTable){
                    if(entry.entry  == token.entrySymbolTable){
                        entry.dataType = "void";
                        entry.isArray = "false";
                        entry.isFunction = "true";
                        entry.isVariable = "false";
                        entry.isGlobal = "true";
                        entry.isLocal = "false";

                        //If the lexeme does not exists in the symbol table then it is added to a new symbol table now parsed
                        if(!existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                            idSymbolTableParsed.push_back(entry);

                            //Object to store the name of the function and if the parameters are void, here set to true
                            FunctionIsVoid func;
                            func.name = entry.lexeme;
                            func.isVoid = false;
                            //The name of the functions is stored to store all the functions of the program
                            functionsUsed.push_back(func);
                        }
                        //If the function is repeated then an error arises
                        else if(existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                            cout<< "Two or more functions are named the same <<" << entry.lexeme << ">> already is declared"<<endl;
                            exit(0);
                        }
                    }
                }
            }
        }
    }
}

//Function to match the current token with the one that should be noted
int Match(int terminal, string error){

    //If it is the same then the current token advances, if not an error arises
    if(currentToken.tokenId == terminal){
        currentToken = getNextToken(tokenList);
    } else{
        raiseError(terminal, error);
    }
}

//Function to check if a token exists in the symbol table
bool existsInIdTableByToken(Token token){
    bool result;
    //Iterates over the entire list of IDs and if it exits set the result variable to true
    for (EntryIdSymbolTable entry : idSymbolTable){
        if(entry.entry  == token.entrySymbolTable){
            if(existsInIdTable(entry.lexeme, idSymbolTableParsed)){
                result = true;
            }
        }
    }
    return result;
}

//From the method argsList() to program()
//This represent the recursive descent algorithm used in the development of the project

//What it does is that based on the first plus set of every non terminal, they are some options for the flow to go,
//depending on how the token list is derived it implicitly constructs the parse tree

void argsList(){
    if(currentToken.tokenId == 21){
        Match(21, "An argument list must be separated by: , ");

        arithmeticExpression();
        argsList();

    }else if(currentToken.tokenId == 23){

        return;

    }else{
        raiseError(currentToken.tokenId, "An argument list must be separated by: , or missing close parenthesis" );
    }
}

void call(){
    if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in function call");

        factorPrime();
        termPrime();
        arithmeticExpressionPrime();
        argsList();

        Match(23, "The function call must be enclosed in parenthesis");

    }else if(currentToken.tokenId == 22){
        Match(22, "The arithmetic expression must be enclosed in parenthesis");

        arithmeticExpression();

        Match(23, "The arithmetic expression must be enclosed in parenthesis");

        termPrime();
        arithmeticExpressionPrime();
        argsList();

        Match(23, "The function call must be enclosed in parenthesis");

    }else if(currentToken.tokenId == 29){
        Match(29, "Number must be specified when calling function");

        termPrime();
        arithmeticExpressionPrime();
        argsList();

        Match(23, "The function call must be enclosed in parenthesis");

    }else if(currentToken.tokenId == 23){
        Match(23, "The function call must be enclosed in parenthesis");

    }else{
        raiseError(currentToken.tokenId, "A Identifier, open parenthesis, Number or close parenthesis is missing in factor");
    }
}

void factorPrime(){
    if(currentToken.tokenId == 24){
        Match(24, "The arithmetic expression must be enclosed in brackets");

        arithmeticExpression();

        Match(25, "The arithmetic expression must be enclosed in brackets");

    }else if(currentToken.tokenId == 22){
        Match(22, "The function call must be enclosed in parenthesis");

        call();

    }else if (currentToken.tokenId == 11 || currentToken.tokenId == 12 ||
             currentToken.tokenId == 9 || currentToken.tokenId == 10 ||
             currentToken.tokenId == 14 || currentToken.tokenId == 13 ||
             currentToken.tokenId == 15 || currentToken.tokenId == 16 ||
             currentToken.tokenId == 17 || currentToken.tokenId == 18 ||
             currentToken.tokenId == 20 || currentToken.tokenId == 23 ||
             currentToken.tokenId == 25 || currentToken.tokenId == 21){
        return;

    }else{
        raiseError(currentToken.tokenId, "An open bracket, open parenthesis, multiplication symbol, division symbol, plus symbol, minus symbol, logic operator is missing(<=, <, >, >= ,==, !=), ; , ), ] or ,  is missing in arithmetic expression");
    }
}
void factor(){
    if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in factor");
        factorPrime();

    }else if(currentToken.tokenId == 22){
        Match(22, "The arithmetic expression must be enclosed in parenthesis");

        arithmeticExpression();

        Match(23, "The arithmetic expression must be enclosed in parenthesis");

    }else if(currentToken.tokenId == 29){
        Match(29, "Number must be specified when declaring number factor");

    }else{
        raiseError(currentToken.tokenId, "A Identifier, open parenthesis or Number is missing in factor");
    }
}

void termPrime(){
    if(currentToken.tokenId == 11){
        Match(11, "A multiplication symbol must be placed when multiplying arithmetic expressions");

        factor();
        termPrime();

    }else if(currentToken.tokenId == 12){
        Match(12, "A division symbol must be placed when dividing arithmetic expressions");

        factor();
        termPrime();

    }else if(currentToken.tokenId == 9 || currentToken.tokenId == 10 || currentToken.tokenId == 14 || currentToken.tokenId == 13 ||
             currentToken.tokenId == 15 || currentToken.tokenId == 16 || currentToken.tokenId == 17 || currentToken.tokenId == 18 ||
             currentToken.tokenId == 20 || currentToken.tokenId == 23 || currentToken.tokenId == 25 || currentToken.tokenId == 21){

        return;

    }else{
        raiseError(currentToken.tokenId, "A multiplication symbol, division symbol, plus symbol, minus symbol, logic operator is missing (<=, <, >, >= ,==, !=), ; , ), ] or ,  is missing in arithmetic expression");
    }
}
void term(){
    if(currentToken.tokenId == 28){
        if(!existsInIdTableByToken(currentToken)){
            //Checks if the variable is declared
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in term");

        factorPrime();
        termPrime();

    }else if(currentToken.tokenId == 22){
        Match(22, "The arithmetic expression must be enclosed in parenthesis");
        arithmeticExpression();
        Match(23, "The arithmetic expression must be enclosed in parenthesis");

        termPrime();

    }else if(currentToken.tokenId == 29){
        Match(29, "Number must be specified when declaring number term");

        termPrime();
    }else{
        raiseError(currentToken.tokenId, "A Identifier, open parenthesis or Number is missing in term");
    }
}

void arithmeticExpressionPrime(){
    if(currentToken.tokenId == 9){
        Match(9, "A plus symbol must be placed when adding arithmetic expressions");

        term();
        arithmeticExpressionPrime();

    }else if(currentToken.tokenId == 10){
        Match(10, "A minus symbol must be placed when adding arithmetic expressions");

        term();
        arithmeticExpressionPrime();

    }else if(currentToken.tokenId == 14 || currentToken.tokenId == 13 || currentToken.tokenId == 15 || currentToken.tokenId == 16 ||
             currentToken.tokenId == 17 || currentToken.tokenId == 18 ||currentToken.tokenId == 20 || currentToken.tokenId == 23 ||
             currentToken.tokenId == 25 || currentToken.tokenId == 21){

        return;

    }

    else{
        raiseError(currentToken.tokenId, "A plus symbol, minus symbol, logic operator is missing (<=, <, >, >= ,==, !=), ; , ), ] or ,  is missing in arithmetic expression");
    }
}

void arithmeticExpression(){
    if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in arithmetic expression");

        factorPrime();
        termPrime();
        arithmeticExpressionPrime();

    }else if(currentToken.tokenId == 22){
        Match(22, "The arithmetic expression must be enclosed in parenthesis");

        arithmeticExpression();

        Match(23, "The arithmetic expression must be enclosed in parenthesis");

        termPrime();
        arithmeticExpressionPrime();

    }else if(currentToken.tokenId == 29){
        Match(29, "Number must be specified when declaring number expression");

        termPrime();
        arithmeticExpressionPrime();

    }else{
        raiseError(currentToken.tokenId, "A Identifier, open parenthesis or Number is missing in arithmetic expression");
    }
}

void expressionPrime(){
    if(currentToken.tokenId == 14){
        Match(14, "A less than or equals is missing in comparison");

        arithmeticExpression();

    }else if(currentToken.tokenId == 13){
        Match(13, "A less than is missing in comparison");

        arithmeticExpression();

    }else if(currentToken.tokenId == 15){
        Match(15, "A more than is missing in comparison");

        arithmeticExpression();

    }else if(currentToken.tokenId == 16){
        Match(16, "A more than or equals is missing in comparison");

        arithmeticExpression();

    }else if(currentToken.tokenId == 17){
        Match(17, "A doble-equals is missing in comparison");

        arithmeticExpression();

    }else if(currentToken.tokenId == 18){
        Match(18, "A not equals is missing in comparison");

        arithmeticExpression();

    }else if(currentToken.tokenId == 20 || currentToken.tokenId == 23){
        return;

    }else{
        raiseError(currentToken.tokenId, "A logic operator is missing (<=, <, >, >= ,==, !=) or ; or ) ");
    }
}

void expression(){
    if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in expression");

        factorPrime();
        termPrime();
        arithmeticExpressionPrime();
        expressionPrime();

    }else if(currentToken.tokenId == 22){
        Match(22, "The arithmetic expression must be enclosed in parenthesis");

        arithmeticExpression();

        Match(23, "The arithmetic expression must be enclosed in parenthesis");

        termPrime();
        arithmeticExpressionPrime();
        expressionPrime();

    }else if(currentToken.tokenId == 29){
        Match(29, "Number must be specified when declaring number expression");

        termPrime();
        arithmeticExpressionPrime();
        expressionPrime();

    }else{
        raiseError(currentToken.tokenId, "A Identifier, open parenthesis or Number is missing in expression");
    }
}

void var(){
    if(currentToken.tokenId == 24){
        Match(24, "The arithmetic expression must be enclosed in brackets");

        arithmeticExpression();

        Match(25, "The arithmetic expression must be enclosed in brackets");

    }else if(currentToken.tokenId == 20){
        return;

    }else{
        raiseError(currentToken.tokenId, "An open bracket or semicolon is missing in variable");
    }
}

void returnStmt(){
    if(currentToken.tokenId == 20){
        Match(20, "Semicolon must be at the end of return statement");

    }else if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in return statement");

        factorPrime();
        termPrime();
        arithmeticExpressionPrime();
        expressionPrime();

        Match(20, "Semicolon must be at the end of return statement");

    }else if(currentToken.tokenId == 22){
        Match(22, "The arithmetic expression must be enclosed in parenthesis");

        arithmeticExpression();

        Match(23, "The arithmetic expression must be enclosed in parenthesis");

        termPrime();
        arithmeticExpressionPrime();
        expressionPrime();

        Match(20, "Semicolon must be at the end of return statement");

    }else if(currentToken.tokenId == 29){
        Match(29, "Number must be specified when declaring number return statement");

        termPrime();
        arithmeticExpressionPrime();
        expressionPrime();

        Match(20, "Semicolon must be at the end of return statement");

    }else{
        raiseError(currentToken.tokenId, "A semicolon, Identifier, open parenthesis or Number is missing in return statement");
    }
}

void elseStmt(){
    if(currentToken.tokenId == 1){
        Match(1, "else must be specified when declaring else-statement");

        statement();

    }else if(currentToken.tokenId == 28 || currentToken.tokenId == 26 ||
             currentToken.tokenId == 2 || currentToken.tokenId == 6 ||
             currentToken.tokenId == 4 || currentToken.tokenId == 7 ||
             currentToken.tokenId == 8 || currentToken.tokenId == 27 ||
             currentToken.tokenId == 1){
        return;

    }else{
        raiseError(currentToken.tokenId, "The else, ID, {, if, while, return, input, output or } is missing in else-statement");
    }
}

void statementPrime(){
    if(currentToken.tokenId == 24){
        Match(24, "The arithmetic expression must be enclosed in brackets");

        arithmeticExpression();

        Match(25, "The arithmetic expression must be enclosed in brackets");
        Match(19, "An equals sign is missing in assignation");

        expression();

        Match(20, "Semicolon must be at the end of assignation");

    }else if(currentToken.tokenId == 19){
        Match(19, "An equals sign is missing in assignation");


        expression();

        Match(20, "Semicolon must be at the end of assignation");

    }else if(currentToken.tokenId == 22){
        Match(22, "The function call must be enclosed in parenthesis");

        call();

        Match(20, "Semicolon must be at the end of function call");

    }else{
        raiseError(currentToken.tokenId, "An open bracket, equals or open parenthesis is missing in statement");
    }
}

void statement(){
    if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in statement");

        statementPrime();

    }else if(currentToken.tokenId == 26){
        Match(26, "The declarations and statements must be enclosed in curly brackets");

        localDeclarations();
        statementList();

        Match(27, "The declarations and statements must be enclosed in curly brackets");

    }else if(currentToken.tokenId == 2){
        Match(2, "If must be specified when declaring if-statement");
        Match(22, "If expression must be enclosed in parenthesis");

        expression();

        Match(23, "If expression must be enclosed in parenthesis");

        statement();
        elseStmt();

    }else if(currentToken.tokenId == 6){
        Match(6, "while must be specified when declaring while-statement");
        Match(22, "while expression must be enclosed in parenthesis");

        expression();

        Match(23, "while expression must be enclosed in parenthesis");

        statement();

    }else if(currentToken.tokenId == 4){
        Match(4, "return must be specified when declaring return-statement");

        returnStmt();

    }else if(currentToken.tokenId == 7){
        Match(7, "input must be specified when declaring input-statement");
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing for input variable");

        var();

        Match(20, "Semicolon must be at the end of input-statement");

    }else if(currentToken.tokenId == 8){
        Match(8, "output must be specified when declaring output-statement");

        expression();

        Match(20, "Semicolon must be at the end of output-statement");

    }else if(currentToken.tokenId == 27){
        return;

    }else{
        raiseError(currentToken.tokenId, "Identifier, open bracket, if, while, return, input, output or close curly bracket is missing in statement");
    }
}

void statementList(){
    if(currentToken.tokenId == 28){
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing in statement");

        statementPrime();
        statementList();

    }else if(currentToken.tokenId == 26){
        Match(26, "The declarations and statements must be enclosed in curly brackets");

        localDeclarations();
        statementList();

        Match(27, "The declarations and statements must be enclosed in curly brackets");

        statementList();

    }else if(currentToken.tokenId == 2){
        Match(2, "If must be specified when declaring if-statement");
        Match(22, "If expression must be enclosed in parenthesis");

        expression();

        Match(23, "If expression must be enclosed in parenthesis");

        statement();
        elseStmt();
        statementList();


    }else if(currentToken.tokenId == 6){
        Match(6, "while must be specified when declaring while-statement");
        Match(22, "while expression must be enclosed in parenthesis");

        expression();

        Match(23, "while expression must be enclosed in parenthesis");

        statement();
        statementList();

    }else if(currentToken.tokenId == 4){
        Match(4, "return must be specified when declaring return-statement");

        returnStmt();
        statementList();

    }else if(currentToken.tokenId == 7){
        Match(7, "input must be specified when declaring input-statement");
        //Checks if the variable is declared
        if(!existsInIdTableByToken(currentToken)){
            cout << "Error variable or function was not declared at:"<< currentToken.codeLineFound<< " "<<endl;
            exit(0);
        }
        Match(28, "An ID is missing for input variable");

        var();

        Match(20, "Semicolon must be at the end of input-statement");

        statementList();

    }else if(currentToken.tokenId == 8){
        Match(8, "output must be specified when declaring output-statement");

        expression();

        Match(20, "Semicolon must be at the end of output-statement");

        statementList();

    }else if(currentToken.tokenId == 27){
        return;

    }else{
        raiseError(currentToken.tokenId, "Identifier, open bracket, if, while, return, input, output or close curly bracket is missing in statement");
    }
}

void localDeclarations(){
    if(currentToken.tokenId == 3){
        Match(3, "In type is needed for local variable");
        Match(28, "An ID is missing after int declaration");

        varDeclaration();
        localDeclarations();

    }else if(currentToken.tokenId == 28 || currentToken.tokenId == 26 ||
             currentToken.tokenId == 2 || currentToken.tokenId == 6 ||
             currentToken.tokenId == 4 || currentToken.tokenId == 7 ||
             currentToken.tokenId == 8 || currentToken.tokenId == 27){
        return;

    }else{
        raiseError(currentToken.tokenId, "The local declarations should start with int type specifier or ID, {, if, while, return, input, output or } missing");
    }
}

void paramsPrime(){
    if(currentToken.tokenId == 24){

        //Here a local variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "L");
        Match(24, "Brackets must exist when an array is declared");
        Match(25, "Brackets must exist when an array is declared");

        paramList();


    }else if(currentToken.tokenId == 21){

        //Here a local variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "L");
        Match(21, "The parameter list must be separated by comas");
        Match(3, "A type for the parameter is missing");
        Match(28, "An ID is missing after int declaration");

        paramsPrime();

    }else if(currentToken.tokenId == 23){

        return;

    }else{
        raiseError(currentToken.tokenId, "A coma , open bracket or close parenthesis is missing in parameters");
    }
}

void paramList(){
    if(currentToken.tokenId == 21){

        Match(21, "The parameter list must be separated by comas");
        Match(3, "A type for the parameter is missing");
        Match(28, "An ID is missing after int declaration");

        paramsPrime();

    }else if(currentToken.tokenId == 23){
        return;

    }else{
        raiseError(currentToken.tokenId, "The parameter list should be separated by comas or close parenthesis missing");
    }
}

void params(){
    if(currentToken.tokenId == 3){

        Match(3, "A type for the parameter is missing");
        Match(28, "An ID is missing after int declaration");

        paramsPrime();

    }else if(currentToken.tokenId == 5){
        Match(5, "If no parameters are needed void must be specified");

    }else{
        raiseError(currentToken.tokenId, "Parameters must start with int or void type specifier");
    }
}

void varDeclaration(){
    if(currentToken.tokenId == 20){
        //Here a local variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "L");
        Match(20, "A semicolon must be at the end of a declaration");

    }else if(currentToken.tokenId == 24){
        //Here a local variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "L");
        Match(24, "The number must be enclosed in brackets");
        Match(29, "A number must be in the brackets");
        Match(25, "The number must be enclosed in brackets");
        Match(20, "A semicolon must be at the end of a declaration");

    }else{
        raiseError(currentToken.tokenId, "A semicolon or open bracket is missing in declaration");
    }
}

void declarationPrime(){
    if(currentToken.tokenId == 20){
        Match(20, "A semicolon must be at the end of a declaration");
        //Here a global variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter-3), tokenCounter, "G");

    }else if(currentToken.tokenId == 24){
        Match(24, "The number must be enclosed in brackets");
        Match(29, "A number must be in the brackets");
        Match(25, "The number must be enclosed in brackets");
        Match(20, "A semicolon must be at the end of a declaration");

        //Here a global variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter-6), tokenCounter-3, "G");

    }else if(currentToken.tokenId == 22){

        //Here a global int function is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "G");

        Match(22, "The parameters must be enclosed in parenthesis");

        params();

        //Here a local variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "L");
        Match(23, "The parameters must be enclosed in parenthesis");
        Match(26, "The declarations and statements must be enclosed in curly brackets");

        localDeclarations();
        statementList();

        Match(27, "The declarations and statements must be enclosed in curly brackets");

    }else{
        raiseError(currentToken.tokenId, "A semicolon, open bracket or open parenthesis is missing in declaration");
    }
}

void declarationList(){
    if(currentToken.tokenId == 3){
        //Here a global int function is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "G");
        Match(3, "A type for the declaration is missing");
        Match(28, "An ID is missing after int declaration");

        declarationPrime();
        declarationList();

    }else if(currentToken.tokenId == 5){

        //Here a global void function is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter), tokenCounter + 3, "G");
        Match(5, "A type for the declaration is missing");
        Match(28, "An ID is missing after void declaration");
        Match(22, "The parameters must be enclosed in parenthesis");

        params();
        //Here a local variable is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter - 2), tokenCounter + 1, "L");
        Match(23, "The parameters must be enclosed in parenthesis");
        Match(26, "The declarations and statements must be enclosed in curly brackets");

        localDeclarations();
        statementList();

        Match(27, "The declarations and statements must be enclosed in curly brackets");

        declarationList();

    }else if(currentToken.tokenId == 90){
        //If the last element of the functions used list is empty an error arise
        if(functionsUsed.empty()){
            cout << "\nThe last declaration of the program must be a function void called main in the form void main (void)"<<endl;
            exit(0);
        }//If the last function is not named main an error arises
        else if(functionsUsed.back().name != "main"){
            cout << "\nThe last declaration of the program must be a function void called main in the form void main (void)"<<endl;
            exit(0);
        }//If the last function is named main, but the parameters are not void an error arise
        else if(functionsUsed.back().name == "main" && functionsUsed.back().isVoid == false){
            cout << "\nThe last declaration of the program must be a function void called main in the form void main (void)"<<endl;
            exit(0);
        }
        return;

    }else{
        raiseError(currentToken.tokenId, "The program declarations must start with int or void type specifier");
    }
}

void programPrime(){
    if(currentToken.tokenId  == 5){

        //Here a global void function is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter), tokenCounter + 3, "G");

        Match(5, "If no parameters are needed void must be specified");
        Match(23, "The parameters must be enclosed in parenthesis");
        Match(26, "The declarations and statements must be enclosed in curly brackets");

        localDeclarations();
        statementList();

        Match(27, "The declarations and statements must be enclosed in curly brackets");

        declarationList();

    }else if(currentToken.tokenId  == 3){

        Match(3, "A type for the declaration is missing");
        Match(28, "An ID is missing after int declaration");

        paramsPrime();

        Match(23, "The parameters must be enclosed in parenthesis");
        Match(26, "The declarations and statements must be enclosed in curly brackets");

        localDeclarations();
        statementList();

        Match(27, "The declarations and statements must be enclosed in curly brackets");

        declarationList();

    }else{
        raiseError(currentToken.tokenId, "The program declarations must start with int or void type specifier");
    }

}

void program(){
    if(currentToken.tokenId == 5){
        //Here a global void function is identified, the function to modify the symbol table is called to update it
        fillSymbolTable(getSpecificToken(tokenList, tokenCounter), tokenCounter + 3, "G");

        Match(5, "A type for the declaration is missing");
        Match(28, "An ID is missing after void declaration");
        Match(22, "The parameters must be enclosed in parenthesis");

        programPrime();

    }else if(currentToken.tokenId == 3){
        Match(3, "A type for the declaration is missing");
        Match(28, "An ID is missing after int declaration");

        declarationPrime();
        declarationList();

    }else{
        raiseError(currentToken.tokenId, "The program must start with int or void type specifier");
    }
}

//Method for the main functionality of the algorithm for a recursive descent parser
void mainParser(){
    //It adds the $ symbol to the token list in order to check if that exists at the end and confirm the correctness of the syntax analysis
    addFinalToken();
    //Sets the current token to the first token in the list
    currentToken = getNextToken(tokenList);
    //Its starts the algorithm by calling the initial symbol of the grammar to start the left-most derivation
    program();

    //When the derivation is finished, it is checked if the current token corresponds to the $ symbol,
    //If it does it is successfull, if it is not is an error
    if(currentToken.tokenId == 90){
        cout<<"\nSyntax analysis OK\n"<<endl;

        //Call to print the output in console
        getOutputSymbolTables(idSymbolTableParsed, numSymbolTable);
    }else{
        cout<<"\nSyntax analysis ERROR\n"<<endl;
    }
}

//Main function of the program
int main(){
    //The file is defined as an ifstream to be able to read from
    ifstream sourceFile;
    //Specification of the file used to read the source code
    string inputFile = "sourceCode.txt";

    //Variable to check if the scan failed or not
    bool scanFail = false;

    //Variables for the algorithm in charge of going through the DFA
    int currentState = 0;
    int newState;
    char nextCharacter;
    string token = "";

    //Opening of the file
    sourceFile.open(inputFile);

    //The first character of the file is read and assigned to nextCharacter char
    sourceFile.get(nextCharacter);

    //If the file cant be opened correctly shows a message to the console indicating it
    if(sourceFile.fail()){
        cout << "The file could not be read or it is empty" << endl;
    } else{
        //If the file could be read the algorithm starts entering in a loop
        //based if the end of file is not reached
        while(!sourceFile.eof()){

            //Its sets the currentState as 0 to start form 0 when a token is identified
            currentState = 0;

            //It also resets the string to store the token
            token = "";

            //It loops if the current state is not acceptor and is not an error state
            //and if the end of file is not reached
            while (!acceptingStates[currentState] && !errorStates[currentState]){
                //If it finds a character of a new line adds one to the total line count
                if(nextCharacter == '\n'){
                    codeLines++;
                }
                //To get the new state it sends the current state and the character that follows
                newState = getNewState(currentState, nextCharacter);

                //When it gets the new state it checks if it is 32, which is an error state
                //in order to add the character into the token and be able to check that token
                //and handle the different errors
                if (newState == 32){
                    token.push_back(nextCharacter);
                    sourceFile.get(nextCharacter);
                }

                //Checks if the new state corresponds to a delimiter or composite to push the character
                //to the token and advance to next character
                if(isDelimOrComposite(newState)){
                    token.push_back(nextCharacter);
                    sourceFile.get(nextCharacter);
                }

                //If the relation between the current state and the index of the character
                //is able to advance in the lecture then it continues to the next character
                if(advanceTransitionTable[currentState][characterIndex]){
                    //To avoid the integration of the white spaces into the tokens
                    //the character must not be one of these symbols to be added to the token
                    if(nextCharacter != '\n' && nextCharacter != ' ' && nextCharacter != '\t'){
                        token.push_back(nextCharacter);
                    }

                    //Then the value read goes to the next character in the file
                    sourceFile.get(nextCharacter);

                    //If the file reaches end of file it breaks to go out of block
                    if(sourceFile.eof()){
                        //Checks if the last state was 2 or 3 in which case, it means an error should be presented
                        if(newState == 2 || newState == 3){
                            handleErrors(token);

                            //Set scan fail to true because it encounter an error
                            scanFail = true;
                        }
                        //It is checked if the token does not finishes empty in order to not send a problematic token
                        // to be recorded
                        else if(token != ""){
                            recordToken(token);
                        }

                        break;
                    }
                }
                //The new state obtained is now assigned as the current state
                currentState = newState;
            }
            //If the current state is an acceptor, it gives the instruction to
            //record the token in the token list and if it the case in the corresponding symbol table
            if(acceptingStates[currentState]){
                recordToken(token);

            }//If the current state is not acceptor then, it gives the instruction to handle the corresponding error
            else{
                //if the end of file is reached then the break instructions takes the run out of the
                //while that loops over the algorithm and evaluates in the while that checks if the
                //the end of file is reached
                if(sourceFile.eof()){
                    break;
                }
                handleErrors(token);
                //Set scan fail to true because it encounter an error
                scanFail = true;
            }
        }
        //Closing of the file used
        sourceFile.close();

        //Checks if the scan did not failed in order to present the output or not
        if(!scanFail){
            //Call to print the output in console and in an output file
            //getOutputSymbolTables(idSymbolTable, numSymbolTable);
            //getOutput(tokenList,idSymbolTable, numSymbolTable);
            //Once the scan process is finished and completely correct then the functionality of the parser is called
            mainParser();
        }
    }
    return 0;
}

