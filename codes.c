//Code 1: recog of tokens
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(const char *word) {
    const char *keywords[] = {
        "int", "float", "if", "else", "while", "for", "return", "void", "char",
        "double", "break", "continue", "switch", "case", "struct"
    };
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isDelimiter(char ch) {
    char delimiters[] = " ,;(){}[]";
    return strchr(delimiters, ch) != NULL;
}

int isOperator(char ch) {
    char operators[] = "+-*/%=";
    return strchr(operators, ch) != NULL;
}

int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    if (!input) {
        printf("Error opening input file.\n");
        return 1;
    }

    char ch, buffer[50];
    int index = 0;
    int insideComment = 0;

    while ((ch = fgetc(input)) != EOF) {
        if (ch == '/') {
            char next = fgetc(input);
            if (next == '/') {
                while ((ch = fgetc(input)) != EOF && ch != '\n');
                continue;
            } else if (next == '*') {
                insideComment = 1;
                while (insideComment && (ch = fgetc(input)) != EOF) {
                    if (ch == '*' && (ch = fgetc(input)) == '/') {
                        insideComment = 0;
                    }
                }
                continue;
            } else {
                ungetc(next, input);
            }
        }

        if (isalnum(ch) || ch == '_') {
            buffer[index++] = ch;
        } else {
            buffer[index] = '\0';
            index = 0;

            if (strlen(buffer) > 0) {
                if (isKeyword(buffer)) {
                    fprintf(output, "<keyword> ");
                } else if (isdigit(buffer[0])) {
                    fprintf(output, "<literal> ");
                } else {
                    fprintf(output, "<identifier> ");
                }
            }

            if (isOperator(ch)) {
                fprintf(output, "<operator> ");
            }

            if (isDelimiter(ch)) {
                fprintf(output, "<delimiter> ");
            }

            if (!isalnum(ch) && ch != ' ' && !isOperator(ch) && !isDelimiter(ch)) {
                fprintf(output, "<special_char> ");
            }
        }
    }

    fclose(input);
    fclose(output);

    output = fopen("output.txt", "r");
    if (output) {
        printf("\n--- Tokenized Output ---\n");
        char line[256];
        while (fgets(line, sizeof(line), output)) {
            printf("%s", line);
        }
        fclose(output);
    }

    return 0;
}


//code 2: left recursion

#include <stdio.h>
#include <string.h>

#define MAX_RULES 10
#define MAX_LEN 100

void eliminateLeftRecursion(char lhs, char rhs[MAX_RULES][MAX_LEN], int count) {
    char alpha[MAX_RULES][MAX_LEN], beta[MAX_RULES][MAX_LEN];
    int alphaCount = 0, betaCount = 0;

    for (int i = 0; i < count; i++) {
        if (rhs[i][0] == lhs) {
            strcpy(alpha[alphaCount++], rhs[i] + 1);
        } else {
            strcpy(beta[betaCount++], rhs[i]);
        }
    }

    if (alphaCount == 0) {
        printf("%c -> ", lhs);
        for (int i = 0; i < count; i++) {
            printf("%s%s", rhs[i], i == count - 1 ? "\n" : " | ");
        }
        return;
    }

    printf("%c -> ", lhs);
    for (int i = 0; i < betaCount; i++) {
        printf("%s%c' %s", beta[i], lhs, i == betaCount - 1 ? "\n" : "| ");
    }

    printf("%c' -> ", lhs);
    for (int i = 0; i < alphaCount; i++) {
        printf("%s%c' %s", alpha[i], lhs, i == alphaCount - 1 ? "\n" : "| ");
    }
    printf("e\n");
}

int main() {
    char lhs;
    int count;
    char rhs[MAX_RULES][MAX_LEN];

    printf("Enter LHS of production: ");
    scanf("%c", &lhs);
    getchar();

    printf("Enter number of RHS alternatives: ");
    scanf("%d", &count);
    getchar();

    for (int i = 0; i < count; i++) {
        printf("Enter RHS alternative %d: ", i + 1);
        fgets(rhs[i], MAX_LEN, stdin);
        rhs[i][strcspn(rhs[i], "\n")] = 0;
    }

    eliminateLeftRecursion(lhs, rhs, count);
    return 0;
}



//code 3: left factoring
#include <stdio.h>
#include <string.h>

#define MAX_RULES 10
#define MAX_LEN 100

void eliminateLeftFactoring(char lhs, char rhs[MAX_RULES][MAX_LEN], int count) {
    char commonPrefix[MAX_LEN];
    strcpy(commonPrefix, rhs[0]);

    for (int i = 1; i < count; i++) {
        int j = 0;
        while (commonPrefix[j] && rhs[i][j] && commonPrefix[j] == rhs[i][j]) {
            j++;
        }
        commonPrefix[j] = '\0';
    }

    if (strlen(commonPrefix) == 0) {
        printf("%c -> ", lhs);
        for (int i = 0; i < count; i++) {
            printf("%s%s", rhs[i], i == count - 1 ? "\n" : " | ");
        }
        return;
    }

    printf("%c -> %s%c'\n", lhs, commonPrefix, lhs);
    printf("%c' -> ", lhs);
    for (int i = 0; i < count; i++) {
        printf("%s%s", rhs[i] + strlen(commonPrefix), i == count - 1 ? "\n" : " | ");
    }
}

int main() {
    char lhs;
    int count;
    char rhs[MAX_RULES][MAX_LEN];

    printf("Enter LHS of production: ");
    scanf("%c", &lhs);
    getchar();

    printf("Enter number of RHS alternatives: ");
    scanf("%d", &count);
    getchar();

    for (int i = 0; i < count; i++) {
        printf("Enter RHS alternative %d: ", i + 1);
        fgets(rhs[i], MAX_LEN, stdin);
        rhs[i][strcspn(rhs[i], "\n")] = 0;
    }

    eliminateLeftFactoring(lhs, rhs, count);
    return 0;
}

//code 4: first and follow

-code not checked yet;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTIONS 50
#define MAX_SYMBOLS 50
#define MAX_SET_SIZE 50
#define MAX_PRODUCTION_LENGTH 100

// Structure to represent a production rule
typedef struct {
    char lhs;
    char rhs[MAX_PRODUCTION_LENGTH];
} Production;

// Structure to represent a set
typedef struct {
    char elements[MAX_SET_SIZE];
    int size;
} Set;

// Function prototypes
void initialize_sets(Set *first, Set *follow, int num_symbols);
int is_terminal(char c);
int is_epsilon(char *str);
void compute_first(Set *first, Production *productions, int num_productions);
void compute_follow(Set *first, Set *follow, Production *productions, int num_productions, char start_symbol);

int main() {
    int n;
    printf("Enter number of productions: ");
    scanf("%d", &n);
    
    Production *productions = (Production *)malloc(n * sizeof(Production));
    
    printf("Enter %d productions (format: A->BC):\n", n);
    for(int i = 0; i < n; i++) {
        printf("Production %d: ", i + 1);
        scanf("%c", &productions[i].lhs);  // Read lhs symbol
        
        // Skip '->' and whitespace
        getchar();  // Consume ->
        getchar();  // Consume space
        
        char temp[MAX_PRODUCTION_LENGTH];
        fgets(temp, MAX_PRODUCTION_LENGTH, stdin);
        temp[strcspn(temp, "\n")] = 0;
        strcpy(productions[i].rhs, temp);
    }
    
    // Initialize sets
    Set first[MAX_SYMBOLS] = {{0}, 0};
    Set follow[MAX_SYMBOLS] = {{0}, 0};
    initialize_sets(first, follow, n);
    
    // Find start symbol (first production's LHS)
    char start_symbol = productions[0].lhs;
    
    // Compute FIRST and FOLLOW sets
    compute_first(first, productions, n);
    compute_follow(first, follow, productions, n, start_symbol);
    
    // Display results
    printf("\nFIRST Sets:\n");
    for(int i = 0; i < n; i++) {
        printf("FIRST(%c) = {", productions[i].lhs);
        for(int j = 0; j < first[i].size; j++) {
            printf("%c", first[i].elements[j]);
            if(j < first[i].size - 1) printf(",");
        }
        printf("}\n");
    }
    
    printf("\nFOLLOW Sets:\n");
    for(int i = 0; i < n; i++) {
        printf("FOLLOW(%c) = {", productions[i].lhs);
        for(int j = 0; j < follow[i].size; j++) {
            printf("%c", follow[i].elements[j]);
            if(j < follow[i].size - 1) printf(",");
        }
        printf("}\n");
    }
    
    free(productions);
    return 0;
}

void initialize_sets(Set *first, Set *follow, int num_productions) {
    for(int i = 0; i < num_productions; i++) {
        first[i].elements[0] = '\0';
        follow[i].elements[0] = '\0';
    }
}

int is_terminal(char c) {
    return islower(c);
}

int is_epsilon(char *str) {
    return str[0] == '\0';
}

void compute_first(Set *first, Production *productions, int num_productions) {
    int changed;
    do {
        changed = 0;
        
        for(int i = 0; i < num_productions; i++) {
            char *rhs = productions[i].rhs;
            
            if(rhs[0] == '\0') {
                // Add epsilon to FIRST set
                int len = strlen(first[i].elements);
                if(len == 0 || first[i].elements[len-1] != '\0') {
                    first[i].elements[len] = '\0';
                    first[i].elements[len+1] = '\0';
                    changed = 1;
                }
            } else if(is_terminal(rhs[0])) {
                // Add terminal to FIRST set
                int len = strlen(first[i].elements);
                if(len == 0 || first[i].elements[len-1] != rhs[0]) {
                    first[i].elements[len] = rhs[0];
                    first[i].elements[len+1] = '\0';
                    changed = 1;
                }
            } else {
                // Handle non-terminal
                int pos = 0;
                while(pos < strlen(rhs)) {
                    // Find next non-terminal
                    while(pos < strlen(rhs) && is_terminal(rhs[pos])) pos++;
                    
                    if(pos < strlen(rhs)) {
                        // Get FIRST set of non-terminal
                        char nt = rhs[pos];
                        int nt_index = -1;
                        for(int j = 0; j < num_productions; j++) {
                            if(productions[j].lhs == nt) {
                                nt_index = j;
                                break;
                            }
                        }
                        
                        if(nt_index != -1) {
                            // Add elements from FIRST(non-terminal)
                            for(int j = 0; first[nt_index].elements[j]; j++) {
                                int len = strlen(first[i].elements);
                                if(len == 0 || first[i].elements[len-1] != first[nt_index].elements[j]) {
                                    first[i].elements[len] = first[nt_index].elements[j];
                                    first[i].elements[len+1] = '\0';
                                    changed = 1;
                                }
                            }
                        }
                    }
                    pos++;
                }
            }
        }
    } while(changed);
}

void compute_follow(Set *first, Set *follow, Production *productions, int num_productions, char start_symbol) {
    int changed;
    do {
        changed = 0;
        
        // Add $ to FOLLOW(start_symbol)
        int start_index = -1;
        for(int i = 0; i < num_productions; i++) {
            if(productions[i].lhs == start_symbol) {
                start_index = i;
                break;
            }
        }
        
        if(start_index != -1) {
            int len = strlen(follow[start_index].elements);
            if(len == 0 || follow[start_index].elements[len-1] != '$') {
                follow[start_index].elements[len] = '$';
                follow[start_index].elements[len+1] = '\0';
                changed = 1;
            }
        }
        
        // Process all productions
        for(int i = 0; i < num_productions; i++) {
            char *rhs = productions[i].rhs;
            for(int j = 0; j < strlen(rhs); j++) {
                if(!is_terminal(rhs[j])) {  // If current symbol is non-terminal
                    char A = productions[i].lhs;
                    char B = rhs[j];
                    int A_index = -1, B_index = -1;
                    
                    // Find indices
                    for(int k = 0; k < num_productions; k++) {
                        if(productions[k].lhs == A) A_index = k;
                        if(productions[k].lhs == B) B_index = k;
                    }
                    
                    if(A_index != -1 && B_index != -1) {
                        // Case 1: B is not the last symbol
                        if(j < strlen(rhs) - 1) {
                            char next = rhs[j + 1];
                            if(is_terminal(next)) {
                                int len = strlen(follow[B_index].elements);
                                if(len == 0 || follow[B_index].elements[len-1] != next) {
                                    follow[B_index].elements[len] = next;
                                    follow[B_index].elements[len+1] = '\0';
                                    changed = 1;
                                }
                            } else {
                                // Add FIRST(next) - {ε}
                                for(int k = 0; first[A_index].elements[k]; k++) {
                                    if(first[A_index].elements[k] != '\0') {
                                        int len = strlen(follow[B_index].elements);
                                        if(len == 0 || follow[B_index].elements[len-1] != first[A_index].elements[k]) {
                                            follow[B_index].elements[len] = first[A_index].elements[k];
                                            follow[B_index].elements[len+1] = '\0';
                                            changed = 1;
                                        }
                                    }
                                }
                            }
                        }
                        
                        // Case 2: B is the last symbol or ε is in FIRST(next)
                        if(j == strlen(rhs) - 1 || 
                           (j < strlen(rhs) - 1 && strchr(first[A_index].elements, '\0') != NULL)) {
                            for(int k = 0; follow[A_index].elements[k]; k++) {
                                int len = strlen(follow[B_index].elements);
                                if(len == 0 || follow[B_index].elements[len-1] != follow[A_index].elements[k]) {
                                    follow[B_index].elements[len] = follow[A_index].elements[k];
                                    follow[B_index].elements[len+1] = '\0';
                                    changed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while(changed);
}

//code 5: predictive parsing table, ll(1) check

//code 6: compute leading and trailing
#include <stdio.h>
#include <string.h>

int nt, t, top = 0;
char s[50], NT[10], T[10], st[50], l[10][10], tr[50][50];

int searchnt(char a) {
    for (int i = 0; i < nt; i++) {
        if (NT[i] == a)
            return i;
    }
    return -1;
}

int searchter(char a) {
    for (int i = 0; i < t; i++) {
        if (T[i] == a)
            return i;
    }
    return -1;
}

void push(char a) {
    s[top++] = a;
}

char pop() {
    return s[--top];
}

void installl(int a, int b) {
    if (l[a][b] == 'f') {
        l[a][b] = 't';
        push(T[b]);
        push(NT[a]);
    }
}

void installt(int a, int b) {
    if (tr[a][b] == 'f') {
        tr[a][b] = 't';
        push(T[b]);
        push(NT[a]);
    }
}

int main() {
    int i, s, k, j, n;
    char pr[30][30], b, c;

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions one by one: \n");
    for (i = 0; i < n; i++)
        scanf("%s", pr[i]);

    nt = 0;
    t = 0;

    // Identify Non-Terminals
    for (i = 0; i < n; i++) {
        if (searchnt(pr[i][0]) == -1)
            NT[nt++] = pr[i][0];
    }

    // Identify Terminals
    for (i = 0; i < n; i++) {
        for (j = 3; j < strlen(pr[i]); j++) {
            if (searchnt(pr[i][j]) == -1) {
                if (searchter(pr[i][j]) == -1)
                    T[t++] = pr[i][j];
            }
        }
    }

    // Initialize Leading and Trailing matrices
    for (i = 0; i < nt; i++) {
        for (j = 0; j < t; j++)
            l[i][j] = 'f';
    }

    for (i = 0; i < nt; i++) {
        for (j = 0; j < t; j++)
            tr[i][j] = 'f';
    }

    // Calculate Leading
    for (i = 0; i < nt; i++) {
        for (j = 0; j < n; j++) {
            if (NT[searchnt(pr[j][0])] == NT[i]) {
                if (searchter(pr[j][3]) != -1)
                    installl(searchnt(pr[j][0]), searchter(pr[j][3]));
                else {
                    for (k = 3; k < strlen(pr[j]); k++) {
                        if (searchnt(pr[j][k]) == -1) {
                            installl(searchnt(pr[j][0]), searchter(pr[j][k]));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (top != 0) {
        b = pop();
        c = pop();
        for (s = 0; s < n; s++) {
            if (pr[s][3] == b)
                installl(searchnt(pr[s][0]), searchter(c));
        }
    }

    // Print Leading Sets
    for (i = 0; i < nt; i++) {
        printf("Leading[%c]\t{", NT[i]);
        for (j = 0; j < t; j++) {
            if (l[i][j] == 't')
                printf("%c,", T[j]);
        }
        printf("}\n");
    }

    top = 0;

    // Calculate Trailing
    for (i = 0; i < nt; i++) {
        for (j = 0; j < n; j++) {
            if (NT[searchnt(pr[j][0])] == NT[i]) {
                if (searchter(pr[j][strlen(pr[j]) - 1]) != -1)
                    installt(searchnt(pr[j][0]), searchter(pr[j][strlen(pr[j]) - 1]));
                else {
                    for (k = strlen(pr[j]) - 1; k >= 3; k--) {
                        if (searchnt(pr[j][k]) == -1) {
                            installt(searchnt(pr[j][0]), searchter(pr[j][k]));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (top != 0) {
        b = pop();
        c = pop();
        for (s = 0; s < n; s++) {
            if (pr[s][3] == b)
                installt(searchnt(pr[s][0]), searchter(c));
        }
    }

    // Print Trailing Sets
    for (i = 0; i < nt; i++) {
        printf("Trailing[%c]\t{", NT[i]);
        for (j = 0; j < t; j++) {
            if (tr[i][j] == 't')
                printf("%c,", T[j]);
        }
        printf("}\n");
    }

    return 0;
}


//code 7: construct operator precedence table

//code 8: construct operator precedence function table

