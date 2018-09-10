#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct command_line {
    char operation[6];
    int regs[3];
    int position;
} typedef command_line;

void parse(FILE *input, int *reg, int *memo);

void parse_debug(FILE *input, int *reg, int *memo);

void execute_command(char *in_string, int *reg, int *memo, int pc);

int main() {
    int regs[8] = {0};
    int memo[1024] = {0};

    char symbols[] = {"1234567890 #;"};
    char letters[] = {"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"};

    int PC = 0;

    char ins[32];
    char *p;
    p = ins;
    char *debug = calloc(1, sizeof(char));

    FILE *input, *output;
    input = fopen("..\\input.txt", "r");

    if (input == NULL) {
        printf("%s", "INPUT FILE ERROR");
        exit(100);
    } else {
        printf("Press 'd' to enter debug mode\n");
        printf("or something else to run in normal mode.\n");
        scanf("%s", debug);
        if (strcmp(debug, "d") == 0) {
            printf("Running in a debug mode..\n");
            parse_debug(input, regs, memo);
        } else {
            printf("Running in normal mode..\n");
            parse(input, regs, memo);

        }
    }

    fclose(input);

    output = fopen("..\\REGISTERS.txt", "w");
    for (int i = 0; i < 8; ++i) {
        fprintf(output, "%s%d%s%d\n", "r", i, " = ", regs[i]);
    }
    return 1;
}

void parse(FILE *input, int *reg, int *memo) {
    char symbols[] = {"1234567890 #;"};
    char letters[] = {"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"};
    int PC = 0;

    while (!feof(input)) {
        char *in_string = calloc(32, sizeof(char));
        fgets(in_string, 32, input);
        execute_command(in_string, reg, memo, PC);
        PC++;
    }
}

void execute_command(char *in_string, int *reg, int *memo, int pc) {
    command_line command;
    command.position = pc;
    for (int j = 0; j < 3; ++j) command.regs[j] = 0;

    char *pch = strtok(in_string, " ,");
    strcpy(command.operation, pch);

    int i = 0;
    while (pch != NULL && i < 3) {
        pch = strtok(NULL, " ,");
        command.regs[i] = atoi(pch);
        i++;
    }
    printf("############\n");
    printf("Position: %d\n", command.position);
    printf("Operation: %s\n", command.operation);
    printf("Regs: %d %d %d\n", command.regs[0], command.regs[1], command.regs[2]);
    printf("############\n");
}

void parse_debug(FILE *input, int *reg, int *memo) {
    char *step = calloc(1, sizeof(char));
    int PC = 0;
    printf("Program now is in debugging mode.\n");
    printf("After each step all registers will be shown here\n");
    while (1) {
        printf("Press 'd' to take a single step.\n");
        printf("Press 'x' to exit the program.\n");
        scanf("%s", step);
        if (strcmp(step, "x") == 0) {
            printf("closing..\n");
            exit(1);
        } else if (strcmp(step, "d") == 0 && !feof(input)) {
            char *in_string = calloc(32, sizeof(char));
            fgets(in_string, 32, input);
            execute_command(in_string, reg, memo, PC);
            PC++;
        } else {
            printf("unknown command, please try again\n");
        }
    }

}


