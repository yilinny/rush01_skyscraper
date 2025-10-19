#include <stdio.h>
int *add_digit(int r, int c, int d, int *arr);
int *fill_row(int row, int *digit, int *arr);
int *fill_col(int col, int *digit, int *arr);
int *init_grid(int *arr);
int *find_marker(int digit, int *dest_buffer, int *marker);
void print_possibilities(int possibilities[4][4][4]);

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
            }
            col++;
        }
        row++;
    }
    return (buffer);
}

int scan_and_fill(int buffer[4][4][4], int *final_grid)
{
    int row;
    int col;
    int target_col;
    int pos;
    int digit;
    int filled = 0;
    row = 0;
    col = 0;
    pos = 0;
    digit = 0;
    while (digit < 4)
    {
        row = 0;
        while (row < 4)
        {
            col = 0;
            pos = 0;
            target_col = -1;
            while (col < 4)
            {
                if (buffer[row][col][digit] == 1)
                {
                    pos++;
                    target_col = col;
                }
                col++;
            }
            if (pos == 1) // only one possible cell in this row
            {
                if (final_grid[row * 4 + target_col] == 0)
                {
                    add_digit(row + 1, target_col + 1, digit + 1, final_grid);
                    filled = 1;
                }
            }
            row++;
        }
        digit++;
    }

    return (filled);
}

// sudoku each row to fill in a specific digit
// CANNOT USE FOR NOW, SCAN AND FILL CHANGED
void sudoku_digit(int digit, int *final_grid)
{
    int buffer[16]; // holds possibility. if it can be the digit, hold 0, else hold 1

    init_grid(buffer);
    mark_possibilities(digit, buffer, final_grid);
    // scan_and_fill(digit, buffer, final_grid);
    printf("end of sudoku digits\n");
    fflush(stdout);
}

// scans each row and fills in missing ones

// marker logic , compares towers in place and places based on what is obstructed
//  indices has to be in order and accounts for direction you are looking from
int *marker_logic(int seen, int *indices, int *final_grid)
{

    int index;
    int count_visible;
    int tallest;
    int zero_positions[4];
    int zero_count;
    int i;

    index = 0;
    count_visible = 0;
    tallest = 0;
    zero_count = 0;

    // Step 1: Scan through the line
    while (index < 4)
    {
        int val = final_grid[indices[index]];
        if (val == 0)
        {
            zero_positions[zero_count] = index;
            zero_count++;
        }
        else if (val > tallest)
        {
            tallest = val;
            count_visible++;
        }
        index++;
    }

    // Step 2: Estimate maximum possible visibility
    if (count_visible + zero_count == seen)
    {
        // Fill missing spots in ascending order after tallest
        i = 0;
        while (i < zero_count)
        {
            final_grid[indices[zero_positions[i]]] = tallest + 1 + i;
            i++;
        }
    }
    return (final_grid);
}

/*
int *three_markers(int *marker, int *final_grid)
{
    int four_poss[16];
    int three_poss[16];
    int check_indicies[4];
    int i;
    int j;
    j = 0;
    int positions[16]; // a buffer holding all the positions of three markers, max 16
    init_grid(four_poss);
    init_grid(three_poss);
    mark_possibilities(4, four_poss, final_grid);
    mark_possibilities(3, three_poss, final_grid);
    find_marker(3, positions, marker);
    while (positions[j] != 17)
    {
        four_poss[positions[j]] = 1;
        three_poss[positions[j]] = 1;
        i = 0;
        while (i < 4)
        {
            if (positions[j] < 4)
            {
                check_indicies[i] = positions[j] + i * 4;
            }
            else if (positions[j] < 8 && positions[j] >= 4)
            {
                check_indicies[i] = positions[j] - 4 * i;
            }
            else if (positions[j] >= 8 && positions[j] < 12)
            {
                check_indicies[i] = (positions[j] % 4) * 4 + i;
            }
            else
            {
                check_indicies[i] = (positions[j] % 4) * 4 + (3 - i);
            }
            i++;
        }
        marker_logic(3, check_indicies, final_grid);
        j++;
    }

    scan_and_fill(4, four_poss, final_grid);
    scan_and_fill(3, three_poss, final_grid);
    return (final_grid);
}
    */

// consider a separate function to loop across marker with a function as a parameter