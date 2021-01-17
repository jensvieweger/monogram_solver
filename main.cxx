#include <iostream>
#include <random>
#include <list>

#ifndef NDEBUG
#define D(x) (x)
#else
#define D(x) \
    do       \
    {        \
    } while (0)
#endif

struct numberline
{
    int cnt;
    int *nums;
};

struct map
{
    int xres, yres;
    struct numberline *cols;
    struct numberline *rows;
    bool **image;
    bool **solution;
};

void make_map(struct map *map)
{
// #define XSIZE 8
// #define YSIZE 9

//     int xsize = XSIZE;
//     int ysize = YSIZE;
//     int cols[XSIZE][2] = {{1, 2}, {3, 1}, {1, 5}, {7, 1}, {5, 0}, {3, 0}, {4, 0}, {3, 0}};
//     int rows[YSIZE][2] = {{3, 0}, {2, 1}, {3, 2}, {2, 2}, {6, 0}, {1, 5}, {6, 0}, {1, 0}, {2, 0}};

//     /* Solution:
// . # # # . . . .
// # # . # . . . .
// . # # # . . # #
// . . # # . . # #
// . . # # # # # #
// # . # # # # # .
// # # # # # # . .
// . . . . # . . .
// . . . # # . . .
// */
//     bool solution[YSIZE][XSIZE] = {{0, 1, 1, 1, 0, 0, 0, 0},
//                                    {1, 1, 0, 1, 0, 0, 0, 0},
//                                    {0, 1, 1, 1, 0, 0, 1, 1},
//                                    {0, 0, 1, 1, 0, 0, 1, 1},
//                                    {0, 0, 1, 1, 1, 1, 1, 1},
//                                    {1, 0, 1, 1, 1, 1, 1, 0},
//                                    {1, 1, 1, 1, 1, 1, 0, 0},
//                                    {0, 0, 0, 0, 1, 0, 0, 0},
//                                    {0, 0, 0, 1, 1, 0, 0, 0}};

#define XSIZE 5
#define YSIZE 5

    int xsize = XSIZE;
    int ysize = YSIZE;
    int cols[XSIZE][2] = {{1, 1}, {5, 0}, {1, 1}, {5, 0}, {1, 1}};
    int rows[YSIZE][2] = {{1, 1}, {5, 0}, {1, 1}, {5, 0}, {1, 1}};

    /* Solution:
. # . # .
# # # # #
. # . # .
# # # # #
. # . # .
*/
    bool solution[YSIZE][XSIZE] = {{0, 1, 0, 1, 0},
                                   {1, 1, 1, 1, 1},
                                   {0, 1, 0, 1, 0},
                                   {1, 1, 1, 1, 1},
                                   {0, 1, 0, 1, 0}};

    map->xres = xsize;
    map->cols = new numberline[map->xres];
    for (int x = 0; x < map->xres; x++)
    {
        map->cols[x].cnt = cols[x][1] != 0 ? 2 : 1;
        map->cols[x].nums = new int[map->cols[x].cnt];

        for (size_t i = 0; i < map->cols[x].cnt; i++)
        {
            map->cols[x].nums[i] = cols[x][i];
        }
    }

    map->yres = ysize;
    map->rows = new numberline[map->yres];

    for (int y = 0; y < map->yres; y++)
    {
        map->rows[y].cnt = rows[y][1] != 0 ? 2 : 1;
        map->rows[y].nums = new int[map->rows[y].cnt];

        for (size_t i = 0; i < map->rows[y].cnt; i++)
        {
            map->rows[y].nums[i] = rows[y][i];
        }
    }

    map->image = new bool *[map->xres];

    for (int x = 0; x < map->xres; x++)
    {
        map->image[x] = new bool[map->yres];
        for (int y = 0; y < map->yres; y++)
        {
            map->image[x][y] = false;
            // map->image[x][y] = solution[y][x];
        }
    }

    map->solution = new bool *[map->xres];

    for (int x = 0; x < map->xres; x++)
    {
        map->solution[x] = new bool[map->yres];
        for (int y = 0; y < map->yres; y++)
        {
            map->solution[x][y] = solution[y][x];
        }
    }
}

void reset_map(struct map *map)
{
    for (int x = 0; x < map->xres; x++)
    {
        for (int y = 0; y < map->yres; y++)
        {
            map->image[x][y] = false;
        }
    }
}

int check_map(struct map *map)
{
    for (int x = 0; x < map->xres; x++)
    {
        int overall_cnt_is = 0;
        int overall_cnt_should = 0;
        bool in_seq = false;
        int seq_idx = 0;
        int seq_len = 0;

        for (size_t i = 0; i < map->cols[x].cnt; i++)
        {
            overall_cnt_should += map->cols[x].nums[i];
        }

        for (int y = 0; y < map->yres; y++)
        {
            if (map->image[x][y])
            {
                overall_cnt_is++;

                if (!in_seq)
                {
                    in_seq = true;
                    if (seq_idx + 1 > map->cols[x].cnt)
                    {
                        D(std::cout << "Error! col " << x << " , " << y << " more sequences than allowed! Should be "
                                    << map->cols[x].cnt << " but is " << seq_idx << " !" << std::endl);
                        return 1;
                    }
                }
                seq_len++;
                if (seq_len > map->cols[x].nums[seq_idx])
                {
                    D(std::cout << "Error! col " << x << " , " << y << " sequence " << seq_idx << " longer than allowed! Should be " << map->cols[x].nums[seq_idx]
                                << " but is " << seq_len << " !" << std::endl);
                    return 1;
                }
            }
            else
            {
                if (in_seq)
                {
                    in_seq = false;
                    if (seq_len != map->cols[x].nums[seq_idx])
                    {
                        D(std::cout << "Error! col " << x << " , " << y << " sequence " << seq_idx << " length does not match! Should be " << map->cols[x].nums[seq_idx]
                                    << " but is " << seq_len << " !" << std::endl);
                        return 1;
                    }
                    seq_len = 0;
                    seq_idx++;
                }
            }
        }

        if (seq_len != map->cols[x].nums[seq_idx])
        {
            D(std::cout << "Error! col " << x << " sequence " << seq_idx << " length does not match! Should be " << map->cols[x].nums[seq_idx]
                        << " but is " << seq_len << " !" << std::endl);
            return 1;
        }

        if (in_seq)
        {
            seq_idx++;
        }

        if (seq_idx != map->cols[x].cnt)
        {
            D(std::cout << "Error! col " << x << " number of sequences do not match! Should be "
                        << map->cols[x].cnt << " but is " << seq_idx << " !" << std::endl);
            return 1;
        }

        if (overall_cnt_is != overall_cnt_should)
        {
            D(std::cout << "Error! col " << x << " pixel count does not match! Should be " << overall_cnt_should
                        << " but is " << overall_cnt_is << " !" << std::endl);
            return 1;
        }
    }

    for (int y = 0; y < map->yres; y++)
    {
        int overall_cnt_is = 0;
        int overall_cnt_should = 0;
        bool in_seq = false;
        int seq_idx = 0;
        int seq_len = 0;

        for (size_t i = 0; i < map->rows[y].cnt; i++)
        {
            overall_cnt_should += map->rows[y].nums[i];
        }

        for (int x = 0; x < map->xres; x++)
        {
            if (map->image[x][y])
            {
                overall_cnt_is++;

                if (!in_seq)
                {
                    in_seq = true;
                    if (seq_idx + 1 > map->rows[y].cnt)
                    {
                        D(std::cout << "Error! row " << y << " , " << x << " more sequences than allowed! Should be "
                                    << map->rows[y].cnt << " but is " << seq_idx << " !" << std::endl);
                        return 1;
                    }
                }
                seq_len++;
                if (seq_len > map->rows[y].nums[seq_idx])
                {
                    D(std::cout << "Error! row " << y << " , " << x << " sequence " << seq_idx << " longer than allowed! Should be " << map->rows[y].nums[seq_idx]
                                << " but is " << seq_len << " !" << std::endl);
                    return 1;
                }
            }
            else
            {
                if (in_seq)
                {
                    in_seq = false;
                    if (seq_len != map->rows[y].nums[seq_idx])
                    {
                        D(std::cout << "Error! row " << y << " , " << x << " sequence " << seq_idx << " length does not match! Should be " << map->rows[y].nums[seq_idx]
                                    << " but is " << seq_len << " !" << std::endl);
                        return 1;
                    }
                    seq_len = 0;
                    seq_idx++;
                }
            }
        }

        if (seq_len != map->rows[y].nums[seq_idx])
        {
            D(std::cout << "Error! row " << y << " sequence " << seq_idx << " length does not match! Should be " << map->rows[y].nums[seq_idx]
                        << " but is " << seq_len << " !" << std::endl);
            return 1;
        }

        if (in_seq)
        {
            seq_idx++;
        }

        if (seq_idx != map->rows[y].cnt)
        {
            D(std::cout << "Error! row " << y << " number of sequences do not match! Should be "
                        << map->rows[y].cnt << " but is " << seq_idx << " !" << std::endl);
            return 1;
        }

        if (overall_cnt_is != overall_cnt_should)
        {
            D(std::cout << "Error! row " << y << " pixel count does not match! Should be " << overall_cnt_should
                        << " but is " << overall_cnt_is << " !" << std::endl);
            return 1;
        }
    }

    D(std::cout << "Check passed!" << std::endl);

    return 0;
}

void print_map(struct map *map)
{
    int max_col_cnt = 0;
    int max_row_cnt = 0;

    for (int x = 0; x < map->xres; x++)
    {
        max_col_cnt = map->cols[x].cnt > max_col_cnt ? map->cols[x].cnt : max_col_cnt;
    }

    for (int y = 0; y < map->yres; y++)
    {
        max_row_cnt = map->rows[y].cnt > max_row_cnt ? map->rows[y].cnt : max_row_cnt;
    }

    std::cout << max_col_cnt << "x" << max_row_cnt << std::endl;
    for (size_t i = 0; i < max_col_cnt; i++)
    {
        std::cout << std::string((max_row_cnt + 1) * 2, ' ');
        for (int x = 0; x < map->xres; x++)
        {
            if (map->cols[x].cnt >= max_col_cnt - i)
            {
                std::cout << " " << map->cols[x].nums[i - (max_col_cnt - map->cols[x].cnt)];
                //std::cout << " " << i - (max_col_cnt - map->cols[x].cnt);
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::string((max_row_cnt + 1 + map->xres) * 2, '=') << std::endl;

    for (int y = 0; y < map->yres; y++)
    {
        for (size_t i = 0; i < max_row_cnt; i++)
        {
            if (map->rows[y].cnt >= max_row_cnt - i)
            {
                std::cout << " " << map->rows[y].nums[i - (max_row_cnt - map->rows[y].cnt)];
                //std::cout << " " << i-(max_row_cnt - map->rows[y].cnt);
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << "||";

        for (int x = 0; x < map->xres; x++)
        {
            std::cout << " " << (map->image[x][y] == true ? '#' : ' ');
        }

        if (NULL != map->solution)
        {
            std::cout << "\t\t";
            for (int x = 0; x < map->xres; x++)
            {
                std::cout << " " << (map->solution[x][y] == true ? '#' : ' ');
            }
        }
        std::cout << std::endl;
    }
}

void delete_map(struct map *map)
{
    for (int x = 0; x < map->xres; x++)
    {
        delete[] map->cols[x].nums;

        delete[] map->image[x];

        delete[] map->solution[x];
    }
    for (int y = 0; y < map->yres; y++)
    {
        delete[] map->rows[y].nums;
    }

    delete[] map->cols;
    delete[] map->rows;
    delete[] map->image;
    delete[] map->solution;

    delete map;
}

void solve_map_random_permutate_col(struct map *map)
{
    int solve_counts = 0;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> disty(0, map->yres - 1); // distribution in range [0, 8]
    std::uniform_int_distribution<std::mt19937::result_type> distx(0, map->xres - 1); // distribution in range [0, 7]

    for (int x = 0; x < map->xres; x++)
    {
        int local_cnt = 0;
        for (size_t i = 0; i < map->cols[x].cnt; i++)
        {
            local_cnt += map->cols[x].nums[i];
        }

        for (size_t i = 0; i < local_cnt; i++)
        {
            int y = 0;
            do
            {
                y = disty(rng);
            } while (map->image[x][y] == true);
            map->image[x][y] = true;
        }
    }

    // correct number of pixels set per col

    while (check_map(map))
    {
        for (int x = 0; x < map->xres; x++)
        {
            //permutate col
            for (int y = 0; y < map->yres; y++)
            {
                int new_y = disty(rng);
                bool entry = map->image[x][new_y];
                map->image[x][new_y] = map->image[x][y];
                map->image[x][y] = entry;
            }
        }
        solve_counts++;
        if (solve_counts % 100000 == 0)
        {
            print_map(map);
            std::cout << "solving tries: " << solve_counts << std::endl;
        }
    }
    std::cout << "solving tries: " << solve_counts << std::endl;
}


void solve_map_random(struct map *map)
{
    long long unsigned int solve_counts = 0;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> disty(0, map->yres - 1); // distribution in range [0, 8]
    std::uniform_int_distribution<std::mt19937::result_type> distx(0, map->xres - 1); // distribution in range [0, 7]
    std::uniform_int_distribution<std::mt19937::result_type> distpix(0, (map->xres - 1)*(map->yres - 1)); // distribution in range [0, 8]

    int num_pixels = 0;
    std::vector<std::pair<int, int> > coords;
    for (int x = 0; x < map->xres; x++)
    {
        for (size_t i = 0; i < map->cols[x].cnt; i++)
        {
            num_pixels += map->cols[x].nums[i];
        }
        for (int y = 0; y < map->yres; y++)
        {
            coords.push_back(std::pair<int, int>(x, y));
        }
    }

    //std::cout << "num_pixels: " << num_pixels << " vlen " << coords.size() << std::endl;
    while (check_map(map))
    {
        reset_map(map);
        std::vector<std::pair<int, int> > this_runs_list(coords);
        for (int i = 0; i < num_pixels; i++)
        {
            //std::cout << "putting pixel: " << i << std::endl;

            // int new_x = 0;
            // int new_y = 0;
            // do
            // {
            //     new_x = distx(rng);
            //     new_y = disty(rng);
            // } while (map->image[new_x][new_y]);
            
            // map->image[new_x][new_y] = true;

            int index = distpix(rng);
            index = index > this_runs_list.size()-1 ? index % (this_runs_list.size()-1) : index;
            map->image[this_runs_list[index].first][this_runs_list[index].second] = true;
            this_runs_list.erase(this_runs_list.begin()+index);
        }
        solve_counts++;
        if (solve_counts % 100000 == 0)
        {
            // print_map(map);
            std::cout << "solving tries: " << solve_counts << std::endl;
        }
    }
    std::cout << "solving tries: " << solve_counts << std::endl;
}

void solve_trying_good(struct map *map)
{
    // iterate over all possible positions for each "run" of pixels per colum.
    // on each position, check validity of all rows
}

void solve_smart(struct map *map)
{
}

int main(int argc, char **argv)
{
    std::cout << "test" << std::endl;

    struct map *mymap = new struct map;

    make_map(mymap);

    print_map(mymap);
    //solve_map_random(mymap);
    solve_map_random_permutate_col(mymap);
    check_map(mymap);

    print_map(mymap);

    delete_map(mymap);

    return 0;
}