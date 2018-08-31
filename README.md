# impactProblem

## Compiling
Compile the code using Makefile or the below command:

g++ -std=c++11 -g swing_sensor_data.cpp main.cpp -o DK -Wall -O2

## Program 
The program loads a csv file with sensor data.

4 helper functions are provided whcih can be called on the columns in the CSV file.

Choose the helper funtions to call from below options:

0. Exit 
1. searchContinuityAboveValue 
2. backSearchContinuityWithinRange 
3. searchContinuityAboveValueTwoSignals 
4. searchMultiContinuityWithinRange 

Columns in the sensor data are indexed as below:

Ax-0, Ay-1, Az-2, wx-3, wy-4, wz-5

Pass appropriate column number as col_num while calling functions

## Function return 

Options 1, 2 and 3 return the first continuous piece found in the range provided satisfying the window condition.

Option 4 returns all the continuous pieces found.

If returned indices are equal to -1 -1, no conitnuous piece was found in the given range.

## Run Example

Sample run example is provided in run_sample.docx file
