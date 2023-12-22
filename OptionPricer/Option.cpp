//
//  Option.cpp
//  OptionPricer
//
//  Created by Aliaksei Kanstantsinau on 12/22/23.
//

#include "Option.hpp"
#include <cmath>
#include <boost/math/distributions/normal.hpp>

//  Constructors
Option::Option(double S, double K, double T, double r, double sigma, double b): S(S), K(K), T(T), r(r), sigma(sigma), b(b)  {}
CallOption::CallOption(double S, double K, double T, double r, double sigma, double b)
    : Option(S, K, T, r, sigma, b) {}
PutOption::PutOption(double S, double K, double T, double r, double sigma, double b)
    : Option(S, K, T, r, sigma, b) {}
AmericanCallOption::AmericanCallOption(double S, double K, double T, double r, double sigma, double b)
    : Option(S, K, T, r, sigma, b) {}
AmericanPutOption::AmericanPutOption(double S, double K, double T, double r, double sigma, double b)
    : Option(S, K, T, r, sigma, b) {}

//  normalCDF function using boost   //
double normalCDF(double value)  {
    boost::math::normal_distribution<> dist(0.0, 1.0);
    return boost::math::cdf(dist, value);
}

//  Mesh Array vector
std::vector<double> generateMeshArray(double start, double end, double h)   {
    std::vector<double> meshArray;
    for (double value = start; value <= end; value += h) {
        meshArray.push_back(value);
    }
    return meshArray;
};
 
//
std::vector<double> computeOptionPrices(const std::vector<OptionParams> &paramsVector, bool isCall) {
    std::vector<double> optionPrices;
    for (const auto &params: paramsVector) {
        if (isCall) {
            CallOption callOption(params.S, params.K, params.T, params.r, params.sigma, params.b);
            optionPrices.push_back(callOption.price());
        } else {
            PutOption putOption(params.S, params.K, params.T, params.r, params.sigma, params.b);
            optionPrices.push_back(putOption.price());
        }
    }
    return optionPrices;
}

std::vector<double> computeOptionDeltas(const std::vector<OptionParams> &paramsVector, bool isCall) {
    std::vector<double> optionDeltas;
    for (const auto &params: paramsVector) {
        if (isCall) {
            CallOption callOption(params.S, params.K, params.T, params.r, params.sigma, params.b);
            optionDeltas.push_back(callOption.delta());
        } else {
            PutOption putOption(params.S, params.K, params.T, params.r, params.sigma, params.b);
            optionDeltas.push_back(putOption.delta());
        }
    }
    return optionDeltas;
}

std::vector<double> computeAmericanOption(const std::vector<OptionParams> &paramsVector, bool isCall) {
    std::vector<double> optionPrices;
    for (const auto &params: paramsVector) {
        if (isCall) {
            CallOption americanCall(params.S, params.K, params.r, params.T,params.sigma, params.b);
            optionPrices.push_back(americanCall.price());
        } else {
            PutOption americanPut(params.S, params.K, params.r, params.T, params.sigma, params.b);
            optionPrices.push_back(americanPut.price());
        }
    }
    return optionPrices;
}

std::vector<OptionParams> generateParamVariations(double start, double end, double step, OptionParams baseParams, char varyingParam)    {
    std::vector<OptionParams> variations;
    for (double value = start; value <= end; value += step) {
        OptionParams params = baseParams;
        switch (varyingParam)   {
            case 'S': params.S = value; break;
            case 'K': params.K = value; break;
            case 'T': params.T = value; break;
            case 'r': params.r = value; break;
            case 's': params.sigma = value; break;
            case 'b': params.b = value; break;
        }
        variations.push_back(params);
    }
    return variations;
}

//  Call Option price   //
double CallOption::price() {
    double d1 = (log(S/K)+(b+(std::pow(sigma, 2)/2))*T)/(sigma*sqrt(T));
    double d2 = d1 - (sigma*sqrt(T));
    
    return S*exp((b-r)*T)*::normalCDF(d1)-K*exp(-r*T)*::normalCDF(d2);
}

//  American Put Option Price
double AmericanCallOption::price() {
    double l = b/pow(sigma, 2) - 1/2;
    double y1 = 1/2 - b/pow(sigma,2) + sqrt(pow(l, 2)+2*r/(b/pow(sigma, 2)));
    double j = ((y1-1)*S)/(S/K);
                                            
    return K/(1-y1)*pow(j, y1);
}

//  Put Option price    //
double PutOption::price() {
    double d1 = (log(S/K)+(b+(std::pow(sigma, 2)/2))*T)/(sigma*sqrt(T));
    double d2 = d1 - (sigma*sqrt(T));
    
    return K*exp(-r*T)*::normalCDF(-d2)-S*exp((b-r)*T)*::normalCDF(-d1);
}

//  American Put Option Price
double AmericanPutOption::price() {
    double l = b/pow(sigma, 2) - 1/2;
    double y1 = 1/2 - b/pow(sigma,2) + sqrt(pow(l, 2)+2*r/(b/pow(sigma, 2)));
    double j = ((y1-1)*S)/(S/K);
                                            
    return K/(y1 - 1)*pow(j, y1);
}

double AmericanCallOption::delta() {
    throw std::runtime_error("Not defined");
}

double AmericanPutOption::delta() {
    throw std::runtime_error("Not defined");
}

double CallOption::delta() {
    double d1 = (log(S/K) + (b + (pow(sigma, 2) / 2)) * T) / (sigma * sqrt(T));
    return exp((b - r) * T) * normalCDF(d1); // Delta for Call
}

double PutOption::delta() {
    double d1 = (log(S/K) + (b + (pow(sigma, 2) / 2)) * T) / (sigma * sqrt(T));
    return -exp((b - r) * T) * normalCDF(-d1); // Delta for Put
}
