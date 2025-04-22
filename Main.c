#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxlength 100
FILE *finput;//pointer to input file
FILE *foutput;//pointer to input file
 typedef struct node {
    char data[10];
    struct node* left;//ptr to left child
    struct node* right;//ptr to right child
}Tnode;
struct stack {
   Tnode* Tree; //struct tree creation
   struct stack* ptrafter;//next pointer
};

void saveToFile(char inputstring[], int Ninput);
void printpostfix(char inputstring[], int Ninput) ;
void evaluateEQ(char inputstring[], int Ninput);
void print(char inputstring[], int Ninput);
void readfromfile(char inputstring[], int *Ninput) ;
int findresult(Tnode* root);
Tnode*NewTree(char* input);
void convertinfexTOPostfix(char* infix, char* postfix);
int Operator(char ch);
int getoperatorprecedence(char ch) ;
Tnode* pop(struct stack** top);
void push(struct stack** top, Tnode* node);
Tnode* insert(char *newdata);
//fun to insert new node
Tnode* insert(char *newdata) {
    Tnode* node1 = (Tnode*) malloc(sizeof(Tnode));
    strcpy(node1->data, newdata);
    node1->left = node1->right = NULL;
    return node1;
}

// fun to select  inorder traversal  tree
void inorder(Tnode* node1) {
    if (node1 != NULL) {
        inorder(node1->left);
        printf("%s ", node1->data);
        inorder(node1->right);
    }
}
// fun to push  node to  stack
void push(struct stack** top, Tnode* node) {
   struct stack* newnode = (struct stack*) malloc(sizeof(struct stack));
    newnode->Tree = node;
    newnode->ptrafter = (*top);
    (*top) = newnode;
}
// fun to pop  node from  stack
Tnode* pop(struct stack** top) {
    //check if  stack  empty
    if (*top == NULL) {
        printf("erorr\n");
        exit(1);
    } else {
        struct stack* node1 = *top;
       Tnode* popped = node1->Tree;
       // top poin to the next node in stack
        *top = node1->ptrafter;
        free(node1);
        return popped;
    }
}
// fun to select the precedence of  operator
int getoperatorprecedence(char ch) {
    if (ch == '^') {
        return 3;  // ^ the first precedence
    } else if (ch == '*' || ch == '/' || ch == '%') {
        return 2;  // % * /  the next  precedence
    } else if (ch == '+' || ch == '-') {
        return 1;  // + ,- the lowest precedence
    } else {
        return -1;
    }
}
// function to check if a char is operator or not
int Operator(char ch) {
    switch (ch) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '%':
        return 1;
        default:
        return 0;
    }
}
//fun to convert  infix  to postfix
void convertinfexTOPostfix(char* infix, char* postfix) {
    int i, j = 0;
    char stack1[100];
    int TOP = -1;

    for (i = 0; infix[i]; ++i) {
        if (isdigit(infix[i])) {
            do {
                postfix[j++] = infix[i++];
                // add space after operand in  postfix
            } while (isdigit(infix[i]));
            postfix[j++] = ' ';
            i--;

        } else if (infix[i] == '(') {
            stack1[++TOP] = infix[i];
        } else if (infix[i] == ')') {
            while (TOP >= 0 && stack1[TOP] != '(') {
                postfix[j++] = stack1[TOP--];
                postfix[j++] = ' ';
            }
            //pop  opening from  stack
            if (TOP >= 0 && stack1[TOP] == '(') {
                TOP--;
            }
            // if char is  operator delete  operator from stack

        } else {
            while (TOP >= 0 && stack1[TOP] != '(' && getoperatorprecedence(infix[i]) <= getoperatorprecedence(stack1[TOP])) {
                postfix[j++] = stack1[TOP--];
                postfix[j++] = ' ';
            }
            stack1[++TOP] = infix[i];
        }
    }

    while (TOP >= 0) {
        postfix[j++] = stack1[TOP--];
        postfix[j++] = ' ';
    }

    postfix[j - 1] = '\0';
}
////fun to create  expression tree from a postfix
Tnode*NewTree(char* input) {//input in postfix
    struct Stack* stack1 = NULL;
    Tnode *node1, *node2, *node3;
    char pos[10];
    for (int i = 0; input[i]; i++) {
        if (input[i] == ' ')
            continue;
        if (!Operator(input[i])) {
            int j = 0;
            while (input[i] != ' ' &&input[i] != '\0') {
                pos[j++] = input[i++];
            }
            pos[j] = '\0';
            node1 = insert(pos);
            push(&stack1, node1);
        } else {
            /* pop 2 node from  stack */
            node1 = insert(&input[i]);
            node2 = pop(&stack1);
            node3 = pop(&stack1);
            //set them as chil of operator node,
            node1->right = node2;
            node1->left = node3;
            //push  operator node in the stack
            push(&stack1, node1);
        }
    }

    node1 = pop(&stack1);
    return node1;
}
// find the result of arithmetic equation  by tree
int findresult(Tnode* root) {
    // if  tree empty
    if (root==NULL)
        return 0;
 // if  node  leaf
    if (!root->left && !root->right)
    return atoi(root->data);

    int left = findresult(root->left);
    int right = findresult(root->right);
//select operation depand on  the operator in the root
    switch (root->data[0]) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            return left / right;
        case '%':
            return left % right;
        default:
            return 0;
    }
}
//function to read equation from file
void readfromfile(char inputstring[], int *Ninput) {
    char fileN[100];
    printf("Enter the input filename: ");
    scanf("%s", fileN);
    //open file
    finput= fopen(fileN, "r");
    if (finput == NULL) {
        printf("cannot open this file \n");
        *Ninput = 0;
        return;
    }

    *Ninput = 0;
    // read  from file
    while (fscanf(finput, "%99[^\n]%*c", &inputstring[*Ninput * 100]) == 1 && *Ninput < 100) {
        (*Ninput)++;
    }

    fclose(finput);
    printf(" equations read sucssfully  from %s\n", fileN);
}
// function to print equations
void print(char inputstring[], int Ninput) {
    printf("\n equations in the file:\n");
    //loop in equation that loaded by input file
    for (int i = 0; i < Ninput; i++) {
        printf("%d %s\n", i + 1, &inputstring[i * 100]);
    }
}
//fun to evaluate equations using  tree
void evaluateEQ(char inputstring[], int Ninput) {
    char post[100];
    printf("\n");
    for (int i = 0; i < Ninput; i++) {
        convertinfexTOPostfix(&inputstring[i * 100], post);
        // evaluate the expression tree
       Tnode* Tree =NewTree(post);
       //print result
        printf("%d\n", findresult(Tree));
    }
}
//fun to print postfix expresions
void printpostfix(char inputstring[], int Ninput) { //num of equations
   char post[100];
   printf("\n");
    for (int i = 0; i < Ninput; i++) {

        convertinfexTOPostfix(&inputstring[i * 100], post);
        printf(" %s\n",post);
    }
}
/*function to save output in file*/
void saveToFile(char inputstring[], int Ninput) {
    char NameFile[100];
    printf("enter File name to save output: ");
    scanf("%s", NameFile);

    foutput= fopen(NameFile, "w");
    //check if file open or not
    if (foutput == NULL) {
        printf("cannot open this file\n");
        return;
    }
    for (int i = 0; i < Ninput; i++) {
        char postfix[100];
        // convert to postfix
        convertinfexTOPostfix(&inputstring[i * 100], postfix);
        struct list* tree =NewTree(postfix);
        //print the result
        fprintf(foutput,"Result %d\n",findresult(tree));
    }

    fclose(foutput);
    printf("Results saved succsfully in %s\n", NameFile);
}
int main() {
    char equations1[maxlength];
    int Ninput = 0;
    int option;
//print menue
    while (1) {
        printf("read equations\n");
        printf("print equations\n");
        printf("evaluate using expression tree\n");
        printf("print postfix expressions\n");
        printf("save to output file\n");
        printf("exit\n");
        printf("Enter your option: ");
        scanf("%d", &option);
//switch to action option input
        switch (option) {
            case 1:
             readfromfile(equations1, &Ninput);
                break;
            case 2:
                print(equations1, Ninput);
                break;
            case 3:
               evaluateEQ(equations1, Ninput);
                break;
            case 4:
                printpostfix(equations1, Ninput);
                break;
            case 5:
                saveToFile(equations1, Ninput);
                break;
            case 6:
                printf("end...\n");
                exit(0);
            default:
                printf("invalid option.\n");
        }
    }

    return 0;
}
