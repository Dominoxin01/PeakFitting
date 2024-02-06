#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "TF1.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TUnixSystem.h"
//This is a rubbish solution, but it works for now :P
#include "Fit_Functions.cpp"
#include "Data_read_in.C"

//Function Declarations:
std::vector<int> DataReadIn_SPE(const char *filename);
std::vector<int> DataReadIn_CHN(const char *filename);
double fit1g(double *x, double *p);

//Global declarations:
TH1I *h1;
TCanvas *c1;

std::vector<int> dat;

void SpecLoad(const char *filename, bool plot = true){
//This function loads in the data and plots it to 
	const char *ext;

	if(h1){
		h1->~TH1I();
	}
	if(c1){
		c1->~TCanvas();
	}
	double low_x, up_x;
	ext = strrchr(filename, '.');
	
	if(strcmp(ext,"Spe")){
		std::vector<int> dat = DataReadIn_SPE(filename);
	}
	else if(strcmp(ext,"Chn")){
		std::vector<int> dat = DataReadIn_CHN(filename);
	}
	else if(!ext){
		std::cout << "Cannot find file extension." << std::endl;
	}
	else {std::cout << "Unknown file extension" << std::endl;}
	
	
	
	
	h1 = new TH1I("h1", filename, dat.size(), 0, dat.size());
	
	for (int i = 0; i < dat.size(); i++){
		h1->SetBinContent(i, dat[i]);
	}
	
	if(plot == false){
		return;
		}
	else if(plot == true){
		c1 = new TCanvas("c1", "histo", 1200, 600); 
		h1->Draw("hist");
		}
	else{
		std::cout<< "Something broke. What did you do?"<<std::endl;
		}
}

void FitData(){
//This function fits a function with a linear background and a Gussian peak, fit1g for one gaussian.
	int low_x, up_x;
	int number_peaks;
	int FWHM_est;
	int cen_1, cen_2, cen_3, cen_4;

	std::cout << "How many peaks: 1, 2, 3 or 4?" << std::endl;
	std::cin >> number_peaks; //taking user input
	 
	//asking the user for the lower and upper fit boundires:
	std::cout << "Lower channel number for the fit boundry:" << std::endl;
	std::cin >> low_x ; //taking user input
	std::cout << "Upper channel number for the fit boundry:" << std::endl;
	std::cin >> up_x ; //taking user input
	
	std::cout << "FWHM estimate:" << std::endl;
	std::cin >> FWHM_est;
	
	h1->GetXaxis()->SetRangeUser(low_x, up_x);
	
	switch (number_peaks){
	case 1:{
		TF1 *f1 = f1g(h1, low_x, up_x, FWHM_est);
    		h1->Fit("f1", "IMRB", "", low_x, up_x);
    		std::cout << "Chi^2: " <<f1 -> GetChisquare()<< std::endl;
		std::cout << "ndf: " << f1 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f1 -> GetChisquare()/f1 -> GetNDF() << std::endl;
		break;
		}
		
	case 2: {
		TF1 *f2 = f2g(h1, low_x, up_x, FWHM_est);
		h1->Fit("f2", "IMRB", "", low_x, up_x);
		std::cout << "Chi^2: " <<f2 -> GetChisquare()<< std::endl;
		std::cout << "ndf: " << f2 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f2 -> GetChisquare()/f2 -> GetNDF() << std::endl;
		break;
		}
	case 3: {
		TF1 *f3 = f3g(h1, low_x, up_x, FWHM_est);
		
		h1->Fit("f3", "IMRB", "", low_x, up_x);
		std::cout << "Chi^2: " <<f3 -> GetChisquare()<< std::endl;
		std::cout << "ndf: " << f3 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f3 -> GetChisquare()/f3 -> GetNDF() << std::endl;
		break;
		}
	case 4: {
		TF1 *f4 = f4g(h1, low_x, up_x, FWHM_est);
		h1->Fit("f4", "IMRB", "", low_x, up_x);
		std::cout << "Chi^2: " << f4 -> GetChisquare() << std::endl;
		std::cout << "ndf: " << f4 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f4 -> GetChisquare()/f4 -> GetNDF() << std::endl;
		break;
		}
	}
	
	
	h1->Draw("Same");
}

void FitDataArg1g(int low_x, int up_x, int FWHM_est, int cen_1){
	//This function is a different implementation of FitData(), which can fit only 1 Gausian, but takes in arguments for fitting parameters, rather than asking from them. Used in FitDirectory().
	TF1 *f1 = f1g(h1, low_x, up_x, FWHM_est);
    	h1->Fit("f1", "IMRB", "", low_x, up_x);
    	std::cout << "Chi^2: " <<f1 -> GetChisquare()<< std::endl;
	std::cout << "ndf: " << f1 -> GetNDF()<< std::endl;
	std::cout << "Chi^2/ndf: " << f1 -> GetChisquare()/f1 -> GetNDF() << std::endl;


}

void FitDirectory(const char *dirname){
	int low_x, up_x;
	int number_peaks;
	int FWHM_est;
	
	std::string answer;
	ask_for_input: std::cout << "This function fits one peak across a given interval in all spectra in the given directory, in alphabetical order. Do you want to continue? [y/n]" << std::endl;
	std::cin >> answer;
	if (!answer.compare("n")){
		std::cout << std::endl;	
		std::cout << "Leaving program." << std::endl;
		return;
	}
	else if(!answer.compare("y")){
		std::cout << std::endl;
		std::cout << "Continuing" << std::endl;
	}

	else{
		std::cout << std::endl;
		std::cout << "Invalid answer." << std::endl;
		goto ask_for_input; //goes to the second line in this functions.
	}
	
	//Opening an interface with a usinx based operating system:
	TUnixSystem *os = new TUnixSystem();
	//Using said interface to open a directory given by the dirname pathname:
	void *dir = os -> OpenDirectory(dirname);
	
	//asking the user for the lower and upper fit boundires:
	std::cout << "Lower channel number for the fit boundry:" << std::endl;
	std::cin >> low_x ; //taking user input
	std::cout << "Upper channel number for the fit boundry:" << std::endl;
	std::cin >> up_x ; //taking user input
	
	std::cout << "FWHM estimate:" << std::endl;
	std::cin >> FWHM_est;
	
	//preparing a filename pointer:
	const char *filename;
	//looping over the files in a given directory and printing them:
	
	TSystem *sy = new TSystem("sy", "sy");
	sy -> RedirectOutput("dir_fit_result.txt", "a");
	while (( filename = os  -> GetDirEntry(dir))){
		SpecLoad(filename, false);
		FitDataArg1g(low_x, up_x, FWHM_est, cen_1);
	std::cout << filename << std::endl;
	}
	sy->RedirectOutput(0);
}



void Zoom(int low_x, int up_x){
//This function re-adjust the display window to specifed channel numbers.
	h1->GetXaxis()->SetRangeUser(low_x, up_x);
	h1->Draw("Same");
}

void Zoom(){
	int low_x, up_x;
	std::cout << "Enter the lower boundry channel:" << std::endl;
	std::cin >> low_x;
	std::cout << "Enter the upper boundry channel:" << std::endl;
	std::cin >> up_x;
	h1->GetXaxis()->SetRangeUser(low_x, up_x);
	h1->Draw("Same");
	
}

void ZoomOut(){
//This funciton goes from the current zoom in state to showing the full spectrum.
	std::vector<int> dat;
	h1->GetXaxis()->SetRangeUser(0, dat.size());
	h1->Draw("Same");
}



double fit1g(double *x, double *p) {
//An exaple function used by Prof. Peter Jones in his Buffit macro, kept in case but not used in the code itself.
  double C_INV_SQRT_2PI = 0.39894228; 
  double arg1 = 0; 
  double dX = 1;
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35);
  if( (p[3]+p[4]*x[0]) < 0. ) return 0.; // background >=0
  double fitval =  
    p[2]*C_INV_SQRT_2PI*dX*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[3]+p[4]*x[0]; 
  return fitval; 
} 
