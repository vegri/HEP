#include "stdlib.h"
#include <iostream>
#include <TROOT.h>
#include <TH2F.h>
#include <TCanvas.h>
#include "TStyle.h"
#include "math.h"
#include "TRandom3.h"
#include "TRandom2.h"

using namespace std;

//Funktionen für Rückweisungsverfahren
float GetCrossFct(float x); //entspricht nur Integral über Theta
bool CheckCrossFct(float x, float y);
float GetGaus(float x, float mu, float sig);
float GetCrossFctComplete(float x);
bool CheckCrossFctComplete(float x, float y);
float GetRndGaus(float mu, float sigma);
bool CheckGaus(float x, float y, float mu, float sigma);
float GetRndGaus(float mu, float xmin, float xmax, float sigma);
float GetEvents(float a, float lumi); //lumi in inverse pb
float GetIntegralTheta(float xmin, float xmax, float ymin, float ymax,
		int loopnumber, float sigma);
int iterations = 1000000;
float i_a;
float xRnd, yRnd, xRndGaus;
int i_events;
TRandom3 tr3;
//float sigma = 0.1;
#define SIGMA 0.1

int main(int argc, char *argv[]) {
	cout << argv[0] << endl;

	int numberIterations=1e6;
	if(argc > 1){
		numberIterations=atoi(argv[1]);
	}


	//tr3 = new TRandom3();

	//A1.(i)
	TH1F histogramGaus("histogramGaus",
			"Gausverteilung um #pi/2 mit #sigma=0.1", numberIterations/100, (M_PI / 2) - 0.6,
			(M_PI / 2) + 0.6);
	for (int i = 0; i < iterations; i++) {
		xRndGaus = GetRndGaus(M_PI / 2, SIGMA);
		histogramGaus.Fill(xRndGaus);
	}

	//A1.(ii)
	float theta, thetaRec;
	TH1F histogramTheta("histogramTheta",
			"Normalverteilung for 0.5 #leq #theta #leq 2.8", 1000, 0, 2.5);
	TH1F histogramThetaFehler("histogramThetaFehler",
			"Faltung Gaus mit Normalverteilung, ", numberIterations/100, 0, 2.5);
	for (int i = 0; i < iterations; i++) {
		theta = tr3.Rndm() * 1.5 + 0.5;
		thetaRec = GetRndGaus(theta, SIGMA);
		histogramTheta.Fill(theta);
		histogramThetaFehler.Fill(thetaRec);
	}

	//A1.(iii)
	// Integration von thete > 1.8 bis
	// Integriere von (pi/2)-0.1 bis (pi/2)+0.1
	i_a = (2 * M_PI) * GetIntegralTheta(1.8, M_PI - 0.05, 0, 8, numberIterations, SIGMA); //Integral über Phi =*2pi
	cout << "(i) A= " << i_a << endl;
	i_events = GetEvents(i_a, 2000);
	cout << "Number of Events: " << i_events << endl;

	TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
	gStyle->SetOptStat(0);
	c1->Divide(2, 2);
	c1->cd(1);
	histogramGaus.Draw();
	c1->cd(2);
	histogramTheta.Draw();
	c1->cd(3);
	histogramThetaFehler.Draw();
	c1->cd(4);
	histogramTheta.SetTitle("Gemeinsames Bildchen");
	//histogramTheta.SetColor("kRed");
	histogramTheta.SetLineStyle(3001);
	histogramTheta.SetLineColor(kRed);
	histogramTheta.Draw();
	histogramThetaFehler.Draw("SAME");
	c1->SaveAs("Gaus.png");

	/*


	 //A1.(ii)
	 //Integral über toten Winkelbereich wird abgezogen
	 float ii_a, ii_dead, ii_res;
	 int ii_events;

	 ii_a = (2*M_PI)*GetIntegra
	 TH1F histogramGaus("histogramGaus", "Gausverteilung um #pi/2 mit #sigma=0.1", 100, 1, 2);
	 for (int i=0; i<iterations; i++){
	 xRndGaus = GetRndGaus();
	 //cout << i << endl;
	 //cout << "Random xGaus: " << xRndGaus << endl;
	 histogramGaus.Fill(xRndGaus);
	 }
	 //xRndGaus = GetRndGaus();
	 //cout << "Random xGaus: " << xRndGaus << endl;

	 //A1.(ii)
	 float theta, thetaRec;
	 TH1F histogramTheta("histogramTheta", "Normalverteilung for 0.5 #leq #theta #leq 2.8", 100, 0, 3);
	 TH1F histogramThetaFehler("histogramThetaFehler", "Faltung Gauß mit Normalverteilung, ", 100,0,3);
	 for (int i=0; i<iterations; i++){
	 theta = tr3.Rndm()*1.5+0.5;
	 thetaRec = GetRndGaus()+
	 TH1F histogramGaus("histogramGaus", "Gausverteilung um #pi/2 mit #sigma=0.1", 100, 1, 2);
	 for (int i=0; i<iterations; i++){
	 xRndGaus = GetRndGaus();
	 //cout << i << endl;
	 //cout << "Random xGaus: " << xRndGaus << endl;
	 histogramGaus.Fill(xRndGaus);
	 }
	 //xRndGaus = GetRndGaus();
	 //cout << "Random xGaus: " << xRndGaus << endl;

	 //A1.(ii)s
	 float theta, thetaRec;
	 TH1F histogramTheta("histogramTheta", "Normalverteilung for 0.5 #leq #theta #leq 2.8", 100, 0, 3);
	 TH1F histogramThetaFehler("histogramThetaFehler", "Faltung Gauß mit Normalverteilung, ", 100,0,3);
	 for (int i=0; i<iterations; i++){
	 theta = tr3.Rndm()*1.5+0.5;
	 thetaRec = GetRndGaus()+lTheta(0.05, M_PI-0.05,0,2,1000000);

	 ii_dead = (0.3-0.2)*GetIntegralTheta(1.0,1.5,0,2,1000000); //Integral über Phi =*0.1
	 ii_res = ii_a-ii_dead;
	 cout << "(ii) A'(ohne Strahlachse)= " << ii_a << endl;
	 cout << "A''(Blind_durch_Katze)= " << ii_dead << endl;
	 cout << "A=A'-A''= " << ii_res << endl;

	 ii_events = GetEvents(ii_res, 2000);
	 cout << "Number of Events: " << ii_events << endl;
	 */
}

/*
 // Funktionen für Cross Section s-Kanal
 float GetCrossFct(float x){
 return ((1+cos(x)*cos(x))*sin(x)) ;
 }

 bool CheckCrossFct(float x, float y){
 if(GetCrossFct(x)>y){
 return true;
 }
 return false;
 }
 */

// Funktion für Cross Section s- und t-Kanal
float GetCrossFctComplete(float x) {
	return (sin(x)
			* ((1 + cos(x / 2) * cos(x / 2) * cos(x / 2) * cos(x / 2))
					/ (sin(x / 2) * sin(x / 2) * sin(x / 2) * sin(x / 2))
					- (2 * cos(x / 2) * cos(x / 2) * cos(x / 2) * cos(x / 2))
							/ (sin(x / 2) * sin(x / 2))
					+ (1 + cos(x / 2) * cos(x / 2)) / 2));
}

bool CheckCrossFctComplete(float x, float y) {
	if (GetCrossFctComplete(x) > y) {
		return true;
	}
	return false;
}

// Umrechnen von pb zu tatsächlichen Events
float GetEvents(float a, float lumi) {
	float crosssection = a * 0.00745 * 0.00745 / (4 * 200 * 200) * 197 * 197
			* pow(10, 4);
	cout << "crosssection = " << crosssection << "pb" << endl;
	return (a * 0.00745 * 0.00745 / (4 * 200 * 200) * 197 * 197 * pow(10, 4)
			* lumi);
}

// Integral Theta
float GetIntegralTheta(float thetaMin, float thetaMax, float ymin, float ymax,
		int loopnumber, float sigma) {
	//xmin und xmax sind die thetas
	float dy, theta, dTheta, y, integral, xmin, xmax, x;
	int accepted = 0;
	dTheta = thetaMax - thetaMin;
	//cout << "dtheta = " << dTheta << endl;
	dy = ymax - ymin;
	for (int i = 0; i < loopnumber; i++) {
		//cout << "NUMBER = " << i << endl;

		theta = tr3.Rndm();
		theta = theta * dTheta + thetaMin;
		//cout << "theta = " << theta << endl;
		y = tr3.Rndm();
		y = y * dy + ymin;
		//cout << "y = " << y << endl;
		xmin = thetaMin - 5*sigma;
		xmax = thetaMax + 5*sigma;
		x = GetRndGaus(theta, xmin, xmax, sigma);
		//cout << "x = " << x << endl;
		if (CheckCrossFctComplete(x, y)) {
			accepted++;
		}
	}
	integral = (((float) accepted ) / (float) loopnumber) * dTheta * dy;
	cout << "Integralwert: " << integral << endl;
	return integral;
}

// Funktionen für Gaußverteilung
float GetGaus(float x, float mu, float sig) {
	return 1/(sig*sqrt(2*M_PI))*exp(
			-0.5 * (x - mu) * (x - mu) / (sig * sig));
}

bool CheckGaus(float x, float y, float mu, float sigma) {
	if (abs(x-mu)<5*sigma && y < GetGaus(x, mu, sigma)) {
		return true;
	}
	return false;
}

// Funktion die Zufallszahl nach Gaus erzeugt
float GetRndGaus(float mu, float sigma) {
	float xRnd = 0.;
	float yRnd = 0.;
	do {
		xRnd = tr3.Rndm() * 2 * M_PI;
		yRnd = tr3.Rndm()/(sigma*sqrt(2*M_PI));
	} while (!CheckGaus(xRnd, yRnd, mu, sigma));
	return xRnd;
}

float GetRndGaus(float mu, float xmin, float xmax, float sigma) {
	float xRnd = 0.;
	float yRnd = 0.;
	do {
		xRnd = tr3.Rndm() * 2 * M_PI;
		yRnd = tr3.Rndm()/(sigma*sqrt(2*M_PI));
	} while (!CheckGaus(xRnd, yRnd, mu, sigma) && xRnd < xmax && xRnd > xmin);
	return xRnd;
}
