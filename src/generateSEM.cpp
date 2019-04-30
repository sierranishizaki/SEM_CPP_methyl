#include "./src/iterativeSEM.hpp"
#include <cmath>
#include "./src/common.hpp"
#include <vector>
#include <cstring>
#include <map>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;


/*  Effect: To generate a SNP effect matrix using maximum signal
*           signals of the generated SNPS and baseline.
*
*/

// TF_name and output_dir are contained in data!!!!
void generateSEM(const Dataset &data) {


// I don't think it's necessary to check that Enumerated is in $line
// as I distinctly separated the data from calling accumSummary_scale on
// the enumerated and alignment data separately.

// in findMaximumAverageSignalWrapper, the file grabs the output from
// accumSummary_scale when ran on enumerated, as well as scrambled,
// if they are present. Two cases, enumerated data, or scrambled and enumerated
// data.



    system( ("mkdir -p " + data.output_dir + "/SIGNAL/").c_str() );
    ofstream signal_out(data.output_dir + "SIGNAL/signal.maximums");
    system( ("mkdir -p " + data.output_dir + "/BASELINE/").c_str() );
    ofstream baseline_out(data.output_dir + "BASELINE/baseline.maximums");



    double enum_ = data.Signal_data.enumerate_maximum;
    double enum_err = data.Signal_data.enumerate_sterr;


    baseline_out << "Enumerated_kmer_filtered.signal\t" << data.Signal_data.enumerate_maximum
              << '\t' << data.Signal_data.enumerate_counter
              << '\t' << data.Signal_data.enumerate_stdev
              << '\t' << data.Signal_data.enumerate_sterr << endl;

    baseline_out << "Scrambled_kmer_filtered.signal\t" << data.Signal_data.scramble_maximum
              << '\t' << data.Signal_data.scramble_counter
              << '\t' << data.Signal_data.scramble_stdev
              << '\t' << data.Signal_data.scramble_sterr << endl;
         
    // cout << "\tenum_: " << enum_ << endl
    //      << "\tenum_err: " << enum_err << endl;


    double score = 0.0, sterr = 0.0;
    int max = 0;

    map<pair<int, char>, double> SNPEffect;
    map<pair<int, char>, double> STDErr;

    
    const vector<char> iterate_over = {'A', 'C', 'G', 'T'};

    int length = getLength(data);

    int loc = 0;
    char bp = '\0';

    // print OUT_HANDLE "$file\t$maximum\t$count\t$stdev\t$sterr\n";
    // use to translate indexes in this file's Perl equivalent
    try{
        for(loc = 0; loc < length; ++loc){
            for(char ch : iterate_over){
                bp = ch;
                score = data.sig_deets_maximum.at( {loc, bp} );
                sterr = data.sig_deets_sterr.at( {loc, bp} );


                signal_out << bp << "_pos" << loc << "_filtered.signal"
                          << '\t' << data.sig_deets_maximum.at({loc, bp})
                          << '\t' << data.sig_deets_counter.at({loc, bp})
                          << '\t' << data.sig_deets_stdev.at({loc, bp}) 
                          << '\t' << data.sig_deets_sterr.at({loc, bp})
                          << endl;        


                // debug
                cout << "score (maximum) for " << loc << ' ' << bp << ": " 
                     << score << '\t' << " and sterr: " << sterr << endl;

                // end debug

                SNPEffect[ {loc, bp} ] = log2(score / enum_);
                STDErr[ {loc, bp} ] = sterr / enum_;

                max = getLength(data);

                // if(loc > max){
                //   max = loc + 1;
                //   // max = loc;
                //   // why does it do this?
                // }
            }
        }
    }
    catch(const out_of_range &e){
        cerr << "bp: " << bp << "\tloc: " << loc << endl
             << "\tout of range error: " << e.what() << "\n\tEXITING" << endl;
        exit(1);
    }
    catch(...){
        cerr << "bp: " << bp << "\tloc: " << loc << endl
             << "problem with data in generateSEM(args)!!\n\tEXITING" << endl;
        exit(1);
    }

    enum_err = enum_err / enum_;

    
    ofstream sem_file(data.output_dir + "/" + data.TF_name + ".sem");
    ofstream sterr_file(data.output_dir + "/" + data.TF_name + ".sterr");

    if(!sem_file){
        cerr << "unable to open sem_file\n\tEXITING" << endl;
        exit(1);
    }
    if(!sterr_file){
        cerr << "unable to open sterr_file\n\tEXITING" << endl;
        exit(1);
    }

    //Making output files for the .sem and .sterr
    sem_file << data.TF_name << "\tA\tC\tG\tT\n";
    sterr_file << data.TF_name << "\tA\tC\tG\tT\n";
    // for (int j = 0; j <= max; ++j){
    for (int j = 0; j < max; ++j){
        sem_file << j + 1 << "\t" << SNPEffect[{j, 'A'}]
                 << "\t" << SNPEffect[{j, 'C'}]
                 << "\t" << SNPEffect[{j, 'G'}]
                 << "\t" << SNPEffect[{j, 'T'}]
                 << "\n";
        sterr_file << j + 1 << "\t" << STDErr[{j, 'A'}]
                   << "\t" << STDErr[{j, 'C'}]
                   << "\t" << STDErr[{j, 'G'}]
                   << "\t" << STDErr[{j, 'T'}]
                   << "\n";
    }

    sem_file.close();
    sterr_file.close();

}
