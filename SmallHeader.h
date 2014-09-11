//
//  SmallHeader.h
//  SmallTalk
//
//  Created by Matt Burdumy on 10/22/13.
//  Copyright 2013 Georgetown University. All rights reserved.
//

#ifndef SmallTalk_SmallHeader_h
#define SmallTalk_SmallHeader_h

#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <stdlib.h>

void readInput();
bool checkParands();
void clearComments();
int isDigits(int);


using namespace std;

const char arrow[2] = {'-','>'};
const char miniArrow = '>';



/*****************************************************************
 *                     DECLARATIONS                              *
 *****************************************************************/
typedef int NUMBER;
typedef int NAME;
const int  NAMELENG = 20;      /* Maximum length of a name */
const int  MAXNAMES = 1000;     /* Maximum number of different names */
const int  MAXINPUT = 50000;     /* Maximum length of an input */
const char*   PROMPT = "-> ";
const char*   PROMPT2 = "> ";
const char  COMMENTCHAR = ';';
const int   TABCODE = 9;        /* in ASCII */

struct ENVREC; // Forward Declaration
typedef ENVREC* ENV;   //forward declaration
enum STVALUETYPE {INT, SYM, USER};
struct  FUNDEFREC; // forward def
typedef FUNDEFREC* FUNDEF;  //forward declaration
struct NAMELISTREC;
typedef NAMELISTREC* NAMELIST;
struct CLASSREC; // Forward Def

typedef CLASSREC* CLASS; // Forward Def
struct STVALUEREC
{
    CLASS owner;
    STVALUETYPE vType;
    int intVal;
    NAME symVal; // stored in symbol table??
    ENV userval;
};

typedef STVALUEREC* STVALUE;

struct EXPLISTREC;
typedef EXPLISTREC* EXPLIST;
enum EXPTYPE {VALEXP,VAREXP,APEXP};
struct EXPREC
{
    EXPTYPE etype; //what type of expression
    STVALUE value;
    NAME varble;
    NAME optr;
    EXPLIST args;
};

struct CLASSREC
{
    NAME name; // name of class 
    FUNDEF fundefs; // list of funcs
    CLASS father; //
    NAMELIST vars; //
    CLASS tail;
    
};

typedef EXPREC* EXP;  //forward declaration
struct EXPLISTREC
{
    EXP head; // points to 'node' EXPREC
    EXPLIST tail; // points to next expression 
};


struct VALUELISTREC
{
    STVALUE  head;
    VALUELISTREC*  tail;
};

typedef VALUELISTREC* VALUELIST;  //forward declaration
struct NAMELISTREC
{
    NAME   head; // this var's position in printNames
    NAMELISTREC* tail;
};

  //forward declaration
struct  ENVREC
{
    NAMELIST vars; // pointer to list of NAMES
    VALUELIST values; // pointer to lis of VALUES
};


struct  FUNDEFREC
{
    NAME  funname;
    NAMELIST  formals;
    EXP  body;
    FUNDEFREC*  nextfundef;
};

VALUELIST mkRepFor ( NAMELIST nl);

STVALUE trueValue;
STVALUE falseValue;
FUNDEF  fundefs = NULL;   //points to the head of the linked list of all the funcions
CLASS  classdefs = NULL; // points to the head of a linked list of all the class defs. ## FIX ME ##
CLASS OBJECTCLASS = NULL; // f
ENV globalEnv;     // keeps track of all the global variables
EXP currentExp;    //pointer to the current expression
STVALUE objectInst;
int   inputleng, pos;       // pos is at the current character of userinput
char userinput[MAXINPUT];
char*   printNames[MAXNAMES];   // symbol table
int   numNames, numBuiltins;
int quittingtime;  //HALT when true
int numCtrlOps = 0;

NAME SELF = 0;

/*****************************************************************
 *                     DATA STRUCTURE OP'S                       *
 *****************************************************************/

ENV emptyEnv() // return an empty environment
{
    ENV env = new ENVREC;
    env->values = NULL;
    env->vars = NULL;
    return env;
}



/* mkVALEXP - return an EXP of type VALEXP with num n            */

EXP mkVALEXP ( STVALUE n)
{
    EXP e;
    e = new EXPREC;
    e->etype = VALEXP;
    e->value = n;
    e->optr = -5; // delete me
    e->varble = -5; // delete me
    e->args = NULL; // delete me
    return e;
}/* mkVALEXP */


/* mkVAREXP - return an EXP of type VAREXP with varble nm  */

EXP mkVAREXP ( NAME nm) // *********** check me
{ 
    EXP e;
    e = new EXPREC;
    e->etype = VAREXP;
    e->varble = nm;
    e->value = NULL;
    e->optr = -4;
    e->args = NULL;
    
    return e; 
    //you write the code
    
}/* mkVAREXP */


/* mkAPEXP - return EXP of type APEXP w/ optr op and args el     */

EXP mkAPEXP (NAME op, EXPLIST el) // *********** CHECK ME
{
    
    EXP e;
    e = new EXPREC;
    e->etype = APEXP;
    e->optr = op;
    e->args = el; 
    return e;
    //you write the code
    
}/* mkAPEXP */

/* mkExplist - return an EXPLIST with head e and tail el         */

EXPLIST mkExplist (EXP e, EXPLIST el)
{
    
    EXPLIST list;
    list = new EXPLISTREC;
    list->head = e;
    list->tail = el;
    return list;
    //you write the code
    
}/* mkExplist */

/* mkNamelist - return a NAMELIST with head n and tail nl        */

NAMELIST mkNamelist (NAME nm, NAMELIST nl)
{
    
    NAMELIST n = new NAMELISTREC;
    n->head = nm;
    n->tail = nl;
    return n;
    
}/* mkNamelist */

/*****************************************************************
 *                     NAME MANAGEMENT                           *
 *****************************************************************/

// fetchFun - get function definition of NAME fname from fenv

FUNDEF fetchFun (NAME fname, FUNDEF fenv)
{
    if (fenv->funname == fname)
        return fenv;
    else if (fenv->nextfundef != NULL)
        return (fetchFun(fname, fenv->nextfundef));
    else
    {
        return NULL;
    }
    
} // fetchFun

/* fetchClass - get class definition of NAME cname   */

CLASS fetchClass (NAME cname)
{
    CLASS temp = classdefs;

    
    while (temp->tail != NULL)
    {
        if (temp->name == cname)
            return temp;
        temp = temp->tail; // traverse
    }
    
    return 0;
    
}/* fetchClass */

/* newClass - add new class cname to classes      */

CLASS newClass (NAME cname, CLASS father)
{
    CLASS addMe = new CLASSREC;
    addMe->name = cname;
    addMe->father = father;
    addMe->tail = classdefs;
    classdefs = addMe;
    return addMe;
    
    
}//* newClass

//  newFunDef - add new function fname to fenv

FUNDEF newFunDef (NAME fname, FUNDEF& fenv)
{
    FUNDEF f = new FUNDEFREC;
    f->funname = fname;
    f->nextfundef = fenv;
    fenv = f;
    return f;
    
} // newFunDef


int isNUM(int pos); // forward dec.
int skipblanks(int pos); // forward dec.

int parseINT()
{
    int n, sign;
    n = 0; sign = 1;
    if ( userinput[pos] == '-' )
    {
        sign = -1;
        pos++;
    }
    while ( (userinput[pos] >= '0') && (userinput[pos] <= '9' ) )
    {
        n = 10*n + userinput[pos] - '0';
        pos++;
    }
    pos = skipblanks(pos); // skip blanks after number
    return (n*sign);
}// parseInt
/* mkValuelist - return an VALUELIST with head n and tail vl     */

STVALUE mkSYM ( NAME s)
{
    STVALUE newval;
    newval = new STVALUEREC;
    newval->symVal = s;
    newval->vType = SYM;
    newval->userval = NULL;
    newval->owner = OBJECTCLASS;
    
    return newval;
    //do this
} /* mkSYM */

STVALUE mkINT ( int value)
{
    STVALUE newval;
    newval = new STVALUEREC;
    newval->symVal = NULL;
    newval->intVal = value;
    newval->userval = NULL;
    newval->owner = OBJECTCLASS;
    newval->vType = INT;
    
    return newval;
    
}

STVALUE mkUSER (ENV rho, CLASS ownr)
{
    STVALUE newval;
    newval = new STVALUEREC;
    newval-> symVal = NULL;
    newval-> userval = rho;
    newval -> owner = ownr;
    newval->vType = USER;
    
    return newval;
    
} /* mkUSER */

VALUELIST mkValuelist (STVALUE n,  VALUELIST vl)
{
    VALUELIST list = new VALUELISTREC;
    list->head = n;
    list->tail = vl;
    return list;
    
    
}/* mkValuelist */


/* initNames - place all pre-defined names into printNames */
/* this is our symbol table */

void initNames()
{
    int i =1;
    fundefs = 0;
    printNames[i] = "if";    i++;   //1
    printNames[i] = "while"; i++;   //2
    printNames[i] = "set";   i++;   //3
    printNames[i] = "begin"; i++;   //4
    numCtrlOps    = i;
    printNames[i] = "new";i++;      //5
    printNames[i] = "+"; i++;       //6
    printNames[i] = "-"; i++;       //7
    printNames[i] = "*"; i++;       //8
    printNames[i] = "/"; i++;       //9
    printNames[i] = "="; i++;       //10
    printNames[i] = "<"; i++;       //11
    printNames[i] = ">"; i++;       //12
    printNames[i] = "print";i++;    //13
    printNames[i] = "self";         //14
    SELF = i;
    numNames = i;
    numBuiltins = i-1;
}//initNames

/* install - insert new name into printNames  */

NAME install ( char* nm)
{
    int i = 1;
    while (i <= numNames)
        if (strcmp( nm,printNames[i] ) == 0)
            break;
        else
            i++;
    if (i > numNames)
    {
        numNames = i;
        printNames[i] = new char[strlen(nm) + 1];
        strcpy(printNames[i], nm);
    }
    return i;
}// install

/* prName - print name nm              */

void prName ( NAME nm)
{
    cout<< printNames[nm];
    if( printNames[nm] == "abs")
    {
        cout<<"This is a garbage line, you found abs"<<endl;
    }
} //prName

/*****************************************************************
 *                        INPUT                                  *
 *****************************************************************/

/* isDelim - check if c is a delimiter   */

int isDelim (char c)
{
    return ( ( c == '(') || ( c == ')') ||( c == ' ')||( c== COMMENTCHAR) );
}

int matches (int s, int leng,  char* nm)
{
    int i=0;
    while (i < leng )
    {
        if( userinput[s] != nm[i] )
            return 0;
        ++i;
        ++s;
    }
    if (!isDelim(userinput[s]) )
        return 0;
    return 1;
}/* matches */

/* skipblanks - return next non-blank position in userinput */

int skipblanks (int p)
{
    while (userinput[p] == ' ' || userinput[p] == '\n' || userinput[p] == '\r' || userinput[p] == '\t')
        ++p;
    return p;
}

/* parseName - return (installed) NAME starting at userinput[pos]*/

NAME parseName()
{
    char nm[25]; // array to accumulate characters
    int leng; // length of name
    leng = 0;
    while ( (pos <= inputleng) && !isDelim(userinput[pos]) )
    {
	    ++leng;
	    nm[leng-1] = userinput[pos];
        ++pos;
    }
    if (leng == 0)
    {
        cout<<"Error: expected name, instead read: "<< userinput[pos]<<endl;
        exit(1);
    }
    nm[leng] = '\0';
    pos = skipblanks(pos); // skip blanks after name
    return ( install(nm) );
    
}// parseName

/* isNUM - check if a number begins at pos  */

int isNUM (int pos) // this needs explaining.
{
    return ( isDigits(pos) || 
            ( (userinput[pos] == '-') && isDigits(pos+1))
            ||( (userinput[pos] == '+') && isDigits(pos+1)));
}// isNUM

int isValue (int pos)
{
   
    return ((userinput[pos] == '#') || isNUM(pos));  
}// isValue


/* parseVal - return number starting at userinput[pos]   */



STVALUE parseVal() // now returns a STVALUE
{
    if( userinput[pos] == '#')
        return mkSYM(parseName()); // pos will be advanced.
    else // must be an int
        return mkINT(parseINT()); // pos will be advanced
    

    
    // you do this
}// parseVal

EXPLIST parseEL(); // forward declaration
/* parseExp - return EXP starting at userinput[pos]  */

EXP parseExp()
{
    
    // decide if VarEXP, ValExp or OptExp
    pos = skipblanks(pos);
    
    if(userinput[pos] == '(')
    {
        pos++;
        // create APEXP
        NAME temp = parseName();
        EXPLIST temp2 = parseEL();
        
        return mkAPEXP(temp,temp2);
    }
    if (isValue(pos))
    {
        //create valexp
        return mkVALEXP(parseVal());
    }
    //create varexp
    return mkVAREXP(parseName());
    
    
    //you write the code;
    //this and parseEL() are mutually recursive
    
}// parseExp

/* parseEL - return EXPLIST starting at userinput[pos]  */

EXPLIST parseEL()
{
    pos = skipblanks(pos);
    if (userinput[pos] == ')' )
    {
        pos = skipblanks(pos+1);
        return 0;
    }
    EXP temp = parseExp();
    EXPLIST temp2 = parseEL();
    return mkExplist(temp, temp2);
    
    //you write the code;
    //this and parseEXP() are mutually recursive
    
}// parseEL

/* parseNL - return NAMELIST starting at userinput[pos]  */

NAMELIST parseNL()
{
    pos = skipblanks(pos);
    if (userinput[pos] == ')' )
    {
        pos = skipblanks(pos+1);
        return 0;
    }
    NAME nm = parseName();
    NAMELIST nl = parseNL();
    return mkNamelist(nm,nl);
    
}// parseNL

/* parseDef - parse function definition at userinput[pos]   */

NAME parseDef(FUNDEF& fenv)
{
    NAME fname;        // function name
    FUNDEF newDef;
    pos = skipblanks(pos+1); // skip '( ..'
    pos = skipblanks(pos+6); // skip 'define ..'
    fname = parseName();
    pos = skipblanks(pos+1); // skip '( ..']
    newDef = newFunDef(fname, fenv);
    newDef->formals = parseNL();
    newDef->body =  parseExp();
    pos = skipblanks(pos+1); // skip ') ..'
    
    return ( fname);
}// parseDef


NAME parseClass()
{
    CLASS newDef;
    pos = skipblanks(pos+1); // skip '( ..'
    pos = skipblanks(pos+5); // skip ' class '
    NAME cname = parseName(); // new class name
    CLASS superClass = fetchClass(parseName()); // super class name
    newDef = newClass(cname, superClass); // memory alloc'd
    FUNDEF methods = 0;
    
    pos = skipblanks(pos); // should be at the "("
    pos = skipblanks(pos+1);
    
    
    // parse the vars list
    newDef->vars = parseNL();
    
    if (newDef->vars == NULL)
    {
        newDef->vars = superClass->vars; // just link to super vars.
    }
    else
    {
        
        NAMELIST temp = newDef->vars;

        while (temp->tail != NULL) // traverse
        {
            temp = temp->tail;
        }
        temp->tail = superClass->vars; // tie on to super
    }
    
    // just to be safe
    pos = skipblanks(pos); // skip any blanks
    
    newDef->fundefs = NULL;
    
    while ( userinput[pos] == '(')
    { // parse this func
        prName(parseDef(methods));
        pos = skipblanks(pos); // should be at the next '(define' or EOF
    }
    
    newDef->fundefs = methods;
     pos = skipblanks(pos);
     pos = skipblanks(pos+1);
    
    return cname;
}

//The following computes the values for operators 5-12

int lengthVL(VALUELIST vl)
{
    int length = 0;
    if (vl == NULL) 
        return 0;
    length++;
    while(vl->tail != NULL)
    {
        vl = vl->tail;
        length++;
    }
    
    // Double check me...
    
    
    return length;
}

int arity ( int op)
{
    if ( ( op > 5) && (op < 13) )
	    return 2;
    return  1;
}// arity


void prValue ( STVALUE v)
{
    if (v->vType == INT )
        cout <<(v->intVal);
    else if( v->vType == SYM )
        prName(v->symVal);
    else
        cout << "<userval>";
} // prValue

bool isBound(NAME varble, ENV env) // if the given varble is in the given environemnt env
{
    
    NAMELIST list = env->vars;
    while (list != NULL)
    {
        if( list->head == varble)
            return true;
        list = list->tail;
    }
    return false; 
}

VALUELIST findVar ( NAME nm, ENV rho)
{
    NAMELIST  nl;
    VALUELIST  vl;
    nl = rho->vars;
    vl = rho->values;
    while  (nl != 0)
        if ( nl->head == nm )
            break;
        else{
		    nl = nl->tail;
		    vl = vl->tail;
		};
    return vl;
}

STVALUE fetch(NAME varble, ENV rho)
{
    return findVar(varble, rho)->head;
    
}

STVALUE  applyValueOp ( int op,  VALUELIST vl)
{
    STVALUE  n, n1, n2;
    
    if ( arity(op) != lengthVL(vl) )// correct number of parameters
    {
        cout <<	"Wrong number of arguments to  ";
        prName(op);
        cout <<endl;
    }
    n1 = vl->head; // 1st actual
    if (arity(op) == 2)
    {
        n2 = vl->tail->head; //2nd actual
        
        if ( (n1->vType != INT || n2->vType != INT) && op != 10  ) // i.e. user is trying to call a math operator on not two ints
        {
            cout<< "Can only perform arithmatic ops on two types INT" <<endl;
            exit(1);
        }
    }
    
  
    if (op == 2)
    {
        if( n1->vType == INT && n2->vType == INT)
        {
            if (n1->intVal == n2->intVal)
            {
                return trueValue;
            }
        }
        if(n1->vType == SYM && n2->vType == SYM)
        {
            if(n1->symVal == n2->symVal)
                return trueValue;
        }
        return falseValue;
    }
    
    switch (op)
    {
        case 6 : n = mkINT(n1->intVal + n2->intVal); break;
        case 7 : n = mkINT(n1->intVal - n2->intVal); break;// do it
        case 8 : n = mkINT(n1->intVal * n2->intVal); break;//do it
        case 9 : n = mkINT(n1->intVal / n2->intVal); break;//do it
        //case 10 : n = mkINT(n1->intVal == n2->intVal); break;//do it
        case 11 : n = mkINT(n1->intVal < n2->intVal); break; //do it
        case 12: n = mkINT(n1->intVal > n2->intVal); break;//do it
        case 13: prValue(n1); cout<<endl; n = n1;break;
    };//switch
    return n;
}// applyValueOp

//forward declaration
STVALUE eval ( EXP e,  ENV rho, STVALUE rcvr);

FUNDEF methodSearch (NAME optr,  CLASS cl)
{
    if (cl == OBJECTCLASS)
    {
        return NULL;
    }

    FUNDEF f = fetchFun(optr, cl->fundefs);
    if (f != NULL)
    {
        return f;
    }
    if( cl != OBJECTCLASS)
        return methodSearch(optr, cl->father);
    
    
    
}// methodSearch



//This eveluates a userdefined function and returns its value

STVALUE applyGlobalFun ( NAME nm, VALUELIST actuals, STVALUE rcvr)
{
    FUNDEF temp = fundefs;
    bool found = false;
    // search fundefs for name nm
    FUNDEF f;
    while (temp != NULL)
    {
        if(nm == temp->funname)
        {
            f = temp;
            found = true;
            break;
        }
        temp = temp->nextfundef;
    }
    
    if (found == false)
    {
        cout<<"function not found"<<endl;
        exit(1);
    }
    
    ENV rho = emptyEnv();
    rho->vars = f->formals;
    rho->values = actuals;
    
    
    return eval(f->body, rho, rcvr);
    
    //do it
}// applyUserFun


void addValue(STVALUE value, NAME name, ENV env)
{
    env->vars = mkNamelist(name, env->vars);
    env->values = mkValuelist(value, env->values);
}

//This evelustes the ctrl operators

int isTrueVal ( STVALUE v)
{
    if ((v->vType == USER) || (v->vType == SYM))
        return 1;
    return (v->intVal != 0 );
} //isTrueVal

void  assign (NAME nm,  STVALUE v, ENV rho)
{
    VALUELIST varloc;
    varloc = findVar(nm, rho);
    varloc->head = v;
}// assign

ENV mkEnv ( NAMELIST nl, VALUELIST vl)
{
    ENV rho;
    rho = new ENVREC;
    rho->vars = nl;
    rho->values = vl;
    return rho;
}/* mkEnv */

STVALUE applyCtrlOp ( int op,  EXPLIST args, ENV rho , STVALUE rcvr)
{
    STVALUE n = 0;
    if (op == 1)  //if statement
        if (isTrueVal((eval(args->head, rho, rcvr))))
            return  eval(args->tail->head, rho, rcvr);
        else
            return  eval(args->tail->tail->head, rho, rcvr);
    
    if ( op == 2) //while statement
    {
        while (isTrueVal(eval(args->head, rho, rcvr)))
        {
            eval(args->tail->head,rho, rcvr);
            
        }
        return 0;
    }
    if( op == 3)  //set
    {
        n = eval(args->tail->head, rho, rcvr);
        if(isBound(args->head->varble, rho)) // the variable already exists in this env, change it
            assign(args->head->varble, n , rho);
        else  if (isBound(args->head->varble, rcvr->userval))
            assign(args->head->varble, n, rcvr->userval);// add it to the environment
        else if (isBound(args->head->varble, globalEnv))
                 assign(args->head->varble, n, globalEnv);
        else // add to global
            addValue(n, args->head->varble, globalEnv);
        return n;
        
        
    }
    if( op == 4) //begin
    {
        while (args!= NULL) // eval all expressions then return value of last.  
        {
            n = eval(args->head,rho, rcvr);
            args = args->tail;
        }
        return n;
    }// applyCtrlOp
    
    if ( op == 5) // new
    {
        CLASS father = fetchClass(args->head->varble);
        ENV classENV = mkEnv(father->vars, mkRepFor(father->vars));
        n = mkUSER(classENV, father);     // new
        
        assign(SELF, n, n->userval);
        
        return n;
    }
    

}

STVALUE applyMethod (FUNDEF f, VALUELIST actuals)
{
    // search vl->head->superclass for f
    // then call eval and pass f-> body. Make the local enviornemnt
    
    ENV rho = mkEnv(f->formals, actuals->tail);
    return eval(f->body, rho, actuals->head);
    
    
}// applyMethod

//this evaluates a list of expressions and 
//return the correspoding values in a valuelist

VALUELIST frontEvalList( STVALUE value, VALUELIST list, STVALUE rcvr); // forward dec.

VALUELIST evalList ( EXPLIST el, ENV rho, STVALUE rcvr)
{
    VALUELIST list = NULL;
    
    while (el != NULL)
    {
        list = frontEvalList(eval(el->head, rho, rcvr), list, rcvr);
        el = el->tail;
    }
    return list;
}// evalList


VALUELIST frontEvalList( STVALUE value, VALUELIST list, STVALUE rcvr)
{
    
    if (list == NULL)
    {
        VALUELIST temp = new VALUELISTREC;
        temp->head = value;
        temp->tail = NULL;
        return temp;
    }
    else
    {
        VALUELIST temp = list;
        while (temp->tail!= NULL)
            temp = temp->tail;
        VALUELIST addMe = new VALUELISTREC;
        addMe->head= value;
        addMe->tail= NULL;
        temp->tail = addMe;
        return list;
    }
}
//The eval function calls the other appropriate functions
//to evaluate an expresion

STVALUE eval( EXP e,  ENV rho, STVALUE rcvr)
{
    
    //cout<<"Operator= "<<e->optr<<endl;
    switch (e->etype)
    {
        case VALEXP: return (e->value);
        case VAREXP: if (isBound(e->varble, rho))
            return fetch(e->varble, rho);
        else if (isBound(e->varble, rcvr->userval))
            return fetch(e->varble, rcvr->userval);
        else if ( isBound(e->varble, globalEnv))
            return fetch(e->varble, globalEnv);
        else{
            cout << "Undefined variable: ";
            prName(e->varble);
            cout <<endl;
            exit(1);
        }
        case APEXP:
            if (e->optr <= numCtrlOps) // apply the conotrol ops
                return  applyCtrlOp(e->optr, e->args, rho, rcvr);
            
            VALUELIST vl = evalList(e->args, rho,rcvr);
            
            
            if(lengthVL(vl) == 0)
                return applyGlobalFun(e->optr, vl, rcvr);
            
            FUNDEF f = methodSearch(e->optr, vl->head->owner);
            if( f!= 0)
                return applyMethod(f,vl);
            
            if(e->optr <= numBuiltins)
                return applyValueOp(e->optr, vl);
            
            return applyGlobalFun(e->optr, vl, rcvr);
        
        }
    }



void initHierarchy()
{
    classdefs = 0;
    OBJECTCLASS = newClass(install("Object"), 0);
    OBJECTCLASS->father= NULL;
    OBJECTCLASS->vars= mkNamelist(SELF, 0);
    objectInst = mkUSER(mkEnv(OBJECTCLASS->vars, mkValuelist(mkINT(0), 0)),OBJECTCLASS);
}// initHierarchy

// mkRepFor - make list of all zeros of same length as nl

VALUELIST mkRepFor ( NAMELIST nl)
{
    if (nl == 0)
        return 0;
    return mkValuelist(falseValue, mkRepFor(nl->tail));
}// mkRepFor




#endif



