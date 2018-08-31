
#include <map>
#include <iostream>
#include <vector>

using namespace std;

typedef std::map<int,vector<double>> DataMap;

class SwingSensorData {
  
  public:
    void setMap(string);
    void getMap() const;
    std::vector<int> searchContinuityAboveValue(int,int,int,double,int);
    std::vector<int> backSearchContinuityWithinRange(int,int,int,double,double,int);
    std::vector<int> searchContinuityAboveValueTwoSignals(int,int,int,int,double,double,int);
    std::vector<vector <int>> searchMultiContinuityWithinRange(int,int,int,double,double,int);

  private:
    DataMap sensor_data; //map which stores the data from csv file
};

