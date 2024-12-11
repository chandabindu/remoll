//#include "include/remolltypes.hh"
int plot1()
{
	gStyle->SetOptStat(0);
        TH1D *h_1 = new TH1D("beam","beam",100,650,1150);
        TH1D *h_2 = new TH1D("ee","ee",100,650,1150);
        TH1D *h_3 = new TH1D("ep","ep",100,650,1150);
        TH1D *ha_1 = new TH1D();
        TH1D *ha_2 = new TH1D();
        TH1D *ha_3 = new TH1D();
        TH1D *htheta_1 = new TH1D("htheta_1","Vertex Theta distributions", 150,0,0.06);
        TH1D *htheta_2 = new TH1D("htheta_2","Vertex Theta distributions", 150,0,0.06);
        TH1D *htheta_3 = new TH1D("htheta_3","Vertex Theta distributions", 150,0,0.06);
/*        TFile f1("./Maindet_beam_V2p4_pzG0.root");
        TFile f2("./Maindet_ee_V2p4_pzG0.root");
        TFile f3("./Maindet_ep_V2p4_pzG0.root");*/
        
        TFile f1("/lustre19/expphy/volatile/halla/moller12gev/chandan/sim_out/SecondarySim/maindet_beam_acceptance.root");
        TFile f2("/lustre19/expphy/volatile/halla/moller12gev/chandan/sim_out/SecondarySim/remollSkimTree_ee.root");
        TFile f3("/lustre19/expphy/volatile/halla/moller12gev/chandan/sim_out/SecondarySim/remollSkimTree_ep.root");
	/*float factor1 = 1.e-9/7./70/4199;//for unshielded
	float factor2 = 1.e-9/7./70./2200.;
	float factor3 = 1.e-9/7./70./2200.;*/

	float factor1 = 1.e-9*6.25e12/(1.5e7);//for shielded
	float factor2 = 1.e-9/85.;
	float factor3 = 1.e-9/85.;

        /*h_1=(TH1D*)f1.Get("h_maindet_neutron_E0_d_r");
        h_2=(TH1D*)f2.Get("h_maindet_neutron_E0_d_r");
        h_3=(TH1D*)f3.Get("h_maindet_neutron_E0_d_r");
        htheta_1=(TH1D*)f1.Get("h_afterCol4_electron_E0_d1_thlab");
        htheta_2=(TH1D*)f2.Get("h_afterCol4_electron_E0_d1_thlab");
        htheta_3=(TH1D*)f3.Get("h_afterCol4_electron_E0_d1_thlab");*/

	TTree *T1 = (TTree*)f1.Get("T");
	TTree *T2 = (TTree*)f2.Get("T");
	TTree *T3 = (TTree*)f3.Get("T");

	if(!T1) return 0;
	if(!T2) return 0;
	if(!T3) return 0;

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
			if(hit1->at(pk).det==28 && hit1->at(pk).r>=650 && hit1->at(pk).r<=1150 && hit1->at(pk).pid==11){
//				cout<<"Test det "<<hit1->at(pk).det<<" r "<<hit1->at(pk).r<<" e "<<hit1->at(pk).e<<endl;
				h_1->Fill(hit1->at(pk).r);
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

	Double_t rate3;
	vector<remollGenericDetectorHit_t> *hit3=0;
	remollEvent_t *ev3=0;
	T3->SetBranchAddress("rate",&rate3);
	T3->SetBranchAddress("hit",&hit3);
	T3->SetBranchAddress("ev",&ev3);
	long entries3 = T3->GetEntries();
	cout<<"Beam entries "<<entries3<<endl;

	for(int ij=0;ij<entries3; ij++){
		T3->GetEntry(ij);
		for(int pk=0;pk<hit3->size();pk++){
			if(hit3->at(pk).det==28 && hit3->at(pk).r>=650 && hit3->at(pk).r<=1150 && hit3->at(pk).pid==11){
	//			cout<<"Test det "<<hit3->at(pk).det<<" r "<<hit3->at(pk).r<<" e "<<hit3->at(pk).e<<"  rate  "<<rate3<<endl;
				h_3->Fill(hit3->at(pk).r,rate3);
			}
		}
	}


//	std::cout<<"Before Scaling h4 integral "<<h4->Integral()<<" entries "<<h4->GetEntries()<<" ee int "<<h_2->Integral()<<" ee entries "<<h_2->GetEntries()<<" ep int "<<h_3->Integral()<<" ep entries "<<h_3->GetEntries()<<" beam int "<<h_1->Integral()<<" beam entries "<<h_1->GetEntries()<<std::endl;

	h_1->Scale(factor1);//ha_1->Scale(factor1);//htheta_1->Scale(factor1);
	h_2->Scale(factor2);//ha_2->Scale(factor2);//htheta_2->Scale(factor2);
	h_3->Scale(factor3);//ha_3->Scale(factor3);//htheta_3->Scale(factor3);
 
	TH1D *h4 = new TH1D("h4","h4",100,650,1150);//for ep+ee at maindet
	TH1D *h5 = new TH1D("h5","h5",100,650,1150);//for ep+ee at aftercol4
	h4->Add(h_2);
	h4->Add(h_3);
  
       	//h_1->Rebin(5.);h_2->Rebin(5.);h_3->Rebin(5.);
       	//ha_1->Rebin(5.);ha_2->Rebin(5.);ha_3->Rebin(5.);
	//std::cout<<"shielded Before rebin h4 integral "<<h4->Integral()<<" ep "<<h_2->Integral()<<std::endl;

	//h5->Add(ha_2);
	//h5->Add(ha_3);

       	//h_1->Rebin(2.);h_2->Rebin(2.);h_3->Rebin(2.);h4->Rebin(2.);
       	//ha_1->Rebin(5.);ha_2->Rebin(5.);ha_3->Rebin(5.);h5->Rebin(5.);
	std::cout<<"After Scaling h4 integral "<<h4->Integral()<<" entries "<<h4->GetEntries()<<" ee int "<<h_2->Integral()<<" ee entries "<<h_2->GetEntries()<<" ep int "<<h_3->Integral()<<" ep entries "<<h_3->GetEntries()<<" beam int "<<h_1->Integral()<<" beam entries "<<h_1->GetEntries()<<std::endl;
	//std::cout<<"Unshielded After rebin h4 integral "<<h4->Integral()<<" entries "<<h4->GetEntries()<<" ep "<<h_2->Integral()<<" ep entries "<<h_2->GetEntries()<<std::endl;
	
	//std::cout<<"Without entries "<<without_w->GetEntries()<<"  with "<<with_w->GetEntries()<<std::endl;
        h_1->SetLineColor(kRed);
        h_3->SetLineColor(kBlue);
        h_2->SetLineColor(kBlack);
        h4->SetLineColor(kMagenta);
        //ha_1->SetLineColor(kRed);
        //ha_2->SetLineColor(kBlue);
        //ha_3->SetLineColor(kBlack);
        //h5->SetLineColor(kMagenta);
        /*htheta_1->SetLineColor(kRed);
        htheta_2->SetLineColor(kBlue);
        htheta_3->SetLineColor(kBlack);*/
        TCanvas *plot = new TCanvas("Plot","plot",800,600);
	//gStyle->SetOptStat(0);
	TLatex latex;
	latex.SetTextSize(0.05);
	latex.SetTextAlign(13);

	//latex.DrawLatex(0.2,0.2,"Red - with old geometry");
/*
	TLatex latex1;
	latex1.SetTextSize(0.05);
	latex1.SetTextAlign(13);
	latex1.DrawLatex(0.5,without_w->GetMaximum()/1.5,"Blue - with new geometry");
*/	
	//plot->SetLogy();
        h_1->Draw("HIST");
        h_2->Draw("HIST&&SAME");
        h_3->Draw("same && HIST");
        h4->Draw("same && HIST");
	h_1->GetXaxis()->SetTitle("Radius at main det. plane (mm)");
	h_1->GetYaxis()->SetTitle("Rate (GHz/(5mm.#muA))");
        h_1->GetXaxis()->SetRangeUser(600,1200);
        //h_1->GetYaxis()->SetRangeUser(0,0.15);
   //     h_1->GetYaxis()->SetRangeUser(8.e-4,1.5e1);
	/*latex.SetTextColor(kRed);
	latex.DrawLatex(900,0.00025,Form("Beam"));
	latex.SetTextColor(kBlue);
	latex.DrawLatex(900,0.00016,Form("EP"));
	latex.SetTextColor(kBlack);
	latex.DrawLatex(900,0.000097,Form("MOLLER"));
	latex.SetTextColor(kMagenta);
	latex.DrawLatex(900,0.00006,Form("MOLLER+ep"));*/
	h_1->GetYaxis()->SetTitleOffset(1.5);
	latex.SetTextColor(kRed);
	latex.DrawLatex(850,0.14,Form("Beam"));
	latex.SetTextColor(kBlue);
	latex.DrawLatex(850,0.13,Form("EP"));
	latex.SetTextColor(kBlack);
	latex.DrawLatex(850,0.12,Form("MOLLER"));
	latex.SetTextColor(kMagenta);
	latex.DrawLatex(850,0.11,Form("MOLLER+ep"));
	plot->cd();
	plot->SaveAs("test.png");

       /* TCanvas *plot1 = new TCanvas("Plot1","Radial distributions for electrons at afterCol4",800,600);
	//gStyle->SetOptStat(0);
	plot1->SetLogy();
        ha_1->Draw("HIST");
        ha_2->Draw("same HIST");
        ha_3->Draw("same HIST");
        h5->Draw("same HIST");
        ha_1->GetXaxis()->SetRangeUser(0,225);
  //      ha_1->GetYaxis()->SetRangeUser(9.e-6,2.e-2);
	ha_1->GetXaxis()->SetTitle("Radius (mm)");
	ha_1->GetYaxis()->SetTitle("Rate (GHz/(5mm.#muA.Sector))");
	latex.SetTextColor(kRed);
	latex.DrawLatex(75,0.001,Form("Beam"));
	latex.SetTextColor(kBlue);
	latex.DrawLatex(75,0.0006,Form("EP"));
	latex.SetTextColor(kBlack);
	latex.DrawLatex(75,0.00035,Form("MOLLER"));
	latex.SetTextColor(kMagenta);
	latex.DrawLatex(75,0.0002,Form("MOLLER+ep"));
	plot1->cd();*/
	//plot1->SaveAs("plot/shielded_Electron_Rate_BoreVsAcceptance_toRing5_AtafterCol4.png");
//	plot1->SaveAs("plot/Unshielded_Electron_Rate_BoreVsAcceptance1_toRing5_AtafterCol4.png");
/*
	gStyle->SetOptStat(0);
	plot2->SetLogy();
        //ha_nopipe->GetYaxis()->SetRangeUser(5.0e3,1.0e5);
        htheta_1->Draw("HIST");
        htheta_2->Draw("same HIST");
        htheta_3->Draw("same HIST");
	htheta_1->GetXaxis()->SetTitle("#theta_{vertex} (rad) ");
	htheta_1->GetYaxis()->SetTitle("Rate (GHz/(1mm.#muA.Sector))");
	latex.SetTextColor(kRed);
	latex.DrawLatex(0.005,0.01,Form("Beam"));
	latex.SetTextColor(kBlue);
	latex.DrawLatex(0.005,0.006,Form("EP"));
	latex.SetTextColor(kBlack);
	latex.DrawLatex(0.005,0.0035,Form("MOLLER"));
	plot2->cd();
	plot2->SaveAs("plot/shielded_Theta_vertex_BoreVsAcceptance.png");*/
return 0;
}


