#include "stdlib.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include "TStyle.h"
#include "TRandom3.h"

using namespace std;

//das funktioniert leider nicht alles so wie es soll...

int main() {
	//Deklaration der Zufallszahlen Funktion
	float GetXSquare(float x);
	float GetXLinear(float x, float m, float b);
	float GetXCircle(float x, float r);
	float GetXSine(float x);
	bool CheckXSquare(float x, float y);
	bool CheckXLinear(float x, float y, float m, float b);
	bool CheckXCircle(float x, float y, float r);
	bool CheckXSine(float x, float y);

	void Draw(string functionName, vector<float> yValuesTrue,
			vector<float> yValuesRandom, Double_t xlow, Double_t xup );
	int GetCorrectBin(float x, int nBins, int xlow);

	/*
	 * Squares
	 */

	vector<float> squares;
	for (float x = 0; x <= 1; x += 0.01) {
		float square = GetXSquare(x);
		squares.push_back(square);
	}

	TRandom3* random = new TRandom3();
	vector<float> squaresRandom(squares.size(), 0);
	for (int i = 0; i < 1000000; i++) {
		float x = random->Rndm();
		float y = random->Rndm();
		//float square = GetXSquare(x);
		if (CheckXSquare(x, y)) {
			squaresRandom[GetCorrectBin(x, squares.size(), 0)] += 1;
		}
	}

	//Draw("histoSquare", squares, squaresRandom);

	/*
	 * Linear
	 */

	float m = 3;
	float b = 0;
	vector<float> linear;
	for (float x = 0; x <= 1; x += 0.01) {
		float linearVal = GetXLinear(x, m, b);
		linear.push_back(linearVal);
	}

	vector<float> linearRandom(linear.size(), 0);
	for (int i = 0; i < 1000000; i++) {
		float x = random->Rndm();
		float y = random->Rndm();
		if (CheckXLinear(x, y, m, b)) {
			linearRandom[GetCorrectBin(x, linear.size(), 0)] += 1;
		}
	}

	//Draw("histoLinear", linear, linearRandom);

	/*
	 * Circle
	 */

	float r = 1;

	vector<float> circle;
	for (float x = -1; x <= 1; x += 0.01) {
		float circleVal = GetXCircle(x, r);
		circle.push_back(circleVal);
	}

	vector<float> circleRandom(circle.size(), 0);
	for (int i = 0; i < 1000000; i++) {
		float x= random->Rndm();
//		if (i%2==0) {
//			x = -x;
//		} 
		float y = random->Rndm();
		if (CheckXCircle(x, y, r)) {
			circleRandom[GetCorrectBin(x, circle.size(), 0)] += 1;
		}
	}

	Draw("histoCircle", circle, circleRandom, -1., 1.);

	/*
	 * Sine
	 */

	vector<float> sine;
	for (float x = 0; x <= 1; x += 0.01) {
		float sineVal = GetXSine(x);
		sine.push_back(sineVal);
	}

	vector<float> sineRandom(circle.size(), 0);
	for (int i = 0; i < 1000000; i++) {
		float x = random->Rndm();
		float y = random->Rndm();
		if (CheckXSine(x, y)) {
			sineRandom[GetCorrectBin(x, sine.size(),0)] += 1;
		}
	}

	//Draw("histoSine", sine, sineRandom);
}

float GetXSquare(float x) {
	return x * x;
}

bool CheckXSquare(float x, float y) {
	if (y < GetXSquare(x)) {
		return true;
	}
	return false;
}

float GetXLinear(float x, float m, float b) {
	return m * x + b;
}

bool CheckXLinear(float x, float y, float m, float b) {
	if (y < GetXLinear(x, m, b)) {
		return true;
	}
	return false;
}

float GetXCircle(float x, float r) {
	return sqrt(r * r - x * x);
}

bool CheckXCircle(float x, float y, float r) {
	if (y < GetXCircle(x, r)) {
		return true;
	}
	return false;
}

float GetXSine(float x) {
	return sin(x);
}

bool CheckXSine(float x, float y) {
	if (y < GetXSine(x)) {
		return true;
	}
	return false;
}

//int GetCorrectBin(float x, int nBins) {
//	int bin = -1;
//	float stepSize = 1. / nBins;
//	for (int i = 0; i < nBins; i++) {
//		float lowerEdge = i * stepSize;
//		float upperEdge = (i + 1) * stepSize;
//		if (x >= lowerEdge && x < upperEdge) {
//			bin = i;
//			break;
//		}
//	}
//	return bin;
//}

int GetCorrectBin(float x, int nBins, int xlow) {
	int bin = -1;
	float stepSize = 1. / nBins;
	for (int i = xlow; i < nBins; i++) {
		float lowerEdge = i * stepSize;
		float upperEdge = (i + 1) * stepSize;
		if (x >= lowerEdge && x < upperEdge) {
			bin = i;
			break;
		}
	}
	return bin;
}

//Ignorieren Sie diese Funktion
void Draw(string functionName, vector<float> yValuesTrue,
		vector<float> yValuesRandom, Double_t xlow, Double_t xup ) {
	gStyle->SetOptStat("");

	unsigned int nBins = yValuesTrue.size();

	TString fname(functionName);
	TH1F* trueHist = new TH1F("true_" + fname, "true_" + fname, nBins, xlow, xup);
	TH1F* rndHist = new TH1F("rnd_" + fname, "rnd_" + fname, nBins, xlow, xlow);

	for (unsigned int i = 0; i < nBins; i++) {
		trueHist->SetBinContent(i + 1, yValuesTrue[i]);
		if (i < yValuesRandom.size())
			rndHist->SetBinContent(i + 1, yValuesRandom[i]);
	}

	TCanvas* c1 = new TCanvas("c1_" + fname, "c1_" + fname, 800, 800);
	trueHist->DrawNormalized();

	rndHist->SetLineColor(2);
	rndHist->DrawNormalized("same");
	c1->SaveAs(fname + ".png");

	delete c1;
	delete trueHist;
	delete rndHist;
}
