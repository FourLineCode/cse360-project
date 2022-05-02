#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

void read_loop();
void read_file(char *file_path);
void execute(char *command);
char *str_lower(char *str, size_t len);
int check_register(char *reg);
int fetch_operand(char *op);
void print_state();

void add(char *reg, char *op);
void sub(char *reg, char *op);
void mul(char *reg, char *op);
void divide(char *reg, char *op);
void bitwise_and(char *reg, char *op);
void bitwise_or(char *reg, char *op);
void bitwise_not(char *reg);
void load(char *reg, char *op);
void store(char *dest, char *op);

bool VERBOSE = false;
bool INTERACTIVE = false;

// Holds registers values (0 - X | 1 - Y)
int registers[2] = {0, 0};
// Holds values for memory address (0-7) (32bit)
int memory[8] = {0, 1, 2, 3, 4, 5, 6, 7};

int main(int argc, char **argv)
{
    if (argc > 2 && !strcmp(argv[2], "-v"))
    {
        VERBOSE = true;
    }

    char *interactive_flag = "-i";
    if (argc > 1 && !strcmp(argv[1], interactive_flag))
    {
        INTERACTIVE = true;
        while (true)
        {
            read_loop();
        }
        printf("# Result:");
        print_state();
    }
    else if (argc > 1)
    {
        read_file(argv[1]);
        printf("# Result:");
        print_state();
    }
    else
    {
        fprintf(stderr, "Error: either pass assembly file path or pass -i for interactive mode\n");
        printf("Usage: ./main <program.asm>\n");
        printf("\t./main -i\n");
    }

    return EXIT_SUCCESS;
}

// Reads instructions from stdin
void read_loop()
{
    char *line = NULL;
    size_t buffer_size = 0;
    printf("asm> ");
    getline(&line, &buffer_size, stdin);

    execute(line);
}

// Reads an assembly file line by line and executes
void read_file(char *file_path)
{
    FILE *file;
    char *line = NULL;
    size_t buffer_len = 0, read;

    file = fopen(file_path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: file does not exist - \"%s\"\n", file_path);
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &buffer_len, file)) != -1)
    {
        execute(line);
    }

    if (line)
    {
        free(line);
    }
    fclose(file);
}

// Execute the given instruction string
void execute(char *command)
{
    char *command_lowered = str_lower(command, strlen(command));

    if (!strcmp(command_lowered, "\n"))
    {
        return;
    }

    char command_args[3][32];
    int arg_count = 0, j = 0;
    for (int i = 0; i <= strlen(command_lowered); i++)
    {
        if (command_lowered[i] == ' ' || command_lowered[i] == '\0' || command_lowered[i] == '\n')
        {
            command_args[arg_count][j] = '\0';
            if (arg_count == 2)
            {
                break;
            }
            else
            {
                arg_count++;
                j = 0;
            }
        }
        else
        {
            command_args[arg_count][j++] = command_lowered[i];
        }
    }

    if (!strcmp(command_args[0], "exit"))
    {
        printf("Exited program successfully\n");
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(command_args[0], "add"))
    {
        add(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "sub"))
    {
        sub(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "mul"))
    {
        mul(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "div"))
    {
        divide(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "and"))
    {
        bitwise_and(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "or"))
    {
        bitwise_or(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "not"))
    {
        bitwise_not(command_args[1]);
    }
    else if (!strcmp(command_args[0], "load"))
    {
        load(command_args[1], command_args[2]);
    }
    else if (!strcmp(command_args[0], "str"))
    {
        store(command_args[1], command_args[2]);
    }
    else
    {
        fprintf(stderr, "Error: unknown operation - \"%s\"\n", command_args[0]);
        exit(EXIT_FAILURE);
    }

    free(command_lowered);
}

// Lowercase the given string
char *str_lower(char *str, size_t len)
{
    char *str_lowered = calloc(len + 1, sizeof(char));

    for (size_t i = 0; i < len; ++i)
    {
        str_lowered[i] = tolower((unsigned char)str[i]);
    }

    return str_lowered;
}

// Checks if given register is a valid register type (X/Y)
int check_register(char *reg)
{
    if (strcmp(reg, "x") != 0 && strcmp(reg, "y") != 0)
    {
        fprintf(stderr, "Error: invalid register - \"%s\"\n", reg);
        exit(EXIT_FAILURE);
    }

    if (!strcmp(reg, "x"))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Fetches value from register or memory by given operand string
int fetch_operand(char *op)
{
    if (!strcmp(op, "x"))
    {
        return registers[0];
    }
    else if (!strcmp(op, "y"))
    {
        return registers[1];
    }
    else if (strlen(op) > 2 && op[0] == '0' && op[1] == 'x')
    {
        int mem_index = atoi(&op[2]);
        return memory[mem_index];
    }
    else
    {
        return atoi(op);
    }
}

// Print current state of cpu registers and memory
void print_state()
{
    printf("\nRegister X: %d | Register Y: %d\n", registers[0], registers[1]);
    printf("Memory locations: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", memory[i]);
    }
    printf("\n\n");
}

// Add value to given register
void add(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] += operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nAdded %d to register %s\n", operand, reg_name);
        print_state();
    }
}

// Subtract value from given register
void sub(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] -= operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nSubtracted %d from register %s\n", operand, reg_name);
        print_state();
    }
}

// Multiply given resiter by value
void mul(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] *= operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nMultiplied register %s by %d\n", reg_name, operand);
        print_state();
    }
}

// Divide given register by value
void divide(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] /= operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nDivided register %s by %d\n", reg_name, operand);
        print_state();
    }
}

// Bitwise AND given register by value
void bitwise_and(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] &= operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nBitwise AND register %s by %d\n", reg_name, operand);
        print_state();
    }
}

// Bitwise OR given register by value
void bitwise_or(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] |= operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nBitwise OR register %s by %d\n", reg_name, operand);
        print_state();
    }
}

// Bitwise NOT given register
void bitwise_not(char *reg)
{
    int reg_index = check_register(reg);

    registers[reg_index] = !registers[reg_index];

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nBitwise NOT register %s\n", reg_name);
        print_state();
    }
}

// Load value to given register
void load(char *reg, char *op)
{
    int reg_index = check_register(reg);
    int operand = fetch_operand(op);

    registers[reg_index] = operand;

    if (VERBOSE || INTERACTIVE)
    {
        char *reg_name = reg_index == 0 ? "X" : "Y";
        printf("\nLoaded %d to register %s\n", operand, reg_name);
        print_state();
    }
}

// Store value to given register or memory address
void store(char *dest, char *op)
{
    int operand = fetch_operand(op);
    char *message = calloc(32, sizeof(char));

    if (!strcmp(dest, "x"))
    {
        registers[0] = operand;
        message = "register X";
    }
    else if (!strcmp(dest, "y"))
    {
        registers[1] = operand;
        message = "register Y";
    }
    else if (strlen(dest) > 2 && dest[0] == '0' && dest[1] == 'x')
    {
        int mem_index = atoi(&dest[2]);
        memory[mem_index] = operand;
        snprintf(message, 32, "memory location %d", mem_index);
    }
    else
    {
        fprintf(stderr, "Error: invalid destination address - \"%s\"\n", dest);
        exit(EXIT_FAILURE);
    }

    if (VERBOSE || INTERACTIVE)
    {
        printf("\nStored value %d to %s\n", operand, message);
        print_state();
    }
}
