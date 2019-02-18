#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

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
		//Store values for export file
		ifstream inputFile;
		inputFile.open(fileName);
		if (inputFile.is_open()) {
			_fileName.append(fileName);
			_isOpen = true;
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
						addToLexerTable("keyword", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (inputToString[lexIt-1] == '$') {
						addToLexerTable("identifier", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isSeparator(inputToString[lexIt])) {
						addToLexerTable("identifier", lexBuilder);
						lexState = 4;
						goto START;
					}
					if (isOperator(inputToString[lexIt])) {
						addToLexerTable("identifier", lexBuilder);
						lexState = 5;
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
						addToLexerTable("numeric", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isSeparator(inputToString[lexIt]) && isFloat == false) {
						addToLexerTable("numeric", lexBuilder);
						lexState = 4;
						goto START;
					}
					if (isOperator(inputToString[lexIt]) && isFloat == false) {
						addToLexerTable("numeric", lexBuilder);
						lexState = 5;
						goto START;
					}
					if (isAlpha(inputToString[lexIt]) && isFloat == true) {
						addToLexerTable("real", lexBuilder);
						lexState = 0;
						goto START;
					}
					if (isSeparator(inputToString[lexIt]) && isFloat == true) {
						addToLexerTable("real", lexBuilder);
						lexState = 4;
						goto START;
					}
					if (isOperator(inputToString[lexIt]) && isFloat == true) {
						addToLexerTable("real", lexBuilder);
						lexState = 5;
						goto START;
					}
				}//End of lexState = 3 while loop
			}//End of isNumeric && lexState ==0


			//Check if reading in a separator; Build record for the separator
			if (isSeparator(inputToString[lexIt]) && lexState == 0 || lexState == 4) {
				lexState = 4;
				while (lexState == 4) {
					lexBuilder += inputToString[lexIt];
					addToLexerTable("separator", lexBuilder);
					lexIt++;
					lexState = 0;
					goto START;
				}//End of lexState == 3 while loop
			}//End of isSeparator && lexState == 0


			//Check if reading in an operator; Build record for the operator
			if (isOperator(inputToString[lexIt]) && lexState == 0 || lexState ==5) {
				lexState = 5;
				while (lexState == 5) {
					lexBuilder += inputToString[lexIt];
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
		cout << setw(30) << left << "Token" << right << "Lexeme" << endl << endl;
		for (int i = 0; i < entryCount; i++) {
			cout << setw(30);
			cout << left << recordTable[i].token << right << recordTable[i].lexeme << endl;
		}
	}


	//Create a txt file of the Lexer Table
	void exportLexerTable() {
		//Create variables for exporting use
		ofstream lexerTable;
		string temp = "lexOf";
		string tempToken;
		string tempLexeme;

		//Set name of output file and create
		temp.append(_fileName);
		lexerTable.open(temp);

		//Begin input to output file
		lexerTable << "Token\t\t\tLexemes\n";
		for (int i = 0; i < entryCount; i++) {
			tempToken = recordTable[i].token;
			if (tempToken.length() < 8) {
				tempToken.append("\t");
			}
			tempLexeme = recordTable[i].lexeme;
			lexerTable << tempToken << "\t=\t" << tempLexeme << endl;
		}
		
	}

private:
	lexRecord recordTable[256];//Table holding lexer records
	int entryCount = 0;//Counter to track current entries
	string _fileName;
	bool _isOpen = false;//Validate input file
};


int main() {


	lexerTable tableOne;
	tableOne.lexer("lex_input.txt");
	tableOne.printLexerTable();
	tableOne.exportLexerTable();


	/*ofstream test;
	string fileName = "waha.txt";
	test.open(fileName);
	test << "WAHAHAHA";*/



	system("pause");
	return 0;
}