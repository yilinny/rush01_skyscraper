#include <unistd.h>
#include <stdio.h>

// write to grid
int *add_digit(int row, int column, int digit, int *arr)
{
    int pointer;

    if (row > 4 || column > 4)
    {
        printf("Invalid input, returning original arr");
        return (arr);
    }

    pointer = (row - 1) * 4 + (column - 1);
    if (arr[pointer] == 0 || arr[pointer] == digit)
    {
        arr[pointer] = digit;
    }
    else
    {
        write(1, "Error\n", 6);
        printf("Overwriting a digit that is already there...");
        return (arr);
    }
    return (arr);
}

int *fill_row(int row, int *digit, int *arr)
{
    int col;

    col = 1;
    if (row > 4 || row < 0)
    {
        printf("Invalid call to fill_row");
        return (arr);
    }
    while (col < 5)
    {
        add_digit(row, col, digit[col - 1], arr);
        col++;
    }
    return (arr);
}

int *fill_col(int col, int *digit, int *arr)
{
    int row;

    row = 1;
    if (col > 4 || col < 0)
    {
        printf("Invalid call to fill_col, column size invalid");
    }
    while (row < 5)
    {
        add_digit(row, col, digit[row - 1], arr);
        row++;
    }
    return (arr);
}
// read marker eg. T1, L2, R3, B4
int read_from_marker(char *position, int *arr)
{
    int result;
    int marker;

    marker = 0;
    result = 0;

    if (position[0] == 'T' || position[0] == 'B')
    {
        marker = position[1] - '0';
        if (position[0] == 'T')
        {
            result = arr[marker - 1];
        }
        else
        {
            result = arr[4 + marker - 1];
        }
    }
    else if (position[0] == 'L' || position[0] == 'R')
    {
        marker = position[1] - '0';
        if (position[0] == 'L')
        {
            result = arr[9 + marker - 1];
        }
        else
        {
            result = arr[13 + marker - 1];
        }
    }
    else
    {
        printf("error in marker, row/column wrong, can only be r1r4 or c1c4");
    }

    return (result);
}

// print grid formats output, space between each character and new line at the end of the row.
void print_grid(int *str)
{
    int counter;
    char c;

    counter = 1;
    while (counter <= 16)
    {
        c = str[counter - 1] + '0'; // converts to character to allow for write function to run. digits only from 1-4, no 2 digit numbers
        write(1, &c, 1);
        if (counter % 4 == 0)
        {
            write(1, "\n", 1);
        }
        else
        {
            write(1, " ", 1);
        }
        counter++;
    }
}
