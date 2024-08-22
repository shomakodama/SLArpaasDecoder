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



const int lengths = 512; // number of data points for each event
const int interval = 8; // 8 ns (125 MHz)
const uint64_t timing_offset = 0x100000000;

const std::string fname = "data_SiPM4/SiPM8180.dat"; // need to be changed
const TString ofilename = "root_SiPM4/SiPM8180.root"; // need to be changed

const int plotevent = 0; // save event or not (1: save)


int DataClassification(int number, uint32_t data);

int EventCheck(int *ADC, uint32_t event);


int Decoder1ch(){

    std::ifstream fs(fname, std::ios::in);

    // open outputfile
    TFile *ofile = new TFile(ofilename, "RECREATE"); // output file: need to change
    TTree *otree = new TTree("tree", "tree");

    TDirectory *subD_waveform = ofile->mkdir("waveform");

    int length = lengths;
    uint64_t timing;
    uint32_t event;
    uint32_t internalevent;
    int ADC0[lengths];
    int eventflag;

    otree->Branch("length", &length, "length/I");
    otree->Branch("timing", &timing, "timing/l");
    otree->Branch("event", &event, "event/i");
    otree->Branch("internalevent", &internalevent, "internalevent/i");
    otree->Branch("ADC0", ADC0, "ADC0[length]/I");
    otree->Branch("eventflag", &eventflag, "eventflag/I");



    int TDC_ns[lengths];
    for(int i=0; i<lengths; i++){
        ADC0[i] = -1;
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
                        TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 700, 500);
                        TString graphTitle; // title of the graph
                        graphTitle.Form("Waveform %d; Time (ns); ADC", event);

                        TH1F *frame = gPad->DrawFrame(xlo, ylo, xhi, yhi, graphTitle);
                        TGraph *gr = new TGraph(lengths, TDC_ns, ADC0);

                        gr->SetMarkerColor(1); // black
                        gr->Draw("P");
                        subD_waveform->cd();
                        c1->Write(); // save to the root file
                    }

                    // event check
                    eventflag = EventCheck(ADC0, event);

                    // fill event to output file
                    otree->Fill();

                    for(int i=0; i<lengths; i++){
                        ADC0[i] = -1;
                    }
                    timing = 0;

                    eventline = 0;
                    packet = 0;
                }
            
                line = 0;
                line++;
                // std::cout << "header found" << std::endl;
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

            // eventline = 0 -> 0, 1
            // eventline = 1 -> 2, 3
            ADC0[eventline*2+1] = ADC_1;
            ADC0[eventline*2] = ADC_2;

            eventline++;

        }

        // draw the last event
        if(plotevent == 1){

            double xlo = 0.;    // lower limit of x
            double xhi = (double)lengths*interval;   // higher limit of x
            double ylo = 0.;    // lower limit of y
            double yhi = 16383.;   // higher limit of y
            TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 400, 400);
            TString graphTitle; // title of the graph
            graphTitle.Form("Waveform %d; Time (ns); ADC", event);

            TH1F *frame = gPad->DrawFrame(xlo, ylo, xhi, yhi, graphTitle);
            TGraph *gr = new TGraph(lengths, TDC_ns, ADC0);

            gr->SetMarkerColor(1); // black
            gr->Draw("P");
            subD_waveform->cd();
            c1->Write(); // save to the root file
        }

        // event check
        eventflag = EventCheck(ADC0, event);

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
        }else if((ADC[i]==0 && flag == 0)||(ADC[i]>=16384 && flag == 0)){
            flag = 1;
        }else if(ADC[i]==-1){
            flag = 2;
        }
    }

    if(flag == 1){
        std::cout << "event " << event << " has point outside input range" << std::endl;
    }else if(flag == 2){
        std::cout << "event " << event << " has missing points" << std::endl;
    }

    return flag;

}