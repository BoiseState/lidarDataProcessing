/*
 * File name: AmplitudeData_unittests.cpp
 * Created on: 18-July-2017
 * Author: ravi
 *
 * This test requres the sample 140823_183115_1_clipped_test.pls file 
 * and its corresponding wvs file to be in the etc directory to run
 *
 */

#include "gtest/gtest.h"
#include "AmplitudeData.hpp"
#include "pulsereader.hpp"
#include "pulsewriter.hpp"


class AmplitudeDataTest: public testing::Test{
  public:  
    AmplitudeData outgoingWave;
    AmplitudeData returningWave;

  protected:

  //Function to set up space used by all tests
  virtual void SetUp(){

    //Read the wave file
    std::string fileName=  "etc/140823_183115_1_clipped_test.pls";
    PULSEreadOpener pOpener;  // Create a pulse read opener object
    PULSEreader *pReader;     // Create a pulse reader object
    WAVESsampling *sampling;  // Create a wave sampling object
    pOpener.set_file_name(fileName.c_str());
    pReader = pOpener.open();
    int maxCount = 60;
    long long pulseIndex = 0; // Keep track of the index

    //Populate the wave data
    while(pReader->read_pulse()){

      //Read the waves
      if(pReader->read_waves()){
        for(int i = 0; i < pReader->waves->get_number_of_samplings(); i++){
          sampling = pReader->waves->get_sampling(i);

          //Based on the type of wave, populate data
          if(sampling->get_type() == PULSEWAVES_OUTGOING){
            outgoingWave.populate(sampling, maxCount, pulseIndex);

          }
          else if(sampling->get_type() == PULSEWAVES_RETURNING){
            returningWave.populate(sampling, maxCount, pulseIndex);
          }
          else{
            std::cout << "Unknown type: " << sampling->get_type() \
                      << std::endl;
          }
        }
      }

      //No waves
      else{
        std::cout <<"NO DATA!\n" << std::endl;
      }
      
      pulseIndex++;
    }

    outgoingWave.calculateFirstDifference();
    outgoingWave.calculateSecondDifference();
    outgoingWave.calculateSmoothSecondDifference();

    returningWave.calculateFirstDifference();
    returningWave.calculateSecondDifference();
    returningWave.calculateSmoothSecondDifference();
    returningWave.findPeaks(returningWave.waveData);


  }
};

/******************************************************************************
* 
* Test outgoing wave data
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, outgoingWaveData){

  I32 truthOutgoingWaveData[244] = {0,2,2,2,3,2,2,8,28,70,128,177,192,167,118,
                     68,31,12,5,4,5,5,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     1,1,2,1,2,2,3,8,24,63,121,173,194,173,126,74,35,14,5,
                     3,4,5,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     2,6,5,5,5,3,2,6,21,59,116,168,192,175,128,75,36,15,5,
                     3,4,5,5,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     3,3,3,2,2,2,3,6,21,59,115,168,192,176,130,79,39,16,7,
                     6,6,7,6,3,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  //Test size
 ASSERT_EQ(244,outgoingWave.waveData.size());

  //Test data
  for(int i = 0; i<=243; i++){
    ASSERT_EQ(truthOutgoingWaveData[i], outgoingWave.waveData[i]);
  }
}

/******************************************************************************
* 
* Test returning wave data
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, returningWaveData){

  I32 truthReturningWaveData[122] = {1,2,2,2,1,1,1,1,1,1,0,0,1,9,35,88,155,212,
                    240,237,200,145,87,42,18,12,13,14,15,15,14,13,10,8,8,8,8,7,
                    6,6,4,4,4,3,4,5,6,4,4,3,2,2,1,1,0,1,2,3,4,4,2,
                    2,1,2,2,3,2,2,1,1,3,2,2,3,5,19,58,121,186,228,238,214,164,
                    106,58,26,13,10,12,15,17,17,16,13,10,7,6,7,6,6,4,6,6,6,5,6,
                    6,6,6,5,4,4,2,2,1,2,2,1,2,2,2,2};
  //Test size
  ASSERT_EQ(122,returningWave.waveData.size()); 

  //Test data
  for(int i = 0; i<=121; i++){
    EXPECT_EQ(truthReturningWaveData[i], returningWave.waveData[i]);
  }
}

/******************************************************************************
* 
* Test first difference of outgoing wave
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, outgoingWavefirstDifference){

  int truthFirstDiffOutgoingWave[236] = {0,0,1,-1,0,6,20,42,58,49,15,-25,-49,
                  -50,-37,-19,-7,-1,1,0,-2,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  1,-1,1,0,1,5,16,39,58,52,21,-21,-47,-52,-39,-21,-9,-2,1,1,-1,
                  -2,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,
                  -1,0,0,-2,-1,4,15,38,57,52,24,-17,-47,-53,-39,-21,-10,-2,1,1,
                  0,-2,-2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,
                  0,-1,0,0,1,3,15,38,56,53,24,-16,-46,-51,-40,-23,-9,-1,0,1,-1,
                  -3,-2,-1,0,0,1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0};
  //Test size
  ASSERT_EQ(236,outgoingWave.firstDifference.size()); 

  //Test data
  for(int i = 0; i<=235; i++){
    ASSERT_EQ(truthFirstDiffOutgoingWave[i],outgoingWave.firstDifference[i]);
  }
}

/******************************************************************************
* 
* Test first difference of returning wave
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, returningWavefirstDifference){

  int truthFirstDiffReturningWave[118] = {0,0,-1,0,0,0,0,0,-1,0,1,8,26,53,67,
                  57,28,-3,-37,-55,-58,-45,-24,-6,1,1,1,0,-1,-1,-3,-2,0,0,0,-1,
                  -1,0,-2,0,0,-1,1,1,1,-2,0,-1,-1,0,-1,0,-1,1,1,1,1,0,-2,
                  1,0,1,-1,0,-1,0,2,-1,0,1,2,14,39,63,65,42,10,-24,-50,-58,-48,
                  -32,-13,-3,2,3,2,0,-1,-3,-3,-3,-1,1,-1,0,-2,2,0,0,-1,1,0,0,0,
                  -1,-1,0,-2,0,-1,1,0,-1,1,0,0,0};
  //Test size
  ASSERT_EQ(118,returningWave.firstDifference.size()); 

  //Test data
  for(int i = 0; i<=117; i++){
    ASSERT_EQ(truthFirstDiffReturningWave[i],returningWave.firstDifference[i]);
  }
}

/******************************************************************************
* 
* Test second difference of outgoing wave
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, outgoingWaveSecondDifference){

  int truthSecondDiffOutgoingWave[232] = {0,1,2,1,6,14,22,16,9,34,40,24,1,13,
                  18,12,6,2,1,2,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  2,2,1,1,4,11,23,19,6,31,42,26,5,13,18,12,7,3,0,2,1,1,0,1,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,
                  1,0,2,1,5,11,23,19,5,28,41,30,6,14,18,11,8,3,0,1,2,0,1,1,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,
                  1,1,0,1,2,12,23,18,3,29,40,30,5,11,17,14,8,1,1,2,2,1,1,1,0,1,
                  2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0};
  //Test size
  ASSERT_EQ(232,outgoingWave.secondDifference.size()); 

  //Test data
  for(int i = 0; i<=231; i++){
    ASSERT_EQ(truthSecondDiffOutgoingWave[i],outgoingWave.secondDifference[i]);
  }
}

/******************************************************************************
* 
* Test second difference of returning wave
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, returningWaveSecondDifference){

  int truthSecondDiffReturnWave[116] = {0,1,1,0,0,0,0,1,1,1,7,18,27,14,10,29,
                  31,34,18,3,13,21,18,7,0,0,1,1,0,2,1,2,0,0,1,0,1,2,2,0,1,2,0,
                  0,3,2,1,0,1,1,1,1,2,0,0,0,1,2,
                  1,1,2,1,1,1,2,3,1,1,1,12,25,24,2,23,32,34,26,8,10,16,19,10,5,
                  1,1,2,1,2,0,0,2,2,2,1,2,4,2,0,1,2,1,0,0,1,0,1,2,2,1,2,1,1,2,
                  1,0,0};
  //Test size
  ASSERT_EQ(116,returningWave.secondDifference.size()); 

  //Test data
  for(int i = 0; i<=115; i++){
    ASSERT_EQ(truthSecondDiffReturnWave[i],returningWave.secondDifference[i]);
  }
}

/******************************************************************************
* 
* Test the medianOfFive function
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, medianOfFive){

  int a,b,c,d,e;
  a = 1;
  b = 2;
  c = 3;
  d = 4;
  e = 5;
  EXPECT_EQ(3,returningWave.medianOfFive(a,b,c,d,e));
  
  a = 5;
  b = 4;
  c = 3;
  d = 2;
  e = 1;
  EXPECT_EQ(3,returningWave.medianOfFive(a,b,c,d,e));
  
  a = 1;
  b = 1;
  c = 1;
  d = 1;
  e = 1;
  EXPECT_EQ(1,returningWave.medianOfFive(a,b,c,d,e));
  
  a = 1;
  b = 5;
  c = 1;
  d = 3;
  e = 5;
  EXPECT_EQ(3,returningWave.medianOfFive(a,b,c,d,e));
  
  a = 5;
  b = 1;
  c = 4;
  d = 3;
  e = 2;
  EXPECT_EQ(3,returningWave.medianOfFive(a,b,c,d,e));
  
  a = 1;
  b = 1;
  c = 3;
  d = 5;
  e = 5;
  EXPECT_EQ(3,returningWave.medianOfFive(a,b,c,d,e));
  
  a = 1;
  b = 2;
  c = 4;
  d = 4;
  e = 5;
  EXPECT_EQ(4,returningWave.medianOfFive(a,b,c,d,e));

  a = 1;
  b = 1;
  c = 2;
  d = 1;
  e = 0;
  EXPECT_EQ(1,returningWave.medianOfFive(a,b,c,d,e));
}

/******************************************************************************
* 
* Test smoothing of second difference of returning wave
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, smoothingReturningWaveSecondDifference){

  int truthSmoothSecondDiffReturnWave[116] = {0,1,0,0,0,0,0,1,1,1,7,14,14,18,
                  27,29,29,29,18,18,18,13,13,7,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,2,
                  1,0,1,2,1,1,1,1,1,1,1,1,1,0,0,0,1,2,
                  1,1,1,1,1,1,1,1,1,1,1,12,12,23,24,24,26,26,26,16,16,10,10,10,
                  5,2,1,1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,0,0,1,1,1,2,2,1,1,1,
                  1,1,0,0};
  //Test size
  EXPECT_EQ(116,returningWave.smoothSecondDifference.size()); 

  //Test data
  for(int i = 0; i<=115; i++){
    EXPECT_EQ(truthSmoothSecondDiffReturnWave[i],
              returningWave.smoothSecondDifference[i]);
  }
}

/******************************************************************************
* 
* Test findPeaks() method on returning wave data
* 
******************************************************************************/
TEST_F(AmplitudeDataTest, findPeaks){

  int truthPeaks[8] = {240,15,6,238,17,7,6,6};
  int truthPeaksLocation[8] = {18,28,46,80,90,97,101,105};

  //Test size
  EXPECT_EQ(8,returningWave.peaks.size()); 

  //Test peaks data
  for(int i = 0; i<=7; i++){
    EXPECT_EQ(truthPeaks[i],
              returningWave.peaks[i]);
  }

  //Test peak's location data
  for(int i = 0; i<=7; i++){
    EXPECT_EQ(truthPeaksLocation[i],
              returningWave.peaksLocation[i]);
  }

}