#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define name_max 50
#define nation_max 30
#define state_max 20
#define year_max 4
#define num_max 5
#define line_max 10000
#define SWAP(x, y, temp) ( (temp)=(x), (x)=(y), (y)=(temp) )
#define PRODUCTION_YEAR 0
#define NUMBER_OF_PRODUCTION 1
#define THEN_COMPARING 2
clock_t start, finish =0;

typedef struct{
    char name[name_max];
    char nation[nation_max];
    char state[state_max];
    char num[num_max];
    char prod_year[year_max];
    int number;
    int production_year;
}JET;
void Print_Jet(JET Jet[],int count);
void Merge_Sort(JET Jet[], int start, int last, int which_one);
void Merge(JET Jet[], int start, int middle, int last, int which_one);
void Bubble_Sort(JET Jet[], int count,int which_one){
    JET temp;
    if(which_one==PRODUCTION_YEAR){
        for (int i = 0; i < count; i++){
            for (int j = 0; j < count-1; j++){
                if (Jet[j].production_year > Jet[j+1].production_year){
                    SWAP(Jet[j], Jet[j+1], temp);
                }
            }
        }
    }
    else if(which_one==NUMBER_OF_PRODUCTION){
        for (int i = 0; i < count; i++){
            for (int j = 0; j < count-1; j++){
                if (Jet[j].number > Jet[j+1].number){
                    SWAP(Jet[j], Jet[j+1], temp);
                }
            }
        }
    }
    else if(which_one==THEN_COMPARING){
        for (int i = 0; i < count-1; i++){
            for (int j = 0; j < count-1; j++){
                if (Jet[j].production_year > Jet[j+1].production_year){
                    SWAP(Jet[j], Jet[j+1], temp);
                }
                else if(Jet[j].production_year==Jet[j+1].production_year&&Jet[j].number>Jet[j+1].number){
                    SWAP(Jet[j], Jet[j+1], temp);
                }
            }
        }
    }
}

void Selection_Sort(JET Jet[],int count, int which_one){
    int i, j, least;
    JET temp;
    if(which_one==PRODUCTION_YEAR){
          for(i=0; i<count; i++){
            least = i;
            for(j=i+1; j<count; j++){
              if(Jet[j].production_year<Jet[least].production_year) least = j;
            }
              if(i != least) SWAP(Jet[i], Jet[least], temp);
          }
    }
    else if(which_one==NUMBER_OF_PRODUCTION){
          for(i=0; i<count; i++){
            least = i;
            for(j=i+1; j<count; j++){
                if(Jet[j].number<Jet[least].number) least = j;
            }
            if(i != least) SWAP(Jet[i], Jet[least], temp);
          }
    }
    else if(which_one==THEN_COMPARING){
        for(i=0; i<count; i++){
          least = i;
          for(j=i+1; j<count; j++){
            if(Jet[j].production_year<Jet[least].production_year) least = j;
            else if(Jet[j].production_year==Jet[least].production_year&&Jet[j].number<Jet[least].number) least = j;
          }
            if(i != least) SWAP(Jet[i], Jet[least], temp);
        }
    }
}

void Shell_Sort(JET Jet[],int count,int which_one){
    if(which_one==PRODUCTION_YEAR){
        for(int i=count/2; i>0; i /=2){
            for(int j=0; j<i; j++){
                for(int k=i+j; k<count; k+=i){
                    int l = Jet[k].production_year;
                    JET temp = Jet[k];
                    int m = k;
                    while(m>i-1 && Jet[m-i].production_year > l){
                        Jet[m]=Jet[m-i];
                        m -= i;
                    }
                    Jet[m]=temp;
                }
            }
        }
    }
    else if(which_one==NUMBER_OF_PRODUCTION){
        for(int i=count/2; i>0; i /=2){
            for(int j=0; j<i; j++){
                for(int k=i+j; k<count; k+=i){
                    int l = Jet[k].number;
                    JET temp = Jet[k];
                    int m = k;
                    while(m>i-1 && Jet[m-i].number > l){
                        Jet[m]=Jet[m-i];
                        m -= i;
                    }
                    Jet[m]=temp;
                }
            }
        }
    }
    else if(which_one==THEN_COMPARING){
        for(int i=count/2; i>0; i /=2){
            for(int j=0; j<i; j++){
                for(int k=i+j; k<count; k+=i){
                    int l = Jet[k].production_year;
                    JET temp = Jet[k];
                    int m = k;
                    while(m>i-1 && Jet[m-i].production_year > l){
                        Jet[m]=Jet[m-i];
                        m -= i;
                    }
                    Jet[m]=temp;
                }
            }
        }
        for(int i=count/2; i>0; i /=2){
            for(int j=0; j<i; j++){
                for(int k=i+j; k<count; k+=i){
                    int l = Jet[k].production_year;
                    int n = Jet[k].number;
                    JET temp = Jet[k];
                    int m = k;
                    while(m>i-1 && Jet[m-i].production_year == l && Jet[m-i].number>n){
                        Jet[m]=Jet[m-i];
                        m -= i;
                    }
                    Jet[m]=temp;
                }
            }
        }
    }
}
void Merge(JET Jet[], int start, int middle, int last, int which_one){
    int i = start, j = middle+1, k = start;
    JET temp[last+1];
    if(which_one==PRODUCTION_YEAR){
        while(i<=middle && j<=last){
            if(Jet[i].production_year <= Jet[j].production_year) temp[k++] = Jet[i++];
            else temp[k++] = Jet[j++];
        }
        while(i<=middle) temp[k++] = Jet[i++];
        while(j<=last) temp[k++] = Jet[j++];
        for(int m = start; m<=last; m++) Jet[m] = temp[m];
    }
    else if(which_one==NUMBER_OF_PRODUCTION){
        while(i<=middle && j<=last){
            if(Jet[i].number <= Jet[j].number) temp[k++] = Jet[i++];
            else temp[k++] = Jet[j++];
        }
        while(i<=middle) temp[k++] = Jet[i++];
        while(j<=last) temp[k++] = Jet[j++];
        for(int m = start; m<=last; m++) Jet[m] = temp[m];
    }
    else if(which_one==THEN_COMPARING){
        while(i<=middle && j<=last){
            if(Jet[i].production_year < Jet[j].production_year) temp[k++] = Jet[i++];
            else if(Jet[i].production_year==Jet[j].production_year && Jet[i].number<=Jet[j].number){
                temp[k++] = Jet[i++];
            }
            else temp[k++] = Jet[j++];
        }
        while(i<=middle) temp[k++] = Jet[i++];
        while(j<=last) temp[k++] = Jet[j++];
        for(int m = start; m<=last; m++) Jet[m] = temp[m];
    }
}


void Merge_Sort(JET Jet[], int start, int last, int which_one){
    int middle;
    if(start<last){
        middle = (start+last)/2;
        Merge_Sort(Jet, start , middle, which_one);
        Merge_Sort(Jet, middle+1, last, which_one);
        Merge(Jet, start, middle, last, which_one);
    }
}


void split(const char* Expression, char* Dest,
const char* Delimiter, unsigned int Index){

    size_t expLen=strlen(Expression);
    size_t delLen=strlen(Delimiter);

    if(expLen==0 || delLen==0){
        memset(Dest,0,1);
        return;
    }

    for(unsigned int i=0,count=0,prev=0;i<expLen;i++){
        if(strncmp(Expression+i,Delimiter,delLen)==0){
            if(count==Index){
                if(count==0) {
                    strncpy(Dest,Expression,i);
                    memset(Dest+i,0,1);
                }else{
                    strncpy(Dest,Expression+prev+delLen,i-prev-delLen);
                    memset(Dest+i-prev-delLen,0,1);
                }
                return;
            }
            prev=i;
            count++;
        }
        if(i==expLen-1 && count==Index){
            if(count==0){
                strcpy(Dest,Expression);
            }else{
                strncpy(Dest,Expression+prev+delLen,expLen-prev-delLen);
                memset(Dest+expLen-prev-delLen,0,1);
            }
            return;
        }
    }
    memset(Dest,0,1);
}
void INI(JET Jet[],JET original[],int count){
    for(int i=0;i<count;i++){
        Jet[i]=original[i];
    }
}

void Print_Jet(JET Jet[],int count){
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    for(int i=0;i<count;i++){
        printf("%-50s %-40s %-10d %-10s %-10d\n",Jet[i].name,Jet[i].nation,Jet[i].production_year,Jet[i].state,Jet[i].number);
    }
    printf("----------------------------------------------------------------------------------------------------------------------\n");
}

int main(void){
    FILE *inp;
    char line[line_max];
    char temp;
    char *t;
    int count=1;
    int i=0;
    inp = fopen("Fighters.csv", "r");

    if (inp == NULL) {
        printf("Cannot open\n");
        return -1;
    }
    while(fscanf(inp,"%c",&temp)!=EOF){
        if(temp=='\n') count++;
    }

    JET *Jet = (JET *)malloc(sizeof(JET)*count);

    rewind(inp);
    while (!feof(inp)) {
        t=fgets(line,line_max,inp);
        split(t,Jet[i].name,",",0);
        split(t,Jet[i].nation,",",1);
        split(t,Jet[i].prod_year,",",2);
        Jet[i].production_year=atoi(Jet[i].prod_year);
        split(t,Jet[i].state,",",3);
        split(t,Jet[i].num,",",4);
        Jet[i].number=atoi(Jet[i].num);
        i++;
    }
    JET original[count];
    for(int i=0;i<count;i++){
        original[i]=Jet[i];
    }

    char keep_going = 'y';
    int choose = 0;
    while(keep_going!='y'||keep_going !='Y'){
        printf("Which Sort do you want?\n");
        printf("----------------------------------------\n");
        printf("1. Bubble Sort by production year\n");
        printf("2. Selection Sort by production year\n");
        printf("3. Shell Sort by production year\n");
        printf("4. Merge Sort by production year\n");
        printf("5. Bubble Sort by production year\n");
        printf("6. Selection Sort by production year\n");
        printf("7. Shell Sort by production year\n");
        printf("8. Merge Sort by production year\n");
        printf("9. Bubble Sort by production year\n");
        printf("10. Selection Sort by production year\n");
        printf("11. Shell Sort by production year\n");
        printf("12. Merge Sort by production year\n");
        printf("----------------------------------------\n");
        scanf("%d",&choose);
        if(choose==1){
            start = clock();
            Bubble_Sort(Jet,count,PRODUCTION_YEAR);
            finish = clock();
            printf("Bubble Sort 생산년도 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==2){
            start = clock();
            Selection_Sort(Jet,count,PRODUCTION_YEAR);
            finish = clock();
            printf("Selection Sort 생산년도 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==3){
            start = clock();
            Shell_Sort(Jet, count, PRODUCTION_YEAR);
            finish = clock();
            printf("Shell Sort 생산년도 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==4){
            start = clock();
            Merge_Sort(Jet,0,count-1,PRODUCTION_YEAR);
            finish = clock();
            printf("Merge Sort 생산년도 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==5){
            start = clock();
            Bubble_Sort(Jet,count,NUMBER_OF_PRODUCTION);
            finish = clock();
            printf("Bubble Sort 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==6){
            start = clock();
            Selection_Sort(Jet,count,NUMBER_OF_PRODUCTION);
            finish = clock();
            printf("Selection Sort 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==7){
            start = clock();
            Shell_Sort(Jet, count, NUMBER_OF_PRODUCTION);
            finish = clock();
            printf("Shell Sort 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==8){
            start = clock();
            Merge_Sort(Jet,0,count-1,NUMBER_OF_PRODUCTION);
            finish = clock();
            printf("Merge Sort 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==9){
            start = clock();
            Bubble_Sort(Jet,count,THEN_COMPARING);
            finish = clock();
            printf("Bubble Sort 생산년도가 같을 경우 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==10){
            start = clock();
            Selection_Sort(Jet,count,THEN_COMPARING);
            finish = clock();
            printf("Selection Sort 생산년도가 같을 경우 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==11){
            start = clock();
            Shell_Sort(Jet, count, THEN_COMPARING);
            finish = clock();
            printf("Shell Sort 생산년도가 같을 경우 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        else if(choose==12){
            start = clock();
            //Merge_Sort(Jet, 0, count-1, PRODUCTION_YEAR);
            Merge_Sort(Jet,0,count-1,THEN_COMPARING);
            finish = clock();
            printf("Merge Sort 생산년도가 같을 경우 생산대수 정렬 시간 : %fms\n",(float)(finish-start));
            Print_Jet(Jet, count);
            INI(Jet,original,count);
        }
        getchar();
        printf("Restart : Y OR y   EXIT : except Y or y anything");
        scanf("%c",&keep_going);
    }
    free(Jet);
    fclose(inp);
}
