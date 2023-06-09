#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>
FILE *fp;
FILE *fp_out;
char *myArray[10000]; int myArray_index = 0;
char *variableList[1000]; int vIndex = 0;
int integerList[1000]; int iIndex = 0;


bool isKeyword(char* str) {
	if (!strcmp(str, "int") || !strcmp(str, "move") || !strcmp(str, "to") || !strcmp(str, "add") ||
		!strcmp(str, "sub") || !strcmp(str, "from") || !strcmp(str, "loop") || !strcmp(str, "times") ||
		!strcmp(str, "out") || !strcmp(str, "newline")) return true;
	return false;
}

bool isInteger(char* str) {
	int len = strlen(str);
	if (len > 100) return false;
	for (int i = 0; i < len; i++) {
		if (i > 0) {
			if (str[i] == '-' || len == 1)return false;
		}
		if (!(isdigit(str[i]) || str[i] == '-')) return false;
	}return true;
}

bool isVariable(char* str){
	int len = strlen(str);
	if (len > 20)return false;
	for (int i = 0; i < len; i++) {
		if (str[0] == '_') return false;
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_'))) return false;
	}return true;
}

bool isIntegerVariable(char* str) {
	for (int i = 0; i < vIndex; i++) {
		if (!strcmp(str, variableList[i])) {
			return true;
		}
	}
	return false;
}
char commentLine[1000]; char a[1000]; char b[1000];
void isComment(char* str) {
	bool trueComment = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '{') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') {
					substring(str, i + 1, j - i - 1, commentLine);
					//printf("Comment Line: '%s'\n", commentLine); Opsiyonel olarak yorum satırı çıktısı verir.
					substring(str, 0, i, a);
					substring(str, j + 1, strlen(str), b);
					strcat(a, b);
					strcpy(str, a);
					trueComment = true;
					isComment(str);
				}
			}
			if (!trueComment) {
				printf("Comment Line is not completed.\n");
				fprintf(fp_out,"Comment Line is not completed.\n");
				exit(0);
			}
		}
	}
}

int substring(char *source, int from, int n, char *target) {
	int length, i;
	for (length = 0; source[length] != '\0'; length++);
	if (from > length) {
		printf("Starting index is invalid.\n");
		fprintf(fp_out,"Starting index is invalid.\n");
		return 1;
	}
	if ((from + n) > length) {
		n = (length - from);
	}
	for (i = 0; i < n; i++) {
		target[i] = source[from + i];
	}
	target[i] = '\0';
	return 0;
}

bool isStringConstant(char *word){
	int c;
	if(word[0] == '\"'){
		c = 1;
		while(word[c]){
			if(word[c] == '\"'){
				if(word[c+1] == '\0') return true;
				else return false;
			}c++;
		}return false;
	}return false;
}

void exitProgram() {
    getch();
    exit(0);
}

void lexicalAnalyzer(char *text){
	int openedParanthesesCounter = 0;
	bool doubleQutoesIsOpened = false;
	bool invalidCharacter = false;
	bool invalidInput = false;
	char *temp =  malloc(1); strcpy(temp, "");
	for (int i = 0; i < strlen(text); i++) {
            if (text[i] == 13) continue;
            else if (!(text[i] >= 48 && text[i] <= 57 || text[i] >= 65 && text[i] <= 90 || text[i] >= 97 && text[i] <= 122 || //a-z A-Z 0-9
                  text[i] == '[' || text[i] == ']' ||
                  text[i] == ' ' ||
                  text[i] == '_' ||
                  text[i] == '-' ||
                  text[i] == '\"' ||
                  text[i] == ',' ||
                  text[i] == '.' ||
                  text[i] == '\n' ||
                  text[i] == '\t'
                  ) && !doubleQutoesIsOpened) {
                //printf("invalid character:*%c*\n", text[i]);
                fprintf(fp_out,"invalid character:*%c*\n", text[i]);
                invalidCharacter = true;
            }
            else if ((text[i] == ' ' || text[i] == '\t') && !doubleQutoesIsOpened) {
                if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
                }
				strcpy(temp,"");
            }
            else if (text[i] == '\n' && !doubleQutoesIsOpened) {
                if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
                }
                myArray[myArray_index] = malloc(sizeof(char)+1);
                strcpy(myArray[myArray_index], "\n");
                myArray_index++;
                strcpy(temp,"");
            }
            else if (text[i] == '.' && !doubleQutoesIsOpened) {
                if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
                }
                myArray[myArray_index] = malloc(sizeof(char)+1);
                strcpy(myArray[myArray_index], ".");
                myArray_index++;
                strcpy(temp,"");
            }
            else if (text[i] == ',' && !doubleQutoesIsOpened) {
                if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
                }
                myArray[myArray_index] = malloc(sizeof(char)+1);
                strcpy(myArray[myArray_index], ",");
                myArray_index++;
                strcpy(temp,"");
            }
            else if (text[i] == '[' && !doubleQutoesIsOpened) {
                if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
                }
                myArray[myArray_index] = malloc(sizeof(char)+1);
                strcpy(myArray[myArray_index], "[");
                myArray_index++;
                openedParanthesesCounter += 1;
                strcpy(temp,"");
            }
            else if (text[i] == ']' && !doubleQutoesIsOpened) {
                if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
                }
                myArray[myArray_index] = malloc(sizeof(char)+1);
                strcpy(myArray[myArray_index], "]");
                myArray_index++;
                openedParanthesesCounter -= 1;
                strcpy(temp,"");
            }
            else if (text[i] == '"') {
                if (doubleQutoesIsOpened == false) {
                    doubleQutoesIsOpened = true;
                    if (strcmp(temp,"")){
                        myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                        strcpy(myArray[myArray_index], temp);
                        myArray_index++;
                    }
                    strcpy(temp,"");
					size_t len = strlen(temp);
				    char *str2 = malloc(len + 1 + 1);
				    strcpy(str2, temp);
				    str2[len] = '"';
				    str2[len + 1] = '\0';
                    temp = str2;
                }
                else {
                    doubleQutoesIsOpened = false;

                    size_t len = strlen(temp);
				    char *str2 = malloc(len + 1 + 1);
				    strcpy(str2, temp);
				    str2[len] = '"';
				    str2[len + 1] = '\0';
                    temp = str2;
                    if (strcmp(temp,"")){
                        myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                        strcpy(myArray[myArray_index], temp);
                        myArray_index++;
                    }
                    strcpy(temp,"");
                }
            }
            else {
			    size_t len = strlen(temp);
			    char *str2 = malloc(len + 1 + 1);
			    strcpy(str2, temp);
			    str2[len] = text[i];
			    str2[len + 1] = '\0';
				temp = str2;
            }
        }
        if (strcmp(temp,"")){
                    myArray[myArray_index] = malloc(sizeof(strlen(temp))+1);
                    strcpy(myArray[myArray_index], temp);
                    myArray_index++;
        }
        for(int i = 0; i < myArray_index; i++) {
			if (isKeyword(myArray[i])) {
				//printf("Keyword %s\n", myArray[i]);
				fprintf(fp_out,"Keyword %s\n", myArray[i]);
			}
			else if (isInteger(myArray[i])) {
                //printf("IntConstant %s\n", myArray[i]);
				fprintf(fp_out,"IntConstant %s\n", myArray[i]);
			}
			else if(isVariable(myArray[i])) {
               //printf("Identifier %s\n", myArray[i]);
				fprintf(fp_out,"Identifier %s\n", myArray[i]);
			}

			else if (!strcmp(myArray[i],"[")) {
                //printf("OpenBlock\n");
                fprintf(fp_out,"OpenBlock\n");
			}
			else if (!strcmp(myArray[i],"]")) {
                //printf("CloseBlock\n");
                fprintf(fp_out,"CloseBlock\n");
			}
			else if (!strcmp(myArray[i],",")) {
                //printf("Seperator\n");
				fprintf(fp_out,"Seperator\n");
			}
			else if (!strcmp(myArray[i],".")) {
                //printf("EndOfLine\n");
				fprintf(fp_out,"EndOfLine\n");
			}
			else if (!strcmp(myArray[i], "\n")) {
                continue;
			}
			else if (isStringConstant(myArray[i])) {
                //printf("StringConstant %s\n", myArray[i]);
				fprintf(fp_out,"StringConstant %s\n", myArray[i]);
			}
			else {
                //printf("Invalid input %s\n", myArray[i]);
				fprintf(fp_out,"Invalid input %s\n", myArray[i]);
				invalidInput = true;
			}
        }
        if (invalidCharacter) {
            printf("-->Invalid character error.\n");
            fprintf(fp_out,"-->Invalid character error.\n");
        }
        if (openedParanthesesCounter != 0) {
            printf("-->Parantheses error.\n");
            fprintf(fp_out,"-->Parantheses error.\n");
        }
        if (doubleQutoesIsOpened){
            printf("-->Quotes error.\n");
            fprintf(fp_out,"-->Quotes error.\n");
        }
        if (invalidInput) {
            printf("-->Invalid input error.\n");
            fprintf(fp_out,"-->Invalid input error.\n");
        }
        if (openedParanthesesCounter != 0 || doubleQutoesIsOpened || invalidCharacter || invalidInput) {
            printf("-->Lexical analysis failed.\n");
            fprintf(fp_out,"-->Lexical analysis failed.\n");
            exitProgram();
        }
}
void parse(int token, int line_index, int *line) {
    for(;token < line_index;) {
        if (myArray[++token] == NULL) break;
        else if(isKeyword(myArray[token])) {
            //Int
            if (!strcmp(myArray[token], "int")) {
                //Variable
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Variable is expected.\n", *line);
                    exitProgram();
                }
                else if (isKeyword(myArray[token])) {
                    printf("\nLine: %d | '%s' is a keyword. Variable are expected.\n", *line, myArray[token]);
                    exitProgram();
                }
                else if (isVariable(myArray[token])) {
                        if (isIntegerVariable(myArray[token])) {
                            printf("\nLine: %d | '%s' is already defined.\n", *line, myArray[token]);
                            exitProgram();
                        }
                        else {
                            variableList[vIndex++] = myArray[token];
                            integerList[iIndex++] = 0;
                        }
                }
                else {
                    printf("\nLine: %d | '%s' is not a variable. Variable is expected.\n", *line, myArray[token]);
                    exitProgram();
                }
                //End Of Line
                if (myArray[++token] == NULL || strcmp(myArray[token], ".")) {
                    printf("\nLine: %d | EndOfLine is expected.\n", *line);
                    exitProgram();
                }
            }
            //Move
            else if (!strcmp(myArray[token], "move")) {
                //Integer Or Variable
                int moveInt = 0;
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Integer or variable is expected.\n", *line);
                    exitProgram();
                }
                else if (isInteger(myArray[token])) {
                    moveInt = atoi(myArray[token]);
                }
                else if (isIntegerVariable(myArray[token])) {
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            moveInt = integerList[i];
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine %d | '%s' is not a integer or variable. Integer or variable are expected.\n",*line, myArray[token]);
                    exitProgram();
                }
                //To
                if (myArray[++token] == NULL || strcmp(myArray[token], "to")) {
                    printf("\nLine: %d | Keyword 'to' is expected.\n", *line);
                    exitProgram();
                }
                //Variable
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Variable is expected.\n", *line);
                    exitProgram();
                }
                else if (isIntegerVariable(myArray[token])) {
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            integerList[i] = moveInt;
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine %d | '%s' is not a variable. Variable is expected.\n",*line,  myArray[token]);
                    exitProgram();
                }

                //End Of Line
                if (myArray[++token] == NULL || strcmp(myArray[token], ".")) {
                    printf("\nLine: %d | EndOfLine is expected.\n", *line);
                    exitProgram();
                }
            }
            //Add
            else if (!strcmp(myArray[token], "add")) {
                //Integer Or Variable
                int addInt = 0;
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Integer or variable are expected.\n", *line);
                    exitProgram();
                }
                else if (isInteger(myArray[token])) {
                    addInt = atoi(myArray[token]);
                }
                else if (isIntegerVariable(myArray[token])) {
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            addInt = integerList[i];
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",*line, myArray[token]);
                    exitProgram();
                }
                //To
                if (myArray[++token] == NULL || strcmp(myArray[token], "to")) {
                    printf("\nLine: %d | Keyword 'to' is expected.\n", *line);
                    exitProgram();
                }
                //Variable
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Variable is expected.\n", *line);
                    exitProgram();
                }
                else if (isIntegerVariable(myArray[token])) {
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            integerList[i] += addInt;
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine: %d | '%s' is not a variable. Variable is expected.\n", *line, myArray[token]);
                    exitProgram();
                }
                //End Of Line
                if (myArray[++token] == NULL || strcmp(myArray[token], ".")) {
                    printf("\nLine: %d | EndOfLine is expected.\n", *line);
                    exitProgram();
                }
            }
            //Sub
            else if (!strcmp(myArray[token], "sub")) {
                //Integer Or Variable
                int subInt = 0;
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Integer or variable are expected.\n", *line);
                    exitProgram();
                }
                else if (isInteger(myArray[token])) {
                    subInt = atoi(myArray[token]);
                }
                else if (isIntegerVariable(myArray[token])) {
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            subInt = integerList[i];
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine: %d | '%s' is not a integer or variable. Integer or variable are expected.\n", *line, myArray[token]);
                    exitProgram();
                }

                //From
                if (myArray[++token] == NULL || strcmp(myArray[token], "from")) {
                    printf("\nLine: %d | Keyword 'to' is expected.\n", *line);
                    exitProgram();
                }
                //Variable
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Variable is expected.\n", *line);
                    exitProgram();
                }
                else if (isIntegerVariable(myArray[token])) {
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            integerList[i] -= subInt;
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine: %d | '%s' is not a variable. Variable is expected.\n", myArray[token]);
                    exitProgram();
                }
                //End Of Line
                if (myArray[++token] == NULL || strcmp(myArray[token], ".")) {
                    printf("\nLine: %d | EndOfLine is expected.\n", *line);
                    exitProgram();
                }
            }
            //Out
            else if (!strcmp(myArray[token], "out")) {
                while(true) {
                    if (myArray[++token] == NULL) {
                        printf("\nLine: %d | Integer, variable or string are expected.\n", *line);
                        exitProgram();
                    }
                    else if (!strcmp(myArray[token], "newline")) {
                        printf("\n");
                    }
                    else if (isIntegerVariable(myArray[token])) {
                        for (int i = 0; i < vIndex; i++) {
                            if (!strcmp(variableList[i], myArray[token])) {
                                printf("%d",integerList[i]);
                                break;
                            }
                        }
                    }
                    else if (isInteger(myArray[token])) {
                        printf("%s",myArray[token]);
                    }
                    else if (isStringConstant(myArray[token])) {
                        char temp[200];
                        strcpy(temp, myArray[token]);
                        for(int i = 0; i<strlen(temp); i++){
                            if(temp[i] != '"'){
                               printf("%c", temp[i]);
                            }
                        }
                    }
                    else{
                        printf("\nLine: %d | Integer, variable or string are expected.\n", *line, myArray[token]);
                        exitProgram();
                    }
                    if (myArray[++token] == NULL || !strcmp(myArray[token], "\n")) {
                        printf("\nLine: %d | EndOfLine or Seperator is expected.\n", *line);
                        exitProgram();
                    }
                    else if (!strcmp(myArray[token], ".")) break;
                    else if (!strcmp(myArray[token], ",")) continue;
                    else {
                        printf("\nLine: %d | EndOfLine or Seperator is expected.\n", *line);
                        exitProgram();
                    }
                }
            }
            //Loop
            else if (!strcmp(myArray[token], "loop")) {
                //Integer Or Variable
                int loopInt = 0;
                int loopVariableToken = -1;
                if (myArray[++token] == NULL) {
                    printf("\nLine: %d | Integer, variable or string are expected.\n", *line);
                    exitProgram();
                    }
                else if (isInteger(myArray[token])) {
                    loopInt = atoi(myArray[token]);
                }
                else if (isIntegerVariable(myArray[token])) {
                    loopVariableToken = token;
                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], myArray[token])) {
                            loopInt = integerList[i];
                            break;
                        }
                    }
                }
                else {
                    printf("\nLine: %d | '%s' is not a integer or variable. Integer or variable are expected.\n", *line, myArray[token]);
                    exitProgram();
                }
                //Times
                if (myArray[++token] == NULL || strcmp(myArray[token], "times")) {
                    printf("\nLine: %d | Keyword 'times' is expected.\n", *line);
                    exitProgram();
                }
                int startIndex;
                int endIndex;
                //Single line
                if (isKeyword(myArray[token + 1])) {
                    startIndex = token;
                    while(true) {
                        if (myArray[++token] == NULL) {
                        printf("\nLine: %d | EndOfLine is expected.\n", *line);
                        exitProgram();
                        }
                        else if(!strcmp(myArray[token], "\n")) {
                            printf("\nLine: %d | EndOfLine is expected.\n", *line);
                            exitProgram();
                        }
                        else if(!strcmp(myArray[token], ".")) {
                            endIndex = token - 1;
                            break;
                        }
                    }
                }
                //Code block
                else {
                    while(true) {
                        if (myArray[++token] == NULL) {
                            printf("\nLine: %d | '[' is expected.\n", *line);
                            exitProgram();
                        }
                        else if(!strcmp(myArray[token], "\n")) {
                            *line += 1;
                        }
                        else if(!strcmp(myArray[token], "[")) {
                            startIndex = token;
                            break;
                        }
                        else {
                            printf("\nLine: %d | '[' is expected.\n", *line);
                            exitProgram();
                        }
                    }
                    int openedBracket = 1;
                    while(true) {
                        if(openedBracket == 0) {
                            endIndex = token - 1;
                            break;
                        }
                        if (myArray[++token] == NULL) {
                            printf("\nLine: %d | ']' is expected.\n", *line);
                            exitProgram();
                        }
                        else if(!strcmp(myArray[token], "[")) {
                            openedBracket++;
                        }
                        else if(!strcmp(myArray[token], "]")) {
                            openedBracket--;
                        }
                    }
                }
                int copyLine = *line;
                int *ptrCopyLine = &copyLine;
                for (;loopInt > 0; loopInt--) {
                    copyLine = *line;
                    parse(startIndex, endIndex, ptrCopyLine);
                    if(loopVariableToken != -1) {
                        for (int i = 0; i < vIndex; i++) {
                            if (!strcmp(variableList[i], myArray[loopVariableToken])) {
                                integerList[i]--;
                                break;
                            }
                        }
                    }
                }
                *line = copyLine;
            }
            else {
                printf("\nLine: %d | '%s' is wrong keyword. 'int', 'move', 'loop', 'add', 'sub' or 'out' are expected.\n",*line, myArray[token]);
                exitProgram();
            }
        }
        else if (!strcmp(myArray[token],"\n")) {
            *line += 1;
        }
        else {
            printf("\nLine: %d | keyword is expected. '%s' is not a keyword\n", *line, myArray[token]);
            exitProgram();
        }
    }
}
int main() {
	long lSize;
	char *code;

    char* inputFileName;
    char* outputFileName;

    inputFileName = (char *)malloc(sizeof(char)*50);
    outputFileName = (char *)malloc(sizeof(char)*50);

    printf("Enter input file name: ");
    scanf("%s",inputFileName);
    strcpy(outputFileName, inputFileName);
    strcat(inputFileName, ".ba");
    strcat(outputFileName, ".lx");

	fp = fopen(inputFileName, "rb");
	if (!fp) perror(inputFileName), exitProgram();

    fp_out = fopen(outputFileName,"w");
    if (!fp_out) perror(outputFileName), exitProgram();

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	code = calloc(1, lSize + 1);
	if (!code) fclose(fp), fputs("Memory alloc fails", stderr), exit(1);

	if (1 != fread(code, lSize, 1, fp)) fclose(fp), free(code), fputs("Entire read fails", stderr), exit(1);

	isComment(code);

	lexicalAnalyzer(code);

    int Line = 1;
    int *ptrLine = &Line;
	parse(-1, myArray_index, ptrLine);

    getch();
    return 1;
}
