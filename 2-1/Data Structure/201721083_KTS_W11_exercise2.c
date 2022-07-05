#include <stdio.h>;
#define MAX 30
typedef int element;
int size;
element sorted[MAX];
 
 
void merge(element a[],int m,int middle,int n){
     int i,j,k,t;
     i = m;
     j=middle+1;
     k=m;
    while(i<=middle && j<=n){
        if(a[i]<=a[j]){
            sorted[k]=a[i];
            i++;
        }
        else{
            sorted[k]=a[j];
            j++;
        }
        k++;
    }
    if(i>middle){
        for(t=j;t<=n;t++,k++){
            sorted[k]=a[t];
        }
    }
    else{
        for(t=i;t<=middle;t++,k++){
            sorted[k]=a[t];
        }
    }
     
     for(t=m;t<=n;t++){
         a[t]=sorted[t];
     }
     printf("\n Merge Sort >>");
     for(t=0;t<size;t++){
         printf("%4d",a[t]);
     }
}

void mergeSort(element a[],int m, int n){
    int middle;
    if(m<n){
        middle = (m+n)/2;
        mergeSort(a,m,middle);
        mergeSort(a,middle+1,n);
        merge(a,m,middle,n);
    }
}
void main(){
     int t;
     element list[8] = {69,10,30,2,16,8,31,22};
     size = 8;
     printf("\n Items>>");
        for (t = 0; t < size; t++) {printf("%4d ", list[t]);}
        printf("\n\n<<<<<<<<<< Execute Merge Sort >>>>>>>>>>\n");
        mergeSort(list, 0, size - 1);
     getchar();
 }
 

