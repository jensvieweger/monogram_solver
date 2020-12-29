#include <iostream>
#include <random>

struct numberline {
    int cnt;
    int* nums;
};

struct map {
    int xres, yres;
    struct numberline* cols;
    struct numberline* rows;
    bool** image;
};

void make_map(struct map * map) {
    int xsize = 8;
    int ysize = 9;
    int cols[xsize][2]= {{1,2},{3,1},{1,5},{7,1},{5,0},{3,0},{4,0},{3,0}};
    int rows[ysize][2]= {{3,0},{2,1},{3,2},{2,2},{6,0},{1,5},{6,0},{1,0},{2,0}};

    map->xres = xsize;
    map->cols = new numberline [map->xres];
    for(int x=0; x< map->xres; x++) {
        map->cols[x].cnt = cols[x][1] != 0 ? 2 : 1;
        map->cols[x].nums = new int [map->cols[x].cnt];

        for (size_t i = 0; i < map->cols[x].cnt; i++)
        {
            map->cols[x].nums[i] = cols[x][i];
        }
    }


    map->yres = ysize;
    map->rows = new numberline [map->yres];

    for(int y=0; y< map->yres; y++) {
        map->rows[y].cnt = rows[y][1] != 0 ? 2 : 1;
        map->rows[y].nums = new int [map->rows[y].cnt];

        for (size_t i = 0; i < map->rows[y].cnt; i++)
        {
            map->rows[y].nums[i] = rows[y][i];
        }
    }



    map->image = new bool* [map->xres];

    for(int x=0; x< map->xres; x++) {
        map->image[x] = new bool [map->yres];
        for(int y=0; y< map->yres; y++) {
            map->image[x][y] = false;
        }
    }
}

void solve_map(struct map * map) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,map->yres-1); // distribution in range [1, 6]

    for(int x=0; x< map->xres; x++)
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
                y = dist6(rng);
            } while (map->image[x][y] == true);
            map->image[x][y] = true;
        }

    }
}

void print_map(struct map * map) {
    int max_col_cnt = 0;
    int max_row_cnt = 0;

    for(int x=0; x< map->xres; x++) {
        max_col_cnt = map->cols[x].cnt > max_col_cnt ? map->cols[x].cnt : max_col_cnt;
    }

    for(int y=0; y< map->yres; y++) {
        max_row_cnt = map->rows[y].cnt > max_row_cnt ? map->rows[y].cnt : max_row_cnt;
    }

    std::cout << max_col_cnt << "x" << max_row_cnt << std::endl;
    for (size_t i = 0; i < max_col_cnt; i++)
    {
        std::cout << std::string((max_row_cnt + 1) * 2, ' ');
        for(int x=0; x< map->xres; x++) {
            if (map->cols[x].cnt >= max_col_cnt - i)
            {
                std::cout << " " << map->cols[x].nums[i-(max_col_cnt - map->cols[x].cnt)];
                //std::cout << " " << i - (max_col_cnt - map->cols[x].cnt);
            } else
            {
                std::cout << "  ";
            }            
        }
        std::cout << std::endl;
    }
    std::cout << std::string((max_row_cnt + 1 + map->xres) * 2, '=') << std::endl;


    for(int y=0; y< map->yres; y++) {
        for (size_t i = 0; i < max_row_cnt; i++) {
            if (map->rows[y].cnt >= max_row_cnt - i)
            {
                std::cout << " " << map->rows[y].nums[i-(max_row_cnt - map->rows[y].cnt)];
                //std::cout << " " << i-(max_row_cnt - map->rows[y].cnt);
            } else
            {
                std::cout << "  ";
            }
        }
        std::cout << "||";

        for(int x=0; x< map->xres; x++) {
            std::cout << " " << (map->image[x][y] == true ? '#' : ' ');
        }

        std::cout << std::endl;

    }
}

void delete_map(struct map * map)
{
    for(int x=0; x< map->xres; x++) {
        delete[] map->cols[x].nums;

        delete[] map->image[x];
    }
    for(int y=0; y< map->yres; y++) {
        delete[] map->rows[y].nums;
    }

    delete[] map->cols;
    delete[] map->rows;        
    delete[] map->image;

    delete map;

}

int main(int argc, char ** argv) {
    std::cout << "test" << std::endl;

    struct map* mymap = new struct map;

    make_map(mymap);

    solve_map(mymap);

    print_map(mymap);

    delete_map(mymap);

    return 0;
}