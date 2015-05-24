#include "stdlib.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include "TStyle.h"
#include "TRandom3.h"

#define FIELD_MEADOW 0
#define FIELD_TREE	1
#define FIELD_TREE_ON_FIRE 2
#define FIELD_TREE_BURNED 3

using namespace std;

unsigned int setOnFire(unsigned char* forrest, unsigned int numberOfFields);
vector<unsigned int> setNeighboursOnFire(unsigned char* forrest,
		vector<unsigned int> ignitedTrees, unsigned int gridwidth,
		unsigned int gridheight);
void setFieldOnFireIfTreefield(unsigned char* forrest,
		vector<unsigned int> &ignitedTrees, unsigned int position);
unsigned int countNumberOfBurningTrees(unsigned char* forrest,
		unsigned int numberOfFields);
float simulateForrest(float treeDensity, TRandom3 &randomNumberCreator);
void Draw(vector<float> yValuesTrue);
void printForrest(unsigned char* forrest, unsigned int numberOfFields,
		unsigned int gridwidth);

int main() {
	TRandom3 randomNumberCreator;
	unsigned int numberOfIterationsPerDensity = 100;
	unsigned int numberOfForrestDensities = 100;
	vector<float> relativeNumbersOfBurntTrees;
	relativeNumbersOfBurntTrees.reserve(numberOfForrestDensities);
	for (unsigned int j = 0; j < numberOfForrestDensities; j++) {
		float sum = 0;
		for (unsigned int i = 0; i < numberOfIterationsPerDensity; i++) {
			sum = sum
					+ simulateForrest(
							(float) (j + 1) / numberOfForrestDensities,
							randomNumberCreator);
		}
		relativeNumbersOfBurntTrees.push_back(
				sum / numberOfIterationsPerDensity);
	}

	Draw(relativeNumbersOfBurntTrees);

	return 0;
}

float simulateForrest(float treeDensity, TRandom3 &randomNumberCreator) {

	/*
	 * creating forrest
	 */

	unsigned int gridwidth = 20, gridheight = 15, numberOfFields = gridheight
			* gridwidth;
//	determine how many trees in percent should be in the forrest:

	unsigned char forrest[numberOfFields];
	unsigned int treeCounter = 0;
	//fill forrest with trees and meadow
	for (unsigned int i = 0; i < numberOfFields; i++) {
		//forrest.push_back(vector<int>()); //initialize a vector in vector< vector<int> >, else segfault -.-
		float randomNum = randomNumberCreator.Rndm();
		if (randomNum > treeDensity) {
			forrest[i] = FIELD_MEADOW; //set field 0 if there is meadow
		} else {
			forrest[i] = FIELD_TREE; //set field 1 if there is a tree
			treeCounter++;
		}
	}
	if (treeCounter == 0.0) {
		return 0.0;
	}

	vector<unsigned int> ignitedTrees;
	ignitedTrees.push_back(setOnFire(forrest, numberOfFields));

	while (!ignitedTrees.empty()) {
		ignitedTrees = setNeighboursOnFire(forrest, ignitedTrees, gridwidth,
				gridheight);
	}
	unsigned int numberOfBurningTrees = countNumberOfBurningTrees(forrest,
			numberOfFields);
	return (float) numberOfBurningTrees / treeCounter;
}

/*
 * Ignites first tree in array
 */
unsigned int setOnFire(unsigned char* forrest, unsigned int numberOfFields) {
	//find first tree in array
	unsigned int position;
	for (position = 0; position < numberOfFields; position++) {
		if (forrest[position] == FIELD_TREE) {
			break;
		}
	}
	//ignite first tree
	forrest[position] = FIELD_TREE_ON_FIRE;
	return position;
}

vector<unsigned int> setNeighboursOnFire(unsigned char* forrest,
		vector<unsigned int> ignitedTrees, unsigned int gridwidth,
		unsigned int gridheight) {
	vector<unsigned int> newlyIgnitedTrees;
	for (unsigned int i = 0; i < ignitedTrees.size(); i++) {
		unsigned int position = ignitedTrees[i];
		unsigned int x = position % gridwidth;
		int y = position / gridwidth;
		forrest[position] = FIELD_TREE_BURNED; // to not check this field again
		//check if neighbours exist and if tree field, set on fire
		for (int dy = -1; dy < 2; dy++) {
			if (y + dy != -1 && (unsigned int) y + dy != gridheight) {
				if (dy != 0) {
					setFieldOnFireIfTreefield(forrest, newlyIgnitedTrees,
							position + dy * gridwidth);
				}
				if (x != 0) {
					setFieldOnFireIfTreefield(forrest, newlyIgnitedTrees,
							position + dy * gridwidth - 1);
				}
				if (x != gridwidth - 1) {
					setFieldOnFireIfTreefield(forrest, newlyIgnitedTrees,
							position + dy * gridwidth + 1);
				}
			}
		}
	}
	return newlyIgnitedTrees;
}

void setFieldOnFireIfTreefield(unsigned char* forrest,
		vector<unsigned int> &ignitedTrees, unsigned int position) {
	if (forrest[position] == FIELD_TREE) {
		forrest[position] = FIELD_TREE_ON_FIRE;
		ignitedTrees.push_back(position);
	}
}

void printForrest(unsigned char* forrest, unsigned int numberOfFields,
		unsigned int gridwidth) {
	for (unsigned int i = 0; i < numberOfFields; i++) {
		cout << (int) forrest[i] << " "; //the grid looks like that
		if ((i + 1) % gridwidth == 0)
			cout << endl;
	}
	cout << endl;
}

unsigned int countNumberOfBurningTrees(unsigned char* forrest,
		unsigned int numberOfFields) {
	unsigned int counter = 0;
	//iterate through grid and count how many trees are burning
	for (unsigned int i = 0; i < numberOfFields; i++) {
		if (forrest[i] == FIELD_TREE_ON_FIRE || forrest[i] == FIELD_TREE_BURNED) {
			counter++;
		}
	}
	return counter;
}

void Draw(vector<float> relativeNumbersOfBurnedTrees) {
	gStyle->SetOptStat("");

	unsigned int nBins = relativeNumbersOfBurnedTrees.size();

	TH1F* hist = new TH1F("hist",
			"Average of burned trees [%] in dependence of tree density", nBins,
			0, 1);

	for (unsigned int i = 0; i < nBins; i++) {
		hist->SetBinContent(i + 1, relativeNumbersOfBurnedTrees[i]);
	}

	TCanvas* c1 = new TCanvas("c1_", "c1_", 800, 800);
	hist->Draw();
	c1->SaveAs("hist.png");

	delete c1;
	delete hist;
}
