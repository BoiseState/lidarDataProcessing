/*
 * File name: CmdLine.hpp
 * Created on: 17-May-2017
 * Author: ravi
 */

#ifndef CMDLINE_HPP_
#define CMDLINE_HPP_

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <stdlib.h>

class CmdLine{

private:
    //possible parameters
    std::string plsFileName;
    std::string wvsFileName;

    // helpful stuff
    std::string usageMessage;
    std::string exeName;

    // If peakFlag == true, use first difference to find peaks
    // else, use smooth second difference 
    bool max_elev_flag;



public:
    //calibration constant (for backscatter option)
    double calibration_constant;

    // help parameters
    bool printUsageMessage;
    // use find_peaks parameter
    // True = gaussian fitting, False = first differencing
    bool useGaussianFitting;

    // Whether or not backscatter coefficient has been requested
    bool calcBackscatter;    

    //True stifles all output statements
    bool quiet;

    CmdLine();


    bool parse_args(int argc, char *argv[]);
    void setUsageMessage();
    std::string getUsageMessage();
    void check_input_file_exists();
    void setInputFileName(char *args);
    void setInputFileName(std::string filename);
    std::string getInputFileName(bool pls);
    std::string getTrimmedFileName(bool pls);
    std::string get_output_filename(int product_id);
    std::string get_product_desc(int product_id);
    std::vector<int> selected_products;
};

#endif /* CMDLINE_HPP_ */