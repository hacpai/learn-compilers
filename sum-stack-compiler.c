#include <stdio.h>
#include <stdlib.h>

#define TODO()                                  \
    do{                                                           \
        printf ("\nAdd your code here: file \"%s\", line %d\n",     \
            __FILE__, __LINE__);                                \
    } while(0)

///////////////////////////////////////////////
// Data structures for the Sum language.
enum Exp_Kind_t {EXP_INT, EXP_SUM};
struct Exp_t
{
    enum Exp_Kind_t kind;
};

struct Exp_Int
{
    enum Exp_Kind_t kind;
    int i;
};

struct Exp_Sum
{
    enum Exp_Kind_t kind;
    struct Exp_t *left;
    struct Exp_t *right;
};

// "constructors"
struct Exp_t *New_Exp_Int (int i)
{
    struct Exp_Int *p = malloc (sizeof(*p));
    p->kind = EXP_INT;
    p->i = i;
    return (struct Exp_t *)p;
}

struct Exp_t *New_Exp_Sum (struct Exp_t *left, struct Exp_t *right)
{
    struct Exp_Sum *p = malloc (sizeof(*p));
    p->kind = EXP_SUM;
    p->left = left;
    p->right = right;
    return (struct Exp_t *)p;
}

// "printer"
void Print_Exp (struct Exp_t *exp)
{
    switch (exp->kind){
        case EXP_INT:{
            struct Exp_Int *p = (struct Exp_Int *)exp;
            printf ("%d", p->i);
            break;
  }
        case EXP_SUM:{
            struct Exp_Sum *p = (struct Exp_Sum *)exp;
            Print_Exp (p->left);
            printf ("+");
            Print_Exp (p->right);
            break;
  }
          default:
            break;
  }
}

//////////////////////////////////////////////
// Data structures for the Stack language.
enum Stack_Kind_t {STACK_ADD, STACK_PUSH};
struct Stack_t
{
    enum Stack_Kind_t kind;
};

struct Stack_Add
{
    enum Stack_Kind_t kind;
};

struct Stack_Push
{
    enum Stack_Kind_t kind;
    int i;
};

// "constructors"
struct Stack_t *New_Stack_Add ()
{
    struct Stack_Add *p = malloc (sizeof(*p));
    p->kind = STACK_ADD;
    return (struct Stack_t *)p;
}

struct Stack_t *New_Stack_Push (int i)
{
    struct Stack_Push *p = malloc (sizeof(*p));
    p->kind = STACK_PUSH;
    p->i = i;
    return (struct Stack_t *)p;
}

/// instruction list
struct List_t
{
    struct Stack_t *instr;
    struct List_t *next;
};

struct List_t *New_List (struct Stack_t *instr, struct List_t *next)
{
    struct List_t *p = malloc (sizeof (*p));
    p->instr = instr;
    p->next = next;
    return p;
}

// "printer"
void Reverse_print_list (struct List_t *list)
{
    if (list == NULL) return;
    Reverse_print_list (list->next);
    //TODO();
    switch (list->instr->kind) {
        case STACK_PUSH: {
            struct Stack_Push *p = (struct Stack_Push *)(list->instr);
            printf ("\npush %d", p->i);
            break;
     }
        case STACK_ADD: {
            //struct Stack_Add *p = (struct Stack_Add *)list;
            printf ("\nadd");
            list=list->next;
            break;
     }
        default:
            break;
      }
}

//////////////////////////////////////////////////
// a compiler from Sum to Stack
struct List_t *all = 0;

void Emit (struct Stack_t *instr)
{
    all = New_List (instr, all);
}

void Compile (struct Exp_t *exp)
{
    switch (exp->kind){
        case EXP_INT:{
            struct Exp_Int *p = (struct Exp_Int *)exp;
            Emit (New_Stack_Push (p->i));
            break;
  }
        //TODO();
        case EXP_SUM:{
            struct Exp_Sum *p = (struct Exp_Sum *)exp;
            Compile (p->left);
            Compile (p->right);
            Emit (New_Stack_Add ());
            break;
  }
        default:
            break;
  }
}

//////////////////////////////////////////////////
// program entry
int main()
{
    printf("Compile starting\n");
    // build an expression tree:
    //            +
    //           / \
    //          +   4
    //         / \
    //        2   3
    struct Exp_t *exp = New_Exp_Sum (New_Exp_Sum(New_Exp_Int (2)
                                               , New_Exp_Int (3))
                                   , New_Exp_Int (4));
    // print out this tree:
    printf ("the expression is:\n");
    Print_Exp (exp);
    // compile this tree to Stack machine instructions
    Compile (exp);

    // print out the generated Stack instructons:
    Reverse_print_list (all);
  
    printf("\nCompile finished\n");
    return 0;
}

