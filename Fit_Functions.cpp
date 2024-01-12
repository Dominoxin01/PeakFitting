#include "Fit_Functions.h"

	int number_peaks;
	int FWHM_est;
	int cen_1, cen_2, cen_3, cen_4;

TF1 *f1g(TH1I *h1, int low_x, int up_x, int FWHM_est){
//setting the fitting function as a scaled normalised Gaussian and a linear background:
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
    	f1->SetParameter(2, FWHM_est/2.35);
    	//std::cout << up_x-low_x << std::endl;
	return f1;
}

TF1 *f2g(TH1I *h1, int low_x, int up_x, int FWHM_est){
//setting the fitting function as two scaled normalised Gaussians and a linear background:
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
	return f2;

}

TF1 *f3g(TH1I *h1, int low_x, int up_x, int FWHM_est) {
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
	return f3;	
	}
	
TF1 *f4g(TH1I *h1, int low_x, int up_x, int FWHM_est){
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
	return f4;

}

