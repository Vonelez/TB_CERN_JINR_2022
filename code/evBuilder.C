#define evBuilder_cxx
#include "evBuilder.h"
#include <TH1.h>

void evBuilder::threePlotDrawF(TH1D *h1, TH1D *h2, TH1D *h3)
{
    h1->SetLineColor(kGreen - 2);
    h2->SetLineColor(kMagenta);
    h3->SetLineColor(kBlack);
    h1->SetStats(0);
    h2->SetStats(0);
    h3->SetStats(0);


    TCanvas *three_plots = new TCanvas("3 det correlation", "3 det correlation", 1400, 900);
    three_plots->cd();
    
    h2->Draw();
    h1->Draw("SAME");
    h3->Draw("SAME");

    h1->Fit("gaus", "", "", -200, 200); // TMM - TScint
    h2->Fit("gaus", "", "", -200, 200); // TStraw - TScint 
    h3->Fit("gaus", "", "", -200, 200); // TStraw - TMM 

    TF1 *g1 = (TF1*)h1->GetListOfFunctions()->FindObject("gaus");
    TF1 *g2 = (TF1*)h2->GetListOfFunctions()->FindObject("gaus");
    TF1 *g3 = (TF1*)h3->GetListOfFunctions()->FindObject("gaus");

    double m1 = g1->GetParameter(1);
    double s1 = g1->GetParameter(2);

    double m2 = g2->GetParameter(1);
    double s2 = g2->GetParameter(2);

    double m3 = g3->GetParameter(1);
    double s3 = g3->GetParameter(2);


    gStyle->SetOptFit(1111);
    // draw fit parameters as legends:
    Char_t ndf[80];
    Char_t sigma[80];
    Char_t mean[80];
    Char_t constant[80];
    auto legend = new TLegend(0.65, 0.9, 1.0, 0.75, "TMM - TScint");
    sprintf(ndf, "#chi^{2}/NDF = %.2f / %.2i", h1->GetFunction("gaus")->GetChisquare(), h1->GetFunction("gaus")->GetNDF());
    legend->AddEntry(h1, ndf);
    sprintf(sigma, "#sigma = %.1f", h1->GetFunction("gaus")->GetParameter(2));
    legend->AddEntry(h1, sigma);
    sprintf(mean, "Mean = %.1f", h1->GetFunction("gaus")->GetParameter(1));
    legend->AddEntry(h1, mean);
    sprintf(constant, "Events under peak: %.f", h1->GetFunction("gaus")->Integral(m1 - 4 * s1, m1 + 4 * s1) / h1->GetBinWidth(1));
    legend->AddEntry(h1, constant);
    legend->Draw("same");

    Char_t ndf1[80];
    Char_t sigma1[80];
    Char_t mean1[80];
    Char_t constant1[80];
    auto legend1 = new TLegend(0.65, 0.75, 1.0, 0.60, "TStraw - TScint");
    sprintf(ndf1, "#chi^{2}/NDF = %.2f / %.2i", h2->GetFunction("gaus")->GetChisquare(), h2->GetFunction("gaus")->GetNDF());
    legend1->AddEntry(h2, ndf1);
    sprintf(sigma1, "#sigma = %.1f", h2->GetFunction("gaus")->GetParameter(2));
    legend1->AddEntry(h2, sigma1);
    sprintf(mean1, "Mean = %.1f", h2->GetFunction("gaus")->GetParameter(1));
    legend1->AddEntry(h2, mean1);
    sprintf(constant1, "Events under peak: %.f", h2->GetFunction("gaus")->Integral(m2 - 4 * s2, m2 + 4 * s2) / h2->GetBinWidth(1));
    legend1->AddEntry(h2, constant1);
    legend1->Draw("same");

    Char_t ndf2[80];
    Char_t sigma2[80];
    Char_t mean2[80];
    Char_t constant2[80];
    auto legend2 = new TLegend(0.65, 0.60, 1.0, 0.45, "TStraw - TMM");
    sprintf(ndf2, "#chi^{2}/NDF = %.2f / %.2i", h3->GetFunction("gaus")->GetChisquare(), h3->GetFunction("gaus")->GetNDF());
    legend2->AddEntry(h3, ndf2);
    sprintf(sigma2, "#sigma = %.1f", h3->GetFunction("gaus")->GetParameter(2));
    legend2->AddEntry(h3, sigma2);
    sprintf(mean2, "Mean = %.1f", h3->GetFunction("gaus")->GetParameter(1));
    legend2->AddEntry(h3, mean2);
    sprintf(constant2, "Events under peak: %.f", h3->GetFunction("gaus")->Integral(m3 - 4 * s3, m3 + 4 * s3) / h3->GetBinWidth(1));
    legend2->AddEntry(h3, constant2);
    legend2->Draw("same");

    three_plots->SaveAs("../out/3plots_" + file + ".pdf");
}

void evBuilder::Loop()
{

    auto straw_vs_sci = new TH1D("straw_vs_sci", Form("%s: straw vs scint;#Deltat, ns", file.Data()), 1000, -500, 500);
    auto straw_vs_mm = new TH1D("straw_vs_mm", Form("%s: straw vs microMegas;#Deltat, ns", file.Data()), 1000, -500, 500);
    auto hits_in_cluster = new TH1D("hits_in_cluster", Form("%s: hits in microMegas per cluster;N", file.Data()), 100, 0, 100);
    auto mm_vs_sci = new TH1D("mm_vs_sci", Form("%s: microMegas vs scint;#Deltat, ns", file.Data()), 1000, -500, 500);

    auto sci0_vs_sci60 = new TH1D("sci0_vs_sci60", Form("%s: sci ch 0 vs sci ch 60;#Deltat, ns", file.Data()), 1000, -500, 500);


    auto straw_vs_sci_3det_corr = new TH1D("straw_vs_sci_3det_corr", Form("%s: 3-det correlations;#Deltat, ns", file.Data()), 1000, -500, 500);
    auto straw_vs_mm_3det_corr = new TH1D("straw_vs_mm_3det_corr", Form("%s: 3-det correlations;#Deltat, ns", file.Data()), 1000, -500, 500);
    auto mm_vs_sci_3det_corr = new TH1D("mm_vs_sci_3det_corr", Form("%s: 3-det correlations;#Deltat, ns", file.Data()), 1000, -500, 500);

    auto straw_vs_mm_spatial_corr = new TH2D("straw_vs_mm_spatial_corr", Form("%s: microMegas vs straw spatial correaltion;straw ch;MM ch", file.Data()), 10, 0, 10, 64, 0, 64);

    auto straw25_rt = new TH2D("straw25_rt", Form("%s: straw ch25 v-shape;R, 1/D;T, ns", file.Data()), 26, 0, 6, 300, -100, 200);

    if (fChain == 0)
        return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;

    // =============================== CORRELATION FINDING ===============================
    for (Long64_t jentry = 0; jentry < nentries; jentry++) // You can remove "/ 10" and use the whole dataset
    {
        if (jentry % 10000 == 0)
        {
            std::cout << "Entry " << jentry << "\t of \t" << nentries << "\n";
        }
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
            break;
        nb = fChain->GetEntry(jentry);
        nbytes += nb;

        double t_srtraw = 0;
        int straw_bcid_ch_srtraw = 0;
        int straw_pdo_ch_srtraw = 0;


        for (int j = 0; j < channel->at(0).size(); j++)
        {
            int fch = channel->at(0).at(j);

            if ((fch > 0 && fch < 4) ||  (fch > 60 && fch < 64)) // All straw ch
            {
                int fpdoUC = pdo->at(0).at(j); // Uncorrected PDO, used at time calibration
                int fpdo = correctPDO(fch, fpdoUC);
                int ftdo = tdo->at(0).at(j);
                int fbcid = grayDecoded->at(0).at(j);

                straw_bcid_ch_srtraw = fbcid;
                straw_pdo_ch_srtraw = fpdo;
                t_srtraw = getTime(fch, fbcid, ftdo, fpdoUC); // 'auto' limits
                // t_srtraw = getTimeByHand(fbcid, ftdo, Y, Y); //'hand' limits

                Long64_t mbytes = 0, mb = 0;
                double t30 = 0;
                double minTsci0 = 1e3;
                double sciT_ch0 = 0;
                int sci_bcid_ch0 = 0;
                double minTsci60 = 1e3;
                double sciT_ch60 = 0;
                int sci_bcid_ch60 = 0;
                vector<array<double, 3> > MmCluster;

                // ========================         LOOP OVER 40 events around         ========================
                //                              jentry to find correlation with MM
                MmCluster.clear();
                mbytes = 0, mb = 0;
                
                for (Long64_t kentry = jentry - 1; kentry < jentry + 1; kentry++)
                {
                    Long64_t iientry = LoadTree(kentry);
                    if (iientry < 0)
                        continue;
                    mb = fChain->GetEntry(kentry);
                    mbytes += mb;

                    for (int k = 0; k < channel->at(0).size(); k++)
                    {
                        int ffch = channel->at(0).at(k);
                        if (ffch < 4 || ffch > 59)
                            continue;

                        int ffpdoUC = pdo->at(0).at(k); // Uncorrected PDO, used at time calibration
                        int ffpdo = correctPDO(ffch, ffpdoUC);
                        int fftdo = tdo->at(0).at(k);
                        int ffbcid = grayDecoded->at(0).at(k);
                        // double fft = getTimeByHand(ffbcid, fftdo, 110, 160); //'hand' limits
                        double fft = getTime(ffch, ffbcid, fftdo, ffpdoUC); // 'auto' limits

                        if (fabs(t_srtraw - fft) < 500)
                        {
                            straw_vs_mm ->Fill(t_srtraw - fft);
                            array<double, 3> mM_hit = {{ffch * 1.0, ffpdo * 1.0, fft}};
                            MmCluster.push_back(mM_hit);
                        }
                    }
                }

                double meanT = 0;
                double meanCh = 0;
                double sum1 = 0;
                double sum2 = 0;
                double w_sum = 0;

                int strawCh = 0;

                if (fch == 1)
                {
                    strawCh = 6;
                }
                else if (fch == 2)
                {
                    strawCh = 4;
                }
                else if (fch == 3)
                {
                    strawCh = 2;
                }
                else if (fch == 61)
                {
                    strawCh = 5;
                }
                else if (fch == 62)
                {
                    strawCh = 1;
                }
                else if (fch == 63)
                {
                    strawCh = 3;
                }
                double minT_straw_mm = 600;
                int mmCh_min = 0;
                
                if (MmCluster.size() != 0)
                {
                    for (size_t l = 0; l < MmCluster.size(); l++)
                    {
                        if (abs(MmCluster.at(l)[2] - t_srtraw) < minT_straw_mm)
                        {
                            minT_straw_mm = abs(MmCluster.at(l)[2] - t_srtraw);
                            mmCh_min = MmCluster.at(l)[0];
                        }
                    }

                    for (size_t l = 0; l < MmCluster.size(); l++)
                    {
                        if (abs(MmCluster.at(l)[0] - mmCh_min) > 5)
                        {
                            MmCluster.erase(MmCluster.begin()+l);
                        }
                    }
                    

                    for (size_t l = 0; l < MmCluster.size(); l++)
                    {
                        sum1 += MmCluster.at(l)[2] * MmCluster.at(l)[1] / 1024.0;
                        sum2 += MmCluster.at(l)[0] * MmCluster.at(l)[1] / 1024.0;
                        w_sum += MmCluster.at(l)[1] / 1024.0;
                    }
                    meanT = sum1 / w_sum;
                    meanCh = sum2 / w_sum;
                    straw_vs_mm_spatial_corr->Fill(strawCh, meanCh);
                    // straw_vs_mm ->Fill(t_srtraw - meanT);
                    hits_in_cluster->Fill( MmCluster.size());
                }
                                

                // ============================= end of sci MM correlation finding ============================

                // ========================         LOOP OVER 40 events around         ========================
                //                           jentry to find correlation with sci 0

                mbytes = 0, mb = 0;
                for (Long64_t kentry = jentry - 1; kentry < jentry + 1; kentry++)
                {
                    Long64_t iientry = LoadTree(kentry);
                    if (iientry < 0)
                        continue;
                    mb = fChain->GetEntry(kentry);
                    mbytes += mb;

                    for (int k = 0; k < channel->at(0).size(); k++)
                    {
                        int ffch = channel->at(0).at(k);
                        if (ffch != 0)
                            continue;

                        int ffpdoUC = pdo->at(0).at(k); // Uncorrected PDO, used at time calibration
                        int ffpdo = correctPDO(ffch, ffpdoUC);
                        int fftdo = tdo->at(0).at(k);
                        int ffbcid = grayDecoded->at(0).at(k);
                        // if (ffbcid < 40)
                        //    continue;
                        double fft = getTimeByHand(ffbcid, fftdo, 88, 140); //'hand' limits
                        // double fft = getTime(ffch, ffbcid, fftdo, ffpdoUC); // 'auto' limits

                        // straw_vs_sci->Fill(t_srtraw - fft);

                        if (fabs(t_srtraw - fft) < minTsci0)
                        {
                            minTsci0 = fabs(t_srtraw - fft);
                            sciT_ch0 = fft;
                            sci_bcid_ch0 = ffbcid;
                        }
                    }
                }

                mbytes = 0, mb = 0;
                for (Long64_t kentry = jentry - 1; kentry < jentry + 1; kentry++)
                {
                    Long64_t iientry = LoadTree(kentry);
                    if (iientry < 0)
                        continue;
                    mb = fChain->GetEntry(kentry);
                    mbytes += mb;

                    for (int k = 0; k < channel->at(0).size(); k++)
                    {
                        int ffch = channel->at(0).at(k);
                        if (ffch != 60)
                            continue;

                        int ffpdoUC = pdo->at(0).at(k); // Uncorrected PDO, used at time calibration
                        int ffpdo = correctPDO(ffch, ffpdoUC);
                        int fftdo = tdo->at(0).at(k);
                        int ffbcid = grayDecoded->at(0).at(k);
                        // if (ffbcid < 40)
                        //    continue;
                        // double fft = getTimeByHand(ffbcid, fftdo, 88, 140); //'hand' limits
                        double fft = getTime(ffch, ffbcid, fftdo, ffpdoUC); // 'auto' limits

                        // straw_vs_sci->Fill(t_srtraw - fft);

                        if (fabs(t_srtraw - fft) < minTsci60)
                        {
                            minTsci60 = fabs(t_srtraw - fft);
                            sciT_ch60 = fft;
                            sci_bcid_ch60 = ffbcid;
                        }
                    }
                }
                if (sciT_ch0 != 0 && sciT_ch60 != 0)
                {
                    sci0_vs_sci60->Fill(sciT_ch0 - sciT_ch60);
                }

                if (sciT_ch0 != 0)
                {
                    if (meanT != 0)
                    {
                        mm_vs_sci->Fill(meanT - sciT_ch0);
                    }
                    straw_vs_sci->Fill(t_srtraw - sciT_ch0);
                }

                if (sciT_ch60 != 0 && meanT != 0)
                {
                    straw_vs_sci_3det_corr->Fill(t_srtraw - sciT_ch60);
                    straw_vs_mm_3det_corr->Fill(t_srtraw - meanT);
                    mm_vs_sci_3det_corr->Fill(meanT - sciT_ch60);
                    if (strawCh == 3 && (meanCh > 19 && meanCh < 46))
                    {
                        straw25_rt->Fill((meanCh - 19) * 6 / (46.0 - 19.0), 100 + t_srtraw - sciT_ch60);
                    }
                }

                // ============================= end of sci 0 correlation finding =============================

                
            }
            else
            {
                continue;
            }
        }
    }
    threePlotDrawF(mm_vs_sci_3det_corr, straw_vs_sci_3det_corr, straw_vs_mm_3det_corr);


    TFile *out = new TFile("../out/out_" + file + ending, "RECREATE"); // PATH where to save out_*.root file
    straw_vs_sci->Write();
    straw_vs_mm->Write();
    mm_vs_sci->Write();
    hits_in_cluster->Write();
    straw_vs_sci_3det_corr->Write();
    straw_vs_mm_3det_corr->Write();
    mm_vs_sci_3det_corr->Write();
    straw_vs_mm_spatial_corr->Write();
    sci0_vs_sci60->Write();
    straw25_rt->Write();
    out->Close();
}