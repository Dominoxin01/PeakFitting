#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//This is a rubbish solution, but it works for now :P
#include "Fit_Functions.cpp"

//Function Declarations:
std::vector<int> DataReadIn(const char *filename);
double fit1g(double *x, double *p);

//Global declarations:
TH1I *h1;
TCanvas *c1;

std::vector<int> data;

void SpecLoad(const char *filename, bool plot = true){
//This function loads in the data and plots it to 
	//h1->Clear();
	double low_x, up_x;
	std::vector<int> data = DataReadIn(filename);
	h1 = new TH1I("h1", filename, data.size(), 0, data.size());
	
	for (int i = 0; i < data.size(); i++){
		h1->SetBinContent(i, data[i]);
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
		TF1 *f1 = f1g(h1, low_x, up_x);
    		h1->Fit("f1", "IMRB", "", low_x, up_x);
    		std::cout << "Chi^2: " <<f1 -> GetChisquare()<< std::endl;
		std::cout << "ndf: " << f1 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f1 -> GetChisquare()/f1 -> GetNDF() << std::endl;
		break;
		}
		
	case 2: {
		TF1 *f2 = f2g(h1, low_x, up_x);
		h1->Fit("f2", "IMRB", "", low_x, up_x);
		std::cout << "Chi^2: " <<f2 -> GetChisquare()<< std::endl;
		std::cout << "ndf: " << f2 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f2 -> GetChisquare()/f2 -> GetNDF() << std::endl;
		break;
		}
	case 3: {
		TF1 *f3 = f3g(h1, low_x, up_x);
		
		h1->Fit("f3", "IMRB", "", low_x, up_x);
		std::cout << "Chi^2: " <<f3 -> GetChisquare()<< std::endl;
		std::cout << "ndf: " << f3 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f3 -> GetChisquare()/f3 -> GetNDF() << std::endl;
		break;
		}
	case 4: {
		TF1 *f4 = f4g(h1, low_x, up_x);
		h1->Fit("f4", "IMRB", "", low_x, up_x);
		std::cout << "Chi^2: " << f4 -> GetChisquare() << std::endl;
		std::cout << "ndf: " << f4 -> GetNDF()<< std::endl;
		std::cout << "Chi^2/ndf: " << f4 -> GetChisquare()/f4 -> GetNDF() << std::endl;
		break;
		}
	}
	
	
	h1->Draw("Same");
}

void FitDirectory(const char *dirname){
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
	//preparing a filename pointer:
	const char *filename;
	//looping over the files in a given directory and printing them:
	while (( filename = os  -> GetDirEntry(dir))){
	std::cout << filename << std::endl;
	}
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
	std::vector<int> data;
	h1->GetXaxis()->SetRangeUser(0, data.size());
	h1->Draw("Same");
}

std::vector<int> DataReadIn(const char *filename){
//This function reads in the data from a .spe file. It recognizes the begining of data by finding a line with a "0 " string and saying that every line until a line containing a "$" contains data.
	std::string line;
	std::ifstream inFile;
	std::vector<int> data;
	bool Data_line = false;
	inFile.open(filename);
	int n;
	//Looping over all lines (line) in a file (inFile):
	while (getline(inFile, line)){
		//loading in a line as a string (iss(line)):
		std::istringstream iss(line);
		//std::cout << iss.str() << std::endl;
		//if the line containes $ charachter stop reading in the data:
		if (Data_line && iss.str().find("$")!= std::string::npos){
			Data_line = false;
		};
		
		if (Data_line == true){
			iss >> n;
			data.push_back (n);
		};
		
		//if the line contains "0 " charachters make sure that the next line is treated as data:
		if ((iss.str().find("0 ")!=std::string::npos)){
			Data_line = true;
		};
	};
	
	//used for checking if output array has the required data:
	//for (int i = 0; i < 1000; i++){
	//std::cout << data[i] << std::endl;
	//};
	return data;
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
