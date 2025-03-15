#pragma once

#include <time.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <vector>
#include <tuple>


class percSM
{
private:
	const int bias = 1;
	const float lr = 0.01;
	const static int size = 2;
	float weights[size];
	std::vector<std::tuple<int, int>> MINMAX;
	std::vector<float> weightsExtra;

	std::function<int(const std::vector<double>&)> answerFunction;

public:
	percSM(std::function<int(const std::vector<double>&)> func, const int sizeE = 2) : answerFunction(func), weightsExtra(sizeE) {
		if (sizeE < 2) {
			std::cout << "The size of the weightsExtra is less than 2, please make sure it is greater than 2, if this is being done intentionally add a function to support this" << std::endl;
		}
		
		//initialize the weightsExtra
		for (int i = 0; i < sizeE; i++) {
			weightsExtra[i] = (static_cast<double>(rand()) / RAND_MAX) * (rand() % 2 == 0 ? -1 : 1);
			std::cout << "initialized weightsExtra once" << std::endl;
		}
		std::cout << std::endl;
	}

	static int Labels(std::string label) {
		if (label == "MINMAX100") {
			return 0;
		}
		else if (label == "MINMAX50") {
			return 1;
		}
		else if (label == "MINMAX10") {
			return 2;
		}
		else if (label == "MINMAX5") {
			return 3;
		}
	}

	static void Minmaxer_TwoDiff(std::vector<double>& inputs, std::vector<std::tuple<int, int>>& MinMaxArrays, std::vector<std::string>& Label) {
		for (int i = 0; i < inputs.size(); i++) {
			auto scaler = [&](double x) {
				double NewNumber = static_cast<double>(x - std::get<0>(MinMaxArrays[Labels(Label[i])])) / (std::get<1>(MinMaxArrays[Labels(Label[i])]) - std::get<0>(MinMaxArrays[Labels(Label[i])]));
				return NewNumber;
				};
			inputs[i] = scaler(inputs[i]);
		}
	} 

	//standard funtions
	int sigmoid(double x) {
		double output = 1.00 / (1.00 + exp(-x));

		//std::cout << "the output for sigmoid is " << output << std::endl;

		if (output > 0.5) {
			return 1;
		}
		return -1;
	}

    int guessSMExtra(std::vector<double>& inputs, std::vector<std::tuple<int, int>>& MinMax, std::vector<std::string>& Label) {
        double sum = 0;
        Minmaxer_TwoDiff(inputs, MinMax, Label);
        for (int i = 0; i < weightsExtra.size(); i++) {
            sum += weightsExtra[i] * inputs[i];
        }

		/*
        // Add debugging code to print the values of weightsExtra and inputs
        std::cout << "weightsExtra: ";
        for (int i = 0; i < weightsExtra.size(); i++) {
            std::cout << weightsExtra[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "inputs: ";
        for (int i = 0; i < inputs.size(); i++) {
            std::cout << inputs[i] << " ";
        }
        std::cout << std::endl;
		*/

        return sigmoid(sum);
    }

	//training the perceptron
	int getAnswer(std::vector<double>& inputs) {
		return answerFunction(inputs);
	}

	int calculateError(int target, int guess) {
		return target - guess;
	}

	void updateWeightsExtra(int error, std::vector<double>& inputs) {
		for (int i = 0; i < weightsExtra.size(); i++) {
			this->weightsExtra[i] += error * lr * inputs[i];
		}
	}

	void trainSMExtra(std::vector<double>& inputs, std::vector<std::tuple<int, int>>& MinMax, std::vector<std::string>& Label) {
		int guessV = guessSMExtra(inputs, MinMax, Label);
		int answerV = getAnswer(inputs);
		int error = calculateError(answerV, guessV);
		updateWeightsExtra(error, inputs);

		// if something gets weird start adding verbose output to get the exact weights everytime, this is already implemented in the getinfo function
	}

	const void getinfo() {
		std::cout << "weights[0]: " << weights[0] << " | weights[1]: " << weights[1] << std::endl;
	}

};
