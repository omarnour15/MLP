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
	float* weightsExtra;
	std::vector<std::tuple<int, int>> MINMAX;

	std::function<int(const std::vector<int>&)> answerFunction;

public:

	//COMMENTS: after polishing everything up remove the weights, and only keep the weightsExtra (change it back to weights normally so you dont need to keep typing all that)
	percSM(std::function<int(const std::vector<int>&)> func, const int sizeE = 2) : answerFunction(func) {
		weightsExtra = new float[sizeE];

		//srand(time(0));
		for (int i = 0; i < size; i++) {
			weights[i] = (static_cast<double>(rand()) / RAND_MAX) * (rand() % 2 == 0 ? -1 : 1);
		}

		for (int i = 0; i < sizeE; i++) {
			weightsExtra[i] = (static_cast<double>(rand()) / RAND_MAX) * (rand() % 2 == 0 ? -1 : 1);
		}
	}

	~percSM() {
		delete[] weightsExtra;
	}


	//COMMENTS: check if we need to add the tuples in order all the time or is there any kind of 'filter' we need to add
	void AddToTupleVector(std::tuple<int, int>& tup) {
		MINMAX.push_back(tup);
	}

	void MinMax(int input, std::tuple<int, int>& XMinMax) {
		auto scaler = [&](int x) { return static_cast<double>(x - std::get<0>(XMinMax)) / (std::get<1>(XMinMax) - std::get<0>(XMinMax)); };
		input = scaler(input);
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

	int guessSM(std::vector<int>& inputs, int vectorTupNum, int MINMAXVersion) {
		double sum = 0;
		MinMax(inputs[vectorTupNum], MINMAX[MINMAXVersion]);
		for (int i = 0; i < vectorTupNum; i++) {
			sum += weights[i] * inputs[i];
		}

		//std::cout << "The sum of the guess is " << sum << std::endl;

		return sigmoid(sum);
	}

	int guessSMExtra(std::vector<int>& inputs, int MINMAXVersion) {
		double sum = 0;
		//MinMax(inputs, MINMAX[MINMAXVersion]);
		for (int i = 0; i < inputs.size(); i++) {
			sum += weightsExtra[i] * inputs[i];
		}

		//std::cout << "The sum of the guess is " << sum << std::endl;

		return sigmoid(sum);
	}

	//old answer function
	/*
	int answer(const std::vector<float> inputs) {
		float tempsum = inputs[0] + inputs[1];

		if (tempsum >= 0) {
			return 1;
		}
		return -1;
	}
	*/

	//training the perceptron
	int getAnswer(std::vector<int> inputs) {
		return answerFunction(inputs);
	}

	int calculateError(int target, int guessV) {
		return target - guessV;
	}

	void updateWeights(int error, std::vector<int> inputs) {
		for (int i = 0; i < size; i++) {
			weights[i] += error * lr * inputs[i];
		}
	}

	void updateWeightsExtra(int error, std::vector<int> inputs) {
		for (int i = 0; i < inputs.size(); i++) {
			weights[i] += error * lr * inputs[i];
		}
	}

	void trainSM(std::vector<int> inputs, int MINMAXVersion) {
		int guessV = guessSM(inputs, inputs.size(), MINMAXVersion);
		int answerV = getAnswer(inputs);
		int error = calculateError(answerV, guessV);
		updateWeights(error, inputs);

		// if something gets weird start adding verbose output to get the exact weights everytime
		/*
		std::cout << "Error: " << error << " | Updated weights: [" << weights[0] << ", " << weights[1] << "]" << std::endl;
		*/
	}
	
	void trainSMExtra(std::vector<int> inputs, int MINMAXVersion) {
		int guessV = guessSMExtra(inputs, MINMAXVersion);
		int answerV = getAnswer(inputs);
		int error = calculateError(answerV, guessV);
		updateWeightsExtra(error, inputs);

		// if something gets weird start adding verbose output to get the exact weights everytime
		/*
		std::cout << "Error: " << error << " | Updated weights: [" << weights[0] << ", " << weights[1] << "]" << std::endl;
		*/
	}

	void getinfo() {
		std::cout << "weights[0]: " << weights[0] << " | weights[1]: " << weights[1] << std::endl;
	}

};

