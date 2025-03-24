#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <tuple>
#include <algorithm>
#include <cmath>

#include "perc.h"

using namespace std;


static double SIGMOID(double x) {
	double output = 1.00 / (1.00 + exp(-x));

	//std::cout << "the output for sigmoid is " << output << std::endl;

	if (output > 0.5) {
		return 1;
	}
	return -1;
}

std::vector<int> relu(const std::vector<double>& vec) {
	vector<int> result(vec.size());
	for (int i = 0; i < vec.size(); i++) {
		result[i] = SIGMOID(vec[i]);
	}
	return result;
}

void sigmoid_in_place(std::vector<double>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		vec[i] = SIGMOID(vec[i]);
	}
}

//create hidden layer random initialized weights:::::: PS this will have multiple layers soon, for now, we will have only one layer
vector<std::vector<double>> create_hidden_layer_random_initialized_weights(int num_of_percpetrons, int num_of_weights) {
	vector<vector<double>> hidden_layer;

	for (int i = 0; i < num_of_percpetrons; i++) {
		vector<double> weights;
		for (int j = 0; j < num_of_weights; j++) {
			double x = (static_cast<double>(rand()) / RAND_MAX) * (rand() % 2 == 0 ? -1 : 1);
			weights.push_back(x);
		}
		hidden_layer.push_back(weights);
	}

	return hidden_layer;
}


//creating a function that will multiply/guess with respect to the weights based on the inputs
vector<double> guess_with_big_hidden_layer(vector<double> inputs, vector<vector<double>> weights) {
	vector<double> outputs(inputs.size(), 0.0);
	for (int i = 0; i < inputs.size(); i++) {
		for (int j = 0; j < weights.size(); j++) {
			outputs[i] += inputs[i] * weights[j][i];
		}
	}

	sigmoid_in_place(outputs);

	return outputs;
}


//answer functions for the perceptrons
int overallAacdemicPerf(const vector<double>& inputs) {
	// Study hours/week    Past academic performance
	return (inputs[0] >= 20 && inputs[1] >= 75) ? 1 : -1;
}

int SchoolQuality(const vector<double>& inputs) {
	//      stess level       sleep quality 
	return (inputs[0] >= 8 && inputs[1] <= 4) ? -1 : 1;
}

int LifeQuality(const vector<double>& inputs) {
	if (inputs[0] >= 80 && inputs[1] >= 7 && inputs[2] >= 2) { return 1; }
	else if (inputs[1] >= 7 && inputs[2] >= 2) { return 1; }
	return -1;
}

int challengingAnswerFunction(const vector<double>& inputs) {
	// Complex decision boundary involving all three inputs
	if ((inputs[0] * inputs[1] + inputs[2] >= 0) && (inputs[0] - inputs[1] + inputs[2] <= 1)) {
		return 1;
	}
	return -1;
}

int simpleAnswerFunction(const vector<double>& inputs) {
	// Simple decision boundary involving all three inputs
	return (inputs[0] + inputs[1] + inputs[2] >= 0) ? 1 : -1;
}

//DRY preinciple
static void trainPerceptronExtra(percSM& perc, int iterations, function<vector<double>()> inputGenerator, vector<tuple<int, int>>& MinMax, vector<std::string>& Label) {
	for (int i = 0; i < iterations; i++) {
		vector<double> inputs = inputGenerator();
		perc.trainSMExtra(inputs, MinMax, Label);
	}
}

static int testPerceptronVerbose(percSM& perc, int iterations, function<vector<double>()> inputGenerator, vector<tuple<int, int>>& MinMax, vector<std::string>& Label) {
	int correct = 0;
	vector<int> failedTestCases;

	for (int i = 0; i < iterations; i++) {
		vector<double> inputs = inputGenerator();
		int guess = perc.guessSMExtra(inputs, MinMax, Label);
		int answer = perc.getAnswer(inputs);

		if (guess != answer) {  // Only print incorrect cases
			failedTestCases.push_back(i + 1);

			cout << "Test Case " << i + 1 << ":" << endl;
			cout << "Inputs: ";
			for (double input : inputs) {
				cout << input << " ";
			}
			cout << endl;
			cout << "Guess: " << guess << " | Answer: " << answer << endl;
			cout << "Result: Incorrect" << endl;
			cout << "------------------------" << endl;
		}
		else {
			correct++;
		}
	}

	// Print summary of failed cases
	cout << "Failed Test Cases: ";
	if (failedTestCases.empty()) {
		cout << "None! All test cases passed." << endl;
	}
	else {
		for (size_t i = 0; i < failedTestCases.size(); i++) {
			cout << failedTestCases[i];
			if (i != failedTestCases.size() - 1) cout << ", ";
		}
		cout << endl;
	}

	return (correct / static_cast<double>(iterations)) * 100;
}



//testing
/*
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


static void Minmaxer_TwoDiff(vector<double>& inputs, vector<tuple<int, int>>& MinMax, vector<std::string>& Label) {
	for (int i = 0; i < inputs.size(); i++) {
		//debug
		/**debug**
		cout << "MinMax for " << Label[i] << ": ("
			<< std::get<0>(MinMax[Labels(Label[i])]) << ", "
			<< std::get<1>(MinMax[Labels(Label[i])]) << ")" << endl;
		//cout << inputs[i] << endl;
		continuation is the comment under this one
		*/
/*
		auto scaler = [&](double x) {
			double NewNumber = static_cast<double>(x - std::get<0>(MinMax[Labels(Label[i])])) / (std::get<1>(MinMax[Labels(Label[i])]) - std::get<0>(MinMax[Labels(Label[i])]));
			return NewNumber;
			};
		inputs[i] = scaler(inputs[i]);
	}
}
*/

//fast testing functions
/*
static void Minmaxer_TwoDiffTest(vector<double>& inputs, vector<tuple<int, int>>& MinMax, vector<std::string>& Label) {
	for (int i = 0; i < inputs.size(); i++) {
		int minmaxIndex = Labels(Label[i]);
		cout << "Label: " << Label[i] << " -> Index: " << minmaxIndex << endl;
		cout << "MinMax used: (" << std::get<0>(MinMax[minmaxIndex]) << ", " << std::get<1>(MinMax[minmaxIndex]) << ")" << endl;
		cout << "Original Input: " << inputs[i] << endl;

		auto scaler = [&](double x) {
			double minVal = std::get<0>(MinMax[minmaxIndex]);
			double maxVal = std::get<1>(MinMax[minmaxIndex]);
			if (maxVal - minVal == 0) {
				cerr << "Error: Division by zero in MinMax scaling!\n";
				return 0.0; // Prevent divide-by-zero
			}
			return (x - minVal) / (maxVal - minVal);
			};

		inputs[i] = scaler(inputs[i]);
		cout << "Scaled Input: " << inputs[i] << endl;
	}
}
*/

int main() {
	//randomness
	srand(time(0));
	random_device rd;
	mt19937 eng(rd());
	uniform_int_distribution<> R100(0, 100); // --> MINMAX0
	uniform_int_distribution<> R50(0, 50); // --> MINMAX1
	uniform_int_distribution<> R10(0, 10); // -->  MINMAX2
	uniform_int_distribution<> R5(0, 5); // --> MINMAX3
	uniform_int_distribution<> R1(0, 1); // --> MINMAX4

	percSM academic(overallAacdemicPerf);
	percSM Squality(SchoolQuality);
	percSM Lquality(LifeQuality, 3);

	//lambda functions for input generation
	auto overallGrades = [&]() {
        vector<int> inputs(2);
        inputs[0] = R50(eng) + (rand() % 3 - 1); // Adding noise in the range [-1, 1]
        inputs[1] = R100(eng) + (rand() % 3 - 1); // Adding noise in the range [-1, 1]
        return vector<double>(inputs.begin(), inputs.end());
    };

	auto QOS = [&]() {
		vector<int> inputs(2);
		inputs[0] = R10(eng);
		inputs[1] = R10(eng);
		return vector<double>(inputs.begin(), inputs.end());
		};

	auto QOF = [&]() {
		vector<int> inputs = {
			R100(eng), // Attendance Rate
			R10(eng), // Nutrition Score
			R5(eng) // Extracullicular activities
		};
		return vector<double>(inputs.begin(), inputs.end());
		};

	

	vector<tuple<int, int>> MINMAXES = {
		{ 0, 100 }, //MINMAX100
		{ 0, 50 },  //MINMAX50
		{ 0, 10 }, //MINMAX10
		{ 0, 5 }, //MINMAX5
		{ 0, 1 } //MINMAX1
	};

	vector<string> labelsA = {"MINMAX50", "MINMAX100"};
	vector<string> labelsSq = {"MINMAX10", "MINMAX10"};
	vector<string> labelsLq = {"MINMAX100", "MINMAX10", "MINMAX5"};

	trainPerceptronExtra(academic, 1000, overallGrades, MINMAXES, labelsA);
	trainPerceptronExtra(Squality, 1000, QOS, MINMAXES, labelsSq);
	trainPerceptronExtra(Lquality, 1000, QOF, MINMAXES, labelsLq);

	//std::cout << "First perceptron's Accuracy: " << testPerceptronVerbose(academic, 1000, overallGrades, MINMAXES, labelsA) << endl;
	//std::cout << "Second perceptron's Accuracy: " << testPerceptronVerbose(Squality, 1000, QOS, MINMAXES, labelsSq) << endl;
	//std::cout << "Third perceptron's Accuracy: " << testPerceptronVerbose(Lquality, 1000, QOF, MINMAXES, labelsLq) << endl;


	//the last stages of the project

	auto LAST = [&]() {
		vector<int> inputs = {
			R1(eng) * (rand() % 2 == 0 ? -1 : 1),
			R1(eng) * (rand() % 2 == 0 ? -1 : 1),
			R1(eng) * (rand() % 2 == 0 ? -1 : 1)
		};
		return vector<double>(inputs.begin(), inputs.end());
		};

	percSM LASTPerc(challengingAnswerFunction, 3);

	vector<string> LASTLabel = { "MINMAX1", "MINMAX1", "MINMAX1"};

	trainPerceptronExtra(LASTPerc, 10000, LAST, MINMAXES, LASTLabel);
	//std::cout << "Last perceptron's Accuracy: " << testPerceptronVerbose(LASTPerc, 1000, LAST, MINMAXES, LASTLabel) << endl;

	//just testing
	vector<vector<double>> hidden_layer = {
		{ -0.4, 0.2 },
		{ -0.1, 0.3 },
		{ 0.2, -0.5 }
	};

	vector<double> inputs = { 0.1, 0.2 };

	vector<double> outputs(hidden_layer.size(), 0.0);

	for (int i = 0; i < inputs.size(); i++) {
		for (int j = 0; j < hidden_layer.size(); j++) {
			outputs[j] += inputs[i] * hidden_layer[j][i];
		}
	}

	vector<int> outputsFinal = relu(outputs);
	sigmoid_in_place(outputs);

	for (double output : outputsFinal) {
		cout << output << " ";
	}
	cout << endl;

	for (double output : outputs) {
		cout << output << " ";
	}
	cout << endl;


	vector<double> ib = { 0.1, 0.2 };
	vector<vector<double>> hl = create_hidden_layer_random_initialized_weights(3, 2);
	vector<double> outputsGuess = guess_with_big_hidden_layer(ib, hl);
	for (const auto val : outputsGuess) {
		cout << val << " ";
	}
	cout << endl;


	/*
	double input1 = 3.0;
	double input2 = 7.0;
	double input3 = 47.0;
	double input4 = 97.0;
	vector<double> inputs = { input3, input4 };
	vector<string> labels = { "MINMAX50", "MINMAX100" };
	academic.Minmaxer_TwoDiff(inputs, MINMAXES, labels);
	cerr << "input 1: " << inputs[0] << endl
		<< "input 2: " << inputs[1] << endl;
	*/

	return 0;
}