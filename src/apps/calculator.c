#include "applications.h"
#include "calculator.h"
#include "string.h"
#include "tty.h"

typedef struct {
    double stack[MAX_CALCULATOR_STACK_SIZE];
    int top;
} calc_stack;

static void stack_push(calc_stack *c_stack, double value)
{
    if (c_stack->top >= MAX_CALCULATOR_STACK_SIZE) {
	puts("Error: Stack overflow!");
	return;
    }

    c_stack->stack[++c_stack->top] = value;
}

static double stack_pop(calc_stack *c_stack)
{
    double val;
    
    if (c_stack->top < 0) {
	puts("Error: Stack underflow!");
    }

    val = c_stack->stack[c_stack->top];
    c_stack->top--;
    return val;
}

int calculator(char **args, int n_args)
{
    calc_stack c_stack;
    double a, b, res;

    memset(&c_stack.stack, 0, sizeof(c_stack.stack));
    c_stack.top = -1;

    if (n_args < 3) {
	puts("Insufficient arguments.");
	return 1;
    }

    for (int i = 1; i < n_args; i++) {
	if (isdigit(*args[i])) {
	    stack_push(&c_stack, atof(args[i]));
	    continue;
	}

	switch(*args[i]) {
	case '+':
	    if (i + 1 < n_args && isdigit(*args[i+1])) {
		// Indicates a +ve number of the form e.g. "+41394"
		stack_push(&c_stack, atof(args[i]));
	    }
	    else {
		a = stack_pop(&c_stack);
		b = stack_pop(&c_stack);
		stack_push(&c_stack, a + b);
	    }
	    break;
	case '-':
	    if (i + 1 < n_args && isdigit(*args[i+1])) {
		// Indicates a -ve number, e.g. "-1902"
		stack_push(&c_stack, atof(args[i]));
	    }
	    else {
		b = stack_pop(&c_stack);
		a = stack_pop(&c_stack);
		stack_push(&c_stack, a - b);
	    }
	    break;
	case '*':
	    a = stack_pop(&c_stack);
	    b = stack_pop(&c_stack);
	    stack_push(&c_stack, a * b);
	    break;
	case '/':
	    b = stack_pop(&c_stack);
	    a = stack_pop(&c_stack);

	    if (b == 0) {
		puts("Division by zero!");
		return 1;
	    }
	    stack_push(&c_stack, a / b);
	    break;
	}
    }

    res = stack_pop(&c_stack);
    puts(double_to_string(res));

    return 0;
}
