#ifndef QUICKSORT_HEADER
#define QUICKSORT_HEADER
#include <vector>
#include <functional>


template <typename T>
void quickSort(std::vector<T*> &array,int start,int end,std::function<bool(T, T)> compare){
    if(start>=end)
        return;
    //order the elements
    int pivot = end;
    int i= start-1;
    int j= start;

    while (j < pivot) {
        if (compare(*array[j],*array[pivot])) {
            i++;
            T *temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
        j++;
    }

    T * temp = array[pivot];
    array[pivot] = array[i+1];
    array[i+1] = temp;

    //update the pivot
    pivot = i+1;

    //partition
    quickSort(array,start,pivot-1,compare);
    quickSort(array,pivot+1,end,compare);
};
#endif



