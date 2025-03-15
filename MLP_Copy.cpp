#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <tuple>

#include "perc.h"

using namespace std;


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
	random_device rd;
	mt19937 eng(rd());
	uniform_int_distribution<> R100(0, 100); // --> MINMAX0
	uniform_int_distribution<> R50(0, 50); // --> MINMAX1
	uniform_int_distribution<> R10(0, 10); // -->  MINMAX2
	uniform_int_distribution<> R5(0, 5); // --> MINMAX3

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
		{ 0, 5 } //MINMAX5
	};

	vector<string> labelsA = {"MINMAX50", "MINMAX100"};
	vector<string> labelsSq = {"MINMAX10", "MINMAX10"};
	vector<string> labelsLq = {"MINMAX100", "MINMAX10", "MINMAX5"};

	trainPerceptronExtra(academic, 1000, overallGrades, MINMAXES, labelsA);
	trainPerceptronExtra(Squality, 1000, QOS, MINMAXES, labelsSq);
	trainPerceptronExtra(Lquality, 1000, QOF, MINMAXES, labelsLq);

	std::cout << "First perceptron's Accuracy: " << testPerceptronVerbose(academic, 1000, overallGrades, MINMAXES, labelsA) << endl;
	std::cout << "Second perceptron's Accuracy: " << testPerceptronVerbose(Squality, 1000, QOS, MINMAXES, labelsSq) << endl;
	std::cout << "Third perceptron's Accuracy: " << testPerceptronVerbose(Lquality, 1000, QOF, MINMAXES, labelsLq) << endl;

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