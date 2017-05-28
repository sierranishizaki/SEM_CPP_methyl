#include "iterativeSEM.hpp"
#include "common.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

    /* Effects: Reads in the data from filterDnaseWrapper
    *           will output integer values into the data struct
    *            containing the maximum, count, stdev, and sterr
    */

//vector<string> split(const string &s, char delim);
                                                    // PASS scramble FOR BASELINE
void findMaximumAverageSignalWrapper(Dataset &data,
                                     Dataset::accumSummary_type::accumSummary_dest dest){

        vector<double> *max_ptr = nullptr;
        //vector<string> *line_ptr = nullptr;

        switch (dest) {
            case Dataset::accumSummary_type::accumSummary_dest::none:
                cerr << "dest shouldn't be none!!!!\n";
                exit(1);
            break;
            case Dataset::accumSummary_type::accumSummary_dest::enumerated:
                max_ptr = &data.accumSummary_data.enum_accum_max;
                //line_ptr = &data.accumSummary_data.enum_accum_lines;
            break;
            case Dataset::accumSummary_type::accumSummary_dest::scrambled:
                max_ptr = &data.accumSummary_data.scramble_accum_max;
                //line_ptr = &data.accumSummary_data.scramble_accum_lines;
            break;
            case Dataset::accumSummary_type::accumSummary_dest::alignment:
                max_ptr = &data.accumSummary_data.align_accum_max;
                //line_ptr = &data.accumSummary_data.align_accum_lines;
            break;
            default:
                cerr << "there is no default for dest's switch statement!!!\n";
                exit(1);
            break;
        }

        if(max_ptr->empty()){
            cerr << "corresponding accumSummary max data is missing!!!!\n";
            exit(1);
        }

        double sum = 0.0;
        int counter = 0;
        double mean = 0.0;
        double stdev = 0.0;
        double sterr = 0.0;

        //Finds maximums of each line and stores into a vector called
        // maximums.

        for(size_t i = 0; i < max_ptr->size(); ++i){
            // checks if the value corresponds to "NA"
            // if(max_ptr->at(i) != numeric_limits<double>::max()){
            if( !isnan( max_ptr->at(i) ) ) {
                sum += max_ptr->at(i);
                ++counter;
            }
        }

        if(counter > 0){
            mean = sum / counter;
        }

        // The following is the calculations for the standard deviation
        // and standard error.
        double sqtotal = 0.0;
        for (size_t i = 0; i < max_ptr->size(); i++){
            sqtotal += pow((mean - max_ptr->at(i)), 2.0);
        }
        stdev = pow( (sqtotal / (max_ptr->size()-1) ), 0.5);
        sterr = stdev / pow(counter, 0.5);

        switch (dest) {
            case Dataset::accumSummary_type::accumSummary_dest::none:
                cerr << "dest shouldn't be none!!!!\n";
                exit(1);
            break;
            case Dataset::accumSummary_type::accumSummary_dest::enumerated:
                data.signal_Data.enumerate_maximum = mean;
                data.signal_Data.enumerate_counter = counter;
                data.signal_Data.enumerate_stdev = stdev;
                data.signal_Data.enumerate_sterr = sterr;
            break;
            case Dataset::accumSummary_type::accumSummary_dest::scrambled:
                data.signal_Data.scramble_maximum = mean;
                data.signal_Data.scramble_counter = counter;
                data.signal_Data.scramble_stdev = stdev;
                data.signal_Data.scramble_sterr = sterr;
            break;
            case Dataset::accumSummary_type::accumSummary_dest::alignment:
                data.signal_Data.alignment_maximum = mean;
                data.signal_Data.alignment_counter = counter;
                data.signal_Data.alignment_stdev = stdev;
                data.signal_Data.alignment_sterr = sterr;
            break;
            default:
                cerr << "there is no default for dest's switch statement!!!\n";
                exit(1);
            break;
        }


}
