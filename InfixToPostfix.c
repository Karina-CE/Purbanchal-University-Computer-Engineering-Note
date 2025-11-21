#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 100

struct Stack {
    int top;
    char items[MAX_SIZE];
};

void push(struct Stack *s, char c) {
    if (s->top == MAX_SIZE - 1) {
        printf("Stack Overflow\n");
        exit(EXIT_FAILURE);
    }
    s->items[++s->top] = c;
}

char pop(struct Stack *s) {
    if (s->top == -1) {
        printf("Stack Underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->items[s->top--];
}

int precedence(char op) {
    if (op == '^') return 3; // Highest precedence for exponential
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0; // For parentheses
}

int isOperand(char c) {
    return isalnum(c);
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int isOpeningParenthesis(char c) {
    return c == '(' || c == '{' || c == '[';
}

int isClosingParenthesis(char c) {
    return c == ')' || c == '}' || c == ']';
}

char* infixToPostfix(char* infix) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    int length = strlen(infix);
    char* postfix = (char*)malloc((length + 1) * sizeof(char));
    int j = 0;

    for (int i = 0; i < length; i++) {
        if (isOperand(infix[i])) {
            postfix[j++] = infix[i];
        } else if (isOpeningParenthesis(infix[i])) {
            push(stack, infix[i]);
        } else if (isClosingParenthesis(infix[i])) {
            while (stack->top != -1 && !isOpeningParenthesis(stack->items[stack->top])) {
                postfix[j++] = pop(stack);
            }
            if (stack->top == -1 || !isOpeningParenthesis(stack->items[stack->top])) {
                printf("Invalid expression: mismatched parenthesis\n");
                exit(EXIT_FAILURE);
            }
            pop(stack); // Discard the opening parenthesis
        } else if (isOperator(infix[i])) {
            while (stack->top != -1 && precedence(stack->items[stack->top]) >= precedence(infix[i])) {
                postfix[j++] = pop(stack);
            }
            push(stack, infix[i]);
        }
    }

    while (stack->top != -1) {
        if (isOpeningParenthesis(stack->items[stack->top])) {
            printf("Invalid expression: mismatched parenthesis\n");
            exit(EXIT_FAILURE);
        }
        postfix[j++] = pop(stack);
    }

    postfix[j] = '\0';
    free(stack);
    return postfix;
}

int main() {
    char infix[MAX_SIZE];
    printf("Enter an infix expression: ");
    fgets(infix, MAX_SIZE, stdin);
    infix[strcspn(infix, "\n")] = 0; // Remove trailing newline

    char* postfix = infixToPostfix(infix);
    printf("Postfix expression: %s\n", postfix);

    free(postfix);
    return 0;
}
