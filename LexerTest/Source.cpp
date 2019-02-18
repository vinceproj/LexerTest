#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define FILE_ERROR 2


//Bool functions to test what input is read in
bool isAlpha(char inputChar) {
	if (inputChar == 'A' || inputChar == 'B' || inputChar == 'C' || inputChar == 'D' || inputChar == 'E' || inputChar == 'F' || inputChar == 'G' || inputChar == 'H' ||
		inputChar == 'I' || inputChar == 'J' || inputChar == 'K' || inputChar == 'L' || inputChar == 'M' || inputChar == 'N' || inputChar == 'O' || inputChar == 'P' ||
		inputChar == 'Q' || inputChar == 'R' || inputChar == 'S' || inputChar == 'T' || inputChar == 'U' || inputChar == 'V' || inputChar == 'W' || inputChar == 'X' ||
		inputChar == 'Y' || inputChar == 'Z' || inputChar == 'a' || inputChar == 'b' || inputChar == 'c' || inputChar == 'd' || inputChar == 'e' || inputChar == 'f' ||
		inputChar == 'g' || inputChar == 'h' || inputChar == 'i' || inputChar == 'j' || inputChar == 'k' || inputChar == 'l' || inputChar == 'm' || inputChar == 'n' ||
		inputChar == 'o' || inputChar == 'p' || inputChar == 'q' || inputChar == 'r' || inputChar == 's' || inputChar == 't' || inputChar == 'u' || inputChar == 'v' ||
		inputChar == 'w' || inputChar == 'x' || inputChar == 'y' || inputChar == 'z' || inputChar == '$') {
		return true;
	}
	else
		return false;
	}

bool isComment(char inputChar) {
	if (inputChar == '!') {
		return true;
	}
	else
		return false;
}

bool isNumeric(char inputChar) {
	if (inputChar == '0' || inputChar == '1' || inputChar == '2' || inputChar == '3' || inputChar == '4' || inputChar == '5' || inputChar == '6' ||
		inputChar == '7' || inputChar == '8' || inputChar == '9') {
		return true;
	}
	else
		return false;
}

bool isSeparator(char inputChar) {
	if (inputChar == '(' || inputChar == ')' || inputChar == ',' || inputChar == ';' || inputChar == '{' || inputChar == '}'){
		return true;
	}
	else
		return false;
}

bool isOperator(char inputChar) {
	if (inputChar == '+' || inputChar == '-' || inputChar == '*' || inputChar == '/' || inputChar == '%' || inputChar == '<' || inputChar == '>' || inputChar == '=') {
		return true;
	}
	else
		return false;
}

bool isKeyword(string inputString) {
	if (inputString.compare("int")==0 || inputString.compare("if")== 0 || inputString.compare("else")==0 || inputString.compare("while") == 0 || inputString.compare("whileend") == 0 ||
		inputString.compare("float") == 0|| inputString.compare("double") == 0 || inputString.compare("real") == 0) {
		return true;
	}
	else
		return false;
}


//Struct defining a lexRecord
struct lexRecord {
	string token;
	string lexeme;
};


//Class to create a list of lexRecords
class lexerTable {
public:

	/*
	State 0 = Building new record
	State 1 = Reading through comment
	State 2 = Reading an alpha
	State 3 = Reading numeric
	State 4 = Encountered separator
	State 5 = Encountered operator

	TODO: Remove cout testing statements; Create export function of recordTable
	*/

	//Use lexer func with fileName to generate a list of lexRecords
	void lexer(string fileName) {
		//Open and validates input file
		ifstream inputFile;
		inputFile.open(fileName);
		if (inputFile.is_open()) {
			_fileName = fileName;
		}

		//Check if inputFile is valid
		if (!inputFile.is_open()) {
			cerr << "Could not open file!" << endl;
			system("pause");
			exit(FILE_ERROR);
		}

		//Create variables for tokenizing
		int lexState = 0;
		int lexIterator = 0;
		int lexArraySize = 0;
		string inputToString;
		string lexBuilder;
		

		//Read through file; Remove all whitespace; Place all char into a char array
		while (inputFile.peek() != EOF) {
			string token;
			inputFile >> token;
			inputToString.append(token);
			
		}

		//cout << inputToString << endl;//Use to see full input
		

		lexArraySize = inputToString.length();
		for (int lexIt = 0; lexIt < lexArraySize; lexIt++) {
			//Begin if/else series to tokenize
			//Use label to return to State 0 from area lower in loop without accruing an extra lexIt++
			//Reset token building tools
		START:
			lexState = 0;
			lexBuilder.clear();


			//Check if reading in a comment; Iterate through comment portion; Do not add to record
			if (isComment(inputToString[lexIt]) && lexState != 1) {
				lexState = 1;
				while (lexState == 1) {
					lexIt++;
					if (inputToString[lexIt] == '!') {
						lexIt++;
						lexState = 0;
					}
				}
			}//isComment loop
			

			//Check if reading in an alpha; Build record for either identifier or keyword
			//Uses bool checks to know when to finish record
			if (isAlpha(inputToString[lexIt]) && lexState == 0) {
				lexState = 2;
				while (lexState == 2) {
					lexBuilder+=inputToString[lexIt];
					lexIt++;
					if (isKeyword(lexBuilder)) {
						//cout << "|full word is " << lexBuilder << endl;
						addToLexerTable("keyword", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (inputToString[lexIt-1] == '$') {
						//cout << "|full word is " << lexBuilder << endl;
						addToLexerTable("identifier", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isSeparator(inputToString[lexIt])) {
						//cout << "|full word is " << lexBuilder << endl;
						addToLexerTable("identifier", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isOperator(inputToString[lexIt])) {
						//cout << "|full word is " << lexBuilder << endl;
						addToLexerTable("identifier", lexBuilder);
						lexState = 0;
						goto START;
					}
				}//End of lexState == 2 while loop
			}//End of isAlpha && lexState == 0 


			//Check if reading in a numeric; Build record for either a numeric or float
			//Use bool checks to know when to finish a record
			if (isNumeric(inputToString[lexIt]) && lexState == 0) {
				lexState = 3;
				bool isFloat = false;
				while (lexState == 3) {
					lexBuilder += inputToString[lexIt];
					if (inputToString[lexIt] == '.') {
						isFloat = true;
					}
					lexIt++;
					if (isAlpha(inputToString[lexIt]) && isFloat == false) {
						//cout << "|full numeric is " << lexBuilder << endl;
						addToLexerTable("numeric", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isSeparator(inputToString[lexIt]) && isFloat == false) {
						//cout << "|full numeric is " << lexBuilder << endl;
						addToLexerTable("numeric", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isOperator(inputToString[lexIt]) && isFloat == false) {
						//cout << "|full numeric is " << lexBuilder << endl;
						addToLexerTable("numeric", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isAlpha(inputToString[lexIt]) && isFloat == true) {
						//cout << "|full real is " << lexBuilder << endl;
						addToLexerTable("real", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isSeparator(inputToString[lexIt]) && isFloat == true) {
						//cout << "|full real is " << lexBuilder << endl;
						addToLexerTable("real", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isOperator(inputToString[lexIt]) && isFloat == true) {
						//cout << "|full real is " << lexBuilder << endl;
						addToLexerTable("real", lexBuilder);
						lexState = 0;
						goto START;
					}
				}//End of lexState = 3 while loop
			}//End of isNumeric && lexState ==0


			//Check if reading in a separator; Build record for the separator
			if (isSeparator(inputToString[lexIt]) && lexState == 0) {
				lexState = 4;
				while (lexState == 4) {
					lexBuilder += inputToString[lexIt];
					//cout << "|separator is " << lexBuilder << endl;
					addToLexerTable("separator", lexBuilder);
					lexIt++;
					lexState = 0;
					goto START;
				}//End of lexState == 3 while loop
			}//End of isSeparator && lexState == 0


			//Check if reading in an operator; Build record for the operator
			if (isOperator(inputToString[lexIt]) && lexState == 0) {
				lexState = 5;
				while (lexState == 5) {
					lexBuilder += inputToString[lexIt];
					//cout << "|operator is " << lexBuilder << endl;
					addToLexerTable("operator", lexBuilder);
					lexIt++;
					lexState = 0;
					goto START;
				}//End of lexState == 5 while loop
			}//End of isOperator && lexState == 5


		}//lexIt loop
		

	}//End of lexer function


	//Add an entry to the recordTable using a string lexeme
	void addToLexerTable(string token, string lexeme) {
		lexRecord temp;
		temp.token = token;
		temp.lexeme = lexeme;
		recordTable[entryCount] = temp;
		entryCount++;
	}

	//Add an entry to the recordTable using a char lexeme
	void addToLexerTable(string token, char lexeme) {
		lexRecord temp;
		temp.token = token;
		temp.lexeme = lexeme;
		recordTable[entryCount] = temp;
		entryCount++;
	}

	//Output current entries in the recordTable
	void printLexerTable() {
		for (int i = 0; i < entryCount; i++) {
			cout << recordTable[i].token + " " + recordTable[i].lexeme << endl;
		}
	}


private:
	lexRecord recordTable[256];//Table holding lexer records
	int entryCount = 0;//Counter to track current entries
	string _fileName;//Hold name of file input
};


int main() {


	lexerTable tableOne;
	tableOne.lexer("lex_input.txt");
	cout << endl << endl << "Lexer table" << endl << endl;
	tableOne.printLexerTable();



	system("pause");
	return 0;
}