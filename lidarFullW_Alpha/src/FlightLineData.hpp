/*
 * File name: FlightLineData.hpp
 * Created on: 09-November-2017
 * Author: ravi
 */

#ifndef FLIGHTLINEDATAUNITTESTS_HPP_
#define FLIGHTLINEDATAUNITTESTS_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include "pulsereader.hpp"
#include "pulsewriter.hpp"
#include "PulseData.hpp"

class FlightLineData{

public:
  //The min and max fields describing the boundary box that includes the 
  //first & last points of the sampled parts of the returning waveforms of 
  //all pulses
  double bb_x_min;
  double bb_y_min;
  double bb_z_min;
  double bb_x_max;
  double bb_y_max;
  double bb_z_max;  

  //Instrument information
  int scanner_id; 
  double wave_length;
  double outgoing_pulse_width;
  int scan_pattern;
  int number_of_mirror_facets;
  double scan_frequency;
  double scan_angle_min;
  double scan_angle_max;
  double pulse_frequency;
  double beam_diameter_at_exit_aperture;
  double beam_divergence;
  double minimal_range;
  double maximal_range;

  //Depends on whether there is a next pulse
  bool next_pulse_exists;

  //Stores pulse data one at a time
  std::vector<int> outgoing_time;
  std::vector<int> outgoing_wave;
  std::vector<int> returning_time;
  std::vector<int> returning_wave;

  FlightLineData();
  void setFlightLineData(std::string fileName);
  void FlightLineDataToCSV();
  bool hasNextPulse();
  void getNextPulse(PulseData* pd);;


private:
  PULSEreadOpener pOpener;
  PULSEreader *pReader;
  WAVESsampling *sampling;
  PULSEscanner scanner;

};

#endif /* FLIGHTLINEDATAUNITTESTS_HPP_ */