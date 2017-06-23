#include "src/iterativeSEM.hpp"
#include "src/common.hpp"
#include <iostream>
using namespace std;

static void align_SNPs(Dataset &data, int length, const vector<string> &nucleotideStack);

// default genome is "hg19"
// INFILE FROM ORIGINAL ALGORITHM IS ENUMERATED_KMER
void alignToGenomeWrapper(Dataset &data, int iteration, const string genome) {

    const vector<string> nucleotideStack{"A", "C", "G", "T"};

    // step 1: get the length of kmer
    int length = getLength(data);
    #ifdef DEBUG
    // cout << "length: " << length << endl;
    #endif
    if(data.settings.verbose){
        cout << "\tAligning\n";
    }

  // step 2: iterate through all the positions and nucleotides,
  //            creating SNP files and aligning to genome
    align_SNPs(data, length, nucleotideStack);
}

// INFILE FROM ORIGINAL ALGORITHM IS ENUMERATED_KMER
static void align_SNPs(Dataset &data, int length,
                       const vector<string> &nucleotideStack){
    string name = "";

    string CWD =  "./" + data.output_dir + "ALIGNMENT/";

    if(system( string("mkdir -p " + CWD).c_str() ) != 0){
        cerr << "problem running mkdir -p " << CWD << endl;
        exit(1);
    }
    

    string genome = "";

    string fa_file = "";

    string bowtie_output = "./";

    vector<string> new_kmer;

    bool non_zero_file_size = false;

    vector<string> cache_output;

    for(int position  = 0; position < length; ++position){
        for(int j = 0; j < static_cast<int>(nucleotideStack.size()); ++j){

            name = nucleotideStack[j] +  "_pos" + to_string(position);

            new_kmer.clear();
                                      // nucleotide
	        string genome = "./data/hg19";
            try{
                changeBase(data, position, nucleotideStack[j], new_kmer,
                           genome);
            }
            catch(...){
                cerr << "exception thrown from changeBase" << endl;
                exit(1);
            }
                        // CHECK THAT THIS IS CORRECT RELATIVE LOCATION

// void checkCache(Dataset &data, vector<string> &in_file, vector<string> &out_cache,
//                 const string &cachefile);

            try{
                checkCache(data, new_kmer, cache_output, data.cachefile,
                        Dataset::accumSummary_type::accumSummary_dest::alignment);
            }
            catch(...){
                cerr << "exception thrown from checkCache" << endl;
                exit(1);
            }
            // pass in a sequence column, which is from output of checkCache
            // cachefile in Dataset is $cache from original algorithm!!!

            fa_file = CWD + name + ".fa";
            bowtie_output = CWD + name + ".bed";

            non_zero_file_size = seq_col_to_fa(cache_output, fa_file);
            if(non_zero_file_size){
                bowtie_genome_map(length, "./data/hg19", fa_file, bowtie_output,
                                  data.settings.verbose);
            }
            cache_output.clear();
        }
    }
}
