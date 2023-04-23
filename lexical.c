#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
FILE *fp;
FILE *fp_out;
char *myArray[10000]; int myArray_index = 0;

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

char commentLine[1000]; char a[1000]; char b[1000];
void isComment(char* str) {
	bool trueComment = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '{') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') {
					substring(str, i + 1, j - i - 1, commentLine);
					//printf("Comment Line: '%s'\n", commentLine);
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

void lexicalAnalyzer(char *text){
	int openedParanthesesCounter = 0;
	bool doubleQutoesIsOpened = false;
	bool invalidCharacter = false;
	bool invalidInput = false;
	char *temp = "";

	for (int i = 0; i < strlen(text); i++) {
            if (!(text[i] >= 48 && text[i] <= 57 || text[i] >= 65 && text[i] <= 90 || text[i] >= 97 && text[i] <= 122 || //a-z A-Z 0-9
                  text[i] == 91 || text[i] == 93 ||// [ ]
                  text[i] == 32 ||// space
                  text[i] == 95 ||// _
                  text[i] == 45 ||// -
                  text[i] == 34 ||// "
                  text[i] == 44 ||// ,
                  text[i] == 46 ||// .
                  text[i] == 13 ||// \n
                  text[i] == 10// \n
                  ) && !doubleQutoesIsOpened) {
                printf("invalid character:*%c*\n", text[i]);
                fprintf(fp_out,"invalid character:*%c*\n", text[i]);
                invalidCharacter = true;
            }
            else if (text[i] == ' ' && !doubleQutoesIsOpened) {
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

        for(int i = 0; i < myArray_index; i++) {

			if (isKeyword(myArray[i])) {
				printf("Keyword %s\n", myArray[i]);
				fprintf(fp_out,"Keyword %s\n", myArray[i]);
			}
			else if (isInteger(myArray[i])) {
                printf("IntConstant %s\n", myArray[i]);
				fprintf(fp_out,"IntConstant %s\n", myArray[i]);
			}
			else if(isVariable(myArray[i])) {
                printf("Identifier %s\n", myArray[i]);
				fprintf(fp_out,"Identifier %s\n", myArray[i]);
			}

			else if (!strcmp(myArray[i],"[")) {
                printf("OpenBlock\n");
                fprintf(fp_out,"OpenBlock\n");
			}
			else if (!strcmp(myArray[i],"]")) {
                printf("CloseBlock\n");
                fprintf(fp_out,"CloseBlock\n");
			}
			else if (!strcmp(myArray[i],",")) {
                printf("Seperator\n");
				fprintf(fp_out,"Seperator\n");
			}
			else if (!strcmp(myArray[i],".")) {
                printf("EndOfLine\n");
				fprintf(fp_out,"EndOfLine\n");
			}
			else if (!strcmp(myArray[i],"\n")) {
                continue;
			}
			else if (isStringConstant(myArray[i])) {
                printf("StringConstant %s\n", myArray[i]);
				fprintf(fp_out,"StringConstant %s\n", myArray[i]);
			}
			else {
                printf("Invalid input %s\n", myArray[i]);
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
            exit(0);
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
	if (!fp) perror(inputFileName), exit(0);

    fp_out = fopen(outputFileName,"w");
    if (!fp_out) perror(outputFileName), exit(0);

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	code = calloc(1, lSize + 1);
	if (!code) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

	if (1 != fread(code, lSize, 1, fp)) fclose(fp), free(code), fputs("entire read fails", stderr), exit(1);

	isComment(code);

	lexicalAnalyzer(code);

}
