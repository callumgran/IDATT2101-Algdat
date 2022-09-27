#include <iostream> 
#include <chrono>

using namespace std;

#define SIZE (8 * 1000000)

class StockPicker {
    private:
        int index_min, 
            index_max, 
            min_val,
            max_val,
            profit;
    
    public:
        StockPicker();
        void stock_analysis(int* data);
        int* get_values();
};

StockPicker::StockPicker()
{
    this->index_min = 1;
    this->index_max = 1;
    this->min_val = 0;
    this->max_val = 0;
    this->profit = 0;
}

void StockPicker::stock_analysis(int* data)
{
    int i = 1,
        curr_val = *data,
        min_temp = curr_val,
        max_temp = curr_val,
        temp_profit = 0, 
        index_min_temp = 1,
        index_max_temp = 1;
    
    while(1)
    {
        if(curr_val > max_temp) 
        { 
            max_temp = curr_val; 
            temp_profit = max_temp - min_temp; 
            index_max_temp = i; 
            if (temp_profit > this->profit) 
            { 
                this->profit = temp_profit; 
                this->min_val = min_temp; 
                this->max_val = max_temp; 
                this->index_min = index_min_temp; 
                this->index_max = index_max_temp; 
            }
        } else if (curr_val < min_temp) 
        { 
            min_temp = curr_val; 
            max_temp = min_temp; 
            index_min_temp = i;
        }
        if (i == SIZE) break;
        i++;
        curr_val = curr_val + *(data + i + 1);
    }
}

int* StockPicker::get_values()
{
    int* values = (int*)(malloc(5 * sizeof(int)));
    *(values) = this->index_min; *(values + 1) = this->index_max; 
    *(values + 2) = this->min_val; *(values + 3) = this->max_val; 
    *(values + 4) = this->profit;
    return values;
}

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void create_values(int* data, int len)
{
    for (int i = 0; i < len; i++) {
        data[i] = (int) RAND(-10, 10);
    }
}

int main() 
{
    int* data = (int*)(malloc(SIZE * sizeof(int)));
    auto stock_picker = new StockPicker();
    srand(time(NULL));
    create_values(data, SIZE);
    auto start = chrono::high_resolution_clock::now();
    stock_picker->stock_analysis(data);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    int* values = stock_picker->get_values();
    cout << "Buy day: " << *(values) << endl;
    cout << "Buy value: " << *(values + 2) << endl;
    cout << "Sell day: " << *(values + 1) << endl;
    cout << "Sell value: " << *(values + 3) << endl;
    cout << "Profit: " << *(values + 4) << endl;
    cout << "Time used: " << duration.count() << "µs" << endl;
    free(stock_picker);
    free(data);
    return 0;
}