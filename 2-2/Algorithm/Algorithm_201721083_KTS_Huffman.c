//파일 경로설정 : Xcode의 Product->Scheme->Edit Scheme->Option->Working Directory 소스파일의 위치
//인코딩 혹은 디코딩 할 파일 설정 :Xcode의 Product->Scheme->Edit Scheme->Arguments
//인코딩 : -encoding _____.txt
//디코딩 : -decoding _____.bin



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SZ 100
#define NUM_ASCII 256

struct node{
    char c;
    int frequency;
    struct node *left;
    struct node *right;
};

struct node **heap;
int lastHeapIdx =0;
char codeBuf[100];
int codeBufIdx = -1;

int charFreq[NUM_ASCII];
char *symCode[NUM_ASCII];

void Encoding (char *fName);
void Decoding (char *fName);
void showCharFrequency(void);
int countNonZeroCharacters(void);
void addToHeap(struct node *cur);
struct node* deleteFromHeap(void);
void traverse(struct node *cur,char c);
void just_traverse(struct node *cur,char c);

int main(int argc, char* argv[]){
    if(strcmp(argv[1],"-encoding")==0){
        //huffman coing
        printf("Start encoding\n");
        Encoding(argv[2]);
    }
    else if (strcmp(argv[1],"-decoding")==0){
        //decoding
        printf("Start decoding\n");
        Decoding(argv[2]);
    }
    return 0;
}
//인코더 함수
void Encoding(char *fName){
    FILE *fin=0;
    char buf[BUF_SZ];
    
    fin = fopen(fName,"rt");
    if(fin==0){
        printf("file is not exist\n");
        return;
    }
    
    memset(charFreq, 0, NUM_ASCII*sizeof(int));
    while(fgets(buf, BUF_SZ, fin)!=0){
        int len = strlen(buf);
        for(int i=0;i<len;i++){
            charFreq[(int)buf[i]]++;
        }
    }
    
    fclose(fin);
 
    int cnt = countNonZeroCharacters();
    heap = (struct node **)malloc((cnt+1)*sizeof(struct node *));
    memset(heap,0,(cnt+1)*sizeof(struct node *));
    
    for (int i=0;i<NUM_ASCII;i++){
        if(charFreq[i]>0){
            struct node *cur = (struct node *)malloc(sizeof(struct node));
            cur->c = (char)i;
            cur->frequency = charFreq[i];
            cur->left = cur->right =0;
            addToHeap(cur);
        }
    }
    
    struct node *first =0;
    struct node *second = 0;
    
    while (1){
        first = deleteFromHeap();
        second = deleteFromHeap();
        
        if(second==0){
            printf("Huffman tree building ended\n");
            break;
        }
        
        struct node *newOne =(struct node *)malloc(sizeof(struct node));
        newOne->c = 0;
        newOne->frequency = first->frequency + second->frequency;
        newOne->left = first;
        newOne->right = second;
        
        addToHeap(newOne);
    }
    
    memset(symCode, 0, sizeof(symCode));
    
    traverse(first->left,'0');
    traverse(first->right,'1');
    int numOfSym = 0;
    
    for(int i=0;i<NUM_ASCII;i++){
        if(symCode[i]!=0){
            numOfSym++;
        }
    }
    printf("Number of character is %d\n",numOfSym);
    
    char outputFileName[100];
    char *period = strchr(fName,(int)'.');
    strncpy(outputFileName, fName,(int)(period-fName));
    outputFileName[(int)(period-fName)]=0;
    strcat(outputFileName,".bin");
    
    printf("Output file name is %s\n",outputFileName);
    
    FILE *fout =0;
    fout=fopen(outputFileName,"wb");
    if(fout !=0){
        fwrite(&numOfSym, sizeof(numOfSym),1,fout);
        
        char writeBuf[100];
        for(int i=0; i<NUM_ASCII;i++){
            if(symCode[i]!=0){
    
                writeBuf[0]=(char)i;
                writeBuf[1]=(char)strlen(symCode[i]);
                strcpy(&writeBuf[2],symCode[i]);
                fwrite(writeBuf, sizeof(char),2+strlen(symCode[i]),fout);
            }
        }
     
        FILE *fin;
        fin=fopen(fName,"rt");
        if(fin!=0){
            
            int locTotalNumBit;
            locTotalNumBit=(int)ftell(fout);
            
            if(fseek(fout, 4, SEEK_CUR)!=0){
                printf("Fail to move the file pointer\n");
                fclose(fin);
                fclose(fout);
                return;
            }
            
            char bitBuf[BUF_SZ];
            int bitBufIdx = 0;
            int bitCount=7;
            int totalBitNum =0;
            char flag =0;
            
       
            memset(bitBuf,0,BUF_SZ);
            

            while(fgets(buf,BUF_SZ,fin)!=0){
                int len=strlen(buf);
                for(int i=0;i<len; i++){
                    char *huffmanCode = symCode[(int)buf[i]];
                    int f=strlen(huffmanCode);
                    for(int j=0;j<f;j++){
                        char val = 0;
                        if(huffmanCode[j]=='0'){
                            val = 0;
                        }
                        else if (huffmanCode[j]=='1'){
                            val = 1;
                        }
                        else{
                            printf("ERROR!!!!!!!!!!!");
                            exit(0);
                        }
                        
                        val=val<<bitCount;
                        bitCount--;
                        
                        bitBuf[bitBufIdx] |= val;
                        flag = 1;
                        totalBitNum++;
                        if(bitCount < 0){
                            bitCount = 7;
                            bitBufIdx++;
                            if(bitBufIdx>=BUF_SZ){
                                fwrite(bitBuf,1,BUF_SZ, fout);
                                bitBufIdx=0;
                                memset(bitBuf,0,BUF_SZ);
                            }
                        }
                    }
                }
            }
            if(bitBufIdx>0){
                fwrite(bitBuf,1,bitBufIdx+1,fout);
            }
            
            if(fseek(fout,locTotalNumBit,SEEK_SET)==0){
                fwrite(&totalBitNum,sizeof(totalBitNum),1,fout);
            }
            else{
                printf("Error! unable to record total numb of bits\n");
            }
            fclose(fin);
        }
        else{
            printf("Error!! Unable to open this file");
        }
        fclose(fout);
    }
    else{
        printf("Error! Unable to open this file");
    }
}

void just_traverse(struct node *cur,char c){
    codeBufIdx++;
    codeBuf[codeBufIdx]=c;
    codeBuf[codeBufIdx+1]=0;
    
    if(cur->left == 0 && cur->right ==0){
        printf("character %d (%c) : %s\n",(char)cur->c,cur->c, codeBuf);
    }
    else{
        just_traverse(cur->left,'0');
        just_traverse(cur->right,'1');
    }
    codeBuf[codeBufIdx]=0;
    codeBufIdx--;
    return;
}

void traverse(struct node *cur,char c){
    codeBufIdx++;
    codeBuf[codeBufIdx]=c;
    codeBuf[codeBufIdx+1]=0;
    
    if(cur->left == 0 && cur->right ==0){
        printf("character %d (%c) : %s\n",(char)cur->c,cur->c, codeBuf);
        
        char *huffCode = (char *)malloc(strlen(codeBuf)+1);
        strcpy(huffCode,codeBuf);
        symCode[(int)cur->c] = huffCode;
    }
    else{
        traverse(cur->left,'0');
        traverse(cur->right,'1');
    }
    codeBuf[codeBufIdx]=0;
    codeBufIdx--;
    return;
}

struct node* deleteFromHeap(void){
    if(lastHeapIdx<=0){
        return 0;
    }
    struct node *retVal = heap[1];
    
    heap[1]=heap[lastHeapIdx];
    lastHeapIdx--;
    
    int parent = 1;
    int left = 2*parent;
    int right = left+1;
    
    while(1){
        if(left>lastHeapIdx){
            break;
        }
        else if(right>lastHeapIdx){
            if(heap[left]->frequency < heap[parent]->frequency){
                struct node *temp = heap[left];
                heap[left]=heap[parent];
                heap[parent]=temp;
                
                parent = left;
                left =2*parent;
                right=left+1;
            }
            else break;
        }
        else{
            int smaller;
            if(heap[left]->frequency <= heap[parent]->frequency){
                smaller = left;
            }
            else{
                smaller = right;
            }
            if(heap[smaller]->frequency < heap[parent]->frequency){
                struct node *temp = heap[smaller];
                heap[smaller]=heap[parent];
                heap[parent]=temp;
                
                parent = smaller;
                left =2*parent;
                right=left+1;
            }
            else break;
        }
    }
    return retVal;
}

void addToHeap(struct node* cur){
    lastHeapIdx++;
    heap[lastHeapIdx] = cur;
    int currentIdx = lastHeapIdx;
    int parentIdx = currentIdx/2;
    
    
    while(parentIdx >=1){
        if(heap[parentIdx]->frequency > heap[currentIdx]->frequency){
            struct node *temp = heap[parentIdx];
            heap[parentIdx]=heap[currentIdx];
            heap[currentIdx]=temp;
            
            currentIdx = parentIdx;
            parentIdx = currentIdx/2;
        }
        else break;
    }
}

int countNonZeroCharacters(){
    int cnt =0;
    for(int i=0; i<NUM_ASCII;i++){
        if(charFreq[i]>0){
            cnt++;
        }
    }
    return cnt;
}

void showCharFrequency(void){
    for(int i=0;i<NUM_ASCII;i++){
        if(charFreq[i]>0){
            printf("ASCII code %d ( %c) : %d \n",i,(char)i,charFreq[i]);
        }
    }
}



void Decoding(char *fName){
    FILE *fin =0;
    fin = fopen(fName,"rb");
    
    if(fin!=0){
        //허프만트리 재구성
        int numOfSym = 0;
        fread(&numOfSym,sizeof(int),1,fin);
        
        printf("Number of Sybols is %d\n",numOfSym);
        
        struct node *huffRoot = (struct node *)malloc(sizeof(struct node));
        huffRoot->left = huffRoot->right=0;
        struct node *cur=huffRoot;
        
        for(int i=0;i<NUM_ASCII;i++){
            char symbolAndLen[2];
            fread(symbolAndLen,2,1,fin);
            char buf[100];
            fread(buf,1,(int)symbolAndLen[1],fin);
            buf[(int)symbolAndLen[1]]=0;
            printf("%c (%d) ==> %s\n",symbolAndLen[0],(int)symbolAndLen[1],buf);
            
            cur=huffRoot;
            for(int i=0;i<(int)symbolAndLen[1];i++){
                if(buf[i]=='0'){
                    if(cur->left==0){
                        cur->left = (struct node *)malloc(sizeof(struct node));
                        cur->left->left=0;
                        cur->left->right=0;
                    }
                    cur=cur->left;
                }
                else if(buf[i]=='1'){
                    if(cur->right==0){
                        cur->right = (struct node *)malloc(sizeof(struct node));
                        cur->right->left=0;
                        cur->right->right=0;
                    }
                    cur=cur->right;
                }
                else{
                    printf("ERROR!!!!!!!!!!!\n");
                    exit(0);
                }
            }
            cur->c=symbolAndLen[0];
            
        }
        codeBufIdx=-1;
        just_traverse(huffRoot->left,'0');
        just_traverse(huffRoot->right,'1');
        //디코딩 수행
        FILE *decodedFile;
        
        char decodedFName[100];
        char *period =strchr(fName, (int)'.');
        strncpy(decodedFName,fName,(int)(period-fName));
        decodedFName[(int)(period-fName)] = 0;
        strcat(decodedFName,"_decoded.txt");
        printf("Decoded file name is %s\n",decodedFName);
        decodedFile = fopen(decodedFName,"wt");
        if(decodedFile==0){
            printf("Unable to create file");
            exit(0);
        }
        
        
        int numBitsToRead=0;
        fread(&numBitsToRead,sizeof(int),1,fin);
        printf("Total number of bits to read is %d\n",numBitsToRead);
        cur=huffRoot;
        char buf[BUF_SZ];
        while(1){
            int sz=fread(buf,1,BUF_SZ,fin);
            if(sz==0){
                printf("End of file\n");
                break;
            }
            else{
                for(int i=0;i<sz;i++){
                    for(int j=0;j<8;j++){
                        if((char)(buf[i]& 0x80)==0){
                            cur=cur->left;
                        }
                        else{
                            cur=cur->right;
                        }
                        buf[i]=buf[i]<<1;
                        numBitsToRead--;
                        
                        if(cur->left==0&&cur->right==0){
                            //printf("%c",cur->c);
                            fputc(cur->c,decodedFile);
                            cur = huffRoot;
                        }
                        if(numBitsToRead==0){
                            printf("End of decoding\n");
                            fclose(decodedFile);
                            fclose(fin);
                            return;
                        }
                    }
                }
            }
        }
        
        
        fclose(fin);
    }
    else{
        printf("Error! Unable to open this file");
    }
}
