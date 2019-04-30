#include "iterativeSEM.hpp"
#include "common.hpp"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
using namespace std;


// Takes fasta file and aligns to genome, filters by DNase, and sorts
//  stores results in final output
// bowtie can take input as - to be stdin (possible direct pipe from kmer enumeration)
void bowtie_genome_map(int length, const string& genome, const string& file, 
                       const string& final_output, const string& dnase_file, int threads, bool verbose){


    string cmd = "./bin/bowtie --threads " + std::to_string(threads) + " --quiet -a -v 0 " + genome + " -r " + file 
       + " | awk '{print $3\"\\t\"$4\"\\t\"$4+" + std::to_string(length) + "\"\\t\"$5\"\\t\"$2}' | grep -vE chr'Y|X|M' | " 
       + "./bin/bedtools intersect -a stdin -b " + dnase_file + " -wa -u | sort | uniq";

    if(verbose){
        cout << "Running command: " << cmd << "\n\tRunning.... " << flush;
    }

    ofstream OUT(final_output);
    if(!OUT){
        cerr << "Failure to open \"" << final_output << "\".\n";
        exit(1);
    }


    std::stringstream IN = exec(cmd.c_str()); // stringstream instead of writing to a file
    string map_line = "";
    vector<string> dat;
    string DNA = "";

    // Read output, reformat, and write to final_output file
    // This could all be kept in memory in the future.
    while(std::getline(IN, map_line, '\n')){
        split_string(map_line, "\t", dat);

        if(dat[4] == "+"){
            DNA = dat[3];
        }
        else if(dat[4] == "-"){
            DNA = revCompDNA(dat[3]);
        }
        else{
            cerr << "unknown strand!\n";
            cerr << map_line << endl;
            exit(1);
        }

        OUT << dat[0] << '\t' << dat[1] << '\t'
            << dat[2] << '\t' << DNA << '\t' << dat[4] << '\n';

        dat.clear();
    }

    if(verbose){
        cout << "FINISH" << endl << flush;
    }

    OUT.close();

    IN.str(std::string());
    IN.clear();
}

