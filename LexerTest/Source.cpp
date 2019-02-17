#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define FILE_ERROR 2

bool isAlpha(char inputChar) {
	if (inputChar == 'A' || 'B' || 'C' || 'D' || 'E' || 'F' || 'G' || 'H' || 'I' || 'J' || 'K' || 'L' || 'M' || 'N' || 'O' || 'P' || 'Q' || 'R' || 'S' || 'T' || 'U' || 'V' || 'W' ||
		'X' || 'Y' || 'Z' || 'a' || 'b' || 'c' || 'd' || 'e' || 'f' || 'g' || 'h' || 'i' || 'j' || 'k' || 'l' || 'm' || 'n' || 'o' || 'p' || 'q' || 'r' || 's' || 't' || 'u' || 'v' || 'w' ||
		'x' || 'y' || 'z' || '$') {
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
	if (inputChar == '0' || '1' || '2' || '3' || '4' || '5' || '6' || '7' || '8' || '9') {
		return true;
	}
	else
		return false;
}

bool isSeparator(char inputChar) {
	if (inputChar == '(' || ')' || ',' || ';' || '{' || '}') {
		return true;
	}
	else
		return false;
}

bool isOperator(char inputChar) {
	if (inputChar == '+' || '-' || '*' || '/' || '%' || '<' || '>') {
		return true;
	}
	else
		return false;
}

bool isKeyword(string inputString) {
	if (inputString == "int" || "if" || "else" || "while" || "whileend") {
		return true;
	}
	else
		return false;
}


struct lexRecord {
	string token;
	string lexeme;
};




class lexerTable {
public:

	/*
	State 0 = Building new record
	State 1 = Reading through comment
	State 2 = Reading an alpha for identifier
	State 3 = Reading numeric for use
	State 4 = Encountered operator
	State 5 = Encountered separator
	State 6 = Encountered keyword
	State 7 = Encountered identifier
	*/

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
		cout << inputToString;

		lexArraySize = inputToString.length();
		for (int lexIt = 0; lexIt < lexArraySize; lexIt++) {
			//Begin if/else series to tokenize
			
			//Check if reading in a comment; Iterate through comment portion; Do not add to record
			if (isComment(inputToString[lexIt]) && lexState != 1) {
				lexState = 1;
				cout << "yeet1 ";//Found first !
				while (lexState == 1) {
					lexIt++;
					if (inputToString[lexIt] == '!') {
						lexIt++;
						lexState = 0;
						cout << "yeet2 ";//Found second !
						break;
					}
				}
			}//isComment loop

			//if (isAlpha(inputToString[lexIt]) && lexState == 0) {//New token + isAlpha
			//	lexState = 2;

			//	while (lexState == 2){
			//		lexBuilder+=inputToString[lexIt];
			//		cout << lexBuilder << " ";
			//		if (isKeyword(lexBuilder)) {
			//			cout << lexBuilder << " " << lexIt << "+yeet6";
			//			break;
			//		}
			//		lexIt++;
			//		//lexState = 0;
			//		//lexBuilder.clear();
			//		//break;
			//	}
			//}


			//cout << lexIt << " ";
		}//lexIt loop
		
	}//End of lexer function


	//Add an entry to the recordTable
	void addToLexerTable(string token, string lexeme) {
		lexRecord temp;
		temp.token = token;
		temp.lexeme = lexeme;
		recordTable[entryCount] = temp;
		entryCount++;
	}

	//Output current entries in the recordTable
	void printLexerTable() {
		for (int i = 0; i < entryCount; i++) {
			cout << recordTable[i].token + " " + recordTable[i].lexeme;
		}
	}


private:
	lexRecord recordTable[256];//Table holding lexer records
	int entryCount = 0;//Counter to track current entries
	string _fileName;
};


int main() {


	lexerTable tableOne;
	tableOne.lexer("lex_input.txt");
	tableOne.printLexerTable();

	cout << endl << endl << isKeyword("yeet");


	//While loop takes precedence over For; while loops itself until broken
	/*int state = 0;
	for (int i = 0; i < 10; i++) {
		while (state == 0) {
			cout << i;
			i+=2;
			if (i > 5) {
				state = 1;
			}
		}
		cout << i;
	}*/

	////open file 
	//ifstream inputFile;

	////check if file opened properlu
	//inputFile.open("lex_input.txt");
	//if (!inputFile.is_open()) {
	//	cerr << "Could not open file!" << endl;
	//	system("pause");
	//	exit(FILE_ERROR);
	//}

	////string to hold all inputs
	////read all chars w/o whitespace into string var
	////print string var
	//string test;
	//while (inputFile.peek() != EOF) {
	//	string token;
	//	inputFile >> token;
	//	cout << token <<" ";
	//	test.append(token + " ");
	//}
	//
	//cout << endl << endl << "new shit" << endl << endl;

	//lexerTable tableOne;
	//lexRecord testOne;
	//testOne.token = "poop";
	//testOne.lexeme = "nugget";
	//tableOne.addToLexerTable(testOne);
	//tableOne.printLexerTable();
	//
	//tableOne.lexer("lex_input.txt");
	//cout << endl << endl << "more shit" << endl;


	system("pause");
	return 0;
}