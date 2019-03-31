// File name: CmdLine.cpp
// Created on: 17-May-2017
// Author: ravi


#include "CmdLine.hpp"

using namespace std;


//off by one to
// match
// index to product ID
static std::string product_desc[22] = {"","max_elev","min_elev","max-min_elev","mean_first_amp",
									   "min_first_amp","max_first_amp","sd_first_amp",
									   "skew_first_amp","kurt_first_amp","mean_last_amp","min_last_amp",
									   "max_last_amp","sd_last_amp","skew_last_amp","kurt_last_amp","mean_all_amp",
									   "min_all_amp","max_all_amp","sd_all_amp","skew_all_amp","kurt_all_amp"};


/****************************************************************************
*
* Begin CmdLine functions
*
****************************************************************************/

// Function that sets the command line arguments
void CmdLine::setInputFileName(char *args){
  inputFileName = args;
}

// Function that prints(sets up) correct usage of this program
void CmdLine::setUsageMessage()
{
  std::stringstream buffer;
  buffer << "\nUsage: " << std::endl;
  buffer << "       path_to_executable [-option argument]+" << std::endl;
  buffer << "Option:  " << std::endl;
  buffer << "       -f  <path to pls file>"
         << "  :Generate a Geotif file" << std::endl;
  buffer << "Option:  " << std::endl;
  buffer << "       -e  <list>"
         << "  :Products to generate; valid ways to format the list include:" << std::endl;
  buffer << "                   -e 1,2,3           (no white-space)" << std::endl;
  buffer << "                   -e 1 -e 3 -e 2     (broken into multiple arguments)" << std::endl;
  buffer << "                   -e \" 1 , 2 , 3 \" (white-space allowed inside quotes)" << std::endl;
  buffer << "Option:  " << std::endl;
  buffer << "       -d"
         << "  :Disables gaussian fitter, using first diff method instead" << std::endl;
  buffer << "       -h" << std::endl;
  buffer << "\nExample: " << std::endl;
  buffer << "       bin/geotiff-driver -f ../etc/140823_183115_1_clipped_test.pls -e 1,2" << std::endl;
  usageMessage.append(buffer.str());
}

// Function that prints correct usage of this program
std::string CmdLine::getUsageMessage(){
    return usageMessage;
}

// Default constructor
CmdLine::CmdLine(){
  printUsageMessage = false;
  useGaussianFitting = true;
  exeName = "";
  setUsageMessage();
}

// Function that returns the file name
std::string CmdLine::getInputFileName(){
  return inputFileName;
}


// Function that parses the command line arguments
void CmdLine::parse(int argc,char *argv[]){
  char optionChar;  //Option character
  char *fArg;       //Argument of the option character f
  char *e_arg;      //Argument of the option character e

  /* If the program is run without any command line arguments, display
   * the correct program usage and quit.*/
  if(argc < 2){
    std::cout << "\nNo command line arguments" <<std::endl;
    std::cout << "-------------------------" <<std::endl;
    printUsageMessage = true;
  }

  exeName.append(argv[0]);

  static struct option long_options[] =
  {
      {"file", required_argument, NULL, 'f'},
      {"help", no_argument, NULL, 'h'},
      {"firstdiff", no_argument, NULL, 'd'},
      {"elevation", required_argument,NULL,'e'},
      {0, 0, 0, 0}
  };

	// getopt_long stores the option index here.
  int option_index = 0;
  /* Using getopt_long to get the arguments with an option.
   * ":hf:s:" indicate that option 'h' is without arguments while
   * option 'f' and 's' require arguments
   */
  while((optionChar = getopt_long (argc, argv, ":hdf:e:",
         long_options, &option_index))!= -1){
    switch(optionChar){
      // Option 'h' shows the help information
      case 'f': //Generate a geotif file
        fArg = optarg;
        setInputFileName(fArg);
        break;
      case 'h':
        printUsageMessage = true;
        break;
      case 'd':
        useGaussianFitting = false;
	      break;
      case 'e':  {// Without curly braces wrapping this case, there are compilation errors
        e_arg = optarg;
        std::stringstream ss(e_arg);
        while(ss.good()) {
        	string substr;
            getline(ss, substr, ',');
            int arg;
            try {
                arg = atoi(substr.c_str());
            } catch (std::invalid_argument e) {
                std::cout << "\nProduct list could not be converted into integers" <<std::endl;
                std::cout << "-------------------------" <<std::endl;
                printUsageMessage = true;
                break;
            }

            //Just making sure it doesn't try pushing broken data to selected_products
            if (printUsageMessage) {
	            break;
            }
            selected_products.push_back(arg);
        }
        break;
        }
      case ':':
        // Missing option argument
		    std::cout << "\nMissing arguments" <<std::endl;
    	  std::cout << "------------------" <<std::endl;
        printUsageMessage = true;
        break;
      default:
        // Invalid option
		    std::cout << "\nInvalid option" <<std::endl;
    	  std::cout << "---------------" <<std::endl;
        printUsageMessage = true;

    }
  }
    selected_products.push_back(4); //mean first amp
	selected_products.push_back(5); //min first amp
	selected_products.push_back(6); //max first amp
	selected_products.push_back(7); //std-dev first amp
	selected_products.push_back(8); //skew first amp
	selected_products.push_back(9); //kurtosis first amp
	selected_products.push_back(10); //mean last amp
	selected_products.push_back(11); //min last amp
	selected_products.push_back(12); //max last amp
	selected_products.push_back(13); //std-dev last amp
	selected_products.push_back(14); //skew last amp
	selected_products.push_back(15); //kurtosis last amp
	selected_products.push_back(16); //mean all amp
	selected_products.push_back(17); //min all amp
	selected_products.push_back(18); //max all amp
	selected_products.push_back(19); //std-dev all amp
	selected_products.push_back(20); //skew all amp
	selected_products.push_back(21); //kurtosis all amp
  // For non option input
  if(optind < argc){
    printUsageMessage = true;
  }
}

/**
 * check if the input file exists, print error message if not
 */
void CmdLine::check_input_file_exists() {
    if (!std::ifstream(getInputFileName().c_str())) {
        std::cout << "File " << getInputFileName() << " not found." << std::endl;
        printUsageMessage = true;
    }
}


/**
 * parse the command line arguments and validate them, return
 * 1 if all arguments parsed, else 0
 * @param argc the count of arguments in argv
 * @param argv the command line arguments
 * @return 1 (true) if all input passed validation, 0 (false) otherwise
 */
int CmdLine::parse_args(int argc, char *argv[]) {
    int rtn = 1;
    parse(argc, argv);
    check_input_file_exists();
    //if arguments were invalid or did not parse correctly
    if (printUsageMessage) {
        std::cout << getUsageMessage() << std::endl;
        rtn = 0;
    }

    return rtn;
}

/**
 * get the input file name, stripped of leading path info and trailing extension info
 * @return input file name stripped of path or extension information
 */
std::string CmdLine::getTrimmedFileName(){
   size_t start = inputFileName.find_last_of("/");
   if(start==string::npos){
     start = 0;
   }else{
     start++;
   }
   size_t end = inputFileName.find_last_of(".");
   int len = end - start;
   return inputFileName.substr(start,len);
}

/**
 * get the output filename based on the command line arguments and input filename
 * @param product_id the id of the product to produce
 * @return the output filename
 */
std::string CmdLine::get_output_filename(int product_id) {
    std::string output_filename = getTrimmedFileName();
    //Name file base on method used
    std::string file_type = ".tif";
    std::string fit_type = useGaussianFitting ? "_gaussian" : "_firstDiff";
    std::string prod_desc = "_" +product_desc[product_id];
    return output_filename +  prod_desc + fit_type + file_type;
}

/**
 * get the description of the product being produced
 * @param product_id the product id
 * @return the short description of the product
 */
std::string CmdLine::get_product_desc(int product_id){
	return product_desc[product_id];
}
