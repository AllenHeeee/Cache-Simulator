#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <limits.h>
typedef struct Cache{
    unsigned int valid;
    unsigned long int tag;
    unsigned long int replacement;
    long int replacement2;
}Cache;
int hit=0;
int miss=0;
int readnum=0;
int writenum=0;
long int push=0;
Cache** cache;
unsigned long int getTagIndex(unsigned long int, int);
unsigned long int getTagIndex(unsigned long int address, int move){
    return address>>move;
}
unsigned long int getSetIndex(unsigned long int, int, int);
unsigned long int getSetIndex(unsigned long int address, int offsetbit,int setindexbit){
    return (address>>offsetbit)&((1<<setindexbit)-1);
}
int getExtraCredit(char*);
int getExtraCredit(char* poli){
    if(strcmp("fifo", poli)==0){
        return 0;
    }
    return 1;
}
int getCachesize(char**);
int getCachesize(char** arg){
    return atoi(arg[1]);
}
char* gettracefile(char**);
char* gettracefile(char** arg){
    return arg[5];
}
int getBlocksize(char**);
int getBlocksize(char** arg){
    return atoi(arg[4]);
}
int getSetnumber(char**);
int getSetnumber(char** arg){
    char* associativity=arg[2];
    int c=getCachesize(arg);//cache size
    int b=getBlocksize(arg);//block size
    if(strcmp("direct", associativity)==0){
        return c/b;
    }
    if(strcmp("assoc", associativity)==0){
        return 1;
    }
    char num[1000];
    for(int i=6,j=0;i<strlen(associativity)&&j<1000;i++,j++){
        num[j]=associativity[i];
    }
    return c/b/atoi(num);
    
}
int getassociativity(char**);
int getassociativity(char** arg){
    char* associativity=arg[2];
    int c=getCachesize(arg);//cache size
    int b=getBlocksize(arg);//block size
    if(strcmp("direct", associativity)==0){
        return 1;
    }
    if(strcmp("assoc", associativity)==0){
        return c/b;
    }
    char num[1000];
    for(int i=6,j=0;i<strlen(associativity)&&j<1000;i++,j++){
        num[j]=associativity[i];
    }
    return atoi(num);
//    return 0;
}
Cache** initialize(char**);
Cache** initialize(char** arg){
    int setnumber=getSetnumber(arg);
    int ass=getassociativity(arg);
    cache=(struct Cache **)malloc(sizeof(struct Cache)*setnumber);
    for(int i=0;i<setnumber;i++){
        cache[i]=(struct Cache *)malloc(sizeof(struct Cache)*ass);
        for(int j=0;j<ass;j++){
            cache[i][j].replacement=0;
            cache[i][j].tag=0;
            cache[i][j].valid=0;
            cache[i][j].replacement2=0;
        }
    }
    hit=0;
    push=0;
    miss=0;
    readnum=0;
    writenum=0;
    return cache;
}
void print(){
//    printf("reads: %d  ",readnum);
//    printf("writes: %d  ",writenum);
//    printf("hits: %d  ",hit);
//    printf("misses: %d\n",miss);
}
void prefet(int , unsigned long int ,unsigned long int );
void prefet(int ass, unsigned long int secondsetindex,unsigned long int secondtagindex){
    for(int i=0;i<ass;i++){
        //printf("%lx\t",cache[setindex][i].tag);
        if(cache[secondsetindex][i].valid==1&&cache[secondsetindex][i].tag==secondtagindex){
            //hit++;
//            if(status=='W'){
//                writenum++;
//            }
            break;
        }
        else if(cache[secondsetindex][i].valid==1&&ass==i+1){
            //occupied
          //  miss++;
            readnum++;
//            if(status=='W'){
//                writenum++;
//            }
            long int maxnumber=LONG_MAX;
            for(int j=0;j<ass;j++){
                if(maxnumber>=cache[secondsetindex][j].replacement){
                    maxnumber=cache[secondsetindex][j].replacement;
                }
            }
            for(int j=0;j<ass;j++){
                if(maxnumber==cache[secondsetindex][j].replacement){
                    cache[secondsetindex][j].tag=secondtagindex;
                    cache[secondsetindex][j].replacement=push;
                    push++;
                    break;
                }
            }
            return;
        }else if(cache[secondsetindex][i].valid==0){
            //empty
          //  miss++;
            readnum++;
//            if(status=='W'){
//                writenum++;
//            }
            cache[secondsetindex][i].valid=1;
            cache[secondsetindex][i].tag=secondtagindex;
            cache[secondsetindex][i].replacement=push;
            push++;
            break;
        }
        else
            continue;
    }
}
void prefetchlru(int,unsigned long int,unsigned long int);
void prefetchlru(int ass,unsigned long int secondsetindex,unsigned long int secondtagindex){
    for(int i=0;i<ass;i++){
        //printf("%lx\t",cache[setindex][i].tag);
        if(cache[secondsetindex][i].valid==1&&cache[secondsetindex][i].tag==secondtagindex){
//            hit++;
//            if(status=='W'){
//                writenum++;
//            }
            
//            cache[secondsetindex][i].replacement2=0;
//            for(int j=0;j<ass;j++){
//                if(i==j){
//
//                }else{
//                    cache[secondsetindex][j].replacement2--;
//                }
//            }
            
            break;
        }
        else if(cache[secondsetindex][i].valid==1&&ass==i+1){
            //occupied
            //miss++;
            readnum++;
            
            long int maxnumber=LONG_MAX;
            for(int j=0;j<ass;j++){
                if(maxnumber>=cache[secondsetindex][j].replacement2){
                    maxnumber=cache[secondsetindex][j].replacement2;
                }
            }
            for(int j=0;j<ass;j++){
                if(maxnumber==cache[secondsetindex][j].replacement2){
                    cache[secondsetindex][j].tag=secondtagindex;
                    cache[secondsetindex][j].replacement2=0;
                    for(int k=0;k<ass;k++){
                        if(j==k){
                            
                        }else{
                            cache[secondsetindex][k].replacement2--;
                        }
                    }
                    break;
                }
            }
            return;
        }else if(cache[secondsetindex][i].valid==0){
            //empty
         //   miss++;
            readnum++;
//            if(status=='W'){
//                writenum++;
//            }
            cache[secondsetindex][i].valid=1;
            cache[secondsetindex][i].tag=secondtagindex;
            
            
            cache[secondsetindex][i].replacement2=0;
            for(int j=0;j<ass;j++){
                if(i==j){
                    
                }else{
                    cache[secondsetindex][j].replacement2--;
                }
            }
            
            
            break;
        }
        else
            continue;
    }
}
void accesss(char,int ,int ,int , unsigned long int , unsigned long int ,unsigned long int ,unsigned long int );
void accesss(char status ,int prefetch,int policy,int ass, unsigned long int tagindex, unsigned long int setindex,unsigned long int secondtagindex,unsigned long int secondsetindex){
    if(policy==0){
        if(prefetch==0){
            //no prefetch
            for(int i=0;i<ass;i++){
                //printf("%lx\t",cache[setindex][i].tag);
                if(cache[setindex][i].valid==1&&cache[setindex][i].tag==tagindex){
                    hit++;
                    if(status=='W'){
                        writenum++;
                    }
                    break;
                }
                else if(cache[setindex][i].valid==1&&ass==i+1){
                    //occupied
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    long int maxnumber=LONG_MAX;
                    for(int j=0;j<ass;j++){
                        if(maxnumber>=cache[setindex][j].replacement){
                            maxnumber=cache[setindex][j].replacement;
                        }
                    }
                    for(int j=0;j<ass;j++){
                        if(maxnumber==cache[setindex][j].replacement){
                            cache[setindex][j].tag=tagindex;
                            cache[setindex][j].replacement=push;
                            push++;
                            break;
                        }
                    }
                    return;
                }else if(cache[setindex][i].valid==0){
                    //empty
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    cache[setindex][i].valid=1;
                    cache[setindex][i].tag=tagindex;
                    cache[setindex][i].replacement=push;
                    push++;
                    break;
                }
                else
                    continue;
            }
        }else{
            //prefetch
            for(int i=0;i<ass;i++){
                //printf("%lx\t",cache[setindex][i].tag);
                if(cache[setindex][i].valid==1&&cache[setindex][i].tag==tagindex){
                    hit++;
                    if(status=='W'){
                        writenum++;
                    }
                    break;
                }
                else if(cache[setindex][i].valid==1&&ass==i+1){
                    //occupied
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    long int maxnumber=LONG_MAX;
                    for(int j=0;j<ass;j++){
                        if(maxnumber>=cache[setindex][j].replacement){
                            maxnumber=cache[setindex][j].replacement;
                        }
                    }
                    for(int j=0;j<ass;j++){
                        if(maxnumber==cache[setindex][j].replacement){
                            cache[setindex][j].tag=tagindex;
                            cache[setindex][j].replacement=push;
                            push++;
                            break;
                        }
                    }
                    prefet(ass,secondsetindex,secondtagindex);
                    return;
                }else if(cache[setindex][i].valid==0){
                    //empty
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    cache[setindex][i].valid=1;
                    cache[setindex][i].tag=tagindex;
                    cache[setindex][i].replacement=push;
                    push++;
                     prefet(ass,secondsetindex,secondtagindex);
                    break;
                }
                else
                    continue;
            }
        }
    }
    else if(policy==1){
        if(prefetch==0){
            //no prefetch
            for(int i=0;i<ass;i++){
                //printf("%lx\t",cache[setindex][i].tag);
                if(cache[setindex][i].valid==1&&cache[setindex][i].tag==tagindex){
                    hit++;
                    if(status=='W'){
                        writenum++;
                    }
                    
                    cache[setindex][i].replacement2=0;
                    for(int j=0;j<ass;j++){
                        if(i==j){
                            
                        }else{
                            cache[setindex][j].replacement2--;
                        }
                    }
                    
                    break;
                }
                else if(cache[setindex][i].valid==1&&ass==i+1){
                    //occupied
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    long int maxnumber=LONG_MAX;
                    for(int j=0;j<ass;j++){
                        if(maxnumber>=cache[setindex][j].replacement2){
                            maxnumber=cache[setindex][j].replacement2;
                        }
                    }
                    for(int j=0;j<ass;j++){
                        if(maxnumber==cache[setindex][j].replacement2){
                            cache[setindex][j].tag=tagindex;
                            cache[setindex][j].replacement2=0;
                            for(int k=0;k<ass;k++){
                                if(j==k){
                                    
                                }else{
                                    cache[setindex][k].replacement2--;
                                }
                            }
                            break;
                        }
                    }
                    return;
                }else if(cache[setindex][i].valid==0){
                    //empty
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    cache[setindex][i].valid=1;
                    cache[setindex][i].tag=tagindex;
                    
                    
                    cache[setindex][i].replacement2=0;
                    for(int j=0;j<ass;j++){
                        if(i==j){
                            
                        }else{
                            cache[setindex][j].replacement2--;
                        }
                    }
                    
                    
                    break;
                }
                else
                    continue;
            }
        }else{
            //prefetch
            for(int i=0;i<ass;i++){
                //printf("%lx\t",cache[setindex][i].tag);
                if(cache[setindex][i].valid==1&&cache[setindex][i].tag==tagindex){
                    hit++;
                    if(status=='W'){
                        writenum++;
                    }
                    
                    cache[setindex][i].replacement2=0;
                    for(int j=0;j<ass;j++){
                        if(i==j){
                            
                        }else{
                            cache[setindex][j].replacement2--;
                        }
                    }
                    
                    break;
                }
                else if(cache[setindex][i].valid==1&&ass==i+1){
                    //occupied
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    long int maxnumber=LONG_MAX;
                    for(int j=0;j<ass;j++){
                        if(maxnumber>=cache[setindex][j].replacement2){
                            maxnumber=cache[setindex][j].replacement2;
                        }
                    }
                    for(int j=0;j<ass;j++){
                        if(maxnumber==cache[setindex][j].replacement2){
                            cache[setindex][j].tag=tagindex;
                            cache[setindex][j].replacement2=0;
                            for(int k=0;k<ass;k++){
                                if(j==k){
                                    
                                }else{
                                    cache[setindex][k].replacement2--;
                                }
                            }
                            break;
                        }
                    }
                    prefetchlru(ass,secondsetindex,secondtagindex);
                    return;
                }else if(cache[setindex][i].valid==0){
                    //empty
                    miss++;
                    readnum++;
                    if(status=='W'){
                        writenum++;
                    }
                    cache[setindex][i].valid=1;
                    cache[setindex][i].tag=tagindex;
                    
                    
                    cache[setindex][i].replacement2=0;
                    for(int j=0;j<ass;j++){
                        if(i==j){
                            
                        }else{
                            cache[setindex][j].replacement2--;
                        }
                    }
                    
                    prefetchlru(ass,secondsetindex,secondtagindex);
                    break;
                }
                else
                    continue;
            }
        }
    }
}



int main(int argc, char** argv){
    if(argc!=6)return 0;
    //int cachesizeC=getCachesize(argv);// cache size
    cache=initialize(argv);// initialize the new cache
//    cache=create(getSetnumber(argv), getassociativity(argv));
    int ass=getassociativity(argv);// associativity
    int block=getBlocksize(argv);// block size
    int set=getSetnumber(argv);//set number
    
    char* poli=argv[3];
    int ExtraCredit=getExtraCredit(poli);
    char status;
    unsigned long int address;
    unsigned long int firstco;
    FILE* fl=fopen(gettracefile(argv), "r");
    int val=fscanf(fl, "%lx: %c %lx",&firstco,&status,&address);
    int switcher=0;
    //non-prefetch
    while(val==3){
//        if(readnum==2867&&writenum==2847&&hit==7068){
//            readnum+=0;
//        }
        int offsetbit=log2(block);
        int setindexbit=log2(set);
        int sum=offsetbit+setindexbit;
        unsigned long int tagindex=getTagIndex(address,sum);
        unsigned long int setindex=getSetIndex(address, offsetbit, setindexbit);
        unsigned long int secondaddress=address+block;
        unsigned long int secondtagindex=getTagIndex(secondaddress,sum);
        unsigned long int secondsetindex=getSetIndex(secondaddress, offsetbit, setindexbit);
        accesss(status,switcher,ExtraCredit, ass, tagindex, setindex, secondtagindex, secondsetindex);
        if(switcher==1)
        print();
       val = fscanf(fl, "%lx: %c %lx",&firstco,&status,&address);
        if(val!=3){
            switcher++;
            if(switcher==1){
                //non-prefetch
                printf("no-prefetch\n");
                printf("Memory reads: %d\n",readnum);
                printf("Memory writes: %d\n",writenum);
                printf("Cache hits: %d\n",hit);
                printf("Cache misses: %d\n",miss);
            }else if(switcher==2){
                //prefetch
                printf("with-prefetch\n");
                printf("Memory reads: %d\n",readnum);
                printf("Memory writes: %d\n",writenum);
                printf("Cache hits: %d\n",hit);
                printf("Cache misses: %d\n",miss);
                
                break;
            }else{
                break;
            }
            fl=fopen(gettracefile(argv), "r");
            val=fscanf(fl, "%lx: %c %lx",&firstco,&status,&address);
            cache=initialize(argv);// initialize the new cache
            //cache=create(getSetnumber(argv), getassociativity(argv));
        }
    }
    
    
    
    
    return 0;
}
