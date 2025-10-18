#include <stdio.h>
int *add_digit(int r, int c, int d, int *arr);
int read_from_marker(char *p, int *arr);
int *fill_row(int row, int *digit, int *arr);
int *fill_col(int col, int *digit, int *arr);
int *init_grid(int *arr);

int *four_markers(int *marker, int *final_grid)
{
    int row;
    int col;
    int pointer;

    pointer = 0;
    row = 0;
    col = 0;
    while (marker[pointer] && pointer < 16)
    {
        if (marker[pointer] == 4)
        {
            if (pointer > 7) // indicates row, fill in row
            {
                row = pointer % 4 + 1;
                if (pointer > 11)
                {
                    fill_row(row, (int[]){4, 3, 2, 1}, final_grid);
                }
                else
                {
                    fill_row(row, (int[]){1, 2, 3, 4}, final_grid);
                }
            }
            else
            {
                col = pointer % 4 + 1;
                if (pointer < 4)
                {
                    fill_col(col, (int[]){1, 2, 3, 4}, final_grid);
                }
                else
                {
                    fill_col(col, (int[]){4, 3, 2, 1}, final_grid);
                }
            }
        }
        pointer++;
    }
    return (final_grid);
}

int *one_markers(int *marker, int *final_grid)
{
    int row;
    int col;
    int index;

    row = 0;
    col = 0;
    index = 0;

    while (index < 16)
    {
        if (marker[index] == 1)
        {
            if (index < 8)
            {
                row = (index < 4) ? 1 : 4;
                col = (index % 4) + 1;
            }
            else
            {
                row = (index % 4) + 1;
                col = (index < 12) ? 1 : 4;
            }
            add_digit(row, col, 4, final_grid);
        }
        index++;
    }
    return (final_grid);
}

int *mark_row_column(int pointer, int *buffer)
{
    int index;
    index = pointer;
    // set everything in column to be 1
    while (index < 16)
    {
        buffer[index] = 1;
        index += 4;
    }
    index = pointer;
    while (index >= 0)
    {
        buffer[index] = 1;
        index -= 4;
    }
    // set_row to be 1
    index = pointer - pointer % 4;
    while (index < pointer - pointer % 4 + 4)
    {
        buffer[index] = 1;
        index++;
    }
    return (buffer);
}

int *mark_possibilities(int digit, int *buffer, int *final_grid)
{
    int row;
    int col;
    int pointer;

    row = 1;
    col = 1;
    while (row <= 4)
    {
        col = 1;
        while (col <= 4)
        {
            pointer = (row - 1) * 4 + (col - 1);
            if (final_grid[pointer])
            {
                buffer[pointer] = 1;
            }
            if (final_grid[pointer] == digit)
            {
                mark_row_column(pointer, buffer);
                row++;
                col = 1;
            }
            else
            {
                col++;
            }
        }
        row++;
    }
    return (buffer);
}

// sudoku each row to fill in a specific digit
int *sudoku_digit(int digit, int *final_grid)
{
    int row;
    int col;
    int pos; // possibilities
    int index;
    int target_col;
    int buffer[16]; // holds possibility. if it can be the digit, hold 0, else hold 1

    init_grid(buffer);
    mark_possibilities(digit, buffer, final_grid);

    row = 1;
    col = 1;
    pos = 0;
    while (row <= 4)
    {
        col = 1;
        pos = 0;
        while (col <= 4)
        {
            index = (row - 1) * 4 + (col - 1);
            if (buffer[index] == 0)
            {
                pos++;
                target_col = col;
            }
            col++;
        }
        if (pos == 1) // write only if there is one possibility in the row
        {
            add_digit(row, target_col, digit, final_grid);
        }
        row++;
    }
    return (final_grid);
}

int *three_markers(int *marker, int *final_grid)
{
    int four_poss[16];
    init_grid(four_poss);
    mark_possibilities(four_poss);

    int row;
    int col;
    int index;
    int pos;

    row = 1;
    col = 1;
    index = 0;

    while (index < 16)
    {
        if (marker[index] == 3)
        {
            if (index < 8)
            {
                row = (index < 4) ? 1 : 4;
                col = index % 4 + 1;
            }
            else
            {
                row = index % 4 + 1;
                col = (index < 12) ? 1 : 4;
            }
            pos = ((row - 1) * 4 + (col - 1));
            four_poss[pos] = 1;
        }
        index++;
    }
}

// consider a separate function to loop across marker with a function as a parameter