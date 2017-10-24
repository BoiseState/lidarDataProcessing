/*
 * File name: GPSInformation.cpp
 * Created on: 20-June-2017
 * Author: ravi
 */

#include <iostream>
#include "GPSInformation.hpp"

//Default constructor
GPSInformation::GPSInformation(){
  // enter default values
  xAnchor = 0;
  yAnchor = 0;
  zAnchor = 0;
  xTarget = 0;
  yTarget = 0;
  zTarget = 0;
  xFirst = 0;
  yFirst = 0;
  zFirst = 0;
  xLast = 0;
  yLast = 0;
  zLast = 0;
  edge = 0;
  facet = 0;
  scanDirection = 0;
  intensity = 0;
  
  //xyzCoordinates.xCoordinate = 0;
  //xyzCoordinates.yCoordinate = 0;
  //xyzCoordinates.zCoordinate = 0;
}


/*
 * Populate all the GPS data
 */
void GPSInformation::populateGPS(PULSEreader *pReader){
  
    gpsTime = pReader->pulse.get_t();
    
    pReader->pulse.compute_anchor_and_target_and_dir();
    xAnchor = pReader->pulse.get_anchor_x();
    yAnchor = pReader->pulse.get_anchor_y();
    zAnchor = pReader->pulse.get_anchor_z();
    xTarget = pReader->pulse.get_target_x();
    yTarget = pReader->pulse.get_target_y();
    zTarget = pReader->pulse.get_target_z();
    
    pReader->pulse.compute_first_and_last();
    xFirst = pReader->pulse.get_first_x();
    yFirst = pReader->pulse.get_first_y();
    zFirst = pReader->pulse.get_first_z();
    xLast = pReader->pulse.get_last_x();
    yLast = pReader->pulse.get_last_y();
    zLast = pReader->pulse.get_last_z();

    edge = pReader->pulse.edge_of_scan_line;
    scanDirection = pReader->pulse.scan_direction;
    facet = pReader->pulse.mirror_facet,
    intensity = pReader->pulse.intensity;
}

/*
 * Displays all GPS data
 */
void GPSInformation::displayGPSData(){
  std::cout << xAnchor << std::endl;
  std::cout << zAnchor << std::endl;
  std::cout << xTarget << std::endl;
  std::cout << yTarget << std::endl;
  std::cout << zTarget << std::endl;
  std::cout << xFirst << std::endl;
  std::cout << yFirst << std::endl;
  std::cout << zFirst << std::endl;
  std::cout << xLast << std::endl;
  std::cout << yLast << std::endl;
  std::cout << zLast << std::endl;
  std::cout << edge << std::endl;
  std::cout << facet << std::endl;
  std::cout << scanDirection << std::endl;
  std::cout << intensity << std::endl;
}
/*
void GPSInformation::getRealPoint(pulseAxisPoint, axisScale, axisOffset) {
    realAxisPoint = (pulseAxisPoint * axisScale) + axisOffset;
    return realAxisPoint;
}

void GPSInformation::getAxisDerivative(axisTarget, axisAnchor, units) {
    axisDerivative = (axisTarget - axisAnchor) / units;
    return axisDerivative;
}

//Idea for method
void GPSInformation::getRealPeakCoordinate(x_axis, y_axis, z_axis, PULSEreader *pReader){
    xyzCoordinates.xCoordinate = getRealPoint(x_axis,pReader.get_scale_x, pReader.get_offset_x);
    xyzCoordinates.yCoordinate = getRealPoint(y_axis,pReader.get_scale_y, pReader.get_offset_y);
    xyzCoordinates.zCoordinate = getRealPoint(z_axis,pReader.get_scale_z, pReader.get_offset_z);
} **/
