#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

///// root 
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TString.h>



const int lengths = 2048+1; // number of data points for each event
const int interval = 8; // 8 ns (125 MHz)
const uint64_t timing_offset = 0x100000000;

// const int voltage = 2000; // need to be changed 25, 50, 75, 100, 150, 200, 300, 500, 750, 1000, 1500, 2000
// const int pulsewidth = 5000; // need to be changed
// const int ch = 8; // need to be changed
// const std::string fname = "data_linearity2/100ev_" + std::to_string(voltage) + "mV_" + std::to_string(pulsewidth) + "ns_ch" + std::to_string(ch) + ".dat";
// const TString ofilename = Form("root_linearity2/100ev_%dmV_%dns_ch%d.root", voltage, pulsewidth, ch);

const std::string fname = "data_extreme/extreme.dat"; // need to be changed
const TString ofilename = "root_extreme/extreme.root"; // need to be changed

const int plotevent = 1; // save event or not (1: save)


int DataClassification(int number, uint32_t data);

int EventCheck(int *ADC, uint32_t event);


int Decoder16ch(){

    std::ifstream fs(fname, std::ios::in);

    // open outputfile
    TFile *ofile = new TFile(ofilename, "RECREATE"); // output file: need to change
    TTree *otree = new TTree("tree", "tree");

    int length = lengths-1;
    uint64_t timing;
    uint32_t event;
    uint32_t internalevent;
    int ADC0[lengths];
    int ADC1[lengths];
    int ADC2[lengths];
    int ADC3[lengths];
    int ADC4[lengths];
    int ADC5[lengths];
    int ADC6[lengths];
    int ADC7[lengths];
    int ADC8[lengths];
    int ADC9[lengths];
    int ADC10[lengths];
    int ADC11[lengths];
    int ADC12[lengths];
    int ADC13[lengths];
    int ADC14[lengths];
    int ADC15[lengths];
    int eventflag;

    otree->Branch("length", &length, "length/I");
    otree->Branch("timing", &timing, "timing/l");
    otree->Branch("event", &event, "event/i");
    otree->Branch("internalevent", &internalevent, "internalevent/i");
    otree->Branch("ADC0", ADC0, "ADC0[length]/I");
    otree->Branch("ADC1", ADC1, "ADC1[length]/I");
    otree->Branch("ADC2", ADC2, "ADC2[length]/I");
    otree->Branch("ADC3", ADC3, "ADC3[length]/I");
    otree->Branch("ADC4", ADC4, "ADC4[length]/I");
    otree->Branch("ADC5", ADC5, "ADC5[length]/I");
    otree->Branch("ADC6", ADC6, "ADC6[length]/I");
    otree->Branch("ADC7", ADC7, "ADC7[length]/I");
    otree->Branch("ADC8", ADC8, "ADC8[length]/I");
    otree->Branch("ADC9", ADC9, "ADC9[length]/I");
    otree->Branch("ADC10", ADC10, "ADC10[length]/I");
    otree->Branch("ADC11", ADC11, "ADC11[length]/I");
    otree->Branch("ADC12", ADC12, "ADC12[length]/I");
    otree->Branch("ADC13", ADC13, "ADC13[length]/I");
    otree->Branch("ADC14", ADC14, "ADC14[length]/I");
    otree->Branch("ADC15", ADC15, "ADC15[length]/I");
    otree->Branch("eventflag", &eventflag, "eventflag/I");

    TDirectory *subD_waveform = ofile->mkdir("waveform");



    int TDC_ns[lengths];
    for(int i=0; i<lengths; i++){
        ADC0[i] = 0;
        ADC1[i] = 0;
        ADC2[i] = 0;
        ADC3[i] = 0;
        ADC4[i] = 0;
        ADC5[i] = 0;
        ADC6[i] = 0;
        ADC7[i] = 0;
        ADC8[i] = 0;
        ADC9[i] = 0;
        ADC10[i] = 0;
        ADC11[i] = 0;
        ADC12[i] = 0;
        ADC13[i] = 0;
        ADC14[i] = 0;
        ADC15[i] = 0;
        TDC_ns[i] = i*8;
    }

    int line, eventline, packet;
    line = 0;
    eventline = 0;
    packet = 0;

    timing = 0;

    uint64_t timing_last;
    uint32_t timing1, timing2;

    uint32_t newEvent;
    uint32_t newInternalEvent;
    event = -1;

    int ADC_1, ADC_2;

    std::string tmp;

    if(!fs){
        std::cout << "Cannot open the input file" << std::endl;
    }else{

        while (getline(fs, tmp)) {
            if(tmp == "\r"){ // end of line
                continue;
            }
            // std::cout << std::dec << tmp << std::endl;
            int n;
            uint32_t buf = std::stoull(tmp, nullptr, 16);  // buffer
            // std::cout << std::hex << std::setw(8) << std::setfill('0')<< buf << std::endl;

            int Data = DataClassification(line, buf);
            // std::cout << "Data = " << Data << std::endl;

            if(Data == 10){ // header!

                if(event != -1){ // new event!

                    // draw the previous event
                    if(plotevent == 1){

                        double xlo = 0.;    // lower limit of x
                        double xhi = (double)lengths*interval;   // higher limit of x
                        double ylo = 0.;    // lower limit of y
                        double yhi = 16383.;   // higher limit of y
                        TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 400, 400);
                        TString graphTitle; // title of the graph
                        graphTitle.Form("Waveform %d; Time (ns); ADC", event);

                        TH1F *frame = gPad->DrawFrame(xlo, ylo, xhi, yhi, graphTitle);

                        TGraph *gr0 = new TGraph(lengths, TDC_ns, ADC0); // waveform for ch0
                        TGraph *gr1 = new TGraph(lengths, TDC_ns, ADC1); // waveform for ch1
                        TGraph *gr2 = new TGraph(lengths, TDC_ns, ADC2); // waveform for ch2
                        TGraph *gr3 = new TGraph(lengths, TDC_ns, ADC3); // waveform for ch3
                        TGraph *gr4 = new TGraph(lengths, TDC_ns, ADC4); // waveform for ch4
                        TGraph *gr5 = new TGraph(lengths, TDC_ns, ADC5); // waveform for ch5
                        TGraph *gr6 = new TGraph(lengths, TDC_ns, ADC6); // waveform for ch6
                        TGraph *gr7 = new TGraph(lengths, TDC_ns, ADC7); // waveform for ch7
                        TGraph *gr8 = new TGraph(lengths, TDC_ns, ADC8); // waveform for ch8
                        TGraph *gr9 = new TGraph(lengths, TDC_ns, ADC9); // waveform for ch9
                        TGraph *gr10 = new TGraph(lengths, TDC_ns, ADC10); // waveform for ch10
                        TGraph *gr11 = new TGraph(lengths, TDC_ns, ADC11); // waveform for ch11
                        TGraph *gr12 = new TGraph(lengths, TDC_ns, ADC12); // waveform for ch12
                        TGraph *gr13 = new TGraph(lengths, TDC_ns, ADC13); // waveform for ch13
                        TGraph *gr14 = new TGraph(lengths, TDC_ns, ADC14); // waveform for ch14
                        TGraph *gr15 = new TGraph(lengths, TDC_ns, ADC15); // waveform for ch15

                        gr0->SetMarkerColor(1); // black
                        gr0->Draw("P");
                        gr1->SetMarkerColor(2); // red
                        gr1->Draw("P");
                        gr2->SetMarkerColor(3); // light green
                        gr2->Draw("P");
                        gr3->SetMarkerColor(4); // blue
                        gr3->Draw("P");
                        gr4->SetMarkerColor(5); // yellow
                        gr4->Draw("P");
                        gr5->SetMarkerColor(6); // magenta
                        gr5->Draw("P");
                        gr6->SetMarkerColor(7); // light blue
                        gr6->Draw("P");
                        gr7->SetMarkerColor(8); // green
                        gr7->Draw("P");
                        gr8->SetMarkerColor(9); // purple
                        gr8->Draw("P");
                        gr9->SetMarkerColor(1); // black
                        gr9->Draw("P");
                        gr10->SetMarkerColor(2); // red
                        gr10->Draw("P");
                        gr11->SetMarkerColor(3); // light green
                        gr11->Draw("P");
                        gr12->SetMarkerColor(4); // blue
                        gr12->Draw("P");
                        gr13->SetMarkerColor(5); // yellow
                        gr13->Draw("P");
                        gr14->SetMarkerColor(6); // magenta
                        gr14->Draw("P");
                        gr15->SetMarkerColor(7); // light blue
                        gr15->Draw("P");

                        subD_waveform->cd();
                        c1->Write(); // save to the root file
                    }

                    // event check
                    int eventflag0 = EventCheck(ADC0, event);
                    int eventflag1 = EventCheck(ADC1, event);
                    int eventflag2 = EventCheck(ADC2, event);
                    int eventflag3 = EventCheck(ADC3, event);
                    int eventflag4 = EventCheck(ADC4, event);
                    int eventflag5 = EventCheck(ADC5, event);
                    int eventflag6 = EventCheck(ADC6, event);
                    int eventflag7 = EventCheck(ADC7, event);
                    int eventflag8 = EventCheck(ADC8, event);
                    int eventflag9 = EventCheck(ADC9, event);
                    int eventflag10 = EventCheck(ADC10, event);
                    int eventflag11 = EventCheck(ADC11, event);
                    int eventflag12 = EventCheck(ADC12, event);
                    int eventflag13 = EventCheck(ADC13, event);
                    int eventflag14 = EventCheck(ADC14, event);
                    int eventflag15 = EventCheck(ADC15, event);
                    eventflag = std::max({eventflag0, eventflag1, eventflag2, eventflag3, eventflag4, eventflag5, eventflag6, eventflag7, eventflag8, eventflag9, eventflag10, eventflag11, eventflag12, eventflag13, eventflag14, eventflag15});

                    // fill event to output file
                    otree->Fill();

                    for(int i=0; i<lengths; i++){
                        ADC0[i] = 0;
                        ADC1[i] = 0;
                        ADC2[i] = 0;
                        ADC3[i] = 0;
                        ADC4[i] = 0;
                        ADC5[i] = 0;
                        ADC6[i] = 0;
                        ADC7[i] = 0;
                        ADC8[i] = 0;
                        ADC9[i] = 0;
                        ADC10[i] = 0;
                        ADC11[i] = 0;
                        ADC12[i] = 0;
                        ADC13[i] = 0;
                        ADC14[i] = 0;
                        ADC15[i] = 0;
                    }
                    timing = 0;

                    eventline = 0;
                    packet = 0;
                }
            
                line = 0;
                line++;
                continue;
            }

            if(Data == 20){
                timing1 = buf;
                line++;
                continue;
            }else if(Data == 21){
                timing2 = buf;
                timing = timing2 * timing_offset + timing1;
                line++;
                continue;
            }

            if(Data == 30){
                internalevent = buf;
                line++;
                continue;
            }

            if(Data == 40 || Data == 41){
                line++;
                continue;
            }

            if(Data == 50){
                event = buf;
                line++;
                continue;
            }

            if(Data == 60){
                line++;
                continue;
            }



            std::bitset<32> tempADC(buf);
            std::bitset<32> tempADC_1 = tempADC >> 16;
            ADC_1 = tempADC_1.to_ullong();
            std::bitset<32> least16(0xFFFF);
            std::bitset<32> tempADC_2 = tempADC & least16;
            ADC_2 = tempADC_2.to_ullong();

            if(eventline%8 == 0){ // eventline = 0, 8, 16, ...
                ADC1[eventline/8] = ADC_1;
                ADC0[eventline/8] = ADC_2;
            }else if(eventline%8 == 1){ // eventline = 1, 9, 17, ...
                ADC3[eventline/8] = ADC_1;
                ADC2[eventline/8] = ADC_2;
            }else if(eventline%8 == 2){ // eventline = 2, 10, 18, ...
                ADC5[eventline/8] = ADC_1;
                ADC4[eventline/8] = ADC_2;
            }else if(eventline%8 == 3){ // eventline = 3, 11, 19, ...
                ADC7[eventline/8] = ADC_1;
                ADC6[eventline/8] = ADC_2;
            }else if(eventline%8 == 4){ // eventline = 4, 12, 20, ...
                ADC9[eventline/8] = ADC_1;
                ADC8[eventline/8] = ADC_2;
            }else if(eventline%8 == 5){ // eventline = 5, 13, 21, ...
                ADC11[eventline/8] = ADC_1;
                ADC10[eventline/8] = ADC_2;
            }else if(eventline%8 == 6){ // eventline = 6, 14, 22, ...
                ADC13[eventline/8] = ADC_1;
                ADC12[eventline/8] = ADC_2;
            }else if(eventline%8 == 7){ // eventline = 7, 15, 23, ...
                ADC15[eventline/8] = ADC_1;
                ADC14[eventline/8] = ADC_2;
            }

            // std::cout << "eventline = " << eventline << std::endl;
            eventline++;
            
        }

        // draw the previous event
        if(plotevent == 1){

            double xlo = 0.;    // lower limit of x
            double xhi = (double)lengths*interval;   // higher limit of x
            double ylo = 0.;    // lower limit of y
            double yhi = 16383.;   // higher limit of y
            TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 700, 500);
            TString graphTitle; // title of the graph
            graphTitle.Form("Waveform %d; Time (ns); ADC", event);

            TH1F *frame = gPad->DrawFrame(xlo, ylo, xhi, yhi, graphTitle);

            TGraph *gr0 = new TGraph(lengths, TDC_ns, ADC0); // waveform for ch0
            TGraph *gr1 = new TGraph(lengths, TDC_ns, ADC1); // waveform for ch1
            TGraph *gr2 = new TGraph(lengths, TDC_ns, ADC2); // waveform for ch2
            TGraph *gr3 = new TGraph(lengths, TDC_ns, ADC3); // waveform for ch3
            TGraph *gr4 = new TGraph(lengths, TDC_ns, ADC4); // waveform for ch4
            TGraph *gr5 = new TGraph(lengths, TDC_ns, ADC5); // waveform for ch5
            TGraph *gr6 = new TGraph(lengths, TDC_ns, ADC6); // waveform for ch6
            TGraph *gr7 = new TGraph(lengths, TDC_ns, ADC7); // waveform for ch7
            TGraph *gr8 = new TGraph(lengths, TDC_ns, ADC8); // waveform for ch8
            TGraph *gr9 = new TGraph(lengths, TDC_ns, ADC9); // waveform for ch9
            TGraph *gr10 = new TGraph(lengths, TDC_ns, ADC10); // waveform for ch0
            TGraph *gr11 = new TGraph(lengths, TDC_ns, ADC11); // waveform for ch1
            TGraph *gr12 = new TGraph(lengths, TDC_ns, ADC12); // waveform for ch2
            TGraph *gr13 = new TGraph(lengths, TDC_ns, ADC13); // waveform for ch3
            TGraph *gr14 = new TGraph(lengths, TDC_ns, ADC14); // waveform for ch4
            TGraph *gr15 = new TGraph(lengths, TDC_ns, ADC15); // waveform for ch5

            gr0->SetMarkerColor(1); // black
            gr0->Draw("P");
            gr1->SetMarkerColor(2); // red
            gr1->Draw("P");
            gr2->SetMarkerColor(3); // green
            gr2->Draw("P");
            gr3->SetMarkerColor(4); // blue
            gr3->Draw("P");
            gr4->SetMarkerColor(5); // yellow
            gr4->Draw("P");
            gr5->SetMarkerColor(6); // magenta
            gr5->Draw("P");
            gr6->SetMarkerColor(7); // light blue
            gr6->Draw("P");
            gr7->SetMarkerColor(8); // green
            gr7->Draw("P");
            gr8->SetMarkerColor(9); // purple
            gr8->Draw("P");
            gr9->SetMarkerColor(1); // black
            gr9->Draw("P");
            gr10->SetMarkerColor(2); // red
            gr10->Draw("P");
            gr11->SetMarkerColor(3); // light green
            gr11->Draw("P");
            gr12->SetMarkerColor(4); // blue
            gr12->Draw("P");
            gr13->SetMarkerColor(5); // yellow
            gr13->Draw("P");
            gr14->SetMarkerColor(6); // magenta
            gr14->Draw("P");
            gr15->SetMarkerColor(7); // light blue
            gr15->Draw("P");

            c1->Write(); // save to the root file
        }

        // event check
        int eventflag0 = EventCheck(ADC0, event);
        int eventflag1 = EventCheck(ADC1, event);
        int eventflag2 = EventCheck(ADC2, event);
        int eventflag3 = EventCheck(ADC3, event);
        int eventflag4 = EventCheck(ADC4, event);
        int eventflag5 = EventCheck(ADC5, event);
        int eventflag6 = EventCheck(ADC6, event);
        int eventflag7 = EventCheck(ADC7, event);
        int eventflag8 = EventCheck(ADC8, event);
        int eventflag9 = EventCheck(ADC9, event);
        int eventflag10 = EventCheck(ADC10, event);
        int eventflag11 = EventCheck(ADC11, event);
        int eventflag12 = EventCheck(ADC12, event);
        int eventflag13 = EventCheck(ADC13, event);
        int eventflag14 = EventCheck(ADC14, event);
        int eventflag15 = EventCheck(ADC15, event);
        eventflag = std::max({eventflag0, eventflag1, eventflag2, eventflag3, eventflag4, eventflag5, eventflag6, eventflag7, eventflag8, eventflag9, eventflag10, eventflag11, eventflag12, eventflag13, eventflag14, eventflag15});

        // fill event to output file
        otree->Fill();
    }

    ofile->cd();
    otree->Write();

    ofile->Close();

    return 0;

}





int DataClassification(int number, uint32_t data){

    const uint32_t header = 0xFFFFFFFF;

    if(data == header){
        return 10;
    }

    if(number == 1){ // timing 1
        return 20;
    }else if(number == 2){ // timing 2
        return 21;
    }else if(number == 3){ // internal counter
        return 30;
    }else if(number == 4){ // hits data1: not used!
        return 40;
    }else if(number == 5){ // hits data2: not used!
        return 41;
    }else if(number == 6){ // user data: counter
        return 50;
    }else if(number == 7){ // filler
        return 60;
    }

    return 0; // data!

}





int EventCheck(int *ADC, uint32_t event){

    int flag;
    flag = 0;

    for(int i=0; i<lengths; i++){
        if(ADC[i]>0){
            continue;
        }else if(ADC[i]==0 && flag == 0){
            flag = 1;
        }else if(ADC[i]==-1){
            flag = 2;
        }
    }

    if(flag == 1){
        std::cout << "event " << event << " has ADC = 0 -> outside input range" << std::endl;
    }else if(flag == 2){
        std::cout << "event " << event << " has missing points" << std::endl;
    }

    return flag;

}