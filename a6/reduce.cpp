//***************************************************************************
//
//  Anthony Chappetto
//  z1888754
//  CSCI 463 Section 0001
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <unistd.h>

using std::cout;
using std::endl;
using std::vector;
using std::thread;
using std::cerr;

constexpr int rows = 1000; /// < the number of rows in the work matrix
constexpr int cols = 100; /// < the number of cols in the work matrix

std::mutex stdout_lock; /// < for serializing access to stdout

std::mutex counter_lock; /// < for dynamic balancing only
volatile int counter = rows; /// < for dynamic balancing only

std::vector <int> tcount; /// < count of rows summed for each thread
std::vector <uint64_t> sum; /// < the calculated sum from each thread

int work [rows][cols]; /// < the matrix to be summed

/*********************************************************************
 * Implements logic needed to sum the rows of the matric using
 * static load balancing determining which rows will be processed
 * by each thread
 *
 * @param tid Thread ID that is passed in from main()
 * @param num_threads Number of threads needed for each row processed
 ********************************************************************/
void sum_static(int tid, int num_threads) {
  stdout_lock.lock(); //locks the current thread
  cout << "Thread " << tid << " starting" << endl;  //displays that threading is statrting
  stdout_lock.unlock(); //unlocks thread

  int length, count = 0;
  bool done = false;	
  
  while (!done) {
    length=tid + (num_threads*count); //calculates next row
    ++count;  //incr count

    if (length >= rows)
      done = true;
    else
      done = false; //dont sum rows if the length is greater than rows
     
    if (!done) {
      uint64_t total=0;
      int i=0;
      while(i < cols) {         //while i less than columns
        total+=work[length][i]; //add value to total
        i++;
      }
      sum[tid]+=total;  //count sum of each row threading has occured
      ++tcount[tid];    //count number threads
    }
  }

  stdout_lock.lock(); //loccks current thread
  cout << "Thread " << tid << " ending tcount=" << tcount[tid] << " sum=" << sum[tid] << endl; //ending statement
  stdout_lock.unlock(); //unlocks current thread
}

/*********************************************************************
 * Implements logic needed to sum rows of matric using dynamic load
 * balancing determining which rows will be processed by each thread
 *
 * @param tid Thread ID that is passed in from main()
 ********************************************************************/
void sum_dynamic(int tid) {
  stdout_lock.lock(); //locks current thread
  cout << "Thread " << tid << " starting" << endl; //threading statement
  stdout_lock.unlock(); //unlocks current thread

  bool done  = false; 

  while (!done) {  //while threading is occuring
    int count_copy; 

    counter_lock.lock(); //lock the counter_lock
      if(counter>0)   //if the counter is greater than 0
        --counter;    //decrement
      else 
        done = true;

      count_copy=counter;	
    
    counter_lock.unlock();  //unlock the counter_lock

    if (!done) { //up to here
      (void)count_copy;	

      uint64_t total = 0;
      int i = 0;
      while(i < cols) {             //while i is less than columns
        total+=work[count_copy][i]; //add value to total for every column in a row
        i++;
      }

      sum[tid] += total;    //sum the rows in the threading
      ++tcount[tid];        //sum the threads 
    }
  }

  stdout_lock.lock(); //lock the thread
  cout << "Thread " << tid << " ending tcount=" << tcount[tid] << " sum=" << sum[tid] << endl; //exit statement
  stdout_lock.unlock(); //unlock the thread
}

/*********************************************************************
 * Function accepts proper command-line parameters from a 
 * Usage statement and parses them.
 *
 * @param argc Command line argument
 * @param argv Command line argument
 * 
 * @return
 ********************************************************************/
int main(int argc, char **argv) {
  unsigned int num_threads = 2; //default amount of threads
  bool d = false;
  int obj;
  vector<thread*> threads;  //holds threads

  while((obj = getopt(argc, argv, "dt:"))!=-1) {
    switch (obj) {
      case 't': //case number of threads
        num_threads=atoi(optarg); //stores thread count

	      if (num_threads >= std::thread::hardware_concurrency()) //# of threads allowed by system
          num_threads=std::thread::hardware_concurrency();
      break;
      case 'd': { //case dynamic load
        d = true;
      }
      break;
      default: {  //usage statement
        cerr << "Usage: reduce [-d] [-t num]" << endl;
        cerr << "    -d use dynamic load balancing" << endl;
        cerr << "    -t specifies the number of threads to use" << endl;
        exit(1);
      }
    }
  }

  srand(0x1234);  //rand() initializes data
  for(int i = 0; i < cols; i++) {
    int j = 0;
    while (j < rows) {
      work[j][i] = rand(); //rng
      j++;
    }
  }

  cout << std::thread::hardware_concurrency() << " concurrent threads supported." << endl; //outputs cores
  tcount.resize(num_threads, 0); 
  sum.resize(num_threads, 0); //resize thread counts

  if(d) { //use dynamic load balancing
    unsigned int i = 0;
    while (i < num_threads) {
      threads.push_back(new thread(sum_dynamic, i));  //push thread into vector
      i++;
    }
  }
  else {  //static load balancing
    for (unsigned int i = 0; i < num_threads; ++i) 
      threads.push_back(new thread(sum_static, i, num_threads)); //push thread into vector
  }

  int total_work = 0;
  for (unsigned int i = 0; i < num_threads; ++i) {
    threads.at(i)->join(); //join the threads together
    delete threads.at(i); //delete threads
    total_work += tcount.at(i); //calculate total work
  }

  uint64_t final_sum=0;
  unsigned int i=0;
  while (i < num_threads) {
    final_sum+=sum[i];  //adds total sum of threads
    i++;
  }

  cout << "main() exiting, total_work=" << total_work << " gross_sum=" << final_sum << endl; //output total_work and final_sum
  return 0; //return statement
}