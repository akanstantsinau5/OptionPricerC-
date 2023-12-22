//
//  main.cpp
//  OptionPricer
//
//  Created by Aliaksei Kanstantsinau on 12/22/23.
//

#include <iostream>
#include "Option.hpp"
#include <vector>
#include <tuple>
#include <iomanip>

int main() {
    //  Block 1, Calculate option pricing
    //  Define batches
    std::vector<std::tuple<double, double, double, double, double, double>> batches = {
        {102, 122, 1.65, .045, .43, 0},
        {100, 100, 1, .0, .2, 0},
        {5, 10, 1, .12, .5, 0.12},
        {100, 100, 30, .08, .3, 0.08}
    };
    
    //  For loop to iterate through batches
    for (const auto& batch : batches) {
        double S, K, T, r, sigma, b;
        //  Unpack tuples and create OptionParams with unpacked values
        std::tie(S, K, T, r, sigma, b) = batch; // Unpack the tuple
        OptionParams params = {S, K, T, r, sigma, b};
        
        //  Create a vector, calculate call and put prices
        std::vector<OptionParams> paramsVector = {params};
        std::vector<double> callPrices = computeOptionPrices(paramsVector, true);
        std::vector<double> putPrices = computeOptionPrices(paramsVector, false);
            
        //  Extract the first price from the vector
        double callPrice = callPrices[0];
        double putPrice = putPrices[0];
        
        //  Calculate price based on put call parity
        double disc_asset = K * exp(-r * T);
        double callParityPrice = putPrice + S - disc_asset;
        double putParityPrice = callPrice + disc_asset - S;

        //  Outputs
        std::cout << "Call Option Price: " << callPrice << std::endl;
        std::cout << "Put Option Price: " << putPrice << std::endl;
        std::cout << "Put-Call Parity Call Option Price: " << callParityPrice << std::endl;
        std::cout << "Put-Call Parity Put Option Price: " << putParityPrice << std::endl;
        std::cout << "----------------------------------" << std::endl;
    }
    
    //  Block 2, Generating matrix
    //  Lets use values from the first batch
    OptionParams baseParams = {50, 65, .25, .08, .3, .08};
    //  Start, end, step
    double S_start = 60, S_end = 70, S_step = 1;
    double sigma_start = .25, sigma_end = .35, sigma_step = .01;
    int width = 10;  // Width of each table cell
    
    //  Set fixed and precision, print the header
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(width) << "S \\ Sigma";

    //  Generate range of sigma values
    std::vector<double> sigma_values = generateMeshArray(sigma_start, sigma_end, sigma_step);
    for (double sigma : sigma_values) {
        std::cout << std::setw(width) << sigma;
    }
    std::cout << std::endl;
    
    //  Generate range of S values and iterate through values
    std::vector<double> S_values = generateMeshArray(S_start, S_end, S_step);
    for (double S : S_values) {
        std::cout << std::setw(width) << S;
        for (double sigma : sigma_values) {
            baseParams.S = S;
            baseParams.sigma = sigma;
            std::vector<OptionParams> paramsVector = {baseParams};
            std::vector<double> prices = computeOptionPrices(paramsVector, true); // true for Call Option
            std::cout << std::setw(width) << prices[0];
        }
        std::cout << std::endl;
    }
    
    //  Block 3, greeks
    OptionParams singleParams = {105, 100, 0.5, 0.1, 0.36, 0};
    
    std::vector<OptionParams> callParamsVector = {singleParams};
    std::vector<double> callDelta = computeOptionDeltas(callParamsVector, true);
    
    std::vector<OptionParams> putParamsVector = {singleParams};
    std::vector<double> putDelta = computeOptionDeltas(putParamsVector, false);
    
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Deltas:" << std::endl;
    std::cout << "Call Delta: " << callDelta[0] << std::endl;
    std::cout << "Put Delta: " << putDelta[0] << std::endl;
    std::cout << "----------------------------------" << std::endl;
    
    // Matrix for deltas
    OptionParams testParams = {105, 100, 0.5, 0.1, 0.36, 0};
    double S_test_start = 100, S_test_end = 110, S_test_step = 1;
    double sigma_test_start = 0.30, sigma_test_end = 0.40, sigma_test_step = 0.01;

    std::vector<double> S_test_values = generateMeshArray(S_test_start, S_test_end, S_test_step);
    std::vector<double> sigma_test_values = generateMeshArray(sigma_test_start, sigma_test_end, sigma_test_step);
    
    std::cout << std::setw(width) << "S \\ Sigma";
    for (double sigma : sigma_test_values) {
        std::cout << std::setw(width) << sigma;
    }
    std::cout << std::endl;

    // Print each row of the table
    for (double S : S_test_values) {
        std::cout << std::setw(width) << S;
        for (double sigma : sigma_test_values) {
            testParams.S = S;
            testParams.sigma = sigma;
            std::vector<OptionParams> paramsVector = {testParams};
            std::vector<double> deltas = computeOptionDeltas(paramsVector, true); // true for Call Option Deltas
            std::cout << std::setw(width) << deltas[0];
        }
        std::cout << std::endl;
    }
    
    //  Block 4 American Options
    OptionParams american = {110, 100, 0, 0.1, 0.1, 0.2};  // Example parameters
    std::vector<OptionParams> paramsVector = {american};

    std::vector<double> americanCallPrices = computeAmericanOption(paramsVector, true);  // true for American Call
    std::vector<double> americanPutPrices = computeAmericanOption(paramsVector, false);  // false for American Put

    std::cout << "----------------------------------" << std::endl;
    std::cout << "American Option:" << std::endl;
    std::cout << "American Call Option Price: " << americanCallPrices[0] << std::endl;
    std::cout << "American Put Option Price: " << americanPutPrices[0] << std::endl;
    std::cout << "----------------------------------" << std::endl;
    
    OptionParams americanParams = {110, 100, 0, 0.1, 0.1, 0.2}; // Modify these values as needed
    double S_american_start = 105, S_american_end = 115, S_american_step = 1;
    double sigma_american_start = 0.05, sigma_american_end = 0.15, sigma_american_step = 0.01;
    

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(width) << "S \\ Sigma";
        
    for (double sigma = sigma_american_start; sigma <= sigma_american_end; sigma += sigma_american_step) {
            std::cout << std::setw(width) << sigma;
        }
        std::cout << std::endl;

    std::vector<double> sigma_american_values = generateMeshArray(sigma_start, sigma_end, sigma_step);
    for (double sigma : sigma_american_values) {
        std::cout << std::setw(width) << sigma;
    }
    std::cout << std::endl;
    
    std::vector<double> S_american_values = generateMeshArray(S_start, S_end, S_step);
    for (double S : S_american_values) {
        std::cout << std::setw(width) << S;
        for (double sigma : sigma_american_values) {
            baseParams.S = S;
            baseParams.sigma = sigma;
            std::vector<OptionParams> paramsVector = {baseParams};

            // Create American option instances for each combination of S and sigma
            std::vector<double> americanCallPrices = computeAmericanOption(paramsVector, true);
            // std::vector<double> americanPutPrices = computeAmericanOption(paramsVector, false);

            std::cout << std::setw(width) << americanCallPrices[0]; // << "/" << americanPutPrices[0]
        }
        std::cout << std::endl;
    }
    
    return 0;
}

