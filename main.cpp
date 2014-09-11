//
//  main.cpp
//  Small Talk Interpreter
//
//  Created by Matt Burdumy on 9/10/13.
//  Copyright 2013 Georgetown University. All rights reserved.
//


/*****************************************************************
 *                        MAHE MAIN                              *
 *****************************************************************/

#include "SmallHeader.h"
#include <stdlib.h>
#include <string>

using namespace std;

void reader();
void maheInput();


int main()
{
    
    
    initNames();
    initHierarchy();
    globalEnv = emptyEnv();
    
    trueValue = mkINT(1);
    falseValue = mkINT(0);
    
    quittingtime = 0;
    
    while (!quittingtime)
    {
        

        
        reader();
        
        //cout<<"Input length: " << inputleng << "Pos: "<<pos<<endl;


        
        if (matches(pos, 4, "quit"))
            quittingtime = 	1;
        else if ( (userinput[pos]=='(') &&
                 ( matches(skipblanks(pos+1),5,"class")))
        {
            prName(parseClass());
            cout << endl;
        }
        else if ((userinput[pos] == '(') &&
                 (matches(skipblanks(pos+1), 6,"define")) )
        {
            prName(parseDef(fundefs));
            cout <<endl;
        }
        else
        {
            
            currentExp = parseExp();
            prValue(eval(currentExp, emptyEnv(), objectInst));
            cout <<endl;
        }
    }// while
} // smalltalk


/* nextchar - read next char - filter tabs and comments */

void nextchar (char& c)
{
    scanf("%c", &c);
    //c = getchar();
    
    if (c == COMMENTCHAR )
    {
        while ( c != '\n' )
            //c = getchar();
            scanf("%c",&c);
    }
    
    
    while (c < ' ' || c > 'z')
    {
        if (c == '\n')
            return;
        //cout<<"scrub loop, Char = "<<c<<endl;
        scanf("%c",&c);
    }
}

/* readParens - read char's, ignoring newlines, to matching ')' */
void readParens()
{
    int parencnt; /* current depth of parentheses */
    char c;
    parencnt = 1; // '(' just read
    do
    {
      
        if (c == '\n')
            cout <<PROMPT2;
        nextchar(c);
        pos++;
        if (pos == MAXINPUT )
        {
            cout <<"User input too long\n";
            exit(1);
        }
        if (c == '\n' )
            userinput[pos] = ' ';
        else if (c >= ' ' && c <= 'z')
            userinput[pos] = c;
        if (c == '(')
            ++parencnt;
        if (c == ')')
            parencnt--;
	}
    while (parencnt != 0 );
} //readParens

/* readInput - read char's into userinput */

void readInput()
{
    char  c;
    cout << PROMPT;
    pos = 0;
    do
    {
	    ++pos ;
	    if (pos == MAXINPUT )
	    {
		    cout << "User input too long\n";
		    exit(1);
	    }
	    nextchar(c);
	    if (c == '\n' )
            userinput[pos] = ' ';
	    else 
            userinput[pos] = c;
        
	    if (userinput[pos] == '(' )
            readParens();
    }
	while (c != '\n');
	inputleng = pos;
	userinput[pos+1] = COMMENTCHAR; // sentinel
}


/* reader - read char's into userinput; be sure input not blank  */

void reader ()
{
    do
    {
        readInput();
        pos = skipblanks(1);
    }
    while( pos > inputleng); // ignore blank lines
}


int isDigits (int pos)
{
    if ( ( userinput[pos] < '0' ) ||  ( userinput[pos] > '9' ) )
        return 0;
    while ( ( userinput[pos] >='0') && ( userinput[pos] <= '9') )
        ++pos;
    if (!isDelim(userinput[pos] ))
        return 0;
    return 1;
}// isDigits


/* isNumber - check if a number begins at pos  */

int isNumber (int pos)
{
    return ( isDigits(pos) || ((userinput[pos] == '-') &&
                               isDigits(pos+1)));
}// isNumber


