# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <string.h>
# include <iomanip>
# include <vector>
# include <cmath>

using namespace std;

FILE *fp, *fout;

char name[50];
char outputname[50];
int casenum = 0 ;
int timeslice = 0;

class Data {
  public:
    int id = 0 ;
    int cpuburst = 0 ;
    int remaincpuburst = 0 ;
    int arrival = 0 ;
    int newarrival = 0 ;
    int priority = 0 ;
    int waitingtime = 0 ;
    int turnaroundtime = 0 ;
    int finishtime = 0 ;
    bool done = false ;
    bool usecpu = false;
};

class Result {
  public:
    int id = 0 ;
    int FCFS = 0 ;
    int RR = 0 ;
    int PSJF = 0 ;
    int NSJF = 0 ;
    int PP = 0 ;

};

class ganttchar{
  public:
    vector<int> FCFS;
    vector<int> RR;
    vector<int> PSJF;
    vector<int> NSJF;
    vector<int> PP;
};

vector<Data> Inputdata;
vector<Result> result_waitingtime, result_turnaroundtime;
vector<int> Gantt ;
vector<ganttchar> Ganttchar;


int Input(){
  int num, eof;
  string inputname, filename, outputfile;
  
  cout << "Enter filename ....Enter 0 to exit" << endl;
  cin >> inputname;
  if(inputname == "0")
    return 1;
    
  filename = inputname + ".txt";
  outputfile = inputname + "_output.txt";
  
  strcpy(name, filename.c_str());
  strcpy(outputname, outputfile.c_str());
  
  fp = fopen(name, "rt");
  
  while( !fp ){
    cout << "*** " << name << " dose not exist! ***" << endl;
    cout << "please enter filename" << endl;
    cin >> inputname;
    
    filename = inputname + ".txt";
    outputfile = inputname + "_output.txt";
    strcpy(name, filename.c_str());
    strcpy(outputname, outputfile.c_str());
  
    fp = fopen(name, "rt");
  }
  
  fout = fopen(outputname,"w+t");
  eof = fscanf(fp, "%d", &casenum);
  eof = fscanf(fp, "%d", &timeslice);
  
  char garbage;
  fscanf(fp,"%c",&garbage );
  fscanf(fp,"%c",&garbage );
	if ( garbage != 0 ){
		while( garbage != '\n') { //把第二行的字讀掉 
			fscanf(fp,"%c",&garbage );		
		}
	}
  
  int id, cpuburst, arrival, priority;
  while( eof != -1 ){
    eof = fscanf(fp, "%d", &id);
    if( eof == -1 )
      break;
      
    fscanf(fp, "%d", &cpuburst);
    fscanf(fp, "%d", &arrival);
    fscanf(fp, "%d", &priority);
    
    Data temp;
    
    temp.id = id;
    temp.cpuburst = cpuburst;
    temp.arrival = arrival;
    temp.priority = priority;
    
    Inputdata.push_back(temp);
  }
  
  return 0 ;
}

bool alldone( vector<Data> temp ){
  for( int i = 0 ; i < temp.size() ; i++ )
    if( temp[i].done == false )
      return false ;
      
  return true;
}

void FCFS(){
  int cputime = 0 ;
  vector<Data> temp, cpu, queue;
  temp = Inputdata ;
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].arrival > temp[j].arrival )
        swap( temp[i], temp[j] );
      else if ( temp[i].arrival == temp[j].arrival ){
        if( temp[i].id > temp[j].id )
          swap( temp[i], temp[j] );
      }
    }
  }

  int arrivalcpu = 0 ;
  while( !alldone(temp) ){
    
    for( int i = 0 ; i < temp.size() ; i++ ){
      if( cputime == temp[i].arrival ) // process arrival time = cpu time 到 queue中排隊 
        queue.push_back( temp[i] );
    }
    
    if( cpu.size() == 1 ){
      Gantt.push_back(cpu[0].id);
      if( cputime - arrivalcpu == cpu[0].cpuburst ){ //做完了 
        for( int i = 0 ; i < temp.size() ; i++ ){
          if( temp[i].id == cpu[0].id ){
            temp[i].done = true;
            temp[i].finishtime = cputime;
            temp[i].turnaroundtime = temp[i].finishtime - temp[i].arrival;
            temp[i].waitingtime = temp[i].turnaroundtime - temp[i].cpuburst ;
          }
        }
        cpu.erase(cpu.begin());
      }
    } 
    
    if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
      if( queue.size() == 0 ) { // 沒有process在queue 
        Gantt.push_back(-1) ;
      }
      else{ // 將queue第一個放入cpu 
        cpu.push_back( queue[0] );
        queue.erase(queue.begin());
        arrivalcpu = cputime;
      }
      
    }
    cputime++; 
  } 
  
  Gantt.pop_back();
  Ganttchar[0].FCFS = Gantt;
  Gantt.clear(); 

  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].id > temp[j].id )
        swap( temp[i], temp[j] );
    }
  }
  
  Result resulttemp_wait, resulttemp_turn ;
  if( result_waitingtime.size() == 0 && result_turnaroundtime.size() == 0){
    for( int i = 0 ; i < temp.size() ; i++ ) {
      resulttemp_wait.id = temp[i].id;
      resulttemp_wait.FCFS = temp[i].waitingtime;  
      result_waitingtime.push_back(resulttemp_wait);
      
      resulttemp_turn.id = temp[i].id;
      resulttemp_turn.FCFS = temp[i].turnaroundtime; 
      result_turnaroundtime.push_back(resulttemp_turn);
    } 
  }
  else{
    for( int i = 0 ; i < result_waitingtime.size() ; i++ ) {
      result_waitingtime[i].id = temp[i].id;
      result_waitingtime[i].FCFS = temp[i].waitingtime;
    }
    for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ) {
      result_turnaroundtime[i].id = temp[i].id;
      result_turnaroundtime[i].FCFS = temp[i].turnaroundtime;
    }  
  }
}

void NSJF(){
  int cputime = 0 ;
  vector<Data> temp, cpu, queue;
  temp = Inputdata ;
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].arrival > temp[j].arrival )
        swap( temp[i], temp[j] );
      else if ( temp[i].arrival == temp[j].arrival ){
        if( temp[i].id > temp[j].id )
          swap( temp[i], temp[j] );
      }
    }
  }

  int arrivalcpu = 0 ;
  while( !alldone(temp) ){
    
    for( int i = 0 ; i < temp.size() ; i++ ){
      if( cputime == temp[i].arrival ) // process arrival time = cpu time 到 queue中排隊 
        queue.push_back( temp[i] );
    }
    
    for( int i = 0 ; i < queue.size() ; i++ ){ // 對在queue裡的process進行排序 
      for( int j = i+1 ; j < queue.size() ; j++ ){
        if( queue[i].cpuburst > queue[j].cpuburst )
          swap( queue[i], queue[j] );
        else if ( queue[i].cpuburst == queue[j].cpuburst ){
          if( queue[i].arrival > queue[j].arrival )
            swap( queue[i], queue[j] );
          else if( queue[i].arrival == queue[j].arrival ){
            if( queue[i].id > queue[j].id )
              swap( queue[i], queue[j] );
          }
        }
      }
    }
    
    
    if( cpu.size() == 1 ){
      Gantt.push_back(cpu[0].id);
      if( cputime - arrivalcpu == cpu[0].cpuburst ){ //做完了 
        for( int i = 0 ; i < temp.size() ; i++ ){
          if( temp[i].id == cpu[0].id ){
            temp[i].done = true;
            temp[i].finishtime = cputime;
            temp[i].turnaroundtime = temp[i].finishtime - temp[i].arrival;
            temp[i].waitingtime = temp[i].turnaroundtime - temp[i].cpuburst ;
          }
        }
        cpu.erase(cpu.begin());
      }
    } 
    
    if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
      if( queue.size() == 0 ) { // 沒有process在queue 
        Gantt.push_back(-1) ;
      }
      else{ // 將queue第一個放入cpu 
        cpu.push_back( queue[0] );
        queue.erase(queue.begin());
        arrivalcpu = cputime;
      }
      
    }

    
    cputime++; 
  } 
  
  
  Gantt.pop_back();
  Ganttchar[0].NSJF = Gantt;
  Gantt.clear();
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].id > temp[j].id )
        swap( temp[i], temp[j] );
    }
  }
  
  Result resulttemp_wait, resulttemp_turn ;
  
  if( result_waitingtime.size() == 0 && result_turnaroundtime.size() == 0 ){
    for( int i = 0 ; i < temp.size() ; i++ ) {
      resulttemp_wait.id = temp[i].id;
      resulttemp_wait.NSJF = temp[i].waitingtime; 
      result_waitingtime.push_back(resulttemp_wait);
      
      resulttemp_turn.id = temp[i].id;
      resulttemp_turn.NSJF = temp[i].turnaroundtime;
      result_turnaroundtime.push_back(resulttemp_turn);
    } 
  }
  else{
    for( int i = 0 ; i < result_waitingtime.size() ; i++ ) {
      result_waitingtime[i].id = temp[i].id;
      result_waitingtime[i].NSJF = temp[i].waitingtime;
    }
    for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ) {
      result_turnaroundtime[i].id = temp[i].id;
      result_turnaroundtime[i].NSJF = temp[i].turnaroundtime;
    }  
  }
  
}

void PSJF(){

  int cputime = 0 ;
  vector<Data> temp, cpu, queue;
  temp = Inputdata ;
  
  for( int i = 0 ; i < temp.size() ; i ++ ){
    temp[i].remaincpuburst = temp[i].cpuburst;
  }
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].arrival > temp[j].arrival )
        swap( temp[i], temp[j] );
      else if ( temp[i].arrival == temp[j].arrival ){
        if( temp[i].id > temp[j].id )
          swap( temp[i], temp[j] );
      }
    }
  }
  
 
    
    
  int arrivalcpu = 0 ;
  while( !alldone(temp) ){
    
    for( int i = 0 ; i < temp.size() ; i++ ){
      if( cputime == temp[i].arrival ) // process arrival time = cpu time 到 queue中排隊 
        queue.push_back( temp[i] );
    }
    
    for( int i = 0 ; i < queue.size() ; i++ ){ // 對在queue裡的process進行排序 
      for( int j = i+1 ; j < queue.size() ; j++ ){
        if( queue[i].remaincpuburst > queue[j].remaincpuburst ) // 若剩餘的cpu burst比較小 就交換 
          swap( queue[i], queue[j] );
        else if ( queue[i].remaincpuburst == queue[j].remaincpuburst ){ // 若剩餘的cpu burst一樣 
          if( queue[i].usecpu == true && queue[j].usecpu == false ){ // 給沒有使用過cpu的先執行 
            swap( queue[i], queue[j] );
          }
          else if( queue[i].usecpu == true && queue[j].usecpu == true ){ // 若無法分別 則依照arrival time小的先執行 
            if( queue[i].arrival > queue[j].arrival )
              swap( queue[i], queue[j] );
            else if( queue[i].arrival == queue[j].arrival ){ // 若arrival time一樣 依照id小的先執行 
              if( queue[i].id > queue[j].id )
                swap( queue[i], queue[j] );
            }
          }
          else if( queue[i].usecpu == false && queue[j].usecpu == false ){
            if( queue[i].arrival > queue[j].arrival )
              swap( queue[i], queue[j] );
            else if( queue[i].arrival == queue[j].arrival ){
              if( queue[i].id > queue[j].id )
                swap( queue[i], queue[j] );
            }
          }
          else{
            if( queue[i].arrival > queue[j].arrival )
              swap( queue[i], queue[j] );
            else if( queue[i].arrival == queue[j].arrival ){
              if( queue[i].id > queue[j].id )
                swap( queue[i], queue[j] );
            }
          }
        }
      }
    }
 
    if( cpu.size() == 1 ){
      Gantt.push_back(cpu[0].id);
      cpu[0].usecpu = true ;
      cpu[0].remaincpuburst = cpu[0].remaincpuburst-1 ;

      if( cpu[0].remaincpuburst > queue[0].remaincpuburst ){ // 
        queue.push_back(cpu[0]);
        cpu[0] = queue[0] ;
        queue.erase(queue.begin());
        arrivalcpu = cputime;
      }

      if( cpu[0].remaincpuburst == 0 ){ //做完了 
        for( int i = 0 ; i < temp.size() ; i++ ){
          if( temp[i].id == cpu[0].id ){
            temp[i].done = true;
            temp[i].finishtime = cputime;
            temp[i].turnaroundtime = temp[i].finishtime - temp[i].arrival;
            temp[i].waitingtime = temp[i].turnaroundtime - temp[i].cpuburst ;
          }
        }
        cpu.erase(cpu.begin());
      }
      
      for( int i = 0 ; i < queue.size() ; i++ ){ // 對在queue裡的process進行排序 
        for( int j = i+1 ; j < queue.size() ; j++ ){
          if( queue[i].remaincpuburst > queue[j].remaincpuburst ) // 若剩餘的cpu burst比較小 就交換 
            swap( queue[i], queue[j] );
          else if ( queue[i].remaincpuburst == queue[j].remaincpuburst ){ // 若剩餘的cpu burst一樣 
            if( queue[i].usecpu == true && queue[j].usecpu == false ){ // 給沒有使用過cpu的先執行 
              swap( queue[i], queue[j] );
            }
            else if( queue[i].usecpu == true && queue[j].usecpu == true ){ // 若無法分別 則依照arrival time小的先執行 
              if( queue[i].arrival > queue[j].arrival )
                swap( queue[i], queue[j] );
              else if( queue[i].arrival == queue[j].arrival ){ // 若arrival time一樣 依照id小的先執行 
                if( queue[i].id > queue[j].id )
                  swap( queue[i], queue[j] );
              }
            }
            else if( queue[i].usecpu == false && queue[j].usecpu == false ){
              if( queue[i].arrival > queue[j].arrival )
                swap( queue[i], queue[j] );
              else if( queue[i].arrival == queue[j].arrival ){
                if( queue[i].id > queue[j].id )
                  swap( queue[i], queue[j] );
              }
            }
            else{
              if( queue[i].arrival > queue[j].arrival )
                swap( queue[i], queue[j] );
              else if( queue[i].arrival == queue[j].arrival ){
                if( queue[i].id > queue[j].id )
                  swap( queue[i], queue[j] );
              }
            }  
          }
        }
      }
    } 
    
    if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
      if( queue.size() == 0 ) { // 沒有process在queue 
        Gantt.push_back(-1) ;
      }
      else{ // 將queue第一個放入cpu 
        cpu.push_back( queue[0] );
        queue.erase(queue.begin());
        arrivalcpu = cputime;
      }
    }
    cputime++; 
  } 
 
  Gantt.pop_back();
  Ganttchar[0].PSJF = Gantt;
  Gantt.clear();
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].id > temp[j].id )
        swap( temp[i], temp[j] );
    }
  }
  
  Result resulttemp_wait, resulttemp_turn ;
  
  if( result_waitingtime.size() == 0 && result_turnaroundtime.size() == 0 ){
    for( int i = 0 ; i < temp.size() ; i++ ) {
      resulttemp_wait.id = temp[i].id;
      resulttemp_wait.PSJF = temp[i].waitingtime;  
      result_waitingtime.push_back(resulttemp_wait);
      
      resulttemp_turn.id = temp[i].id;
      resulttemp_turn.PSJF = temp[i].turnaroundtime; 
      result_turnaroundtime.push_back(resulttemp_turn);
    } 
  }
  else{
    for( int i = 0 ; i < result_waitingtime.size() ; i++ ) {
      result_waitingtime[i].id = temp[i].id;
      result_waitingtime[i].PSJF = temp[i].waitingtime;
    }
    for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ) {
      result_turnaroundtime[i].id = temp[i].id;
      result_turnaroundtime[i].PSJF = temp[i].turnaroundtime;
    }  
  }
  
}

void PP(){

  int cputime = 0 ;
  vector<Data> temp, cpu, queue;
  temp = Inputdata ;
  
  for( int i = 0 ; i < temp.size() ; i ++ ){
    temp[i].remaincpuburst = temp[i].cpuburst;
  }
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].arrival > temp[j].arrival )
        swap( temp[i], temp[j] );
      else if ( temp[i].arrival == temp[j].arrival ){
        if( temp[i].id > temp[j].id )
          swap( temp[i], temp[j] );
      }
    }
  }
  
  
    
  int arrivalcpu = 0 ;
  while( !alldone(temp) ){

    for( int i = 0 ; i < temp.size() ; i++ ){
      if( cputime == temp[i].arrival ) // process arrival time = cpu time 到 queue中排隊 
        queue.push_back( temp[i] );
    }
    
    for( int i = 0 ; i < queue.size() ; i++ ){ // 對在queue裡的process進行排序 
      for( int j = i+1 ; j < queue.size() ; j++ ){
        if( queue[i].priority > queue[j].priority ) // 若prioirty比較大 就交換 
          swap( queue[i], queue[j] );
        else if ( queue[i].priority == queue[j].priority ){ // 若piority一樣 
          if( queue[i].usecpu == true && queue[j].usecpu == false ){ // 給沒有使用過cpu的先執行 
            swap( queue[i], queue[j] );
          }
          else if( queue[i].usecpu == true && queue[j].usecpu == true ){ // 若無法分別 則依照arrival time小的先執行 
            if( queue[i].arrival > queue[j].arrival )
              swap( queue[i], queue[j] );
            else if( queue[i].arrival == queue[j].arrival ){ // 若arrival time一樣 依照id小的先執行 
              if( queue[i].id > queue[j].id )
                swap( queue[i], queue[j] );
            }
          }
          else if( queue[i].usecpu == false && queue[j].usecpu == false ){
            if( queue[i].arrival > queue[j].arrival )
              swap( queue[i], queue[j] );
            else if( queue[i].arrival == queue[j].arrival ){
              if( queue[i].id > queue[j].id )
                swap( queue[i], queue[j] );
            }
          }
          else{
            if( queue[i].arrival > queue[j].arrival )
              swap( queue[i], queue[j] );
            else if( queue[i].arrival == queue[j].arrival ){
              if( queue[i].id > queue[j].id )
                swap( queue[i], queue[j] );
            }
          }
        }
      }
    }
 
    if( cpu.size() == 1 ){
    
      Gantt.push_back(cpu[0].id);
      cpu[0].usecpu = true ;
      cpu[0].remaincpuburst = cpu[0].remaincpuburst-1 ;
      
      if( cpu[0].remaincpuburst == 0 ){ //做完了 
        for( int i = 0 ; i < temp.size() ; i++ ){
          if( temp[i].id == cpu[0].id ){
            temp[i].done = true;
            temp[i].finishtime = cputime;
            temp[i].turnaroundtime = temp[i].finishtime - temp[i].arrival;
            temp[i].waitingtime = temp[i].turnaroundtime - temp[i].cpuburst ;
          }
        }
        cpu.erase(cpu.begin());
      }
      
      for( int i = 0 ; i < queue.size() ; i++ ){ // 對在queue裡的process進行排序 
        for( int j = i+1 ; j < queue.size() ; j++ ){
          if( queue[i].priority > queue[j].priority ) // 若剩餘的cpu burst比較小 就交換 
            swap( queue[i], queue[j] );
          else if ( queue[i].priority == queue[j].priority ){ // 若piority一樣 
            if( queue[i].usecpu == true && queue[j].usecpu == false ){ // 給沒有使用過cpu的先執行 
              swap( queue[i], queue[j] );
            }
            else if( queue[i].usecpu == true && queue[j].usecpu == true ){ // 若無法分別 則依照arrival time小的先執行 
              if( queue[i].arrival > queue[j].arrival )
                swap( queue[i], queue[j] );
              else if( queue[i].arrival == queue[j].arrival ){ // 若arrival time一樣 依照id小的先執行 
                if( queue[i].id > queue[j].id )
                  swap( queue[i], queue[j] );
              }
            }
            else if( queue[i].usecpu == false && queue[j].usecpu == false ){
              
              if( queue[i].arrival > queue[j].arrival ){
                swap( queue[i], queue[j] );
              } 
              else if( queue[i].arrival == queue[j].arrival ){
                if( queue[i].id > queue[j].id )
                  swap( queue[i], queue[j] );
              }
            }
            else{
              if( queue[i].arrival > queue[j].arrival )
                swap( queue[i], queue[j] );
              else if( queue[i].arrival == queue[j].arrival ){
                if( queue[i].id > queue[j].id )
                  swap( queue[i], queue[j] );
              }
            }  
          }
        }
      }    
    
      if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
        
        if( queue.size() == 0 ) { // 沒有process在queue 
          Gantt.push_back(-1) ;
        }
        else{ // 將queue第一個放入cpu 
          cpu.push_back( queue[0] );
          queue.erase(queue.begin());
          arrivalcpu = cputime;
        }
      }
      else{
        if( cpu[0].priority > queue[0].priority ){ // 遇到piority比較大的 要讓出cpu 
          queue.push_back(cpu[0]);
          cpu[0] = queue[0] ;
          queue.erase(queue.begin());
          arrivalcpu = cputime;
        }
      }
    
    } 
    
    if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
      if( queue.size() == 0 ) { // 沒有process在queue 
        Gantt.push_back(-1) ;
      }
      else{ // 將queue第一個放入cpu 
        cpu.push_back( queue[0] );
        queue.erase(queue.begin());
        arrivalcpu = cputime;
      }
    }
    cputime++; 
  } 
 
  Gantt.pop_back();
  Gantt.pop_back();
  Ganttchar[0].PP = Gantt;
  Gantt.clear();
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].id > temp[j].id )
        swap( temp[i], temp[j] );
    }
  }
  
  Result resulttemp_wait, resulttemp_turn ;
  
  if( result_waitingtime.size() == 0 && result_turnaroundtime.size() == 0 ){
    for( int i = 0 ; i < temp.size() ; i++ ) {
      resulttemp_wait.id = temp[i].id;
      resulttemp_wait.PP = temp[i].waitingtime;  
      result_waitingtime.push_back(resulttemp_wait);
      
      resulttemp_turn.id = temp[i].id;
      resulttemp_turn.PP = temp[i].turnaroundtime; 
      result_turnaroundtime.push_back(resulttemp_turn);
    } 
  }
  else{
    for( int i = 0 ; i < result_waitingtime.size() ; i++ ) {
      result_waitingtime[i].id = temp[i].id;
      result_waitingtime[i].PP = temp[i].waitingtime;
    }
    for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ) {
      result_turnaroundtime[i].id = temp[i].id;
      result_turnaroundtime[i].PP = temp[i].turnaroundtime;
    }  
  }
  
}

void RR(){

  int cputime = 0 ;
  vector<Data> temp, cpu, queue;
  temp = Inputdata ;
  
  for( int i = 0 ; i < temp.size() ; i ++ ){
    temp[i].remaincpuburst = temp[i].cpuburst;
    temp[i].newarrival = temp[i].arrival;
  }
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].newarrival > temp[j].newarrival )
        swap( temp[i], temp[j] );
      else if ( temp[i].newarrival == temp[j].newarrival ){
        if( temp[i].id > temp[j].id )
          swap( temp[i], temp[j] );
      }
    }
  }

  
  
  int arrivalcpu = 0 ;
  int time = 0 ;
  while( !alldone(temp) ){
    
    for( int i = 0 ; i < temp.size() ; i++ ){
      if( cputime == temp[i].arrival ) // process arrival time = cpu time 到 queue中排隊 
        queue.push_back( temp[i] );
    }

    if( cpu.size() == 1 ){
      time++;
      Gantt.push_back(cpu[0].id);
      cpu[0].usecpu = true ;
      cpu[0].remaincpuburst = cpu[0].remaincpuburst-1 ;
      if( cpu[0].remaincpuburst == 0 ){ //做完了 
        for( int i = 0 ; i < temp.size() ; i++ ){
          if( temp[i].id == cpu[0].id ){
            temp[i].done = true;
            temp[i].finishtime = cputime;
            temp[i].turnaroundtime = temp[i].finishtime - temp[i].arrival;
            temp[i].waitingtime = temp[i].turnaroundtime - temp[i].cpuburst ;
          }
        }
        cpu.erase(cpu.begin());
        time = 0 ;
      }
      
      /*for( int i = 0 ; i < queue.size() ; i++ ){ // 對在queue裡的process進行排序 
        for( int j = i+1 ; j < queue.size() ; j++ ){
          if( queue[i].arrival > queue[j].arrival ) // 依照arrival 排序 
            swap( queue[i], queue[j] );
          else if ( queue[i].arrival == queue[j].arrival ){ // 若arrival time一樣 
            if( queue[i].id > queue[j].id )
              swap( queue[i], queue[j] );
          }
        }
      }*/
    
      if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
        if( queue.size() == 0 ) { // 沒有process在queue 
          Gantt.push_back(-1) ;
        }
        else{ // 將queue第一個放入cpu 
          cpu.push_back( queue[0] );
          queue.erase(queue.begin());
          arrivalcpu = cputime;
        }
      }
      else{
        
        if( time == timeslice ){ // timeslice到了 要讓出cpu 
          int index = 0 ;
          cpu[0].newarrival = cputime+1;
          
          int insert = 0 ;
          for( int i = 0 ; i < queue.size() ; i++,index++){
            
            if( cpu[0].newarrival <= queue[index].newarrival ){
              if( cpu[0].newarrival < queue[index].newarrival )
                queue.insert(queue.begin()+index, cpu[0] );
              else if( cpu[0].newarrival == queue[index].newarrival ){
                if( queue[index].usecpu == false )
                  queue.insert(queue.begin()+index+1, cpu[0] );
                else
                  queue.insert(queue.begin()+index, cpu[0] );  
              }
              
              insert++;
              break;
            }
            
            
          }
          
          if( insert == 0 ){
            queue.push_back(cpu[0]);
          }
          else
            insert = 0 ;
            
          
          cpu[0] = queue[0] ;
          queue.erase(queue.begin());
          arrivalcpu = cputime;
          time = 0 ;
        }
      }
    
    } 
   
    if( cpu.size() == 0 ) { // 代表沒有process在cpu 要從queue抓一個進去 
      if( queue.size() == 0 ) { // 沒有process在queue 
        Gantt.push_back(-1) ;
      }
      else{ // 將queue第一個放入cpu 
        cpu.push_back( queue[0] );
        queue.erase(queue.begin());
        arrivalcpu = cputime;
      }
    }
    

      
    cputime++; 
    
  } 
 
  Gantt.pop_back();
  Gantt.pop_back();
  Ganttchar[0].RR = Gantt;
  Gantt.clear();
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[i].id > temp[j].id )
        swap( temp[i], temp[j] );
    }
  }
  
  Result resulttemp_wait, resulttemp_turn ;
  
  if( result_waitingtime.size() == 0 && result_turnaroundtime.size() == 0 ){
    for( int i = 0 ; i < temp.size() ; i++ ) {
      resulttemp_wait.id = temp[i].id;
      resulttemp_wait.RR = temp[i].waitingtime;  
      result_waitingtime.push_back(resulttemp_wait);
      
      resulttemp_turn.id = temp[i].id;
      resulttemp_turn.RR = temp[i].turnaroundtime; 
      result_turnaroundtime.push_back(resulttemp_turn);
    } 
  }
  else{
    for( int i = 0 ; i < result_waitingtime.size() ; i++ ) {
      result_waitingtime[i].id = temp[i].id;
      result_waitingtime[i].RR = temp[i].waitingtime;
    }
    for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ) {
      result_turnaroundtime[i].id = temp[i].id;
      result_turnaroundtime[i].RR = temp[i].turnaroundtime;
    }  
  }
  
}

OutputGantt( vector<int> Gantt ){
  for( int i = 0 ; i < Gantt.size() ; i++ ){
    if( Gantt[i] == -1 ){
      cout << "-" ;
      fprintf( fout, "-" ) ;
    }
    else if( Gantt[i] >= 10 ){
      if( Gantt[i] == 10 ) {
        fprintf( fout, "A" ) ;
        cout << "A" ;
      }
      if( Gantt[i] == 11 ) {
        fprintf( fout, "B" ) ;
        cout << "B" ;
      }
      if( Gantt[i] == 12 ) {
        fprintf( fout, "C" ) ;
        cout << "C" ;
      }
      if( Gantt[i] == 13 ) {
        fprintf( fout, "D" ) ;
        cout << "D" ;
      }
      if( Gantt[i] == 14 ) {
        fprintf( fout, "E" ) ;
        cout << "E" ;
      }
      if( Gantt[i] == 15 ) {
        fprintf( fout, "F" ) ;
        cout << "F" ;
      }
      if( Gantt[i] == 16 ) {
        fprintf( fout, "G" ) ;
        cout << "G" ;
      }
      if( Gantt[i] == 17 ) {
        fprintf( fout, "H" ) ;
        cout << "H" ;
      }
      if( Gantt[i] == 18 ) {
        fprintf( fout, "I" ) ;
        cout << "I" ;
      }
      if( Gantt[i] == 19 ) {
        fprintf( fout, "J" ) ;
        cout << "J" ;
      }
      if( Gantt[i] == 20 ) {
        fprintf( fout, "K" ) ;
        cout << "K" ;
      }
      if( Gantt[i] == 21 ) {
        fprintf( fout, "L" ) ;
        cout << "L" ;
      }
      if( Gantt[i] == 22 ) {
        fprintf( fout, "M" ) ;
        cout << "M" ;
      }
      if( Gantt[i] == 23 ) {
        fprintf( fout, "N" ) ;
        cout << "N" ;
      }
      if( Gantt[i] == 24 ) {
        fprintf( fout, "O" ) ;
        cout << "O" ;
      }
      if( Gantt[i] == 25 ) {
        fprintf( fout, "P" ) ;
        cout << "P" ;
      }
      if( Gantt[i] == 26 ) {
        fprintf( fout, "Q" ) ;
        cout << "Q" ;
      }
      if( Gantt[i] == 27 ) {
        fprintf( fout, "R" ) ;
        cout << "R" ;
      }
      if( Gantt[i] == 28 ) {
        fprintf( fout, "S" ) ;
        cout << "S" ;
      } 
      if( Gantt[i] == 29 ) {
        fprintf( fout, "T" ) ;
        cout << "T" ;
      }
      if( Gantt[i] == 30 ) {
        fprintf( fout, "U" ) ;
        cout << "U" ;
      }
      if( Gantt[i] == 31 ) {
        fprintf( fout, "V" ) ;
        cout << "V" ;
      }
      if( Gantt[i] == 32 ) {
        fprintf( fout, "W" ) ;
        cout << "W" ;
      }
      if( Gantt[i] == 33 ) {
        fprintf( fout, "X" ) ;
        cout << "X" ;
      }
      if( Gantt[i] == 34 ) {
        fprintf( fout, "Y" ) ;
        cout << "Y" ;
      }
      if( Gantt[i] == 35 ) {
        fprintf( fout, "Z" ) ;
        cout << "Z" ;
      }
    }
    else{
      fprintf( fout, "%d", Gantt[i] ) ;
      cout << Gantt[i] ;
    }
      
      
  }
  
  fprintf( fout, "\n" ) ;
  cout << endl;
}

int main() {
  int command = 0 ;
  while( command == 0 ){
    ganttchar temp;
    Ganttchar.push_back(temp);
    command = Input();
    if( casenum == 1 ){
      FCFS();
    }
    else if( casenum == 2 ){
      RR();
    }
    else if( casenum == 3 ){
      PSJF();
    }
    else if( casenum == 4 ){
      NSJF();
    }
    else if( casenum == 5 ){
      PP();
    }
    else if( casenum == 6 ){
      FCFS();
      RR();
      PSJF();
      NSJF();
      PP();
    }
    if( Ganttchar[0].FCFS.size() != 0 ) {
      fprintf( fout, "==    FCFS==\n" ) ;
      cout << "==    FCFS==" << endl;
      OutputGantt( Ganttchar[0].FCFS ) ;
    }
    if( Ganttchar[0].RR.size() != 0 ) {
      fprintf( fout, "==      RR==\n" ) ;
      cout << "==      RR==" << endl;
      OutputGantt( Ganttchar[0].RR ) ;
    }
    if( Ganttchar[0].PSJF.size() != 0 ) {
      fprintf( fout, "==    PSJF==\n" ) ;
      cout << "==    PSJF==" << endl;
      OutputGantt( Ganttchar[0].PSJF ) ;
    }
    if( Ganttchar[0].NSJF.size() != 0 ) {
      fprintf( fout, "==Non-PSJF==\n" ) ;
      cout << "==Non-PSJF==" << endl;
      OutputGantt( Ganttchar[0].NSJF ) ;
    }
    if( Ganttchar[0].PP.size() != 0 ) {
      fprintf( fout, "==Priority==\n" ) ;
      cout << "==Priority==" << endl;
      OutputGantt( Ganttchar[0].PP ) ;
    }
    
    fprintf( fout, "===========================================================\n\n" ) ;
    cout << "===========================================================" << endl << endl;
    fprintf( fout, "Waiting Time\n" ) ;
    cout << "Waiting Time" << endl;
    
    /*
    fprintf( fout, "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority\n" ) ;
    cout << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl;
    */
    
    
    if( casenum == 1 ){
      fprintf( fout, "ID\tFCFS\n" ) ;
      cout << "ID\tFCFS" << endl;
    }
    else if( casenum == 2 ){
      fprintf( fout, "ID\tRR\n" ) ;
      cout << "ID\tRR" << endl;
    }
    else if( casenum == 3 ){
      fprintf( fout, "ID\tPSJF\n" ) ;
      cout << "ID\tPSJF" << endl;
    }
    else if( casenum == 4 ){
      fprintf( fout, "ID\tNPSJF\n" ) ;
      cout << "ID\tNPSJF" << endl;
    }
    else if( casenum == 5 ){
      fprintf( fout, "ID\tPriority\n" ) ;
      cout << "ID\tPriority" << endl;
    }
    else if( casenum == 6 ){
      fprintf( fout, "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority\n" ) ;
      cout << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl;
    }
    
    
    fprintf( fout, "===========================================================\n" ) ;
    cout << "===========================================================" << endl;
    
    
    if( casenum == 1 ){
      for( int i = 0 ; i < result_waitingtime.size() ; i++ ){
        cout << result_waitingtime[i].id << "\t" << result_waitingtime[i].FCFS << endl;
        fprintf( fout, "%d\t%d\n",result_waitingtime[i].id,result_waitingtime[i].FCFS );
      }
    }
    else if( casenum == 2 ){
      for( int i = 0 ; i < result_waitingtime.size() ; i++ ){
        cout << result_waitingtime[i].id << "\t" << result_waitingtime[i].RR << endl;
        fprintf( fout, "%d\t%d\n",result_waitingtime[i].id,result_waitingtime[i].RR );
      }
    }
    else if( casenum == 3 ){
      for( int i = 0 ; i < result_waitingtime.size() ; i++ ){
        cout << result_waitingtime[i].id << "\t" << result_waitingtime[i].PSJF << endl;
        fprintf( fout, "%d\t%d\n",result_waitingtime[i].id,result_waitingtime[i].PSJF );
      }
    }
    else if( casenum == 4 ){
      for( int i = 0 ; i < result_waitingtime.size() ; i++ ){
        cout << result_waitingtime[i].id << "\t" << result_waitingtime[i].NSJF << endl;
        fprintf( fout, "%d\t%d\n",result_waitingtime[i].id,result_waitingtime[i].NSJF );
      }
    }
    else if( casenum == 5 ){
      for( int i = 0 ; i < result_waitingtime.size() ; i++ ){
        cout << result_waitingtime[i].id << "\t" << result_waitingtime[i].PP << endl;
        fprintf( fout, "%d\t%d\n",result_waitingtime[i].id,result_waitingtime[i].PP );
      }
    }
    else if( casenum == 6 ){
      for( int i = 0 ; i < result_waitingtime.size() ; i++ ){
        cout << result_waitingtime[i].id << "\t" << result_waitingtime[i].FCFS << "\t" << result_waitingtime[i].RR << "\t" << result_waitingtime[i].PSJF << "\t" << result_waitingtime[i].NSJF << "\t" << result_waitingtime[i].PP <<  endl;
        fprintf( fout, "%d\t%d\t%d\t%d\t%d\t%d\n",result_waitingtime[i].id,result_waitingtime[i].FCFS,result_waitingtime[i].RR,result_waitingtime[i].PSJF,result_waitingtime[i].NSJF,result_waitingtime[i].PP ) ;
      }
    }
    
    
    fprintf( fout, "===========================================================\n\n" ) ;
    cout << "===========================================================" << endl << endl;
    fprintf( fout, "Turnaround Time\n" ) ;
    cout << "Turnaround Time" << endl;
    
    
    /*fprintf( fout, "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority\n" ) ;
    cout << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl;
    */
    
    if( casenum == 1 ){
      fprintf( fout, "ID\tFCFS\n" ) ;
      cout << "ID\tFCFS" << endl;
    }
    else if( casenum == 2 ){
      fprintf( fout, "ID\tRR\n" ) ;
      cout << "ID\tRR" << endl;
    }
    else if( casenum == 3 ){
      fprintf( fout, "ID\tPSJF\n" ) ;
      cout << "ID\tPSJF" << endl;
    }
    else if( casenum == 4 ){
      fprintf( fout, "ID\tNPSJF\n" ) ;
      cout << "ID\tNPSJF" << endl;
    }
    else if( casenum == 5 ){
      fprintf( fout, "ID\tPriority\n" ) ;
      cout << "ID\tPriority" << endl;
    }
    else if( casenum == 6 ){
      fprintf( fout, "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority\n" ) ;
      cout << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl;
    }
    
    fprintf( fout, "===========================================================\n" ) ;
    cout << "===========================================================" << endl;
  
    if( casenum == 1 ){
      for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ){
        cout << result_turnaroundtime[i].id << "\t" << result_turnaroundtime[i].FCFS << endl;
        fprintf( fout, "%d\t%d\n",result_turnaroundtime[i].id,result_turnaroundtime[i].FCFS );
      }
    }
    else if( casenum == 2 ){
      for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ){
        cout << result_turnaroundtime[i].id << "\t" << result_turnaroundtime[i].RR << endl;
        fprintf( fout, "%d\t%d\n",result_turnaroundtime[i].id,result_turnaroundtime[i].RR );
      }
    }
    else if( casenum == 3 ){
      for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ){
        cout << result_turnaroundtime[i].id << "\t" << result_turnaroundtime[i].PSJF << endl;
        fprintf( fout, "%d\t%d\n",result_turnaroundtime[i].id,result_turnaroundtime[i].PSJF );
      }
    }
    else if( casenum == 4 ){
      for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ){
        cout << result_turnaroundtime[i].id << "\t" << result_turnaroundtime[i].NSJF << endl;
        fprintf( fout, "%d\t%d\n",result_turnaroundtime[i].id,result_turnaroundtime[i].NSJF );
      }
    }
    else if( casenum == 5 ){
      for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ){
        cout << result_turnaroundtime[i].id << "\t" << result_turnaroundtime[i].PP << endl;
        fprintf( fout, "%d\t%d\n",result_turnaroundtime[i].id,result_turnaroundtime[i].PP );
      }
    }
    else if( casenum == 6 ){
      for( int i = 0 ; i < result_turnaroundtime.size() ; i++ ){
        cout << result_turnaroundtime[i].id << "\t" << result_turnaroundtime[i].FCFS << "\t" << result_turnaroundtime[i].RR << "\t" << result_turnaroundtime[i].PSJF << "\t" << result_turnaroundtime[i].NSJF << "\t" << result_turnaroundtime[i].PP <<  endl;
        fprintf( fout, "%d\t%d\t%d\t%d\t%d\t%d\n",result_turnaroundtime[i].id,result_turnaroundtime[i].FCFS,result_turnaroundtime[i].RR,result_turnaroundtime[i].PSJF,result_turnaroundtime[i].NSJF,result_turnaroundtime[i].PP ) ;
      }
    }
  
    fprintf( fout, "===========================================================\n" ) ;
    cout << "===========================================================" << endl;


    Inputdata.clear();
    result_waitingtime.clear();
    result_turnaroundtime.clear();
    Gantt.clear();
    Ganttchar.clear();

    fclose(fout);
    fclose(fp);
  }
}
