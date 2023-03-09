#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <numeric>
#include <random>

using namespace std;

void generate_k_random_numbers(int k, vector<int>& numbers, vector<double>& probabilities) {
    // Normalize the probabilities
    double total_probability = accumulate(probabilities.begin(), probabilities.end(), 0.0);
    for (int i = 0; i < probabilities.size(); i++) {
        probabilities[i] /= total_probability;
    }

    // Generate k random numbers using the defined probability distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(probabilities.begin(), probabilities.end());
    for (int i = 0; i < k; i++) {
        int number = d(gen) ;  // scale the generated number to the range of 0 to 40000
        numbers.push_back(number);
    }
}

int main() {
    int k = 5;  // number of random numbers to generate
    vector<int> random_numbers;
    vector<double> probabilities = {0.1, 0.2, 0.3, 0.56 , .13 , 0 , .1 , .15 , .44 , .39 , 0.1 , .4 , .9};  // define the probability distribution for each number from 0 to 40000
    generate_k_random_numbers(k, random_numbers, probabilities);

    // Output the generated numbers to the console
    cout << "Random numbers:";
    for (int i = 0; i < k; i++) {
        cout << " " << random_numbers[i];
    }
    cout << endl;

    return 0;
}
