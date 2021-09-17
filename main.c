#include <stdio.h>

#define MAX_HEIGHT 25
#define MAX_WIDTH 25
#define point_is_within_image(x, y) x >= 0 && y >= 0 && x < MAX_HEIGHT && y < MAX_WIDTH
#define red(x, y) x < 0 && y < 0
#define yellow(x, y) x < 0 && y >= 0 && y < MAX_WIDTH
#define green(x, y) x < 0 && y >= MAX_WIDTH
#define magenta(x, y) x >= 0 && x < MAX_HEIGHT && y >= MAX_WIDTH
#define white(x, y) x >= MAX_HEIGHT && y >= MAX_WIDTH
#define blue(x, y) x >= MAX_HEIGHT && y >= 0 && y < MAX_WIDTH
#define black(x, y) x >= MAX_HEIGHT && y < 0
#define cyan(x, y) x >= 0 && x < MAX_HEIGHT && y < 0

void bucket_fill(int x, int y, int seed_color, int target_color);
void print_image();
int color_chosen(int x, int y);
void standardize_coors(int copy_point1[], int copy_point2[]);
int normal_copy_paste(int copy_point1[], int copy_point2[], int paste_point1[], int paste_point2[]);
int cosine(int theta);
int sine(int theta);
void rotate_and_copy(int aux_arr[][12], int image_arr[][12],
                     int startx, int endx, int starty, int endy);
int paste_rotated(int aux_arr[][12], int startx, int endx, int starty, int endy);

/* declaring image array as a global variable */
int image_arr[MAX_HEIGHT][MAX_WIDTH];

int main(void)
{
    char op_code;
    int i, j;

    /* Take input for image */
    for (i = 0; i < MAX_HEIGHT; i++)
    {
        for (j = 0; j < MAX_WIDTH; j++)
        {
            scanf("%d", &image_arr[i][j]);
        }
    }

    /* Get user's choice for the operation */
    scanf(" %c", &op_code);
    
    if (op_code == 'F')
    {
        int seed_x, seed_y, color_x, color_y;

        scanf("%d %d %d %d", &color_x, &color_y, &seed_x, &seed_y);

        bucket_fill(seed_x, seed_y, image_arr[seed_x][seed_y], color_chosen(color_x, color_y));
        print_image();
    }
    else if (op_code == 'P')
    {
        int copy_point1[2], copy_point2[2], paste_point1[2], paste_point2[2],
            num_of_changes = 0;

        scanf("%d %d %d %d %d %d %d %d", &copy_point1[0], &copy_point1[1],
              &copy_point2[0], &copy_point2[1], &paste_point1[0], &paste_point1[1],
              &paste_point2[0], &paste_point2[1]);

        standardize_coors(copy_point1, copy_point2);
        standardize_coors(paste_point1, paste_point2);

        num_of_changes = normal_copy_paste(copy_point1, copy_point2, paste_point1, paste_point2);

        print_image();
        printf("%d\n", num_of_changes);
    }
    else if (op_code == 'R')
    {
        int aux_arr90[12][12], aux_arr180[12][12], aux_arr270[12][12];
        int copy_point1[2], copy_point2[2], paste_point1[2], paste_point2[2],
            num_of_changes = 0, degree, size;
        char direction;
        
        scanf(" %c %d %d %d %d %d %d %d %d %d", &direction, &degree, &copy_point1[0], &copy_point1[1],
              &copy_point2[0], &copy_point2[1], &paste_point1[0], &paste_point1[1],
              &paste_point2[0], &paste_point2[1]);
        
        size = copy_point2[0] - copy_point1[0] + 1;

        if (degree == 0)
        {
            num_of_changes = normal_copy_paste(copy_point1, copy_point2, paste_point1, paste_point2);
            print_image();
            printf("%d\n", num_of_changes);
        }
        else
        {
            int temp_arr[12][12], i, j, l, m;
            for (i = copy_point1[0], l = 0; i <= copy_point2[0]; i++, l++)
            {
                for (j = copy_point1[1], m = 0; j <= copy_point2[1]; j++, m++)
                {
                    temp_arr[l][m] = image_arr[i][j];
                }
            }

            rotate_and_copy(aux_arr90, temp_arr, 0, size - 1, 0, size - 1);
            rotate_and_copy(aux_arr180, aux_arr90, 0, size - 1, 0, size - 1);
            rotate_and_copy(aux_arr270, aux_arr180, 0, size - 1, 0, size - 1);

            if (direction == 'R')
            {
                if (degree == 90)
                {
                    num_of_changes = paste_rotated(aux_arr90, paste_point1[0], paste_point2[0], paste_point1[1], paste_point2[1]);
                }
                else if (degree == 180)
                {
                    num_of_changes = paste_rotated(aux_arr180, paste_point1[0], paste_point2[0], paste_point1[1], paste_point2[1]);
                }
                else if (degree == 270)
                {
                    num_of_changes = paste_rotated(aux_arr270, paste_point1[0], paste_point2[0], paste_point1[1], paste_point2[1]);
                }
            }
            else if (direction == 'L')
            {
                if (degree == 90)
                {
                    num_of_changes = paste_rotated(aux_arr270, paste_point1[0], paste_point2[0], paste_point1[1], paste_point2[1]);
                }
                else if (degree == 180)
                {
                    num_of_changes = paste_rotated(aux_arr180, paste_point1[0], paste_point2[0], paste_point1[1], paste_point2[1]);
                }
                else if (degree == 270)
                {
                    num_of_changes = paste_rotated(aux_arr90, paste_point1[0], paste_point2[0], paste_point1[1], paste_point2[1]);
                }
            }

            print_image();
            printf("%d\n", num_of_changes);
        }
    }

    
    return 0;
}




















void bucket_fill(int x, int y, int seed_color, int target_color)
{
    if (!(point_is_within_image(x, y)))
    {
        return;
    }
    else if (image_arr[x][y] == target_color)
    {
        return;
    }
    else if (image_arr[x][y] != seed_color)
    {
        return;
    }

    image_arr[x][y] = target_color;

    bucket_fill(x - 1, y, seed_color, target_color); /* North */
    bucket_fill(x + 1, y, seed_color, target_color); /* South */
    bucket_fill(x, y + 1, seed_color, target_color); /* East */
    bucket_fill(x, y - 1, seed_color, target_color); /* West */
    bucket_fill(x - 1, y + 1, seed_color, target_color); /* North East */
    bucket_fill(x - 1, y - 1, seed_color, target_color); /* North West */
    bucket_fill(x + 1, y + 1, seed_color, target_color); /* South East */
    bucket_fill(x + 1, y - 1, seed_color, target_color); /* South West */
}

void print_image()
{
    int i, j;
    for (i = 0; i < MAX_HEIGHT; i++)
    {
        for (j = 0; j < MAX_WIDTH; j++)
        {
            printf("%d ", image_arr[i][j]);
        }
        printf("\n");
    }
}

int color_chosen(int x, int y)
{
    if (point_is_within_image(x, y))
    {
        return image_arr[x][y];
    }
    else if (red(x, y))
    {
        return 0;
    }
    else if (yellow(x, y))
    {
        return 1;
    }
    else if (green(x, y))
    {
        return 2;
    }
    else if (magenta(x, y))
    {
        return 3;
    }
    else if (white(x, y))
    {
        return 4;
    }
    else if (blue(x, y))
    {
        return 5;
    }
    else if (black(x, y))
    {
        return 6;
    }
    else
    {
        return 7;
    }
}

void standardize_coors(int copy_point1[], int copy_point2[])
{
    if (copy_point1[0] >= copy_point2[0] && copy_point2[1] >= copy_point1[1])
    {
        int temp_x;

        temp_x = copy_point1[0];
        copy_point1[0] = copy_point2[0];
        copy_point2[0] = temp_x;
    }
    else if (copy_point1[0] >= copy_point2[0] && copy_point1[1] >= copy_point2[1])
    {
        int temp_x, temp_y;

        temp_x = copy_point1[0];
        copy_point1[0] = copy_point2[0];
        copy_point2[0] = temp_x;

        temp_y = copy_point1[1];
        copy_point1[1] = copy_point2[1];
        copy_point2[1] = temp_y;
    }
    else if (copy_point1[0] <= copy_point2[0] && copy_point1[1] >= copy_point2[1])
    {
        int temp_y;

        temp_y = copy_point1[1];
        copy_point1[1] = copy_point2[1];
        copy_point2[1] = temp_y;
    }
}

int normal_copy_paste(int copy_point1[], int copy_point2[], int paste_point1[], int paste_point2[])
{
    int num_of_changes = 0, l, m, i, j;
    for (i = copy_point1[0], l = paste_point1[0];
             i <= copy_point2[0] && l <= paste_point2[0];
             i++, l++)
    {
        for (j = copy_point1[1], m = paste_point1[1];
             j <= copy_point2[1] && m <= paste_point2[1];
             j++, m++)
        {
            if (image_arr[l][m] != image_arr[i][j])
            {
                image_arr[l][m] = image_arr[i][j];
                num_of_changes++;
            }                        
        }
    }

    return num_of_changes;
}

int cosine(int theta)
{
    if (theta == 0)
    {
        return 1;
    }
    else if (theta == 180 || theta == -180)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int sine(int theta)
{
    if (theta == 90 || theta == -270)
    {
        return 1;
    }
    else if (theta == 270 || theta == -90)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void rotate_and_copy(int aux_arr[][12], int image_arr[][12],
                     int startx, int endx,
                     int starty, int endy)
{
    int i, j, x, y, size = endx - startx + 1;

    for (i = startx, y = size - 1; i <= endx; i++, y--)
    {
        for (j = starty, x = 0; j <= endy; j++, x++)
        {
            aux_arr[x][y] = image_arr[i][j];
        }
    }
}

int paste_rotated(int aux_arr[][12], int startx, int endx, int starty, int endy)
{
    int i, j, temp = starty, num_of_changes = 0;

    for (i = 0; startx <= endx; startx++, i++)
    {
        for (j = 0, starty = temp; starty <= endy; starty++, j++)
        {
            if (image_arr[startx][starty] != aux_arr[i][j])
            {
                image_arr[startx][starty] = aux_arr[i][j];
                num_of_changes++;
            }
        }
    }
    
    return num_of_changes;
}
