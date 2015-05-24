#include "stdlib.h"
#include <iostream>
#include <TROOT.h>
#include <TH2F.h>
#include <TCanvas.h>
#include "TStyle.h"
#include "TRandom3.h"
#include "TRandom2.h"

/*
 * Deklaration von u_j als globale Variable. Damit wird, wenn in getRandomNumber() die j-te
 *  Zahl berechnet wird, die j-1-te Zahl nicht nochmal berechnet, da sie noch gespeichert ist
 *  (vermindert Rechenaufwand, die andere Methode brachte meinen PC arg an die Grenzen für
 *  viele Zufallszahlen)
 */
unsigned int u_j;
unsigned int a;
unsigned long m;

int main() {
	//Deklaration der Zufallszahlen Funktion
	double GetRandomNumber(unsigned int u_0, unsigned int a, unsigned long m,
			unsigned long j);
	double getRandomNumber();
	void init(unsigned int u_0, unsigned int a_, unsigned long m_);

	init(1, 476, 657285961);
	TRandom2* tr2 = new TRandom2();
	TRandom3* tr3 = new TRandom3();

	//Ausblenden der Style Box im Histogramm
	gStyle->SetOptStat("");

	//Erstellen einer Zeichenfläche
	TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);

	//Erstellen eines 2D-Histogramms
	TH2F* rndHist = new TH2F("hist1", "Random numbers, TRandom2", 100, 0., 1.,
			100, 0., 1.);

	//TODO: Zufallszahlen in Histogramm füllen

	/*
	 * Das hier ist für den alten eigenen Zufallszahlengenerator
	 */
//	for (int i = 0; i < 1000; i += 2) {
//		double x = GetRandomNumber(1, 40692, 2147483399, i);
//		double y = GetRandomNumber(1, 40692, 2147483399, i + 1);
//		rndHist->Fill(x, y);
//	}
	/*
	 * Das hier ist für TRandom2
	 */
//	for (int i = 0; i < 1000000; i++) {
//		double x = tr2->Rndm();
//		double y = tr2->Rndm();
//		rndHist->Fill(x, y);
//	}
	/*
	 * Das hier ist für TRandom3
	 */
//	for (int i = 0; i < 1000000; i++) {
//		double x = tr3->Rndm();
//		double y = tr3->Rndm();
//		rndHist->Fill(x, y);
//	}
	/*
	 * Das hier ist für den neuen eigenen Zufallszahlengenerator
	 */
	for (int i = 0; i < 1000000; i++) {
		double x = getRandomNumber();
		double y = getRandomNumber();
		rndHist->Fill(x, y);
	}

	//Zeichnen und speichern des Histogramms
	rndHist->Draw("colz");
	c1->SaveAs("RandomNumbers.png");

}

//Initialisierung von u_j, a und m als globale Variablen
void init(unsigned int u_0, unsigned int a_, unsigned long m_) {
	u_j = u_0;
	a = a_;
	m = m_;
}

//TODO: Funktion zum erstellen von Zufallszahlen
/*
 * Hier die Funktion wie (vermutlich) ursprünglich gedacht. Für ein bestimmtes j wird die
 * Zufallszahl x_j berechnet. Dabei läuft die for-Schleife j-mal durch. Da in der obigen
 * for-Schleife zum Berechnen von sehr vielen Zufallszahlen die for-Schleife (unsinnigerweise)
 * immer wieder durchlaufen wird, obwohl die Zahlen eigentlich für das x_(j-1) schon
 * berechnet wurden, wurde die Funktion kurzerhand umgeschrieben (getRandomNumber).
 */

//double GetRandomNumber(unsigned int u_0, unsigned int a, unsigned long m, unsigned long j){
//	unsigned int u_j = u_0;
//	double x_j;
//
//	for(int i=0; i<j+1; i++){
//		u_j=(a*u_j)%m;
//	}
//	x_j=(double)u_j/m;
//	return x_j;
//}
double getRandomNumber() {
	double x_j = (double) u_j / m;
	u_j = (a * u_j) % m; // calculate u_j for next random number

	return x_j;
}

