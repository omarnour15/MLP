#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <tuple>

#include "perc.h"

using namespace std;


//Attendance Rate กร 80 and Nutrition Score กร 7, and at least one of the following :
//Extracurricular Activities กร 2


//answer functions for the perceptrons
int overallAacdemicPerf(const vector<int>& inputs) {
	// Study hours/week    Past academic performance
	return (inputs[0] >= 20 && inputs[1] >= 75) ? 1 : -1;
}

int SchoolQuality(const vector<int>& inputs) {
	//      stess level       sleep quality 
	return (inputs[0] >= 8 && inputs[1] <= 4) ? -1 : 1;
}

int LifeQuality(const vector<int>& inputs) {
	if (inputs[0] >= 80 && inputs[1] >= 7 && inputs[2] >= 2) { return 1; }
	else if (inputs[1] >= 7 && inputs[2] >= 2) { return 1; }
	return -1;
}

//DRY preinciple
static void trainPerceptron(percSM& perc, int iterations, function<vector<int>()> inputGenerator) {
	for (int i = 0; i < iterations; i++) {
		vector<int> inputs = inputGenerator();
		perc.trainSM(inputs);
	}
}

static void trainPerceptronExtra(percSM& perc, int iterations, function<vector<int>()> inputGenerator) {
	for (int i = 0; i < iterations; i++) {
		vector<int> inputs = inputGenerator();
		perc.trainSMExtra(inputs);
	}
}

static int testPerceptron(percSM& perc, int iterations, function<vector<int>()> inputGenerator) {
	float correct = 0;
	for (float i = 0; i < iterations; i++) {
		vector<int> inputs = inputGenerator();
		if (perc.guessSM(inputs) == perc.getAnswer(inputs)) {
			correct++;
		}
	}
	return (correct / iterations) * 100;
}

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
		inputs[0] = R50(eng);
		inputs[1] = R100(eng);
		return inputs;
		};
	
	auto QOS = [&]() {
		vector<int> inputs = { R10(eng), R10(eng) };
		return inputs;
		};

	auto QOF = [&]() {
		vector<int> inputs = { 
			R100(eng), // Attendance Rate
			R10(eng), // Nutrition Score
			R5(eng) // Extracullicular activities
		};
		return inputs;
		};


	tuple<int, int> MINMAX0 = { 0, 100 };
	tuple<int, int> MINMAX1 = { 0, 50 };
	tuple<int, int> MINMAX2 = { 0, 10 };
	tuple<int, int> MINMAX3 = { 0, 5 };




	trainPerceptron(academic, 1000, overallGrades);
	trainPerceptron(Squality, 100, QOS);
	trainPerceptronExtra(Lquality, 1000, QOF);

	cout << "First perceptron's Accuracy: " << testPerceptron(academic, 1000, overallGrades) << endl;
	cout << "Second perceptron's Accuracy: " << testPerceptron(Squality, 1000, QOS) << endl;
	cout << "Third perceptron's Accuracy: " << testPerceptron(Lquality, 1000, QOF) << endl;
}