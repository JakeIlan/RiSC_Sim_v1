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

void execute_command(char *in_string, int *reg, int *memo, FILE *input);

void branch_to(int pos, FILE *input);

int main() {
    int regs[8] = {0};
    int memo[1024] = {0};

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
    for (int i = 0; i < 7; ++i) {
        fprintf(output, "%s%d%s%d\n", "r", i, " = ", regs[i]);
    }
    fprintf(output, "%s%d%s%d", "r", 7, " = ", regs[7]);
    fclose(output);
    return 1;
}

void parse(FILE *input, int *reg, int *memo) {

    char *in_string = calloc(32, sizeof(char));

    while (!feof(input)) {
        fgets(in_string, 32, input);
        execute_command(in_string, reg, memo, input);
    }
}

void execute_command(char *in_string, int *reg, int *memo, FILE *input) {
    static int PC = 0;
    printf("PC = %d\n", PC);
    printf("-----------\n");
    command_line command;
    command.position = PC;
    for (int j = 0; j < 3; ++j) command.regs[j] = 0;

    char *pch = strtok(in_string, " ,");
    if (pch == NULL){
        printf("Input is empty.");
        exit(101);
    }
    strcpy(command.operation, pch);

    int i = 0;
    while (pch != NULL && i < 3) {
        pch = strtok(NULL, " ,");
        command.regs[i] = atoi(pch);
        i++;
    }
    if ((command.regs[0] < 0 || command.regs[0] > 7 ||
        command.regs[1] < 0 || command.regs[1] > 7) &&
        strcmp(command.operation, "lui") != 0) {
        printf("Input error:\n");
        printf("Can't reach register at line %d\n", command.position);
        printf("Registers most be in a range 0 - 7");
        exit(102);
    }

    if (strcmp(command.operation, "add") == 0) {
        if (command.regs[2] < 0 || command.regs[2] > 7) {
            printf("Input error:\n");
            printf("Can't reach register at line %d\n", command.position);
            printf("Registers most be in a range 0 - 7");
            exit(102);
        }
        printf("performing ADD\n");
        reg[command.regs[0]] = reg[command.regs[1]] + reg[command.regs[2]];
        PC++;
    } else if (strcmp(command.operation, "addi") == 0) {
        printf("performing ADDI\n");
        reg[command.regs[0]] = reg[command.regs[1]] + command.regs[2];
        PC++;

    } else if (strcmp(command.operation, "nand") == 0) {
        if (command.regs[2] < 0 || command.regs[2] > 7) {
            printf("Input error:\n");
            printf("Can't reach register at line %d\n", command.position);
            printf("Registers most be in a range 0 - 7");
            exit(102);
        }
        reg[command.regs[0]] = reg[command.regs[1]] - reg[command.regs[2]];
        PC++;

    } else if (strcmp(command.operation, "lui") == 0) {
        if (command.regs[0] < 0 || command.regs[0] > 7) {
            printf("Input error:\n");
            printf("Can't reach register at line %d\n", command.position);
            printf("Registers most be in a range 0 - 7");
            exit(102);
        }
        reg[command.regs[0]] = (int)(command.regs[1] & 01111111111000000);
        PC++;

    } else if (strcmp(command.operation, "sw") == 0) {
        memo[command.regs[1] + command.regs[2]] = reg[command.regs[0]];
        PC++;
    } else if (strcmp(command.operation, "lw") == 0) {
        reg[command.regs[0]] = memo[command.regs[1] + command.regs[2]];
        PC++;
    } else if (strcmp(command.operation, "nop") == 0) {
        PC++;
    } else if (strcmp(command.operation, "beq") == 0) {
        printf("performing BEQ\n");
        if (reg[command.regs[0]] == reg[command.regs[1]]) {
            branch_to(command.position + 1 + command.regs[2], input);
            PC += 1 + command.regs[2];
            printf("jumping to %d\n", PC);

        } else {
            printf("not jumping\n");
            PC++;
        }
    } else if (strcmp(command.operation, "jalr") == 0) {
        branch_to(reg[command.regs[1]], input);
        PC = reg[command.regs[1]];
        reg[command.regs[0]] = PC + 1;
    } else if (strcmp(command.operation, "halt") == 0) {
        printf("Complete.");
        exit(10);
    } else {
        printf("Input error:\n");
        printf("Unknown command in line %d\n", command.position);
        exit(102);
    }

    FILE *output;

    output = fopen("..\\REGISTERS.txt", "w");
    for (int j = 0; j < 7; ++j) {
        fprintf(output, "%s%d%s%d\n", "r", j, " = ", reg[j]);
    }
    fprintf(output, "%s%d%s%d", "r", 7, " = ", reg[7]);
    fclose(output);
}

void branch_to(int pos, FILE *input) {
    freopen("..\\input.txt", "r", input);
    char *in_string = calloc(32, sizeof(char));

    while (!feof(input) && pos != 0) {
        fgets(in_string, 32, input);
        pos--;
    }
}

void parse_debug(FILE *input, int *reg, int *memo) {
    char *step = calloc(1, sizeof(char));
    char *in_string = calloc(32, sizeof(char));
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
            fgets(in_string, 32, input);
            execute_command(in_string, reg, memo, input);
            for (int i = 0; i < 8; ++i) {
                printf("%s%d%s%d\n", "r", i, " = ", reg[i]);
            }
        } else if (!feof(input)) {
            printf("unknown command, please try again\n");
        } else {
            printf("You have reached the EoF, closing..\n");
            exit(1);
        }
    }
}


