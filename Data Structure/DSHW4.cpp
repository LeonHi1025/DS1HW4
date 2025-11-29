// 11327144 莊有隆 ＆ 11327158 謝亞諺

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <chrono>

class Data { 
 private:
  // std::string file_name;
  std::string oid, arr, dura, time;
  std::vector<unsigned long long> order_number;
  std::vector<unsigned long long> minute; //  「下單時刻」（第幾分鐘）
  std::vector<unsigned long long> duration; //「製作耗時」（多少分鐘）
  std::vector<unsigned long long> delay; //   「逾時時刻」（第幾分鐘）
  unsigned long long reading_data_time;
  unsigned long long sorting_data_time;
  unsigned long long writing_data_time;

 public:
  bool LoadFile(std::string file_name);
  void OutputSortedFile(std::string file_name);
  void SaveFile(unsigned long long t_ord, unsigned long long t_min, unsigned long long t_du, unsigned long long t_de);
  void ClearFile(std::vector<unsigned long long>);
  void PrintFile();
  void Shell_Sort();
  // void SetFileName(std::string name);
  void InsertData(std::vector<unsigned long long> &swap, int smaller, int front, int back);
  void Show_data_time();
};

class Queue {
 private:
  
 public:

};

// 載入數據
bool Data::LoadFile(std::string file_name) { 
  auto start = std::chrono::high_resolution_clock::now();
  std::ifstream in;
  std::string intput_file_name = "input" + file_name + ".txt";

  // 嘗試開啟檔案，若找不到檔案名，輸出錯誤訊息
  in.open(intput_file_name);
  if(in.fail()) {
    return false;
  }

  ClearFile(order_number);
  ClearFile(minute);
  ClearFile(duration);
  ClearFile(delay);

  in >> oid >> arr >> dura >> time;
  unsigned long long t_ord, t_min, t_du, t_de;

  // https://hackmd.io/@ndhu-programming-2021/BkZukG4jK#隱藏的-flag
  while(!in.eof()) {
    in >> t_ord >> t_min >> t_du >> t_de;
    if (!in.fail()){
      SaveFile(t_ord, t_min, t_du, t_de);
    }
  }
  // 關閉讀取
  in.close();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  this->reading_data_time = duration.count();
  return true;
  }

// 輸出整理過的數據
void Data::OutputSortedFile(std::string file_name) {
  auto start = std::chrono::high_resolution_clock::now();
  std::ofstream outputFile("sorted" + file_name + ".txt", std::ios::out);
  outputFile << "OID ";
  outputFile << "Arrival " ;
  outputFile << "Duration ";
  outputFile << "TimeOut" << std::endl;

  for (int i = 0; i < this->order_number.size(); i++) {
    outputFile << this->order_number.at(i) << " ";
    outputFile << this->minute.at(i) << " ";
    outputFile << this->duration.at(i) << " ";
    outputFile << this->delay.at(i) << std::endl;
  }

  outputFile.close();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  this->writing_data_time = duration.count();
}

// 存進vector
void Data::SaveFile(unsigned long long t_ord, unsigned long long t_min, unsigned long long t_du, unsigned long long t_de) {
  this->order_number.push_back(t_ord);
  this->minute.push_back(t_min);
  this->duration.push_back(t_du);
  this->delay.push_back(t_de);
}

// 檔案清空哦
void Data::ClearFile(std::vector<unsigned long long> trash) {
  trash.clear();
}

void Data::PrintFile() {
  std::cout << oid << "\t";
  std::cout << arr << "\t";
  std::cout << dura << "\t";
  std::cout << time << std::endl;

  for (int i = 0; i < this->order_number.size(); i++) {
    std::cout << order_number.at(i) << "\t";
    std::cout << minute.at(i) << "\t";
    std::cout << duration.at(i) << "\t";
    std::cout << delay.at(i) << std::endl;
  }

  std::cout << std::endl;
}

// 普希爾排序法
void Data::Shell_Sort() {
  auto start = std::chrono::high_resolution_clock::now();
  int size = this->order_number.size();
  size /= 2;
  while(size != 0) {
    for (int i = 0; i + size < order_number.size(); i++) {
      for (int j = i + size; j < order_number.size(); j = j + size) {
        // 前大後小請交換
        if (minute.at(i) > minute.at(j)) {
          InsertData(this->order_number, order_number.at(j), i, j);
          InsertData(this->minute, minute.at(j), i, j);
          InsertData(this->duration, duration.at(j), i, j);
          InsertData(this->delay, delay.at(j), i, j);
        }

        else if (minute.at(i) == minute.at(j)) {
          if (order_number.at(i) > order_number.at(j)) {
            InsertData(this->order_number, order_number.at(j), i, j);
            InsertData(this->minute, minute.at(j), i, j);
            InsertData(this->duration, duration.at(j), i, j);
            InsertData(this->delay, delay.at(j), i, j);
          }
        }
      }
    }
    size /= 2;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  this->sorting_data_time = duration.count();
}

/*void Data::SetFileName(std::string name) {
  this->file_name = name;
}*/

void Data::InsertData(std::vector<unsigned long long> &swap, int smaller, int front, int back) {
  int bigger = swap.at(front);
  swap.erase(swap.begin() + front);
  swap.insert(swap.begin() + front, smaller);
  swap.erase(swap.begin() + back);
  swap.insert(swap.begin() + back, bigger);
}

void Data::Show_data_time() {
  std::cout << "Reading data: " << this->reading_data_time << " us." << std::endl;
  std::cout << std::endl;
  std::cout << "Sorting data: " << this->sorting_data_time << " us." << std::endl;
  std::cout << std::endl;
  std::cout << "Weiting data: " << this->writing_data_time << " us." << std::endl;
  std::cout << std::endl;
}


int main() {
  Data data;
  std::cout << "testing 401...." << std::endl;
  data.LoadFile("401");
  data.PrintFile();
  data.Shell_Sort();
  data.OutputSortedFile("401");
  data.Show_data_time();
  return 0;
}