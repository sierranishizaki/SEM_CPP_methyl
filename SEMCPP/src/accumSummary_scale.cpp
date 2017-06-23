extern "C" {
    #include "./lib/libBigWig-master/bigWig.h"
}
#include "iterativeSEM.hpp"
#include "common.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
using namespace std;



			// contains all data, contains bigwig filename, region file, scale
//REQUIRES: data is valid Dataset, receives bigwig file, 
//          file containing regions to center, and scale size
//MODIFIES: data, specifically accumsummary data
//EFFECTS: fills appropriate accumSummary_data vectors

void accumSummary_scale(Dataset &data, const string &hfile,
                        const string &cfile, int scale,
                        Dataset::accumSummary_type::accumSummary_dest dest){

// clear accum data of corresponding type
    switch (dest) {
            case Dataset::accumSummary_type::accumSummary_dest::none:
                cerr << "dest shouldn't be none!!!!" << endl;;
                exit(1);
            break;
            case Dataset::accumSummary_type::accumSummary_dest::enumerated:
                data.accumSummary_data.enum_accum_max.clear();
                data.accumSummary_data.enum_accum_lines.clear();
            break;
            case Dataset::accumSummary_type::accumSummary_dest::scrambled:
                data.accumSummary_data.scramble_accum_max.clear();
                data.accumSummary_data.scramble_accum_lines.clear();
            break;
            case Dataset::accumSummary_type::accumSummary_dest::alignment: 
                data.accumSummary_data.align_accum_max.clear();
                data.accumSummary_data.align_accum_lines.clear();
            break;
            default:
                cerr << "there is no default for dest's switch statement!!!" << endl;
                exit(1);
            break;
    }


	// open file using library, below code is necessary
	// because of C++ type system regarding const
	char *fname = new char[hfile.length() + 1];
	strcpy(fname, hfile.c_str());
	bigWigFile_t *bwFile = bwOpen(fname, NULL, "r");
    	
    if(bwFile == NULL){
    	cerr << "Failed to open hfile: " << hfile << endl;
    	exit(1);
	}

	int dist = 500;
	int total_size = dist * 2 + scale;
	float max = 0.0;
	// int hitcount = 0;

    // FOR FIXING OUT OF RANGE ERROR WHEN RUNNING ACCUMSUMMARY_SCALE(ARGS)
    // ON SCRAMBLED DATA
    ++total_size;
    // FIX DONE


	//////////////////////////////////
	// Read each peak location and add signal values
	/////////////////////////////////

	ifstream input(cfile);
	if(!input){
		cout << "Failure to open cfile in accumSummary_scale.cpp" << endl;
		exit(1);
	}

	const string splitBy = "\t";
	char *chrom = nullptr;
	string line = "", seqid = "", direction = "";

	vector<string> temp;
    // vector<float> signal_array(total_size, 0.0);
    // vector<bool> signal_array_is_nan(total_size, false);

	int start = 0, end = 0;
     // counter = 0;
    int upstart = 0, upend = 0;
	// pointer to hold double values from bigwig library function;
	// double *values = nullptr;


	while(getline(input, line)){
		// initialize vairables
        temp.clear();
		chrom = nullptr;
		split_string(line, splitBy, temp);
		seqid = temp[0];
        upstart = 0, upend = 0;

		// if lines begins with chr
		if(temp[0][0] == 'c'){
			if(temp[0][1] == 'h')
				if(temp[0][2] == 'r'){
					seqid = temp[0];
#ifdef DEBUG
                    // cout << "temp[0] begins with chr: " << temp[0] << '\n';
#endif
                }
		}

		// if line doesn't begin with chr
		else{
			seqid = "chr" + temp[0];
		}
		start = stoi(temp[1]) - 1;
		end = stoi(temp[2]);
		direction = temp[4];

		upstart = start - dist;
		upend = end + dist;

		chrom = new char[seqid.length() + 1];
		strcpy(chrom, seqid.c_str());

        // cout << "chrom: " << chrom << endl
        // << "upstart: " << static_cast<uint32_t>(upstart) << endl
        // << "upend: " << static_cast<uint32_t>(upend) << endl
        // << "nBins: " << static_cast<uint32_t>(upend - upstart) << endl;

  //       try{
  //           #ifdef DEBUG
  //           // cout << "chrom: " << chrom << endl
  //           //      << "upstart: " << upstart << endl
  //           //      << "upend: " << upend << endl
  //           //      << "upend - upstart: " << upend - upstart << endl;

  //           #endif

  //           #ifdef DEBUG
  //                // cout << "running bwStats(args)..." << flush;
  //           #endif
  //   		values = bwStats(bwFile, chrom, static_cast<uint32_t>(upstart),
  //                            static_cast<uint32_t>(upend),
  //                            static_cast<uint32_t>(upend - upstart), 
  //                            bwStatsType::mean);
  //           #ifdef DEBUG
  //               // cout << "FINISH" << endl;
  //           // cout << "upend: " << upend << endl << "upstart: " << upstart << endl;
  //           #endif
  //       }
  //       catch(...){
  //           cerr << "problem with bwStats\n\tEXITING" << endl;
  //           exit(1);
  //       }

  //       if(values == NULL){
  //           cerr << "Failure to use bwStats!\n\tEXITING" << endl;
  //           exit(1);
  //       }

  //       // cout << "chrom: " << chrom << endl;
		// try{
  //   		for(counter = 0; counter < upend - upstart; ++counter){
  //   			values[counter] = roundf(values[counter] * 1000) / 1000;
  //               // cout << "value at values[" << counter << "]: " << values[counter] << endl;
  //   			signal_array.at(counter) = values[counter];
  //   		}
  //       }
  //       catch(...){
  //           cerr << "problem with line about 5 lines above!!" << endl
  //                << "parameters:\n\t counter: " << counter << endl
  //                << "\tscale: " << scale << endl
  //                << "\tupend - upstart: " << upend - upstart << endl
  //                << "\ttotal_size: " << total_size << endl;

  //           exit(1);
  //       }

  //       // cout << "\tfreed values" << endl;
  //       free(values);

        bwOverlappingIntervals_t *ptr = bwGetValues(bwFile, chrom, 
                                   static_cast<uint32_t>(upstart),
                                   static_cast<uint32_t>(upend),
                                   1);

        // cout << "\t for chrom: " << seqid << endl;
        // cout << "\tdeleted chrom" << endl;
		delete [] chrom;


        int nan_count = 0;

        try{
            // '+' is found
            // cout << "\tline 179" << endl;
    		if(direction.find('+') != string::npos){
    			for(int k = 0; k < static_cast<int>(ptr->l); ++k){
                    ptr->value[k] = roundf(ptr->value[k] * 1000.0) / 1000.0;
                    if(!isnan( ptr->value[k] )){
                        // output[k] = signal_array[k];
                        if(ptr->value[k] > max){
                            max = ptr->value[k];
                        }
                    }
                    else{
                        // output[k] = "N";
                        // signal_array_is_nan.at(k) = true;
                        ++nan_count;
                    }
                }
            }
    		else{
    			for(int k = static_cast<int>(ptr->l) - 1; k >= 0; --k){
                    if(!isnan( ptr->value[k] )){
                        ptr->value[k] = roundf(ptr->value[k] * 1000.0) / 1000.0;
                        // output[k] = signal_array[k];
                        if(ptr->value[k] > max){
                            max = ptr->value[k];
                        }
                    }
                    else{
                        // output[k] = "N";
                        ++nan_count;
                    }
                }
            }
        }
        catch(...){
            cerr << "nan exception thrown" << endl;
            exit(1);
        }


        free(ptr->start);
        free(ptr->end);
        free(ptr->value);
        free(ptr);
		// max = 0;
		// hitcount = 0;
  //       // cout << "\tline 213" << endl;
		// for(int l = 0; l < static_cast<int>(signal_array.size()); ++l){
		// 	if(!signal_array_is_nan.at(l)){ 
  //               ++hitcount;
  //           }
  //           else{
  //               if(signal_array.at(l) > max){ 
  //                   max = signal_array.at(l);
  //               }
  //           }
		// }
        // cout << "\tupend: " << upend 
        //      << "\n\tupstart: " << upstart << "\n\thitcount / upend - upstart: "
        //      << 1.0 - ( static_cast<float>(nan_count) / static_cast<float>(ptr->l) ) 
        //      << endl;
        if(static_cast<float>(nan_count) / static_cast<float>(ptr->l) > 0.10){
            max = NAN_VALUE;
        }
        // if max is maximum possible double value, then it is not applicable
		// if( ( hitcount / static_cast<double>( signal_array.size() ) ) < 0.9){
		// 	max = NAN_VALUE;
  //       }
        // cout << "\tline 232" << endl;
        switch (dest) {
            case Dataset::accumSummary_type::accumSummary_dest::none:
                cerr << "dest shouldn't be none!!!!" << endl;
                exit(1);
            break;
            case Dataset::accumSummary_type::accumSummary_dest::enumerated:
                data.accumSummary_data.enum_accum_lines.push_back(line);
                data.accumSummary_data.enum_accum_max.push_back(max);
            break;
            case Dataset::accumSummary_type::accumSummary_dest::scrambled:
                data.accumSummary_data.scramble_accum_lines.push_back(line);
                data.accumSummary_data.scramble_accum_max.push_back(max);
            break;
            case Dataset::accumSummary_type::accumSummary_dest::alignment:          
                data.accumSummary_data.align_accum_lines.push_back(line);
                data.accumSummary_data.align_accum_max.push_back(max);
            break;
            default:
                cerr << "there is no default for dest's switch statement!!!" << endl;
                exit(1);
            break;
        }
	}
	bwClose(bwFile);
	delete [] fname;
}
