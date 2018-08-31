#include "swing_sensor_data.h"
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>

using namespace std;

//Function to check if file being read exists
inline bool check_file_exists(const std::string& filename) 
{
    struct stat buffer;   
      return (stat (filename.c_str(), &buffer) == 0); 
}

int main()
{

  //Input variables
  string filename;
  int option;
  int col_num1,col_num2,indexBegin,indexEnd,winLength;
  double threshold,thresholdLo,thresholdHi,threshold1,threshold2;

  //output containers
  std::vector<int> sCAV;
  std::vector<int> bSCWR;
  std::vector<int> sCAVTS;
  std::vector<vector<int>> sMCWR;

  cout << "Enter the path to sensor data csv file:" << endl;
  cin >> filename;

  //Check if the file being read exists
  if(!check_file_exists(filename))
  {
   cerr << filename << " file does not exists: " << endl;
   exit (EXIT_FAILURE);
  }

  //Create a SwingSensorData object which will contain the sensor data
  SwingSensorData s;

  //Reading sensor data  
  s.setMap(filename);

  //Printing read data from map created above
  //s.getMap();

  cout << endl;
  //Functions can be called again and agian with diff paraemeters
  //makes testing easier
  cout << "Choose the helper funtions to call from below options:" << endl;
  cout << "0. Exit " << endl;
  cout << "1. searchContinuityAboveValue " << endl;
  cout << "2. backSearchContinuityWithinRange " << endl;
  cout << "3. searchContinuityAboveValueTwoSignals " << endl;
  cout << "4. searchMultiContinuityWithinRange " << endl;
 
  cout << endl;
  cout << "Columns in the sensor data are named as below" << endl;
  cout << "Ax-0, Ay-1, Az-2, wx-3, wy-4, wz-5" << endl;
  cout << "Pass appropriate column number as col_num while calling functions" << endl;
  cout << endl;

  while(true)
  {
    cin >> option;
    switch(option)
    {
      case 0:
        exit (EXIT_FAILURE);
        break;
      case 1:
        cout << "searchContinuityAboveValue :\n";
        cout << "Enter column #,indexBegin,indexEnd,threshold,winLength\n";
        cin >> col_num1 >> indexBegin >> indexEnd >> threshold >> winLength;
        sCAV = s.searchContinuityAboveValue(col_num1,indexBegin,indexEnd,threshold,winLength);
        cout << sCAV[0] << " " << sCAV[1] << endl;
        break;

      case 2:
        cout << "backSearchContinuityWithinRange :" << endl;
        cout << "Enter column #,indexBegin,indexEnd,thresholdLo,thresholdHi,winLength\n";
        cin >> col_num1 >> indexBegin >> indexEnd >> thresholdLo >> thresholdHi  >> winLength;
        //s.backSearchContinuityWithinRange(1,1000,800,-5,5,10);
        bSCWR = s.backSearchContinuityWithinRange(col_num1,indexBegin,indexEnd,thresholdLo, \
                                                                thresholdHi,winLength);
        cout << bSCWR[0] << " " << bSCWR[1] << endl;
        break;

      case 3:
        cout << "searchContinuityAboveValueTwoSignals :" << endl;
        cout << "Enter column1 #,column2 #,indexBegin,indexEnd,threshold1,threshold2,winLength\n";
        cin >> col_num1 >> col_num2 >> indexBegin >> indexEnd >> threshold1 >> \
                                                threshold2  >> winLength;
        //s.searchContinuityAboveValueTwoSignals(1,2,800,1000,0,-10,10);
        sCAVTS = s.searchContinuityAboveValueTwoSignals(col_num1,col_num2,indexBegin, \
                                                        indexEnd,threshold1,threshold2,winLength);
        cout << sCAVTS[0] << " " << sCAVTS[1] << endl;
        break;

      case 4:
        cout << "searchMultiContinuityWithinRange :" << endl;
        cout << "Enter column1 #,indexBegin,indexEnd,thresholdLo,thresholdHi,winLength\n";
        cin >> col_num1 >> indexBegin >> indexEnd >> thresholdLo >> thresholdHi  >> winLength;
        //s.searchMultiContinuityWithinRange(1,800,1000,-5,5,10);
        sMCWR = s.searchMultiContinuityWithinRange(col_num1,indexBegin,indexEnd,\
                                                                thresholdLo,thresholdHi,winLength);
        //Printing the continuous pieces
        for(unsigned int i=0; i<sMCWR.size();++i)
        {
          for(unsigned int j=0;j<sMCWR[i].size();++j)
          {
            cout << sMCWR[i][j] << " ";
          }
          cout << endl;
        }

        break;
      
      default:
        cout << "Invalid choice, choose again!\n";
    }
  }

  return 0;

}
