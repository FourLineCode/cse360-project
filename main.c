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

bool VERBOSE = false;

long double register_x = 0, register_y = 0;
long double memory[8];

int main(int argc, char **argv)
{
    if (argc > 2 && !strcmp(argv[2], "-v"))
    {
        VERBOSE = true;
    }

    char *interactive_flag = "-i";
    if (argc > 1 && !strcmp(argv[1], interactive_flag))
    {
        while (true)
        {
            read_loop();
        }
    }
    else if (argc > 1)
    {
        read_file(argv[1]);
    }
    else
    {
        // TODO: print instructions
        fprintf(stderr, "Error: either pass assembly file path or pass -i for interactive mode\n");
    }

    return EXIT_SUCCESS;
}

void read_loop()
{
    char *line = NULL;
    size_t buffer_size = 0;
    getline(&line, &buffer_size, stdin);

    execute(line);
}

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

void execute(char *command)
{
    char *command_lowered = str_lower(command, strlen(command));

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
        exit(EXIT_SUCCESS);
        return;
    }
    else if (!strcmp(command_args[0], "add"))
    {
        printf("add\n");
    }

    free(command_lowered);
}

char *str_lower(char *str, size_t len)
{
    char *str_lowered = calloc(len + 1, sizeof(char));

    for (size_t i = 0; i < len; ++i)
    {
        str_lowered[i] = tolower((unsigned char)str[i]);
    }

    return str_lowered;
}
