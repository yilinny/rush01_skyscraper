#include <stdio.h>

int *add_digit(int r, int c, int d, int *arr);
void print_grid(int *str);
int scan_and_fill(int possibilities[4][4][4], int *final_grid);
int *apply_basic_rules(int possibilities[4][4][4], int *final_grid);

void init_possibilities(int possibilities[4][4][4])
{
    int row, col, digit;

    row = 0;
    while (row < 4)
    {
        col = 0;
        while (col < 4)
        {
            digit = 0;
            while (digit < 4)
            {
                possibilities[row][col][digit] = 1; // all possible
                digit++;
            }
            col++;
        }
        row++;
    }
}

// creates array of indices based on direction of marker
int *create_line_indices(int index, int *line)
{
    int is_col, from_top_or_left, i;

    is_col = (index < 8);
    from_top_or_left = (index % 8) < 4; // 0-3 when %8 = top or from left if row

    i = 0;
    while (i < 4)
    {
        if (is_col)
        {
            line[i] = from_top_or_left ? (i * 4 + index % 4) : ((3 - i) * 4 + index % 4);
        }
        else
        {
            line[i] = from_top_or_left ? ((index % 4) * 4 + i) : ((index % 4) * 4 + (3 - i));
        }
        i++;
    }
    return (line);
}

// Helper: compute visibility for a full line of 4 digits
int compute_visibility(int *line_values)
{
    int tallest = 0;
    int visible = 0;
    for (int i = 0; i < 4; i++)
    {
        if (line_values[i] > tallest)
        {
            tallest = line_values[i];
            visible++;
        }
    }
    return visible;
}

// Helper: generate all permutations of an array (backtracking)
void permute(int *arr, int l, int r, int perms[][4], int *count)
{
    if (l == r)
    {
        for (int i = 0; i < 4; i++)
            perms[*count][i] = arr[i]; // manually copy array
        (*count)++;
    }
    else
    {
        for (int i = l; i <= r; i++)
        {
            int temp = arr[l];
            arr[l] = arr[i];
            arr[i] = temp;

            permute(arr, l + 1, r, perms, count);

            temp = arr[l];
            arr[l] = arr[i];
            arr[i] = temp;
        }
    }
}

// Forward-checking with full permutations (no memcpy)
void check_partial_visibility(int clue, int *line, int possibilities[4][4][4], int *final_grid)
{
    int empty_cells[4];
    int empty_count = 0;
    int used_digits[5] = {0}; // digits 1..4

    // Identify empty cells and used digits
    for (int i = 0; i < 4; i++)
    {
        int val = final_grid[line[i]];
        if (val == 0)
            empty_cells[empty_count++] = line[i];
        else
            used_digits[val] = 1;
    }

    // Generate remaining possible digits for empty cells
    int remaining_digits[4];
    int rem_count = 0;
    for (int d = 1; d <= 4; d++)
        if (!used_digits[d])
            remaining_digits[rem_count++] = d;

    // Generate all permutations of remaining digits
    int perms[24][4]; // 4! = 24 max permutations
    int perm_count = 0;
    permute(remaining_digits, 0, rem_count - 1, perms, &perm_count);

    // Check each empty cell and each possible digit
    for (int i = 0; i < empty_count; i++)
    {
        int cell = empty_cells[i];

        for (int d = 1; d <= 4; d++)
        {
            if (possibilities[cell / 4][cell % 4][d - 1] == 0)
                continue;

            int can_satisfy = 0;

            // Try placing d in this cell, then fill others with all permutations
            for (int p = 0; p < perm_count; p++)
            {
                int temp_line[4];
                int perm_idx = 0;

                // Fill line with existing digits + candidate
                for (int j = 0; j < 4; j++)
                {
                    int idx = line[j];
                    if (final_grid[idx] != 0)
                        temp_line[j] = final_grid[idx];
                    else if (idx == cell)
                        temp_line[j] = d;
                    else
                        temp_line[j] = perms[p][perm_idx++];
                }

                if (compute_visibility(temp_line) == clue)
                {
                    can_satisfy = 1;
                    break;
                }
            }

            if (!can_satisfy)
                possibilities[cell / 4][cell % 4][d - 1] = 0; // mark impossible
        }
    }
}

void print_possibilities(int possibilities[4][4][4])
{
    int layer[16];

    printf("\n=== POSSIBILITY LAYERS ===\n");

    for (int digit = 0; digit < 4; digit++)
    {
        // Build a 4×4 grid showing where this digit is possible (0 = possible, 1 = blocked)
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                int index = row * 4 + col;
                layer[index] = possibilities[row][col][digit];
            }
        }

        printf("Digit %d possibilities:\n", digit + 1);
        print_grid(layer);
        printf("\n");
    }

    printf("===========================\n\n");
}

// update possibility matrix by visbility
int *update_poss_by_visibility(int possibilities[4][4][4], int *marker, int *final_grid)
{
    int index;
    int clue;
    int line[4];

    index = 0;
    while (index < 16)
    {
        clue = marker[index];
        if (clue > 0)
        {
            create_line_indices(index, line);
        }
        if (clue == 2)
        {
            // first cell cannot be 4, and if last cell is empty (will only be filled if empty), it cannot be 1
            possibilities[line[0] / 4][line[0] % 4][3] = 0;
        }
        if (clue == 3)
        {
            // first cell cannot be 4 or 3
            possibilities[line[0] / 4][line[0] % 4][2] = 0;
            possibilities[line[0] / 4][line[0] % 4][3] = 0;
        }
        check_partial_visibility(clue, line, possibilities, final_grid);
        index++;
    }
    return (final_grid);
}
// remove impossible digits if number already placed
int *update_possibilities(int possibilities[4][4][4], int *marker, int *final_grid)
{
    int row, col, digit;
    row = 0;
    while (row < 4)
    {
        col = 0;
        while (col < 4)
        {
            int val = final_grid[row * 4 + col];
            if (val != 0)
            {
                // This cell fixed: only that digit possible
                digit = 0;
                while (digit < 4)
                {
                    possibilities[row][col][digit] = (digit == val - 1);
                    digit++;
                }

                // Eliminate same digit from row and column
                int i = 0;
                while (i < 4)
                {
                    if (i != col)
                        possibilities[row][i][val - 1] = 0;
                    if (i != row)
                        possibilities[i][col][val - 1] = 0;
                    i++;
                }
            }
            col++;
        }
        row++;
    }

    // visibility constraints for probability matrix
    update_poss_by_visibility(possibilities, marker, final_grid);
    return final_grid;
}

// find cells with only one possible number
int fill_determined_cells(int possibilities[4][4][4], int *final_grid)
{
    int r, c, d, count, last_possible, filled_any;
    filled_any = 0;

    r = 0;
    // fills if it is the only possibility in that cell
    while (r < 4)
    {
        c = 0;
        while (c < 4)
        {
            if (final_grid[r * 4 + c] == 0)
            {
                d = 0;
                count = 0;
                while (d < 4)
                {
                    if (possibilities[r][c][d] == 1)
                    {
                        count++;
                        last_possible = d + 1;
                    }
                    d++;
                }

                if (count == 1)
                {
                    add_digit(r + 1, c + 1, last_possible, final_grid);
                    filled_any = 1;
                }
            }
            c++;
        }
        r++;
    }
    // check row and column, fills if it is the only possible digit for that row/column
    filled_any = scan_and_fill(possibilities, final_grid);
    return filled_any;
}

void recursive_fill(int possibilities[4][4][4], int *final_grid, int *marker)
{
    int changed;

    update_possibilities(possibilities, marker, final_grid);
    changed = fill_determined_cells(possibilities, final_grid);

    if (changed)
    {
        recursive_fill(possibilities, final_grid, marker);
    }
}
