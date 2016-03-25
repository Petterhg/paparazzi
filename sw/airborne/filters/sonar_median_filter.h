
#define MEDIAN_DATASIZE 5

#include "std.h"
#include "math/pprz_algebra_int.h"


struct MedianFilter {
  float data[MEDIAN_DATASIZE], sortData[MEDIAN_DATASIZE];
  int dataIndex;
};


void init_median_filter(struct MedianFilter *filter);
float update_median_filter(struct MedianFilter *filter, float new_data);
float get_median_filter(struct MedianFilter *filter);

// initializes the median filter with zeros. Is it good for me?
void init_median_filter(struct MedianFilter *filter)
{
  int i;
  for (i = 0; i < MEDIAN_DATASIZE; i++) {
    filter->data[i] = 0;
    filter->sortData[i] = 0;
  }
  filter->dataIndex = 0;
}


float update_median_filter(struct MedianFilter *filter, float new_data)
{
        int i, j; // used to sort array
        float temp;
        
        // new_data is added to data        
        filter->data[filter->dataIndex] = new_data;             
        
        // dataIndex moved        
        if (filter->dataIndex < (MEDIAN_DATASIZE - 1)) {        
                filter->dataIndex++;
        } else {
                filter->dataIndex = 0;
        }
          // Copy raw data to sort data array
        memcpy(filter->sortData, filter->data, sizeof(filter->data));
        
        // Sorting data
        for (i = 1; i <= (MEDIAN_DATASIZE - 1); i++) {
                temp = filter->sortData[i];
                j = i - 1;
                
                while (temp < filter->sortData[j] && j >= 0) {
                        filter->sortData[j + 1] = filter->sortData[j];
                        j = j - 1;
                }
                filter->sortData[j + 1] = temp;
        }
        return filter->sortData[(MEDIAN_DATASIZE) >> 1]; // return middle

}

float get_median_filter(struct MedianFilter *filter)
{
  return filter->sortData[(MEDIAN_DATASIZE) >> 1];
}

