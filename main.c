#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct command_line {
    char operation[6];
    int regs[3];
}typedef command_line;

void parse(FILE *input, int *reg, int *memo);

void parse_debug(FILE *input, int *reg, int *memo);

void execute_command(char *in_string, int *reg, int *memo);

int main() {
    int regs[8] = {0};
    int memo[1024] = {0};


//    for (int i = 0; i < 8; ++i) {
//        printf("%s%d%s%d\n", "r", i, " = ", regs[i]);
//    }

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
        printf("press 'd' to enter debug mode\n");
        scanf("%s", debug);
        if (strcmp(debug, "d") == 0) {
            printf("running in a debug mode..\n");
            parse_debug(input, regs, memo);
        } else {
            printf("running in normal mode..\n");
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
        PC ++;
        execute_command(in_string, reg, memo);
//        char *pch = strtok(in_string, " ");
//        while (pch != NULL) {
//            printf("%s ", pch);
//            pch = strtok(NULL, " ");
//        }
//        printf("\n");
    }

}

void execute_command(char *in_string, int *reg, int *memo) {
    command_line command;
    char *pch = strtok(in_string, " ,");
    strcpy(command.operation, pch);
    printf("operation: %s\n", command.operation);
    printf("regs: ");
    int i = 0;
    while (pch != NULL && i < 3) {
      //  printf("%s\n",pch);
        pch = strtok(NULL, " ,");
        command.regs[i] = atoi(pch);
        printf("%d ", command.regs[i]);
        i++;
    }
    printf("\n");
}

void parse_debug(FILE *input, int *reg, int *memo) {
    printf("test");
}


