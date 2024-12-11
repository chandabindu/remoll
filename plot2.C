#include "include/remolltypes.hh"
int plot2()
{
	gStyle->SetOptStat(0);
        TH1D *h_1 = new TH1D("h1","h1",100,650,1150);
        TH1D *h_2 = new TH1D("h2","h2",100,650,1150);
        TH1D *h_3 = new TH1D("ep","ep",100,650,1150);

        //TFile f1("./Maindet_test_beam_pzG0.root");
        TFile f1("./Maindet_test_ee2_pzG0.root");
        //TFile f1("./Maindet_test_ep1_pzG0.root");
        
        //TFile f2("analysis/maindet_beam_acceptance.root");
        TFile f2("analysis/remollSkimTree_ee2.root");
        //TFile f2("analysis/remollSkimTree_ep1.root");
	/*float factor1 = 1.e-9/7./70/4199;//for unshielded
	float factor2 = 1.e-9/7./70./2200.;
	float factor3 = 1.e-9/7./70./2200.;*/

	float factor1 = 1.e-9*6.25e12/(1.5e7);//for shielded
	float factor2 = 1.e-9/(85.*3.);
	float factor3 = 1.e-9/85.;

	TTree *T1 = (TTree*)f1.Get("T");
	TTree *T2 = (TTree*)f2.Get("T");

	if(!T1) return 0;
	if(!T2) return 0;

	Double_t rate1;
	vector<remollGenericDetectorHit_t> *hit1=0;
	remollEvent_t *ev1=0;
	T1->SetBranchAddress("rate",&rate1);
	T1->SetBranchAddress("hit",&hit1);
	T1->SetBranchAddress("ev",&ev1);
	long entries1 = T1->GetEntries();
	cout<<"Beam entries "<<entries1<<endl;

	for(int ij=0;ij<entries1; ij++){
		T1->GetEntry(ij);
		for(int pk=0;pk<hit1->size();pk++){
			if(hit1->at(pk).det==1 && hit1->at(pk).r>=650 && hit1->at(pk).r<=1150 && hit1->at(pk).pid==11){
//				cout<<"Test det "<<hit1->at(pk).det<<" r "<<hit1->at(pk).r<<" e "<<hit1->at(pk).e<<endl;
				h_1->Fill(hit1->at(pk).r,2*rate1);
			}
		}
	}

	Double_t rate2;
	vector<remollGenericDetectorHit_t> *hit2=0;
	remollEvent_t *ev2=0;
	T2->SetBranchAddress("rate",&rate2);
	T2->SetBranchAddress("hit",&hit2);
	T2->SetBranchAddress("ev",&ev2);
	long entries2 = T2->GetEntries();
	cout<<"Beam entries "<<entries2<<endl;

	for(int ij=0;ij<entries2; ij++){
		T2->GetEntry(ij);
		for(int pk=0;pk<hit2->size();pk++){
			if(hit2->at(pk).det==28 && hit2->at(pk).r>=650 && hit2->at(pk).r<=1150 && hit2->at(pk).pid==11){
//				cout<<"Test det "<<hit2->at(pk).det<<" r "<<hit2->at(pk).r<<" e "<<hit2->at(pk).e<<" rate "<<rate2<<endl;
				h_2->Fill(hit2->at(pk).r,rate2);
			}
		}
	}


//	std::cout<<"Before Scaling h4 integral "<<h4->Integral()<<" entries "<<h4->GetEntries()<<" ee int "<<h_2->Integral()<<" ee entries "<<h_2->GetEntries()<<" ep int "<<h_3->Integral()<<" ep entries "<<h_3->GetEntries()<<" beam int "<<h_1->Integral()<<" beam entries "<<h_1->GetEntries()<<std::endl;

	h_1->Scale(factor2);//ha_1->Scale(factor1);//htheta_1->Scale(factor1);
	h_2->Scale(factor2);//ha_2->Scale(factor2);//htheta_2->Scale(factor2);
 
	std::cout<<"After Scaling h1 integral "<<h_1->Integral()<<" h_1 entries "<<h_1->GetEntries()<<" h_2 integral "<<h_2->Integral()<<" h_2 entries "<<h_2->GetEntries()<<std::endl;
	//std::cout<<"Unshielded After rebin h4 integral "<<h4->Integral()<<" entries "<<h4->GetEntries()<<" ep "<<h_2->Integral()<<" ep entries "<<h_2->GetEntries()<<std::endl;
	
	//std::cout<<"Without entries "<<without_w->GetEntries()<<"  with "<<with_w->GetEntries()<<std::endl;
        h_1->SetLineColor(kRed);
        h_2->SetLineColor(kBlack);

        TCanvas *plot = new TCanvas("Plot","plot",800,600);
	//gStyle->SetOptStat(0);
	TLatex latex;
	latex.SetTextSize(0.05);
	latex.SetTextAlign(13);

	//plot->SetLogy();
        h_2->Draw("HIST");
        h_1->Draw("HIST&&SAME");
	h_1->GetXaxis()->SetTitle("Radius at main det. plane (mm)");
	h_1->GetYaxis()->SetTitle("Rate (GHz/(5mm.#muA))");
        h_1->GetXaxis()->SetRangeUser(600,1200);
        //h_1->GetYaxis()->SetRangeUser(0,0.15);
   //     h_1->GetYaxis()->SetRangeUser(8.e-4,1.5e1);
	h_1->GetYaxis()->SetTitleOffset(1.5);
	h_2->SetTitle("ee");
	latex.SetTextColor(kRed);
	latex.DrawLatex(800,0.14,Form("Secondary"));
	latex.SetTextColor(kBlack);
	latex.DrawLatex(800,0.13,Form("Primary"));
	plot->cd();
	plot->SaveAs("test.png");

return 0;
}


