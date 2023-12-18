#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//Function Declarations:
std::vector<int> DataReadIn(const char *filename);
double fit1g(double *x, double *p);

//Global declarations:
TH1I *h1;
TCanvas *c1;

std::vector<int> data;

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
	case 1: {//setting the fitting function as a scaled normalised Gaussian and a linear background:
		TF1 *f1 = new TF1("f1", "gausn(0) + pol1(3)", low_x, up_x); // fit1g last 5
		f1->SetParName(0 , "1 Counts");
		f1->SetParName(1 , "1 Centroid");
		f1->SetParName(2 , "1 Sigma");
		f1->SetParName(3 , "Intercept");
		f1->SetParName(4 , "Slope");
		//setting limits on the counts parameter:
		f1->SetParLimits(0, 0., 1000000000000);
		//giving an initial guess on the counts:
    		f1->SetParameter(0, h1->GetBinContent(int(0.5*(up_x-low_x))));
    		//assuming that the centroid will be within the selected region:
    		f1->SetParLimits(1, low_x, up_x);
    		//assumming that the centroid is in the middle of the selected region:
    		f1->SetParameter(1, 0.5*(up_x-low_x)+low_x);
    		//the standard deviation should be less than the selected region:
    		f1->SetParLimits(2,0., up_x-low_x);
    		//assuming that the standard deviation is half the selected width (may be sub-optimal:)
    		f1->SetParameter(2, 0.5*(up_x-low_x)/2.35);
    		//std::cout << up_x-low_x << std::endl;
    		
    		f1 -> GetChisquare();
		f1 -> GetNDF();
    		h1->Fit("f1", "IM", "", low_x, up_x);
		break;
		}
		
	case 2: {//setting the fitting function as two scaled normalised Gaussians and a linear background:
		TF1 *f2 = new TF1("f2", "gausn(0) + gausn(3) + pol1(6)", low_x, up_x);
		f2->SetParName(0 , "1 Counts");
		f2->SetParName(1 , "1 Centroid");
		f2->SetParName(2 , "1 Sigma");
		f2->SetParName(3 , "2 Counts");
		f2->SetParName(4 , "2 Centroid");
		f2->SetParName(5 , "2 Sigma");
		f2->SetParName(6 , "Intercept");
		f2->SetParName(7 , "Slope");
		//Assuming tha both centroids are seen in the selected region:
		f2->SetParLimits(1, low_x, up_x);
		f2->SetParLimits(4, low_x, up_x);
		//asking th user for estimated centroid valuesand taking the extimate of the scaling:
		std::cout << "Centroid 1:" << std::endl;
		std::cin >> cen_1;
		f2->SetParameter(1, cen_1);
		f2->SetParameter(0, h1->GetBinContent(int(cen_1)));
		
		std::cout << "Centroid 2:" << std::endl;
		std::cin >> cen_2;
		f2->SetParameter(4, cen_2);
		f2->SetParameter(3, h1->GetBinContent(int(cen_2)));
		
		//setting up the fitting of the standard deviation:
		f2->SetParLimits(2,0., up_x-low_x);
		f2->SetParLimits(5,0., up_x-low_x);
		f2->SetParameter(2, FWHM_est/2.35);
		f2->SetParameter(5, FWHM_est/2.35);
		
		f2 -> GetChisquare();
		f2 -> GetNDF();
		h1->Fit("f2", "IM", "", low_x, up_x);
		break;
		}
	case 3: {
		TF1 *f3 = new TF1("f3", "gausn(0) + gausn(3) + gausn(6) + pol1(9)", low_x, up_x);
		f3->SetParName(0 , "1 Counts");
		f3->SetParName(1 , "1 Centroid");
		f3->SetParName(2 , "1 Sigma");
		f3->SetParName(3 , "2 Counts");
		f3->SetParName(4 , "2 Centroid");
		f3->SetParName(5 , "2 Sigma");
		f3->SetParName(6 , "3 Counts");
		f3->SetParName(7 , "3 Centroid");
		f3->SetParName(8 , "3 Sigma");
		f3->SetParName(9 , "Intercept");
		f3->SetParName(10, "Slope");
		//Assuming tha both centroids are seen in the selected region:
		f3->SetParLimits(1, low_x, up_x);
		f3->SetParLimits(4, low_x, up_x);
		f3->SetParLimits(7, low_x, up_x);
		// asking th user for estimated centroid values and taking the extimate of the scaling:
		std::cout << "Centroid 1:" << std::endl;
		std::cin >> cen_1;
		f3->SetParameter(1, cen_1);
		f3->SetParameter(0, h1->GetBinContent(int(cen_1)));
		
		std::cout << "Centroid 2:" << std::endl;
		std::cin >> cen_2;
		f3->SetParameter(4, cen_2);
		f3->SetParameter(3, h1->GetBinContent(int(cen_2)));
		
		std::cout << "Centroid 3:" << std::endl;
		std::cin >> cen_3;
		f3->SetParameter(7, cen_3);
		f3->SetParameter(6, h1->GetBinContent(int(cen_3)));
		//setting up the fitting of the standard deviation:
		f3->SetParLimits(2,0., up_x-low_x);
		f3->SetParLimits(5,0., up_x-low_x);
		f3->SetParLimits(8, 0., up_x-low_x);
		f3->SetParameter(2, FWHM_est/2.35);
		f3->SetParameter(5, FWHM_est/2.35);
		f3->SetParameter(8, FWHM_est/2.35);
		
		f3 -> GetChisquare();
		f3 -> GetNDF();
		h1->Fit("f3", "IM", "", low_x, up_x);
		break;
		}
	case 4: {
		TF1 *f4 = new TF1("f4", "gausn(0) + gausn(3) + gausn(6) + gausn(9) + pol1(12)", low_x, up_x);
		f4->SetParName(0 , "1 Counts");
		f4->SetParName(1 , "1 Centroid");
		f4->SetParName(2 , "1 Sigma");
		f4->SetParName(3 , "2 Counts");
		f4->SetParName(4 , "2 Centroid");
		f4->SetParName(5 , "2 Sigma");
		f4->SetParName(6 , "3 Counts");
		f4->SetParName(7 , "3 Centroid");
		f4->SetParName(8 , "3 Sigma");
		f4->SetParName(9 , "4 Counts");
		f4->SetParName(10, "4 Centroid");
		f4->SetParName(11, "4 Sigma");
		f4->SetParName(12, "Intercept");
		f4->SetParName(13, "Slope");
		//Assuming tha both centroids are seen in the selected region:
		f4->SetParLimits(1, low_x, up_x);
		f4->SetParLimits(4, low_x, up_x);
		f4->SetParLimits(7, low_x, up_x);
		f4->SetParLimits(10, low_x, up_x);
		// asking th user for estimated centroid values and taking the extimate of the scaling:
		std::cout << "Centroid 1:" << std::endl;
		std::cin >> cen_1;
		f4->SetParameter(1, cen_1);
		f4->SetParameter(0, h1->GetBinContent(int(cen_1)));
		
		std::cout << "Centroid 2:" << std::endl;
		std::cin >> cen_2;
		f4->SetParameter(4, cen_2);
		f4->SetParameter(3, h1->GetBinContent(int(cen_2)));
		
		std::cout << "Centroid 3:" << std::endl;
		std::cin >> cen_3;
		f4->SetParameter(7, cen_3);
		f4->SetParameter(6, h1->GetBinContent(int(cen_3)));
		
		std::cout << "Centroid 4" << std::endl;
		std::cin >> cen_4;
		f4->SetParameter(10, cen_4);
		f4->SetParameter(9, h1->GetBinContent(int(cen_4)));
		
		//setting up the fitting of the standard deviation:
		f4->SetParLimits(2,0., up_x-low_x);
		f4->SetParLimits(5,0., up_x-low_x);
		f4->SetParLimits(8, 0., up_x-low_x);
		f4->SetParLimits(11, 0., up_x-low_x);
		f4->SetParameter(2, FWHM_est/2.35);
		f4->SetParameter(5, FWHM_est/2.35);
		f4->SetParameter(8, FWHM_est/2.35);
		f4->SetParameter(11, FWHM_est/2.35);
		
		f4 -> GetChisquare();
		f4 -> GetNDF();
		h1->Fit("f4", "IM", "", low_x, up_x);

		break;
		}
	}
	
	
	h1->Draw("Same");
}

void FitDirectory(const char *dirname){
	std::string answer;
	ask_for_input: std::cout << "This function fits one peak across a given interval in all spectra in the given directory. Do you want to continue? [y/n]" << std::endl;
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
		goto ask_for_input;
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
