#include <stdio.h>
#include <unistd.h>

int *add_digit(int r, int c, int d, int *arr);
int read_from_marker(char *p, int *arr);
void print_grid(int *str);
int *four_markers(int *marker, int *final_grid);
int *one_markers(int *marker, int *final_grid);
int *three_markers(int *marker, int *final_grid);
void sudoku_digit(int digit, int *final_grid);

int *init_grid(int *arr)
{
    int pointer;
    pointer = 0;
    while (pointer < 16)
    {
        arr[pointer] = 0;
        pointer++;
    }
    return (arr);
}

int *extract_markers(char *input, int *dest)
{
    int pointer;
    int digit;
    int len;

    len = 0;
    while (input[len])
    {
        len++;
    }
    if (len != 31)
    {
        write(1, "Error\n", 6); // invalid input, not right amount of char
        printf("%d", len);
        printf("wrong number of input\n");
        return (dest);
    }
    pointer = 0;
    while (*input && pointer < 16)
    {
        digit = *input - '0';
        if (digit > 4 || digit < 1)
        {
            write(1, "Error\n", 6); // invalid input
            printf("Invlaid, marker can only be 0-4\n");
            return (dest);
        }
        dest[pointer] = digit;
        input += 2;
        pointer++;
    }
    return (dest);
}

int main(int argc, char *argv[])
{
    int final_grid[16];
    int markers[16];

    if (argc != 2 || !argv)
    {
        write(1, "Error\n", 6); // markers are taken as one input string
        return (0);
    }
    init_grid(final_grid);
    extract_markers(argv[1], markers);
    four_markers(markers, final_grid);
    one_markers(markers, final_grid);
    print_grid(final_grid);
    three_markers(markers, final_grid);
    sudoku_digit(3, final_grid);
    print_grid(final_grid);
    return (0);
}

/*helper function tests
int test_grid[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7};
print_grid(test_grid);
printf("now modifying grid, expecting 2 on r1c1\n");
add_digit(2, 3, 1, test_grid);
print_grid(test_grid);
*/

/*testing initilaization functions
print_grid(final_grid);
for (int i = 0; i < 16; i++)
{
    printf("%d", markers[i]);
}*/

/*testing possibility matrix

printf("Printing possibilities as a string ...\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
*/
