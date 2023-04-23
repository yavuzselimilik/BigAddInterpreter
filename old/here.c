#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
FILE *fp_out;
char* Loops[1000];
char *variableList[1000]; int vIndex = 0;
int integerList[1000] = { 0 }; int iIndex = 0;

bool isKeyword(char* str) {
	if (!strcmp(str, "int") || !strcmp(str, "move") || !strcmp(str, "to") || !strcmp(str, "add") ||
		!strcmp(str, "sub") || !strcmp(str, "from") || !strcmp(str, "loop") || !strcmp(str, "times") ||
		!strcmp(str, "out") || !strcmp(str, "newline")) {
		return true;
	}
	return false;
}

bool isInteger(char* str) {
	int len = strlen(str);
	if (len > 100) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (i > 0) {
			if (str[i] == '-' || len == 1) {
				return false;
			}
		}
		if (!(isdigit(str[i]) || str[i] == '-')) {
			return false;
		}
	}
	return true;
}

bool isIntegerVariable(char* str) {
	for (int i = 0; i < vIndex; i++) {
		if (!strcmp(str, variableList[i])) {
			return true;
		}
	}
	return false;
}

bool isVariable(char* str){
	int len = strlen(str);
	if (len > 20) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (str[0] == '_') {
			return false;
		}
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_'))) {
			return false;
		}
	}
	return true;
}

int comma_deletor(char *word){
	int i = 0;
	while(word[i]){
		i++;
	}
	if(word[i-1] == ','){
		word[i-1] = '\0';
		return 1;
	}
	else return 0;


}

int appost(char *word){

	int i;

	if(word[0] == '\''){
		i = 1;
		while(word[i]){
			if(word[i] == '\''){
				if(word[i+1] == '\0'){

					for(i=0;i<strlen(word)-1;i++){
						word[i] = word[i+1];
					}
					word[i-1] ='\0';
					return 1;
				}
				else{
					return 0;
				}
			}
			i++;
		}
		return 0;
	}

	if(word[0] == '\"'){
		i = 1;
		while(word[i]){
			if(word[i] == '\"'){
				if(word[i+1] == '\0'){

					for(i=0;i<strlen(word)-1;i++){
						word[i] = word[i+1];
					}
					word[i-1] ='\0';

					return 1;
				}
				else{
					return 0;
				}
			}
			i++;
		}
		return 0;
	}
}
void parse(char* code) {
	int lineCount = 0;
	char *line[1000];
	char *splitCode = strtok(code, ".");
	while (splitCode != NULL) {
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, ".");
	}
	for (int i = 0; i < lineCount; i++) {
		int wordCount = 0;
		char *word[1000];

		char *splitLine = strtok(line[i], " \n\r");
		while (splitLine != NULL) {
			word[wordCount++] = splitLine;
			splitLine = strtok(NULL, " \n\r");
		}

		if (wordCount == 0) { return; }

		//Int
		if (!strcmp(word[0], "int")) {
			printf("Keyword int\n"); //sil
			fprintf(fp_out,"Keyword int\n");

			//Variable
			if (word[1] == NULL) {
				printf("Line: %d | Variable is expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Variable is expected.\n", i);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("Line: %d | '%s' is a keyword. Variable is expected.\n", i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Variable is expected.\n", i+1, word[1]);
				return 0;
			}
			else if (isVariable(word[1])) {
				printf("Identifier %s\n", word[1]);
                fprintf(fp_out,"Identifier %s\n", word[1]);
				variableList[vIndex++] = word[1];
				integerList[iIndex++] = 0;
			}
			else {
				printf("Line: %d | '%s' is not a variable. Variable is expected.\n", i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is not a variable. Variable is expected.\n", i+1, word[1]);
				return 0;
			}

			//End Of Line
			if (wordCount == 2) {
				printf("EndOfLine\n");
				fprintf(fp_out,"EndOfLine\n");
			}
			else {
				printf("Line: %d | EndOfLine is expected.\n", i+1);
				fprintf(fp_out,"Line: %d | EndOfLine is expected.\n", i+1);
				return 0;
			}
		}

        //Move
		else if (!strcmp(word[0], "move")) {
			printf("Keyword move\n");
            fprintf(fp_out,"Keyword move\n");
			//Integer Or Variable
			int moveInt = 0;
			if (word[1] == NULL) {
				printf("Line: %d | Integer or variable are expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Integer or variable are expected.\n", i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("Line: %d | '%s' is a keyword. Integer or variable are expected.\n", i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Integer or variable are expected.\n", i+1, word[1]);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("IntConstant %s\n", word[1]);
                fprintf(fp_out,"IntConstant %s\n", word[1]);
				moveInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("Identifier %s\n", word[1]);
                fprintf(fp_out,"Identifier %s\n", word[1]);
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						moveInt = integerList[i];
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n", i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n", i+1, word[1]);
				return 0;
			}
            //To
			if (word[2] == NULL) {
				printf("Line: %d | Keyword 'to' is expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Keyword 'to' is expected.\n", i+1);
				return 0;
			}
			else if (!strcmp(word[2], "to")) {
				printf("Keyword to\n");
				fprintf(fp_out,"Keyword to\n");
			}
			else {
				printf("Line: %d | Keyword 'to' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'to' is expected.\n",i+1);
				return 0;
			}

			//Variable
			if (word[3] == NULL) {
				printf("Line: %d | Variable is expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Variable is expected.\n", i+1);
				return 0;
			}
			else if (isKeyword(word[3])) {
				printf("Line: %d | '%s' is a keyword. Variable is expected.\n", i+1, word[3]);
				fprintf(fp_out,"'Line: %d | %s' is a keyword. Variable is expected.\n", i+1, word[3]);
				return 0;
			}
			else if (isIntegerVariable(word[3])) {
				printf("Identifier %s\n", word[3]);
                fprintf(fp_out,"Identifier %s\n", word[3]);
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] = moveInt;
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a variable. Variable is expected.\n", i+1, word[3]);
				fprintf(fp_out,"Line: %d | '%s' is not a variable. Variable is expected.\n",i+1, word[3]);
				return 0;
			}

			//End Of Line
			if (wordCount == 4) {
				printf("EndOfLine\n");
				fprintf(fp_out,"EndOfLine\n");
			}
			else {
				printf("Line: %d | Endofline is expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Endofline is expected.\n", i+1);
				return 0;
			}
		}

        //Add
		else if (!strcmp(word[0], "add")) {
			printf("Keyword add\n");
			fprintf(fp_out,"Keyword add\n");

			//Integer Or Variable
			int addInt = 0;
			if (word[1] == NULL) {
				printf("Line: %d | Integer or variable are expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Integer or variable are expected.\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("Line: %d | '%s' is a keyword. Integer or variable are expected.\n",i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Integer or variable are expected.\n",i+1, word[1]);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);
				fprintf(fp_out,"'%s' is an integer.\n", word[1]);

				addInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("Identifier %s\n", word[1]);
				fprintf(fp_out,"Identifier %s\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						addInt = integerList[i];
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",i+1, word[1]);
				return 0;
			}

			//To
			if (word[2] == NULL) {
				printf("Line: %d | Keyword 'to' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'to' is expected.\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "to")) {
				printf("Keyword to\n");
				fprintf(fp_out,"Keyword to\n");
			}
			else {
				printf("Line: %d | Keyword 'to' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'to' is expected.\n",i+1);
				return 0;
			}

			//Variable
			if (word[3] == NULL) {
				printf("Line: %d | Variable is expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Variable is expected.\n",i+1);
				return 0;
			}
			else if (isKeyword(word[3])) {
				printf("Line: %d | '%s' is a keyword. Variable is expected.\n",i+1, word[3]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Variable is expected.\n",i+1, word[3]);
				return 0;
			}
			else if (isIntegerVariable(word[3])) {
				printf("Identifier %s\n", word[3]);
                fprintf(fp_out,"Identifier %s\n", word[3]);
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] += addInt;
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a variable. Variable is expected.\n",i+1, word[3]);
				fprintf(fp_out,"Line: %d | '%s' is not a variable. Variable is expected.\n",i+1, word[3]);
				return 0;
			}

			//End Of Line
			if (wordCount == 4) {
				printf("EndOfLine\n");
				fprintf(fp_out,"EndOfLine\n");
			}
			else {
				printf("Line: %d | EndOfLine is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | EndOfLine is expected.\n",i+1);
				return 0;
			}
		}

        //Sub
		else if (!strcmp(word[0], "sub")) {
			printf("Keyword sub\n");
			fprintf(fp_out,"Keyword sub\n");

			//Integer Or Variable
			int subInt = 0;
			if (word[1] == NULL) {
				printf("Line: %d | Integer or variable are expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Integer or variable are expected.\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("Line: %d | '%s' is a keyword. Integer or variable are expected.\n",i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Integer or variable are expected.\n",i+1, word[1]);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);
				fprintf(fp_out,"'%s' is an integer.\n", word[1]);

				subInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("Identifier %s\n", word[1]);
				fprintf(fp_out,"Identifier %s\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						subInt = integerList[i];
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",i+1, word[1]);
				return 0;
			}

			//To
			if (word[2] == NULL) {
				printf("Line: %d | Keyword 'from' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'from' is expected.\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "from")) {
				printf("Keyword from\n");
				fprintf(fp_out,"Keyword from\n");
			}
			else {
				printf("Line: %d | Keyword 'from' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'from' is expected.\n",i+1);
				return 0;
			}

			//Variable
			if (word[3] == NULL) {
				printf("Line: %d | Variable is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Variable is expected.\n",i+1);
				return 0;
			}
			else if (isKeyword(word[3])) {
				printf("Line: %d | '%s' is a keyword. Variable is expected.\n",i+1, word[3]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Variable is expected.\n",i+1, word[3]);
				return 0;
			}
			else if (isIntegerVariable(word[3])) {
				printf("Identifier %s\n", word[3]);
				fprintf(fp_out,"Identifier %s\n", word[3]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] -= subInt;
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a variable. Variable is expected.\n",i+1, word[3]);
				fprintf(fp_out,"Line: %d | '%s' is not a variable. Variable is expected.\n",i+1, word[3]);
				return 0;
			}

			//End Of Line
			if (wordCount == 4) {
				printf("EndOfLine\n");
				fprintf(fp_out,"EndOfLine\n");
			}
			else {
				printf("Line: %d | EndOfLine is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | EndOfLine is expected.\n",i+1);
				return 0;
			}
		}

        //Out
		else if (!strcmp(word[0], "out")) {
			printf("Keyword out\n");
			fprintf(fp_out,"Keyword out\n");

            if (word[1] == NULL) {
                    printf("Line: %d | Integer, variable or string are expected.\n",i+1);
                    fprintf(fp_out,"Line: %d | Integer, variable or string are expected.\n",i+1);
                    return 0;
                }
            //Integer, Variable Or String
			for(int s = 1; s < wordCount; s++){

                int comme = comma_deletor(word[s]);

                if (isInteger(word[s])) {
                    printf("IntConstant %s\n", word[s]);
                    fprintf(fp_out,"IntConstant %s\n", word[s]);
                }
                else if (isIntegerVariable(word[s])) {
                    printf("Identifier %s\n", word[s]);
                    fprintf(fp_out,"Identifier %s\n", word[s]);

                    for (int i = 0; i < vIndex; i++) {
                        if (!strcmp(variableList[i], word[s])) {
                            //printf("'%s' variable value: %i\n", word[s], integerList[i]);
                            //fprintf(fp_out,"'%s' variable value: %i\n", word[s], integerList[i]);
                            break;
                        }
                    }
                }
                else if (!strcmp(word[s], "newline")) {
                    printf("Keyword newline\n");
                    fprintf(fp_out,"Keyword newline\n");
                }
                else if (isKeyword(word[s])) {
                    printf("Line: %d | EndOfLine is expected.\n",i+1,word[s]);
                    fprintf(fp_out,"Line: %d | EndOfLine is expected.\n",i+1,word[s]);
                    return 0;
                }
                else if (appost(word[s]) == 1) {
                    printf("StringConstant '%s'\n", word[s]);
                    fprintf(fp_out,"StringConstant '%s'\n", word[s]);
                }
                else {
                    printf("Line: %d | Integer, variable or string are expected.\n",i+1);
                    fprintf(fp_out,"Line: %d | Integer, variable or string are expected.\n",i+1);
                    return 0;


                }
                if(comme) {
                    printf("Seperator\n");
                    fprintf(fp_out,"Seperator\n");
                    if (s==wordCount-1) {
                        printf("Line: %d | Integer, variable or string are expected.\n",i+1);
                        fprintf(fp_out,"Line: %d | Integer, variable or string are expected.\n",i+1);
                        return 0;
                    }
                }
                else if(wordCount > 1 && s != wordCount - 1){
                    if (isKeyword(word[s])) {
                        printf("Line: %d | EndOfLine is expected.\n",i+1,word[s]);
                        fprintf(fp_out,"Line: %d | EndOfLine is expected.\n",i+1,word[s]);
                        return 0;
                    }
                    printf("Line: %d | Seperator is expected.\n",i+1,word[s]);
                    fprintf(fp_out,"Line: %d | Seperator is expected.\n",i+1,word[s]);
                    return 0;
                }

            }
                printf("EndOfLine\n");
                fprintf(fp_out,"EndOfLine\n");
		}

        //Loop
		else if (!strcmp(word[0], "loop")) {
			printf("Keyword loop\n");
			fprintf(fp_out,"Keyword loop\n");

			//Integer Or Variable
			int loopInt = 0;
			if (word[1] == NULL) {
				printf("Line: %d | Integer or variable are expected.\n", i+1);
				fprintf(fp_out,"Line: %d | Integer or variable are expected.\n", i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("Line: %d | '%s' is a keyword. Integer or variable are expected.\n",i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is a keyword. Integer or variable are expected.\n",i+1, word[1]);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);
				fprintf(fp_out,"'%s' is an integer.\n", word[1]);

				loopInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("Identifier %s\n", word[1]);
				fprintf(fp_out,"Identifier %s\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						loopInt = integerList[i];
						break;
					}
				}
			}
			else {
				printf("Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",i+1, word[1]);
				fprintf(fp_out,"Line: %d | '%s' is not a integer or variable. Integer or variable are expected.\n",i+1, word[1]);
				return 0;
			}

			//Times
			if (word[2] == NULL) {
				printf("Line: %d | Keyword 'times' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'times' is expected.\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "times")) {
				printf("Keyword times\n");
				fprintf(fp_out,"Keyword times\n");
			}
			else {
				printf("Line: %d | Keyword 'times' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | Keyword 'times' is expected.\n",i+1);
				return 0;
			}

            //OpenBlock
			if (word[3] == NULL) {
				printf("Line: %d | '[' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | '[' is is expected.\n",i+1);
				return 0;
			}
			else if (!strcmp(word[3], "[")) {
				printf("OpenBlock\n");
				fprintf(fp_out,"OpenBlock\n");
			}
			else {
				printf("Line: %d | '[' is expected.\n",i+1);
				fprintf(fp_out,"Line: %d | '[' is expected.\n",i+1);
				return 0;
			}

			//Code
			for (int i = 0; i < loopInt; i++) {
				char newCode[100] = "";
				for (int i = 3; i < wordCount; i++) {
					strcat(newCode, " ");
					strcat(newCode, word[i]);
				}
				strcat(newCode, ".");
				parse(newCode);
			}

            printf("CloseBlock\n");
            fprintf(fp_out,"CloseBlock\n");

		}

        else {
			printf("Line: %d | '%s' is not a Keyword!\n", i+1, word[0]);
			fprintf(fp_out,"Line: %d | '%s' is not a Keyword!\n", i+1, word[0]);
			return 0;
		}

	}


    /*
    int lastChar;
    for(int i=0; i<lineCount; i++){
        /*
        for(int j=0; line[i][j]!='\0'; j++){
            printf("%c",line[i][j]);
        }

        //lastChar = strlen(line[i])-2;
        printf("%d: ",i);
		printf("%s",line[i]);
        printf("\n");
    }

    printf("\n\n\n\n\n");

    for(int i=0; Loops[i]!=NULL; i++){
        printf("Loop %d:%s\n",i, Loops[i]);
    }
    */
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
				exit(0);
			}
		}
	}
}

char LoopPart[1000]; char c[1000]; char d[1000];
int indexLoop = 0;
void isLoop(char* str) {
    bool trueLoop = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '[') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == ']') {
					substring(str, i + 1, j - i - 1, LoopPart);
					//printf("Loop Part:\n%s\n", LoopPart);
                    Loops[indexLoop] = (char*)malloc(sizeof(char)*128);
                    strcpy(Loops[indexLoop], LoopPart);
                    indexLoop++;
                    //printf("Loop Part:\n%s\n", Loops[indexLoop]);

					substring(str, 0, i, c);
					substring(str, j + 1, strlen(str), d);
					strcat(c, d);
					strcpy(str, c);

					trueLoop = true;
					isLoop(str);
				}
			}
			if (!trueLoop) {
				printf("Loop part is not completed.\n");
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

int main() {
	FILE *fp;
	long lSize;
	char *code;

	fp = fopen("myscript.ba", "rb");
	if (!fp) perror("myscript.ba"), exit(1);

    fp_out = fopen("myscript.lx","w");
    if (!fp_out) perror("myscript.lx"), exit(1);

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	code = calloc(1, lSize + 1);
	if (!code) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

	if (1 != fread(code, lSize, 1, fp)) {
		fclose(fp), free(code), fputs("entire read fails", stderr), exit(1);
	}

	isComment(code);
	isLoop(code);
    parse(code);
}
