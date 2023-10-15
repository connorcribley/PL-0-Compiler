// PL/0 Compiler
// Program created by Connor Cribley

/*
    DISCLAIMER: This program was created in its entirety by Connor Cribley. I do not authorize anybody to
    replicate, alter, submit, or otherwise use this program in a way which violates the academic integrity of any
    educational institution. Any attempt to use this code to commit academic plagiarism was done so without my
    permission.
*/


// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants
#define MAX 10000
#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_IDENTIFIER 11
#define MAX_DIGIT 5
#define MAX_LEV 10

// Enums
typedef enum
{
    oddsym = 1,
    identsym,
    numbersym,
    plussym,
    minussym,
    multsym,
    slashsym,
    xorsym,
    eqsym,
    neqsym,
    lessym,
    leqsym,
    gtrsym,
    geqsym,
    lparentsym,
    rparentsym,
    commasym,
    semicolonsym,
    periodsym,
    becomessym,
    beginsym,
    endsym,
    ifsym,
    thensym,
    whilesym,
    dosym,
    callsym,
    constsym,
    varsym,
    procsym,
    writesym,
    readsym,
    elsesym
} token_type;

typedef enum {
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, WRITE, READ, HALT
} opcode;

typedef enum {
    NEG = 0, ADD, SUB, MUL, DIV, EQL, NEQ, LSS, LEQ, GTR, GEQ, ODD
} operations;

typedef struct lexeme {
    char lexeme[MAX_IDENTIFIER];
    int token_type;
} lexemeStruct;


lexemeStruct lexemes[MAX];

typedef struct instruction {
    int op;
    int l;
    int m;
} instruction;

instruction code[MAX];

typedef struct symbol {
int kind; // const = 1, var = 2, proc = 3
char name[12]; // name up to 11 chars
int val; // number (ASCII value)
int level; // L level
int addr; // M address
int mark; // to indicate unavailable or deleted
} symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

void lexical_analyzer(char *input, lexemeStruct *lexemes);
void emit(int op, int l, int m);
int symbol_table_check(char* sym);
void program();
void block();
int const_declaration();
int var_declaration();
void statement();
void condition();
void expression();
void term();
void factor();


int lev = 0;
int i = 0;
char input[MAX];
int len = 0;
char charToString[MAX]; // Stores the current input char as a string for string functions;
char tokens[MAX];
char lexemeList[MAX];
char lexeme[MAX];
int numLexemes = 0;
int lexemeIndex = 0;
int sTableIndex = 0;
int codeIndex = 0;
int num_procs = 0; //Number of procedure calls (starts at zero)


int main(int argc, char *argv[])
{

    // Error case if there is not one and only one file entered in the command line
    if (argc != 2)
    {
        printf("Error: there should be one and only one command line argument!");
        exit(1);
    }

    // Create the File pointer for the input file
    FILE *f = fopen(argv[1], "r");

    // Error case if the file could not be read
    if (f == NULL)
    {
        printf("Error: file could not be read!");
        exit(1);
    }

    // Read the entirety of the input file and store it
    do
    {
        input[i] = fgetc(f);
        i++;

        if (feof(f))
            break;
    } while (1);

    //Generate the lexeme list (aka list of tokens)
    lexical_analyzer(input, lexemes);

    //emit JMP
    // emit(JMP, 0, 3);

    program();

    // Reset the file pointer
    f = fopen(argv[1], "r");

    // Print source code as specified
    printf("Source Program:\n");
    while (fgets(input, MAX, f) != NULL)
    {
        printf("%s", input);
    }
    printf("\n");

    //Start printing
    printf("\nNo errors, program is syntactically correct\n\n");

    //Edit the file pointer to write to the elf.txt file
    f = fopen("elf.txt", "w");

    printf("Assembly Code:\n");
    printf("Line\tOP\tL\tM\n");

    for (i = 0; i < codeIndex; i++) {
        switch (code[i].op) 
        {
            case LIT:
                printf("%d\tLIT\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", LIT, code[i].l, code[i].m);
                break;
            case OPR:
                switch (code[i].m)
                {
                    case NEG:
                        printf("%d\tNEG\t%d\t0\n", i, code[i].l);
                        fprintf(f, "%d %d 0\n", OPR, code[i].l);
                        break;
                    case ADD:
                        printf("%d\tADD\t%d\t1\n", i, code[i].l);
                        fprintf(f, "%d %d 1\n", OPR, code[i].l);
                        break;
                    case SUB:
                        printf("%d\tSUB\t%d\t2\n", i, code[i].l);
                        fprintf(f, "%d %d 2\n", OPR, code[i].l);
                        break;
                    case MUL:
                        printf("%d\tMUL\t%d\t3\n", i, code[i].l);
                        fprintf(f, "%d %d 3\n", OPR, code[i].l);
                        break;
                    case DIV:
                        printf("%d\tMUL\t%d\t4\n", i, code[i].l);
                        fprintf(f, "%d %d 4\n", OPR, code[i].l);
                        break;
                    case EQL:
                        printf("%d\tEQL\t%d\t5\n", i, code[i].l);
                        fprintf(f, "%d %d 5\n", OPR, code[i].l);
                        break;
                    case NEQ:
                        printf("%d\tNEQ\t%d\t6\n", i, code[i].l);
                        fprintf(f, "%d %d 6\n", OPR, code[i].l);
                        break;
                    case LSS:
                        printf("%d\tLSS\t%d\t7\n", i, code[i].l);
                        fprintf(f, "%d %d 7\n", OPR, code[i].l);
                        break;
                    case LEQ:
                        printf("%d\tLEQ\t%d\t8\n", i, code[i].l);
                        fprintf(f, "%d %d 8\n", OPR, code[i].l);
                        break;
                    case GTR:
                        printf("%d\tGTR\t%d\t9\n", i, code[i].l);
                        fprintf(f, "%d %d 9\n", OPR, code[i].l);
                        break;
                    case GEQ:
                        printf("%d\tGEQ\t%d\t10\n", i, code[i].l);
                        fprintf(f, "%d %d 10\n", OPR, code[i].l);
                        break;
                    case ODD:
                        printf("%d\tODD\t%d\t11\n", i, code[i].l);
                        fprintf(f, "%d %d 11\n", OPR, code[i].l);
                        break;
                    default:
                        break;
                }
                break;
            case LOD:
                printf("%d\tLOD\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", LOD, code[i].l, code[i].m);
                break;
            case STO:
                printf("%d\tSTO\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", STO, code[i].l, code[i].m);
                break;
            case CAL:
                printf("%d\tCAL\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", CAL, code[i].l, code[i].m);
                break;
            case INC:
                printf("%d\tINC\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", INC, code[i].l, code[i].m);
                break;
            case JMP:
                printf("%d\tJMP\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", JMP, code[i].l, code[i].m);
                break;
            case JPC:
                printf("%d\tJPC\t%d\t%d\n", i, code[i].l, code[i].m);
                fprintf(f, "%d %d %d\n", JPC, code[i].l, code[i].m);
                break;
            case WRITE:
                printf("%d\tSYS\t0\t1\n", i);
                fprintf(f, "9 0 1\n");
                break;
            case READ:
                printf("%d\tSYS\t0\t2\n", i);
                fprintf(f, "9 0 2\n");
                break;
            case HALT:
                printf("%d\tSYS\t0\t3\n", i);
                fprintf(f, "9 0 3\n");
                break;
            default:
                break;
        }
    }

    fclose(f);
    return 0;
}

void emit(int op, int l, int m) {
    code[codeIndex].op = op;    //opcode
    code[codeIndex].l = l;      //lexicographical level
    code[codeIndex].m = m;      //M
    codeIndex++;
}


int symbol_table_check(char* symbolName) {
    int correctSymbolIndex = -1;
    int highestLevel = 0;

    // Here, we are looking for a symbol that, if found, is at the highest level less than or equal to the current level, else return -1
    for (int j = 0; j < (sizeof(symbol_table)/sizeof(symbol_table[0])); j++) {
        if (strcmp(symbol_table[j].name, symbolName) == 0 && symbol_table[j].level >= highestLevel && symbol_table[j].level <= lev) {
            correctSymbolIndex = j;
            highestLevel = symbol_table[j].level;
        }
    }
    return correctSymbolIndex;
}

void program() {

    //Run block
    block();
    
    //Error case for program not ending with period
    if(lexemes[lexemeIndex].token_type != periodsym) {
        printf("Error: program must end with period");
        exit(1);
    } else {
        //emit HALT
        emit(HALT, 0, 3);
    }
}

void block() {

    int numVars = 0; 
    int numConsts = 0;

    //save jump address
    int jmpIdx = codeIndex;

    //emit JMP
    emit(JMP, 0, 0);

    //too many levels error
    if (lev >= MAX_LEV) {
        printf("Error: too many procedure calls!");
        exit(1);
    }
    
    //Get number of consts
    if(lexemes[lexemeIndex].token_type == constsym) {
        numConsts = const_declaration();
    }
    
    //Get number of vars
    if (lexemes[lexemeIndex].token_type == varsym) {
        numVars = var_declaration();
    }

    //Get number of procs
    while (lexemes[lexemeIndex].token_type == procsym) {
        //Increase number of procedures;
        num_procs++;

        //Create a new proc symbol
        symbol newSymbol;

        newSymbol.kind = 3;
        newSymbol.val = num_procs;
        newSymbol.addr = num_procs;
        newSymbol.level = lev;
        newSymbol.mark = 0;

        //Get next token
        lexemeIndex++;

        //identifier error
        if (lexemes[lexemeIndex].token_type != identsym) {
            printf("Error: const, var, procedure must be followed by identifier.");
            exit(1);
        }

        // //check symbol table
        // if (symbol_table_check(lexemes[lexemeIndex].lexeme) != -1) {
        //     printf("Error: symbol name has already been declared");
        //     exit(1);
        // }

        //copy symbol name
        strcpy(newSymbol.name, lexemes[lexemeIndex].lexeme);

        //Add to symbol table, increase index
        symbol_table[sTableIndex] = newSymbol;
        sTableIndex++;

        //Get next token
        lexemeIndex++;

        //Semicolon error
        if (lexemes[lexemeIndex].token_type != semicolonsym) {
            printf("Error: semicolon missing.");
            exit(1);
        }
            
        //Get next token
        lexemeIndex++;

        //increment level for each procedure
        lev++;

        //block call
        block();

        if (lexemes[lexemeIndex].token_type == semicolonsym) {
            //Get next token
            lexemeIndex++;
        }
        //Semicolon error
        else {
            printf("Error: semicolon missing.");
            exit(1);
        }
    }

    //save current code index into m of jump index
    code[jmpIdx].m = codeIndex * 3;

    //Emit INC
    emit(INC, 0, (3 + numVars));

    //Statement call
    statement();

    //If we are inside another level, emit RTN
    if (lev > 0) {
        emit(OPR, 0, 0);
    }

    //Decrement level after each procedure
    lev--;
}

int const_declaration() {
    
    int num_consts = 0;
    
    do {
        //Create a new const symbol
        symbol newSymbol;
        
        newSymbol.kind = 1;
        newSymbol.addr = 0;
        newSymbol.level = lev;
        newSymbol.mark = 0;
        
        //Get next token
        lexemeIndex++;
        
        //check if identifier
        if (lexemes[lexemeIndex].token_type != identsym) {
            printf("Error: const, var, procedure must be followed by identifier.");
            exit(1);
        }

        // //check symbol table
        // if (symbol_table_check(lexemes[lexemeIndex].lexeme) != -1) {
        //     printf("Error: symbol name has already been declared");
        //     exit(1);
        // }

        //copy symbol name
        strcpy(newSymbol.name, lexemes[lexemeIndex].lexeme);
        
        //get next token
        lexemeIndex++;
        
        //check if equal sign
        if (lexemes[lexemeIndex].token_type != eqsym) {
            printf("Error: constants must be assigned with =");
            exit(1);
        }
        
        //get next token
        lexemeIndex++;
        
        //consts must be integers
        if ( lexemes[lexemeIndex].token_type != numbersym ) {
            printf("Error: constants must be assigned an integer value");
            exit(1);
        }
        
        
        //save const value to new symbol
        newSymbol.val = atoi(lexemes[lexemeIndex].lexeme);
        
        //Add to symbol table, increase index
        symbol_table[sTableIndex] = newSymbol;
        sTableIndex++;
        
        //Get next token
        lexemeIndex++;
        
        //increment num_consts
        num_consts++;
        
    } while(lexemes[lexemeIndex].token_type == commasym);
    
    if (lexemes[lexemeIndex].token_type != semicolonsym) {
        printf("Error: constant and variable declarations must be followed by a semicolon");
        exit(1);
    }
    
    //get next token
    lexemeIndex++;
    
    //return num_consts
    return num_consts;
}

int var_declaration() {
    int num_vars = 0;
    
    do {
        num_vars++;

        //Create a var symbol
        symbol newSymbol;
        
        newSymbol.kind = 2;
        newSymbol.val = 0;
        newSymbol.addr = num_vars + 2;
        newSymbol.level = lev;
        newSymbol.mark = num_vars + 2;
        
        //get next token
        lexemeIndex++;
        
        //check if identifier
        if(lexemes[lexemeIndex].token_type != identsym) {
            printf("Error: const, var, procedure must be followed by identifier.");
            exit(1);
        }

        //copy symbol name
        strcpy(newSymbol.name, lexemes[lexemeIndex].lexeme);

        symbol_table[sTableIndex] = newSymbol;
        sTableIndex++;
        
        //Get next token
        lexemeIndex++;
        
    } while(lexemes[lexemeIndex].token_type == commasym);
    
    //check for semicolon
    if (lexemes[lexemeIndex].token_type != semicolonsym) {
        printf("Error: constant and variable declarations must be followed by a semicolon");
        exit(1);
    }
    
    //get next token
    lexemeIndex++;
    
    //return numvars
    return num_vars;
}

void statement() {
    
    int symIdx, jpcIdx, loopIdx, varIdx, jmpIdx;
    
    if (lexemes[lexemeIndex].token_type == identsym) {
        
        //get symbol index
        symIdx = symbol_table_check(lexemes[lexemeIndex].lexeme);

        //undeclared identifier error
        if (symIdx == -1) {
            printf("Error: undeclared identifier");
            exit(1);
        }
        
        //not a variable error
        if (symbol_table[symIdx].kind != 2) {
            printf("Error: Assignment to constant or procedure is not allowed");
            exit(1);
        }
        
        //get next token
        lexemeIndex++;
        
        //becomes symbol error
        if (lexemes[lexemeIndex].token_type != becomessym) {
            printf("Error: assignment statements must use :=");
            exit(1);
        }
        
        //get next token
        lexemeIndex++;

        //expression call
        expression();
        
        //emit STO
        emit(STO, lev - symbol_table[symIdx].level, symbol_table[symIdx].addr);

        return; 
    } 
    else if (lexemes[lexemeIndex].token_type == beginsym) {
        
        do {
            //get next token
            lexemeIndex++;
        
            //statement call
            statement();
        } while(lexemes[lexemeIndex].token_type == semicolonsym);
        
        //end symbol error
        if (lexemes[lexemeIndex].token_type != endsym) {
            printf("Error: begin must be followed by end");
            exit(1);
        }
        //get next token
        lexemeIndex++;

        return;
        
    } 
    else if (lexemes[lexemeIndex].token_type == callsym) {

        //Get next token
        lexemeIndex++;

        //call identifier error
        if (lexemes[lexemeIndex].token_type != identsym) {
            printf("Error: call must be followed by an identifier");
            exit(1);
        }
        else {
            //get procedure index
            int symIdx = symbol_table_check(lexemes[lexemeIndex].lexeme);

            //Undeclared identifier error
            if (symIdx == -1) {
                printf("Error: undeclared identifier");
                exit(1);
            }

            if (symbol_table[symIdx].kind == 3) {
                //emit CAL
                emit(CAL, lev - symbol_table[symIdx].level, symbol_table[symIdx].addr * 3);
            }
            //Call not procedure error
            else {
                printf("Error: call of a constant or variable is meaningless");
                exit(1);
            }

            //Get next token
            lexemeIndex++;
        }

        return;
    }
    else if (lexemes[lexemeIndex].token_type == ifsym) {
        
        //Get next token
        lexemeIndex++;
        
        //condition call
        condition();

        //Save jump instruction index inside symbol table
        jpcIdx = codeIndex;
        
        //emit JPC 
        emit(JPC, 0, 0);           
        
        //then symbol error
        if (lexemes[lexemeIndex].token_type != thensym) {
            printf("Error: if must be followed by then");
            exit(1);
        }
        
        //get next token
        lexemeIndex++;
        
        //statement call
        statement();
        
        //save current code index into m of jump index
        code[jpcIdx].m = codeIndex * 3;

        return;
    
    }
    else if (lexemes[lexemeIndex].token_type == xorsym) {
        //Get next token
        lexemeIndex++;
        
        //condition call
        condition();

        //Save jump instruction index inside symbol table
        jpcIdx = codeIndex;
        
        //emit JPC 
        emit(JPC, 0, 0);            //M = jpcIdx ???
        
        //then symbol error
        if (lexemes[lexemeIndex].token_type != thensym) {
            printf("Error: then expected");
            exit(1);
        }
        
        //get next token
        lexemeIndex++;
        
        //statement call
        statement();
        
        //semicolon error
        if (lexemes[lexemeIndex].token_type != semicolonsym) {
            printf("Error: constant and variable declarations must be followed by a semicolon");
            exit(1);
        }
        else
            //get next token
            lexemeIndex++;

        //else error
        if (lexemes[lexemeIndex].token_type != elsesym) {
            printf("Error: xor must be followed by else");
            exit(1);
        }

        //store jump index
        jmpIdx = codeIndex;

        //emit jump
        emit(JMP, 0, 0);

        //get next token
        lexemeIndex++;

        //save current code index into m of jump index
        code[jpcIdx].m = codeIndex * 3;

        //statement call
        statement();

        //save current code index into m of jump index
        code[jmpIdx].m = codeIndex * 3;

        return;
    }

    else if (lexemes[lexemeIndex].token_type == whilesym) {
        
        //get next token
        lexemeIndex++;
        
        //save current code index to loop index
        loopIdx = codeIndex * 3;

        //condition call
        condition();

        //do symbol error
        if (lexemes[lexemeIndex].token_type != dosym)
        {
            printf("Error: while must be followed by do");
            exit(1);
            
        }

        //Get next token    
        lexemeIndex++;
        
        //save current code index to jump index
        jpcIdx = codeIndex;

        //emit JPC 
        emit(JPC, 0, 0);            

        //Statement call
        statement();
        
        //emit JMP
        emit(JMP, 0, loopIdx);
        
        //set m of code at loop index to code index
        code[jpcIdx].m = codeIndex * 3;

        return;

    } else if (lexemes[lexemeIndex].token_type == readsym) {
        
        //get next token
        lexemeIndex++;
        
        //ident symbol error
        if (lexemes[lexemeIndex].token_type != identsym) {
            printf("Error: read must be followed by identifier");
            exit(1);
        }
        
        //check symbol table
        symIdx = symbol_table_check(lexemes[lexemeIndex].lexeme);

        //undeclared identifier error
        if (symIdx == -1) {
            printf("Error: undeclared identifier");
            exit(1);
        }
        
        //symbol not a var error
        if (symbol_table[symIdx].kind != 2) {
            printf("Error: only variables may be altered");
            exit(1);
        }

        //get next token
        lexemeIndex++;
        
        //emit READ
        emit(READ, 0, 2);
        
        //emit STO
        emit(STO, lev - symbol_table[symIdx].level, symbol_table[symIdx].addr);
        
        return;
        
    } else if (lexemes[lexemeIndex].token_type == writesym) {
        
        //get next token
        lexemeIndex++;

        //expression call
        expression();
        
        //emit write
        emit(WRITE, 0, 1);
        
        return;
    }
}

void condition() {
    if (lexemes[lexemeIndex].token_type == oddsym) {
        //get next token
        lexemeIndex++;
        //expression call
        expression();
        //emit odd
        emit(OPR, 0, ODD);
    }
    else {
        //expression call
        expression();

        if (lexemes[lexemeIndex].token_type == eqsym) {
            //get next token
            lexemeIndex++;
            //expression call
            expression();
            //emit eql
            emit(OPR, 0, EQL);
        }
        else if (lexemes[lexemeIndex].token_type == neqsym) {
            //get next token
            lexemeIndex++;
            //expression call
            expression();
            //emit neq
            emit(OPR, 0, NEQ);
        }
        else if (lexemes[lexemeIndex].token_type == lessym) {
            //get next token
            lexemeIndex++;
            //expression call
            expression();
            //emit lss
            emit(OPR, 0, LSS);
        }
        else if (lexemes[lexemeIndex].token_type == leqsym) {
            //get next token
            lexemeIndex++;
            //expression call
            expression();
            //emit leq
            emit(OPR, 0, LEQ);
        }
        else if (lexemes[lexemeIndex].token_type == gtrsym) {
            //get next token
            lexemeIndex++;
            //expression call
            expression();
            //emit gtr
            emit(OPR, 0, GTR);
        }
        else if (lexemes[lexemeIndex].token_type == geqsym) {
            //get next token
            lexemeIndex++;
            //expression call
            expression();
            //emit geq
            emit(OPR, 0, GEQ);
        }
        else {
            //condition error
            printf("Error: condition must contain comparison operator");
            exit(1);
        }
    }
}

void expression() {
    //term call
    term();
    while ((lexemes[lexemeIndex].token_type == plussym) || (lexemes[lexemeIndex].token_type == minussym)) {
        if (lexemes[lexemeIndex].token_type == plussym) {
            //get next token
            lexemeIndex++;
            //term call
            term();
            //emit ADD
            emit(OPR, 0, ADD);
        }
        else {
            //get next token
            lexemeIndex++;
            //term call
            term();
            //emit ADD
            emit(OPR, 0, SUB);
        }
    }                                                              
}

void term() {
    
    //factor call
    factor();
    
    while((lexemes[lexemeIndex].token_type == multsym) || (lexemes[lexemeIndex].token_type == slashsym)) {
        if (lexemes[lexemeIndex].token_type == multsym) {
            //get next token
            lexemeIndex++;
            //factor call
            factor();
            //emit mul
            emit(OPR, 0, MUL);
        }
        else {
            //get next token
            lexemeIndex++;
            //factor call
            factor();
            //emit div
            emit(OPR, 0, DIV);
        }
    }
}

void factor() {
    int symIdx;

    if (lexemes[lexemeIndex].token_type == identsym) {
        //check symbol table
        symIdx = symbol_table_check(lexemes[lexemeIndex].lexeme);
        
        //undeclared identifier error
        if (symIdx == -1) {
            printf("Error: undeclared identifier");
            exit(1);
        }
        
        //If var
        if (symbol_table[symIdx].kind == 1) {
            //emit LIT
            emit(LIT, lev - symbol_table[symIdx].level, symbol_table[symIdx].val);
        }
        //If const
        else if (symbol_table[symIdx].kind == 2) {
            //emit LOD
            emit(LOD, lev - symbol_table[symIdx].level, symbol_table[symIdx].addr);
        }
        //If proc, error
        else {
            printf("Error: expression must not contain a procedure identifier.");
            exit(1);
        }

        //get next token
        lexemeIndex++;
    }
    else if (lexemes[lexemeIndex].token_type == numbersym) {
        //emit LIT
        int numVal = atoi(lexemes[lexemeIndex].lexeme);
        emit(LIT, 0, numVal);

        //get next token
        lexemeIndex++;
    }
    else if (lexemes[lexemeIndex].token_type == lparentsym) {
        //get next token
        lexemeIndex++;

        //expression call
        expression();

        //no right parentheses error
        if (lexemes[lexemeIndex].token_type != rparentsym) {
            printf("Error: right parenthesis must follow left parenthesis");
            exit(1);
        }

        //get next token
        lexemeIndex++;
    }
    else {
        printf("Error: the preceding factor cannot begin with this symbol");
        exit(1);
    }
}

void lexical_analyzer(char *input, lexemeStruct *lexemes) {
    i = 0;

    // While we have not yet reached the end of the input
    while (input[i] != EOF && input[i] != '\0')
    {
        lexemeStruct thisLexeme;

        // If the input at the current pointer is a whitespace character
        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\t')
        {
            // ignore it by simply incrementing the iteration pointer and continuing the loop
            i++;
            continue;
        }

        // Reserved Words List

        // odd
        else if (input[i] == 'o' && input[i + 1] == 'd' && input[i + 2] == 'd' && isalnum(input[i + 3]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = oddsym;
            strcpy(thisLexeme.lexeme, "odd");

            // Increment i
            i += 3;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // const
        else if (input[i] == 'c' && input[i + 1] == 'o' && input[i + 2] == 'n' && input[i + 3] == 's' && input[i + 4] == 't' && isalnum(input[i + 5]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = constsym;
            strcpy(thisLexeme.lexeme, "const");

            // Increment i
            i += 5;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // var
        else if (input[i] == 'v' && input[i + 1] == 'a' && input[i + 2] == 'r' && isalnum(input[i + 3]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = varsym;
            strcpy(thisLexeme.lexeme, "var");

            // Increment i
            i += 3;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // procedure
        else if (input[i] == 'p' && input[i + 1] == 'r' && input[i + 2] == 'o' && input[i + 3] == 'c' && input[i + 4] == 'e' && input[i + 5] == 'd' && input[i + 6] == 'u' &&
                 input[i + 7] == 'r' && input[i + 8] == 'e' && isalnum(input[i + 9]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = procsym;
            strcpy(thisLexeme.lexeme, "procedure");

            // Increment i
            i += 9;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // call
        else if (input[i] == 'c' && input[i + 1] == 'a' && input[i + 2] == 'l' && input[i + 3] == 'l' && isalnum(input[i + 4]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = callsym;
            strcpy(thisLexeme.lexeme, "call");

            // Increment i
            i += 4;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // begin
        else if (input[i] == 'b' && input[i + 1] == 'e' && input[i + 2] == 'g' && input[i + 3] == 'i' && input[i + 4] == 'n' && isalnum(input[i + 5]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = beginsym;
            strcpy(thisLexeme.lexeme, "begin");

            // Increment i
            i += 5;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // end
        else if (input[i] == 'e' && input[i + 1] == 'n' && input[i + 2] == 'd' && isalnum(input[i + 3]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = endsym;
            strcpy(thisLexeme.lexeme, "end");

            // Increment i
            i += 3;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // if
        else if (input[i] == 'i' && input[i + 1] == 'f' && isalnum(input[i + 2]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = ifsym;
            strcpy(thisLexeme.lexeme, "if");

            // Increment i
            i += 2;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // then
        else if (input[i] == 't' && input[i + 1] == 'h' && input[i + 2] == 'e' && input[i + 3] == 'n' && isalnum(input[i + 4]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = thensym;
            strcpy(thisLexeme.lexeme, "then");

            // Increment i
            i += 4;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // xor
        else if (input[i] == 'x' && input[i + 1] == 'o' && input[i + 2] == 'r' && isalnum(input[i + 3]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = xorsym;
            strcpy(thisLexeme.lexeme, "xor");

            // Increment i
            i += 3;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // else
        else if (input[i] == 'e' && input[i + 1] == 'l' && input[i + 2] == 's' && input[i + 3] == 'e' && isalnum(input[i + 4]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = elsesym;
            strcpy(thisLexeme.lexeme, "else");

            // Increment i
            i += 4;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // while
        else if (input[i] == 'w' && input[i + 1] == 'h' && input[i + 2] == 'i' && input[i + 3] == 'l' && input[i + 4] == 'e' && isalnum(input[i + 5]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = whilesym;
            strcpy(thisLexeme.lexeme, "while");

            // Increment i
            i += 5;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // do
        else if (input[i] == 'd' && input[i + 1] == 'o' && isalnum(input[i + 2]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = dosym;
            strcpy(thisLexeme.lexeme, "do");

            // Increment i
            i += 2;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // read
        else if (input[i] == 'r' && input[i + 1] == 'e' && input[i + 2] == 'a' && input[i + 3] == 'd' && isalnum(input[i + 4]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = readsym;
            strcpy(thisLexeme.lexeme, "read");

            // Increment i
            i += 4;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // write
        else if (input[i] == 'w' && input[i + 1] == 'r' && input[i + 2] == 'i' && input[i + 3] == 't' && input[i + 4] == 'e' && isalnum(input[i + 5]) == 0)
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = writesym;
            strcpy(thisLexeme.lexeme, "write");

            // Increment i
            i += 5;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // Special Symbols List

        // plus (+)
        else if (input[i] == '+')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = plussym;
            strcpy(thisLexeme.lexeme, "+");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // minus (-)
        else if (input[i] == '-')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = minussym;
            strcpy(thisLexeme.lexeme, "-");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // mult (*)
        else if (input[i] == '*')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = multsym;
            strcpy(thisLexeme.lexeme, "*");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // slash (/)
        else if (input[i] == '/' && input[i + 1] != '*') // comments are demarked by /*
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = slashsym;
            strcpy(thisLexeme.lexeme, "/");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // lparent (()
        else if (input[i] == '(')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = lparentsym;
            strcpy(thisLexeme.lexeme, "()");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // rparent ())
        else if (input[i] == ')')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = rparentsym;
            strcpy(thisLexeme.lexeme, ")");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // neq (<>)
        else if (input[i] == '<' && input[i + 1] == '>')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = neqsym;
            strcpy(thisLexeme.lexeme, "<>");

            // Increment i
            i += 2;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // comma (,)
        else if (input[i] == ',')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = commasym;
            strcpy(thisLexeme.lexeme, ",");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // period (.)
        else if (input[i] == '.')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = periodsym;
            strcpy(thisLexeme.lexeme, ".");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // leq (<=)
        else if (input[i] == '<' && input[i + 1] == '=')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = leqsym;
            strcpy(thisLexeme.lexeme, "<=");

            // Increment i
            i += 2;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // les (<)
        else if (input[i] == '<')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = lessym;
            strcpy(thisLexeme.lexeme, "<");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // geq (>=)
        else if (input[i] == '>' && input[i + 1] == '=')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = geqsym;
            strcpy(thisLexeme.lexeme, ">=");

            // Increment i
            i += 2;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // gtr (>)
        else if (input[i] == '>')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = gtrsym;
            strcpy(thisLexeme.lexeme, ">");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // becomes (:=)
        else if (input[i] == ':' && input[i + 1] == '=')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = becomessym;
            strcpy(thisLexeme.lexeme, ":=");

            // Increment i
            i += 2;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // eq (=)
        else if (input[i] == '=')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = eqsym;
            strcpy(thisLexeme.lexeme, "=");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // semicolon (;)
        else if (input[i] == ';')
        {
            // Set the lexeme and token type;
            thisLexeme.token_type = semicolonsym;
            strcpy(thisLexeme.lexeme, ";");

            // Increment i
            i += 1;

            // Concatinate the lexeme list with the current lexeme
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // Comments (/*...*/)
        else if (input[i] == '/' && input[i + 1] == '*')
        {
            i += 2;
            // Comments should be ignored until their terminating symbols
            while (input[i] != EOF && input[i] != '\0')
            {
                if (input[i] == '*' && input[i + 1] == '/')
                    break;
                i++;
            }
            // Ignore the terminating symbols as well
            i += 2;

            // Continue the next loop iteration
            continue;
        }

        // Identifiers and Numbers

        // Identifiers
        else if (isalpha(input[i]))
        {
            // Initialize the lexeme string
            char ident[MAX_IDENTIFIER];
            int c = 0;

            for (c = 0; c < MAX_IDENTIFIER; c++) {
                ident[c] = '\0';
            }

            c = 0;

            ident[c] = input[i];
            i++;

            // While the identifier is still being read
            while (isalnum(input[i]))
            {   
                c++;

                //Error case if identifier is longer than 11 characters
                if (c > MAX_IDENTIFIER) {
                    printf("Error: identifiers names cannot exceed 11 characters\n");
                    exit(1);
                }

                // Append the rest of the identifier to the lexeme string
                ident[c] = input[i];
                i++;
            }

            // Set the token type and concatinate the lexeme list with the current lexeme and the identifier
            thisLexeme.token_type = identsym;
            strcpy(thisLexeme.lexeme, ident);
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // Numbers
        else if (isdigit(input[i]))
        {
            // Initialize the lexeme string
            char digit[MAX_IDENTIFIER];
            int c = 0;

            for (c = 0; c < MAX_IDENTIFIER; c++) {
                digit[c] = '\0';
            }

            c = 0;

            digit[c] = input[i];
            i++;

            // Error case for identifiers beginning with digit(s)
            if (isalpha(input[i]))
            {
                printf("Error : Identifiers cannot begin with a digit");
                exit(1);
            }

            // While the number is still being read
            while (isdigit(input[i]))
            {
                c++;

                // Error case if number is more than 5 digits long
                if (c > MAX_DIGIT) {                    
                    printf("Error : Numbers cannot exceed 5 digits");
                    exit(1);
                }
                

                // Append the rest of the number to the lexeme string
                digit[c] = input[i];
                i++;
                
            }

            // Set the token type and concatinate the lexeme list with the current lexeme and the identifier
            thisLexeme.token_type = numbersym;
            strcpy(thisLexeme.lexeme, digit);
            lexemes[numLexemes] = thisLexeme;
            numLexemes++;

            // Continue the next loop iteration
            continue;
        }

        // At this point, an invalid symbol has been encountered (unless it's the EOF character)
        else
        {
            if (input[i] != EOF)
                printf("Error : Invalid Symbol\n");
            i++;
        }
    }
}