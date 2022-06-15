#ifndef LEXER_H

unsigned char checkSyntax(unsigned char finalInput[80], unsigned char numChars);
void tokenizeInputString(unsigned char finalInput[256], unsigned char numChars);
unsigned char* doCalculation(void);
void cleanTokenEntries(void);
#endif