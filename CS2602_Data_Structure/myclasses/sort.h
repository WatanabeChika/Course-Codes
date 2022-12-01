#ifndef SORT_INCLUDED
#define SORT_INCLUDED

template <class elemType>
struct pair {
    elemType data;
    int index;
    pair(elemType d, int i) : data(d), index(i) {};
};


template <class elemType>
class sort {
    private:
        elemType *a;
        int n;

        void insert(elemType *array, int num, elemType data);
        void merge(elemType *array, int low, int mid, int high);
        void mergeSort(elemType *array, int low, int high);
        void quickSort(elemType *array, int start, int end);
        void adjust(elemType *array, int num, int index);

    public:
        sort(elemType *a, int n) : a(a), n(n) {};
        void bubbleSort();
        void insertSort();
        void shellSort();
        void mergeSort();
        void quickSort();
        void selectSort();
        void heapSort();
        void LSDSort();

};

// 冒泡排序
template<class elemType>
void sort<elemType>::bubbleSort() 
{
    int i, j;
    elemType tmp;
    bool change = true;

    for (j = n-1; j > 0 && change; --j) {
        change = false;
        for (i = 0; i < j; ++i) {
            if (a[i+1] < a[i]) {
                tmp = a[i];
                a[i] = a[i+1];
                a[i+1] = tmp;
                change = true;
            }
        }
    }
}

// 插入排序
template<class elemType>
void sort<elemType>::insert(elemType *array, int num, elemType data) 
{
    int i;

    for (i = num-1; i >= 0; --i) {
        if (array[i] <= data)
            break;
        else
            array[i+1] = array[i];
    }
    array[i+1] = data;
}

template<class elemType>
void sort<elemType>::insertSort()
{
    int i;

    for (i = 1; i < n; ++i) {
        insert(a, i, a[i]);
    }
}

// 希尔排序
template<class elemType>
void sort<elemType>::shellSort()
{
    int step, i, j;
    elemType tmp;

    for (step = n/2; step > 0; step/=2) {
        for (i = step; i < n; ++i) {
            tmp = a[i];
            j = i;
            while (j-step >= 0 && a[j-step] > tmp) {
                a[j] = a[j-step];
                j-=step;
            }
            a[j] = tmp;
        }
    }
}

// 归并排序
template<class elemType>
void sort<elemType>::merge(elemType *array, int low, int mid, int high)
{
    elemType tmp[high-low+1];
    int i = low, j = mid+1, k = 0;

    while (i<=mid && j<=high) {
        if (array[i] <= array[j]) {
            tmp[k] = array[i];
            ++i;
        } 
        else {
            tmp[k] = array[j];
            ++j;
        }
        ++k;
    }
    while (i<=mid) {
        tmp[k] = array[i];
        ++k; ++i;
    }
    while (j<=high) {
        tmp[k] = array[j];
        ++k; ++j;
    }
    for (i = low; i < high+1; ++i) 
        array[i] = tmp[k];
    delete []tmp;
}

template<class elemType>
void sort<elemType>::mergeSort(elemType *array, int low, int high) 
{
    if (low >= high) return;
    int mid = (low + high)/2;

    mergeSort(array, low, mid);
    mergeSort(array, mid+1, high);
    merge(array, low, mid, high);
}

template<class elemType>
void sort<elemType>::mergeSort() 
{
    mergeSort(a, 0, n-1);
}

// 快速排序
template<class elemType>
void sort<elemType>::quickSort(elemType *array, int start, int end)
{
    if (end <= start) return;
    elemType flag = array[start];
    int i = start, j = end;
    int hole = start;

    while (i < j) {
        while (j > i && array[j] >= flag) --j;
        if (j == i) break;
        array[hole] = array[j];
        hole = j;
        
        while (j > i && array[i] < flag) ++i;
        if (j == i) break;
        array[hole] = array[i];
        hole = i;
    }
    array[hole] = flag;

    quickSort(array, start, hole-1);
    quickSort(array, hole+1, end);
}

template<class elemType>
void sort<elemType>::quickSort()
{
    quickSort(a, 0, n-1);
}

// 选择排序
template<class elemType>
void sort<elemType>::selectSort()
{
    int minIndex;
    int i, j;
    elemType tmp;

    for (j = 0; j < n; ++j) {
        minIndex = j;
        for (i = j+1; i < n; ++i) {
            if (a[i] < a[minIndex])
                minIndex = i;
        }
        if (minIndex == j)
            continue;
        tmp = a[minIndex];
        a[minIndex] = a[j-1];
        a[j-1] = tmp;
    }
}

// 堆排序
template<class elemType>
void adjust(elemType *array, int num, int index)
{
    int maxChild;
    while (true) {
        maxChild = 2*index+1;
        if (maxChild > num-1) return;
        if (maxChild+1 <= num-1) {
            if (array[maxChild+1] > array[maxChild])
                ++maxChild;
        }
        // 交换最大子和根
        if (array[index] >= array[maxChild]) return;
        elemType tmp = array[index];
        array[index] = array[maxChild];
        array[maxChild] = tmp;
        // 向下调整
        index = maxChild;
    }
}

template<class elemType>
void sort<elemType>::heapSort()
{
    int i, j;
    // 从最后一个非叶子结点开始
    for (i = (n-1)/2; i >= 0; --i) {
        adjust(a, n, i);
    }
    // 换大顶 调大顶
    for (j = n-1; j > 0; --j) {
        elemType tmp = a[0];
        a[0] = a[j];
        a[j] = tmp;
        adjust(a, j, 0);
    }
}

// 基数排序--低位优先法
template<class elemType>
void sort<elemType>::LSDSort()
{
    elemType pocket[10][n] = -1;
    int i, j, anum = 0;
    int pos = 1;
    bool outPos = false;

    while (!outPos) {
        outPos = true;
        for (i = 0; i < n; ++i) {
            if (outPos && a[i]/pos != 0)
                outPos = false; 
            pocket[(a[i]/pos)%10][i] = a[i];
        }

        for (i = 0; i < 10; ++i) {
            if (anum >= n) break;
            for (j = 0; j < n; ++j) {
                if (pocket[i][j] != -1) {
                    a[anum] = pocket[i][j];
                    ++anum;
                }
            }
        }
        pos *= 10;
    }
}

#endif