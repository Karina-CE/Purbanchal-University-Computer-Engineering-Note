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
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

int isOperand(char c) {
    return isalnum(c);
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

char* infixToPrefix(char* infix) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    int length = strlen(infix);
    char* prefix = (char*)malloc((length + 1) * sizeof(char));
    int j = 0;

    for (int i = length - 1; i >= 0; i--) {
        if (isOperand(infix[i])) {
            prefix[j++] = infix[i];
        } else if (infix[i] == ')') {
            push(stack, infix[i]);
        } else if (infix[i] == '(') {
            while (stack->top != -1 && stack->items[stack->top] != ')') {
                prefix[j++] = pop(stack);
            }
            if (stack->top == -1) {
                printf("Invalid expression: mismatched parenthesis\n");
                exit(EXIT_FAILURE);
            }
            pop(stack);
        } else if (isOperator(infix[i])) {
            while (stack->top != -1 && precedence(stack->items[stack->top]) > precedence(infix[i])) {
                prefix[j++] = pop(stack);
            }
            push(stack, infix[i]);
        }
    }

    while (stack->top != -1) {
        if (stack->items[stack->top] == '(') {
            printf("Invalid expression: mismatched parenthesis\n");
            exit(EXIT_FAILURE);
        }
        prefix[j++] = pop(stack);
    }

    prefix[j] = '\0';
    // Reverse the prefix string
    int len = strlen(prefix);
    for (int i = 0; i < len / 2; i++) {
        char temp = prefix[i];
        prefix[i] = prefix[len - i - 1];
        prefix[len - i - 1] = temp;
    }

    free(stack);
    return prefix;
}

int main() {
    char infix[MAX_SIZE];
    printf("Enter an infix expression: ");
    fgets(infix, MAX_SIZE, stdin);
    // Remove trailing newline character
    infix[strcspn(infix, "\n")] = 0;

    char* prefix = infixToPrefix(infix);
    printf("Prefix expression: %s\n", prefix);

    free(prefix);
    return 0;
}
