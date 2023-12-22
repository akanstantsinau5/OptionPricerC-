//
//  Option.hpp
//  OptionPricer
//
//  Created by Aliaksei Kanstantsinau on 12/22/23.
//

#ifndef Option_hpp
#define Option_hpp

#include <stdio.h>
#include <vector>

//  Initiate class options, and call/put option afterwards that will inherit from option base class.
class Option {
public:
    double S;   //  Underlying
    double K;   //  Strike
    double T;   //  Time to expiration
    double r;   //  Rf rate
    double sigma;   //  Volatility
    double b;   //  Carry cost
    Option(double S, double K, double T, double r, double sigma, double b);
    virtual double price() = 0;
    virtual double delta() = 0;
};

//  Add a structure to hold option parameters
struct OptionParams {
    double S;   //  Underlying
    double K;   //  Strike
    double T;   //  Time to expiration
    double r;   //  Rf rate
    double sigma;   //  Volatility
    double b;   //  Carry cost
};

//  Define vectors, meshArray for iteration and computePrices for calculations
std::vector<double> generateMeshArray(double start, double end, double h);
std::vector<double> computeOptionPrices(const std::vector<OptionParams> &paramsVector, bool isCall);
std::vector<double> computeOptionDeltas(const std::vector<OptionParams> &paramsVector, bool isCall);
std::vector<double> computeAmericanOption(const std::vector<OptionParams> &paramsVector, bool isCall);

//  Define Call Option
class CallOption : public Option {
public:
    CallOption(double S, double K, double T, double r, double sigma, double b);
    double price() override; // Override with call option pricing formula
    double delta() override;
};

//  Define Put Option
class PutOption : public Option {
public:
    PutOption(double S, double K, double T, double r, double sigma, double b);
    double price() override; // Override with put option pricing formula
    double delta() override;
};

class AmericanCallOption : public Option {
public:
    AmericanCallOption(double S, double K, double T, double r, double sigma, double b);
    double price() override;
    double delta() override;
};

class AmericanPutOption : public Option {
public:
    AmericanPutOption(double S, double K, double T, double r, double sigma, double b);
    double price() override;
    double delta() override;
};

// Vector function to generate variations of parameters, just like with computeOption Prices
std::vector<OptionParams> generateParamVariations(double start, double end, double step, OptionParams baseParams, char varyingParam);

#endif /* Option_hpp */

