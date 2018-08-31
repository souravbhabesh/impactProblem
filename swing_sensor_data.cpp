#include <iostream>
#include<fstream>
#include<sstream>
#include <map>
#include <stdlib.h>
#include "swing_sensor_data.h"


void SwingSensorData::setMap(string filePath)
{
  ifstream file(filePath);
  string line = "";
  int key;
  vector<double> values;
  int line_cnt=-1;


  while(getline(file,line))
  {
    line_cnt++;//Line count starts from 0
    stringstream linestream(line);
    string cell_value;
    getline(linestream,cell_value,',');
    key=line_cnt;
    //cout << key << endl;
    while(getline(linestream,cell_value,','))
        {
            values.push_back(stod(cell_value));
        }

    sensor_data.insert(DataMap::value_type(key,values));
    values.clear();
  }
  //close csv file
  file.close();

}

void SwingSensorData::getMap() const
{
  cout << "Printing the file" << endl;
  for(DataMap::const_iterator iter = sensor_data.begin(); iter!=sensor_data.end();++iter)
  {
    cout << iter->first << '\t';
    for(int i=0;i<6;i++)
      cout << iter->second[i]<<' ';
    cout << '\n';
  }
}


//Function returns the start and end index of the First continuity in the given range
std::vector<int> SwingSensorData::searchContinuityAboveValue(int data,int indexBegin,int indexEnd \
                                                ,double threshold,int winLength)
{

  //Iterators to iterate over the sensor data map
  auto iter1 = sensor_data.find(indexBegin);
  auto iter_end = sensor_data.find(indexEnd);

  //Vectors to hold the filtered output, start & end index of continuity found
  vector<int> filter_vec;
  vector<int> cStart,cEnd;

  //Return Vector : Holds the start and end index of the First continuity in the given range
  std::vector<int> result;

  //We apply the filter to each data point in the range indexBegin to indexEnd
  //Points satisfying the filter condition output 1 else 0
  //Once we have got the sequence of 0's and 1's, sub arrays of 1's represents continuity in range
  //Identify these sub arrays by looking for 0->1 transition (START of sub array ) and
  //1->0 marks the End of sub array
  //Trick: We pad an extra 0 each in the front and back to handle cases which start or end with 1

  filter_vec.push_back(0);//Front padding with 0

  //Apply filter to data points in the range provided
  while(iter1 != iter_end)
  {
    iter1->second[data] > threshold ? filter_vec.push_back(1) : filter_vec.push_back(0);
    iter1++;
  }

   //Reading in the last index sensor data
   iter_end->second[data] > threshold ? filter_vec.push_back(1) : filter_vec.push_back(0);

  filter_vec.push_back(0);//Back pad with 0

  //Search for 0->1 (Start) and 1->0 (End) which marks start and end of a continuous piece 
  for (unsigned int i = 1; i < filter_vec.size(); ++i)
  {
    if(filter_vec[i]==1 && filter_vec[i-1]==0)
      cStart.push_back(i);
    if(filter_vec[i]==0 && filter_vec[i-1]==1)
      cEnd.push_back(i);
  }

  //Sanity check, filter vector partitioning is correct
  if(cStart.size() != cEnd.size())
    cerr << "Incorrect partitioning of filtered data in backSearchContinuityWithinRange()!\n";

  //Searching for First continuous piece greater than equal to winLength
  for (unsigned int i = 0; i < cStart.size(); ++i)
  {
    if((cEnd[i]-cStart[i]) >= winLength)
    {
      //cout << indexBegin+cStart[i]-1 << " " << indexBegin+cEnd[i]-2 << " " << endl;
      result.push_back(indexBegin+cStart[i]-1);
      result.push_back(indexBegin+cEnd[i]-2);
      break;
    }
  }
  //Case when no continuous piece is found
  if(result.size() == 0)
    result = {-1,-1};

  return result;
}


std::vector<int> SwingSensorData::backSearchContinuityWithinRange(int data,int indexBegin,int indexEnd \
                                      ,double thresholdLo,double thresholdHi,int winLength)

{
  //indexBegin is greater than indexEnd
  if(indexBegin < indexEnd)
  {
    cerr << "IndexBegin should be greater than index end, exiting!\n";
    exit (EXIT_FAILURE);
  }
  //Iterators to iterate over the sensor data map
  auto iter1 = sensor_data.find(indexBegin);
  auto iter_end = sensor_data.find(indexEnd);

  //Vectors to hold the filtered output, start & end index of continuity found
  vector<int> filter_vec;
  vector<int> cStart,cEnd;

  //Return Vector : Holds the start and end index of the First continuity in the given range
  std::vector<int> result;

  //We apply the filter to each data point in the range indexBegin to indexEnd
  //Points satisfying the filter condition output 1 else 0
  //Once we have got the sequence of 0's and 1's, sub arrays of 1's represents continuity in range
  //Identify these sub arrays by looking for 0->1 transition (START of sub array ) and
  //1->0 marks the End of sub array
  //Trick: We pad an extra 0 each in the front and back to handle cases which start or end with 1

  filter_vec.push_back(0);//front padding with 0
  //Apply filter to data points in the range provided
  while(iter1 != iter_end)
  {
    (iter1->second[data] > thresholdLo && iter1->second[data] < thresholdHi) ? \
                                    filter_vec.push_back(1) : filter_vec.push_back(0);
    iter1--;
  }

  //Reading in the last index sensor data 
  (iter_end->second[data] > thresholdLo && iter_end->second[data] < thresholdHi) ? \
                                            filter_vec.push_back(1) : filter_vec.push_back(0);

  filter_vec.push_back(0); //back pad with 0


  //Search for 0->1 (Start) and 1->0 (End) which marks start and end of a continuous piece 
  for (unsigned int i = 1; i < filter_vec.size(); ++i)
  {
    if(filter_vec[i]==1 && filter_vec[i-1]==0)
      cStart.push_back(i);
    if(filter_vec[i]==0 && filter_vec[i-1]==1)
      cEnd.push_back(i);
  }

  //Sanity check, filter vector partitioning is correct
  if(cStart.size() != cEnd.size())
    cerr << "Incorrect partitioning of filtered data in backSearchContinuityWithinRange()!\n";
  
  //Searching for first continuous piece with size greater than equal to winLength
  for (unsigned int i = 0; i < cStart.size(); ++i)
  {
    if((cEnd[i]-cStart[i]) >= winLength)
        {
          //cout << indexBegin-cStart[i]+1 << " " << indexBegin-cEnd[i]+2 << endl;
          result = {indexBegin-cStart[i]+1,indexBegin-cEnd[i]+2};
          break;
        }
  }

  
  //Case when no continuous piece is found
  if(result.size() == 0)
    result = {-1,-1};

  return result;
}

std::vector<int> SwingSensorData::searchContinuityAboveValueTwoSignals(int data1,int data2,int indexBegin, \
                            int indexEnd,double threshold1,double threshold2, int winLength)
{
  //Iterators to iterate over the sensor data map
  auto iter1 = sensor_data.find(indexBegin);
  auto iter_end = sensor_data.find(indexEnd);

  //Vectors to hold the filtered output, start & end index of continuity found
  vector<int> filter_vec;
  vector<int> cStart,cEnd;

  //Return Vector : Holds the start and end index of the First continuity in the given range
  vector<int> result;

  //We apply the filter to each data point in the range indexBegin to indexEnd
  //Points satisfying the filter condition output 1 else 0
  //Once we have got the sequence of 0's and 1's, sub arrays of 1's represents continuity in range
  //Identify these sub arrays by looking for 0->1 transition (START of sub array ) and
  //1->0 marks the End of sub array
  //Trick: We pad an extra 0 each in the front and back to handle cases which start or end with 1

  filter_vec.push_back(0);//front padding with 0
  //Apply filter to data points in the range provided
  while(iter1 != iter_end)
  {
    (iter1->second[data1] > threshold1 && iter1->second[data2] > threshold2) ? \
                                    filter_vec.push_back(1) : filter_vec.push_back(0);
    iter1++;
  }
  //Reading in the last index sensor data
  (iter_end->second[data1] > threshold1 && iter_end->second[data2] > threshold2) ? \
                                        filter_vec.push_back(1) : filter_vec.push_back(0);
  filter_vec.push_back(0); //back pad with 0


  //Search for 0->1 (Start) and 1->0 (End) which marks start and end of a continuous piece 
  for (unsigned int i = 1; i < filter_vec.size(); ++i)
  {
    if(filter_vec[i]==1 && filter_vec[i-1]==0)
      cStart.push_back(i);
    if(filter_vec[i]==0 && filter_vec[i-1]==1)
      cEnd.push_back(i);
  }

  //Sanity check, filter vector partitioning is correct
  if(cStart.size() != cEnd.size())
    cerr << "Incorrect partitioning of filtered data in backSearchContinuityWithinRange()!\n";

  //Searching for continuous piece greater than equal to winLength for both data1 and data2 colmns
  for (unsigned int i = 0; i < cStart.size(); ++i)
  {
    if((cEnd[i]-cStart[i]) >= winLength)
    {
      //cout << indexBegin+cStart[i]-1 << " " << indexBegin+cEnd[i]-2 << " " << endl;
      result = {indexBegin+cStart[i]-1,indexBegin+cEnd[i]-2};
      break;
    }
  }

  //Case when no continuous piece is found
  if(result.size() == 0)
    result = {-1,-1};

  return result;
}

std::vector<vector <int>>  SwingSensorData::searchMultiContinuityWithinRange(int data,int indexBegin,int indexEnd \
                                      ,double thresholdLo,double thresholdHi,int winLength)

{
  //Iterators to iterate over the sensor data map
  auto iter1 = sensor_data.find(indexBegin);
  auto iter_end = sensor_data.find(indexEnd);

  //Vectors to hold the filtered output, start & end index of continuity found
  vector<int> filter_vec;
  vector<int> cStart,cEnd;

  //Return vector containg the start index and end index of all continuous pieces
  //within range whose size is greater than winLength
  vector<vector<int> > result;

  //We apply the filter to each data point in the range indexBegin to indexEnd
  //Points satisfying the filter condition output 1 else 0
  //Once we have got the sequence of 0's and 1's, sub arrays of 1's represents continuity in range
  //Identify these sub arrays by looking for 0->1 transition (START of sub array ) and
  //1->0 marks the End of sub array
  //Trick: We pad an extra 0 each in the front and back to handle cases which start or end with 1

  filter_vec.push_back(0);//front padding with 0
  //Apply filter to data points in the range provided
  while(iter1 != iter_end)
  {
    (iter1->second[data] > thresholdLo && iter1->second[data] < thresholdHi) ? \
                                    filter_vec.push_back(1) : filter_vec.push_back(0);
    iter1++;
  }
  //Reading in the last index sensor data 
  (iter_end->second[data] > thresholdLo && iter_end->second[data] < thresholdHi) ? \
                                        filter_vec.push_back(1) : filter_vec.push_back(0);
  filter_vec.push_back(0); //back pad with 0


  //Search for 0->1 (Start) and 1->0 (End) which marks start and end of a continuous piece 
  for (unsigned int i = 1; i < filter_vec.size(); ++i)
  {
    if(filter_vec[i]==1 && filter_vec[i-1]==0)
      cStart.push_back(i);
    if(filter_vec[i]==0 && filter_vec[i-1]==1)
      cEnd.push_back(i);
  }

  //Sanity check, filter vector partitioning is correct
  if(cStart.size() != cEnd.size())
    cerr << "Incorrect partitioning of filtered data in backSearchContinuityWithinRange()!\n";

  for (unsigned int i = 0; i < cStart.size(); ++i)
  {
    //Discarding continuous pieces whose size is less than winLength
    if((cEnd[i]-cStart[i]) < winLength)
        {
          cStart.erase(cStart.begin()+i);
          cEnd.erase(cEnd.begin()+i);
          i--;
        }
  }

  //Printing the start and end index of continuous pieces
  for (unsigned int i = 0; i < cStart.size(); ++i)
  {
    //cout << indexBegin+cStart[i]-1 << " " << indexBegin+cEnd[i]-2 << " " << endl;
    result.push_back({indexBegin+cStart[i]-1,indexBegin+cEnd[i]-2});
  }

  //Case when no continuous piece is found
  if(result.size() == 0)
    result = {{-1,-1}};

  return result;
}


