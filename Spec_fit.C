#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//Function Declarations:
std::vector<int> DataReadIn(const char *filename);
double fit1g(double *x, double *p);
TH1I *h1;
TCanvas *c1;

void SpecLoad(const char *filename){
//This function loads in the data and plots it to 
	//h1->Clear();
	double low_x, up_x;
	std::vector<int> data = DataReadIn(filename);
	std::cout << "Worsk so far (Data Read):D"<< std::endl;
	c1 = new TCanvas("c1", "histo", 1200, 600); 
	h1 = new TH1I("h1", filename, data.size(), 0, data.size());
	
	
	for (int i = 0; i < data.size(); i++){
		h1->SetBinContent(i, data[i]);
	}
	h1->Draw("hist");

}

void FitData(){
//This function fits a function with a linear background and a Gussian peak, fit1g for one gaussian.
	int low_x, up_x;

	//asking the user for the lower and upper fit boundires:
	std::cout << "Lower channel number for the fit boundry:" << std::endl;
	std::cin >> low_x ; //taking user input
	std::cout << "Upper channel number for the fit boundry:" << std::endl;
	std::cin >> up_x ; //taking user input
	h1->GetXaxis()->SetRangeUser(low_x, up_x);
	
	TF1 *f1 = new TF1("f1", "gausn(0) + pol1(3)", low_x, up_x); // fit1g last 5
	f1->SetParNames("Counts", "Centroid" ,"Sigma","Intercept","Slope");
    	f1->SetParLimits(1, low_x, up_x);
    	f1->SetParameter(1, 0.5*(up_x-low_x)+low_x);
    	f1->SetParLimits(2,0., up_x-low_x);
    	f1->SetParameter(2, 0.5*(up_x-low_x)/2.35);
    	std::cout << up_x-low_x << std::endl;
    	//f1->SetParLimits(1,0., 1000000000);
    	f1->SetParLimits(0, 0., 1000000000000);
    	f1->SetParameter(0, h1->GetBinContent(int(0.5*(up_x-low_x))));
	h1->Fit("f1", "IM", "", low_x, up_x);
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
