#include "lexer.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static const unsigned char operators[6] = {'E','/','x','-','+','.'};
static unsigned char token[39][39] = {};
static unsigned char numTok = 0;
	
double executeOperation(double num1, unsigned char op, double num2);
unsigned char* setExponentFormatSubstring(unsigned char* resultString);
void delCalculatedElement(unsigned char i);

unsigned char checkSyntax(unsigned char finalInput[256], unsigned char numChars)
{
	unsigned char currentIndex = 0;	
	unsigned char afterDot = 0;
	while(numChars > currentIndex)
	{
		// digits cannot start with 0
		if(!afterDot && ((finalInput[0] == '0' && isdigit(finalInput[1])) || 
				(!isdigit(finalInput[currentIndex-1]) && finalInput[currentIndex] == '0' && isdigit(finalInput[currentIndex+1]))))
			return 0;
		
		if(afterDot && !isdigit(finalInput[currentIndex]))
		{
			afterDot = 0;
			if(finalInput[currentIndex] == '.')
				return 0;
		}
		
		if(finalInput[currentIndex] == '.')
		{	
			if(finalInput[currentIndex+1] == '-')
				return 0;
			afterDot = 1;
		}
		
		for(unsigned char i = 0; i < 6; i++ )
		{
			// first/last character cannot be a operator
			if((finalInput[numChars-1] == operators[i] || finalInput[0] == operators[i]) && finalInput[0] != '-') 
				return 0;
				
			for(unsigned char j = 0; j < 6; j++ )
			{
				// check if operators are next to each other on the string and return 0 signaling a syntax error, - on the right side is fine since we had to represent -number
				if(finalInput[currentIndex] == operators[i] && finalInput[currentIndex+1] == operators[j] && finalInput[currentIndex+1] != '-')
					return 0;
			}
		}
		currentIndex++;
	}
	return 1;
}  

unsigned char* doCalculation(void)
{	
	for(unsigned char j = 0; j < 5; j++)
	{
		for(unsigned char i = 0; i < numTok; i++)
		{
			if((token[i][0] == operators[j]) && i != 0)
			{
				double calculatedNum = executeOperation(strtod(token[i-1], NULL), operators[j], strtod(token[i+1], NULL));
				printf("%s %c %s = %f\n", token[i-1], operators[j], token[i+1], calculatedNum);
				snprintf(token[i-1], sizeof(token[i-1]), "%g", calculatedNum);
		
				unsigned char *result = setExponentFormatSubstring(token[i-1]);
				strncpy(token[i-1],result,sizeof(result));
				delCalculatedElement(i);
				i = 0;
			}
		}
		if(numTok == 1)
			break;
	}
	return token[0];
}

void tokenizeInputString(unsigned char finalInput[256], unsigned char numChars)
{
	unsigned char currentIndex = 0;
	//first number is negative number
	if(finalInput[0] == '-')
	{
		token[numTok][0] = '-';
		currentIndex++;
	}
	
	while(numChars > currentIndex)
	{
		while(isdigit(finalInput[currentIndex]) || finalInput[currentIndex] == '.')
			sprintf(token[numTok], "%s%c", token[numTok], finalInput[currentIndex++]);
		numTok++;
		
		if(numChars == currentIndex)
			break;
		
		sprintf(token[numTok], "%s%c", token[numTok], finalInput[currentIndex++]);
		numTok++;
		
		if(finalInput[currentIndex] == '-')
		{
			token[numTok][0] = '-';
			currentIndex++;
		}
	}
}

unsigned char* setExponentFormatSubstring(unsigned char* resultString)
{
  unsigned char * lowercaseExpo = strstr(resultString,"e");
	unsigned char * pExpo = strstr(resultString,"e+");
	unsigned char * pTrailingZerosExpo = strstr(resultString,"e+0");
	unsigned char * nTrailingZerosExpo = strstr(resultString,"e-0");
	
	if(pTrailingZerosExpo)
	{
		memcpy(pTrailingZerosExpo, "E", 1);
    memmove(pTrailingZerosExpo + 2, pTrailingZerosExpo + 3, strlen(pTrailingZerosExpo + 3) + 1);
	}

	if(nTrailingZerosExpo)
	{
		memcpy(nTrailingZerosExpo, "E-", 2);
    memmove(nTrailingZerosExpo + 2, nTrailingZerosExpo + 3, strlen(nTrailingZerosExpo + 3) + 1);
	}
	
	if(pExpo)
	{
		memcpy(pExpo, "E", 1);
    memmove(pExpo + 1, pExpo + 2, strlen(pExpo + 2) + 1);
	}
	
	if(lowercaseExpo)
		strncpy (lowercaseExpo,"E",1);
	
	return resultString;
}

void delCalculatedElement(unsigned char i)
{
	if(numTok > 3)
		for(unsigned char j = i; j < numTok - 2; j++)
			strncpy(token[j], token[j+2], sizeof(token[j+2]));
	token[numTok-1][0]= '\0';
	token[numTok-2][0]= '\0';
	numTok = numTok - 2;
	
}

double executeOperation(double num1, unsigned char op, double num2)
{
	switch(op)
	{
		case 'x':
			return num1*num2;
		case '+': 
			return num1+num2;
		case '-': 
			return num1-num2;
		case '/': 
			return num1/num2;
		case 'E': 
			return num1*pow(10, num2);
	}
	return 0;
}
void cleanTokenEntries(void)
{
	memset(&token[0], '\0', sizeof(token));
	numTok = 0;
}