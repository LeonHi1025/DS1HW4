// 11327144 莊有隆 ＆ 11327158 謝亞諺

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>

class Data { 
 private:
  std::string file_name;
  std::string oid, arr, dura, time;
  std::vector<int> order_number;
  std::vector<int> minute;   //「下單時刻」（第幾分鐘）
  std::vector<int> duration; //「製作耗時」（多少分鐘）
  std::vector<int> delay;    //「逾時時刻」（第幾分鐘），允許最晚完成時間
  std::vector<int> cid;
  std::vector<int> delay_order;
  std::vector<int> leave;
  int reading_data_time;
  int sorting_data_time;
  int writing_data_time;
  int current_time;

 public:
  bool LoadFile(std::string file_name);
  bool LoadSortedFile(std::string file_name);
  void OutputSortedFile(std::string file_name);
  void SaveFile(int t_ord, int t_min, int t_du, int t_de);
  void SaveCookList(int current_time);
  void ClearFile(std::vector<int>);
  void PrintFile();
  void Shell_Sort();
  void PopHead();
  void SetFileName(std::string name);
  void InsertData(std::vector<int> &swap, int smaller, int front, int back);
  void Show_data_time();
  void SingleCooker();
  void DoubleCooker();
  void OutputCookerList(Data abort, Data timeout, float total, int cmd);
  void DealWithOrder(Data &abort, Data &timeout, int &current_time, int cook);
};
void Data::SetFileName(std::string name) {
  file_name = name;
}

void Data::PopHead() {
  this->order_number.erase(order_number.begin());
  this->minute.erase(minute.begin());
  this->duration.erase(duration.begin());
  this->delay.erase(delay.begin());
}

// 載入數據
bool Data::LoadSortedFile(std::string file_name) { 
  auto start = std::chrono::high_resolution_clock::now();
  std::ifstream in;
  std::string intput_file_name = "sorted" + file_name + ".txt";

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
void Data::SaveFile(int t_ord, int t_min, int t_du, int t_de) {
  this->order_number.push_back(t_ord);
  this->minute.push_back(t_min);
  this->duration.push_back(t_du);
  this->delay.push_back(t_de);
}

// 檔案清空哦
void Data::ClearFile(std::vector<int> trash) {
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

// 希爾排序法
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

void Data::InsertData(std::vector<int> &swap, int smaller, int front, int back) {
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

void Data::SingleCooker() {
  float total_size = this->order_number.size();
  int current_time = minute[0] + duration[0];
  int total_delay = 0;
  Data do_order; // 不應同時超過三筆
  Data abort;    
  Data timeout;
  PopHead();     // 第一筆已消失
  while(!order_number.empty()) {
    /*std::cout << order_number[0] << std::endl;
    std::cout << current_time << std::endl;*/
    if (minute[0] + duration[0] > delay[0] || duration[0] <= 0) {
      total_size--;
      PopHead();
    }

    /*if (order_number[0] == 102) {
      std::cout << "cur " << current_time << std::endl;
      std::cout << "===order===" << std::endl;
      do_order.PrintFile();
      std::cout << "===abort===" << std::endl;
      abort.PrintFile();
      PrintFile();
      return;
    }*/

    if (this->minute[0] == current_time) {
      do_order.DealWithOrder(abort, timeout, current_time, 1);
    }

    if(this->minute[0] <= current_time) {
      // 如果處理訂單未滿三筆，請讀入
      if (do_order.order_number.size() < 3) { 
        do_order.SaveFile(order_number[0], minute[0], duration[0], delay[0]);
        PopHead();
        continue;
      }

      // 如果滿三筆後續請棄單
      else { 
        abort.SaveFile(order_number[0], minute[0], duration[0], delay[0]);
        abort.cid.push_back(0);
        abort.delay_order.push_back(0);
        abort.leave.push_back(this->minute[0]);
        PopHead();
        continue;
      }
    }
    // 阿就真的沒有了，直接更新辣
    else if (this->minute[0] > current_time) {
      while (!do_order.order_number.empty()) {
      // 開始處理訂單內容，如果current_time一發生變化就會返回讀資料
        do_order.DealWithOrder(abort, timeout, current_time, 1);
      }
      if (this->minute[0] > current_time) {
        current_time = minute[0] + duration[0];
      }

      else {
        do_order.SaveFile(order_number[0], minute[0], duration[0], delay[0]);
      }

      PopHead();
      continue;
    }

    // 開始處理訂單內容，如果current_time一發生變化就會返回讀資料
    do_order.DealWithOrder(abort, timeout, current_time, 1);
  }

  while (!do_order.order_number.empty()) {
    // 開始處理訂單內容，如果current_time一發生變化就會返回讀資料
    do_order.DealWithOrder(abort, timeout, current_time, 1);
  }
  OutputCookerList(abort, timeout, total_size, 2);
}

void Data::DoubleCooker() {
  float total_size = this->order_number.size();
  int total_delay = 0;
  std::vector<Data> do_order; // 不應同時超過三筆
  // 感覺可以迴圈但我懶
  Data in; 
  do_order.push_back(in);
  do_order.push_back(in);
  Data abort;    
  Data timeout;
  int cooker = 0;
  //初始化
  do_order[0].current_time = minute[0] + duration[0];
  PopHead();
  do_order[1].current_time = minute[0] + duration[0];
  PopHead();
  while(!order_number.empty()) {
    //std::cout << order_number[0] << std::endl;
      /*if (order_number[0] == 102) {
      std::cout << "cur " << do_order[0].current_time << std::endl;
      std::cout << "===order1===";
      do_order[0].PrintFile();
      std::cout << "cur " << do_order[1].current_time << std::endl;
      std::cout << "===order2===";
      do_order[1].PrintFile();
      std::cout << "===abort===";
      abort.PrintFile();
      std::cout << "===timeout===";
      timeout.PrintFile();
      return;
    }*/

    if (minute[0] + duration[0] > delay[0] || duration[0] <= 0) {
      total_size--;
      PopHead();
    }

    if (this->minute[0] == do_order[0].current_time ||
        this->minute[0] == do_order[1].current_time) {
      if (this->minute[0] == do_order[0].current_time) {
        do_order[0].DealWithOrder(abort, timeout, do_order[0].current_time, 1);
        if (this->minute[0] >= do_order[1].current_time) {
          do_order[1].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
        }
      }

      else if (this->minute[0] == do_order[1].current_time) {
        do_order[1].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
        if (this->minute[0] >= do_order[0].current_time) {
          do_order[0].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
        }
      }
    }

    // 兩個都空閒
    if (do_order[0].current_time <= minute[0] &&
        do_order[1].current_time <= minute[0]) {
      cooker = 0;
    }

    // case 1
    else if (do_order[0].current_time <= minute[0] && 
             do_order[0].order_number.size() == 0) {
      cooker = 0;
    } 

    else if(do_order[1].current_time <= minute[0] && 
            do_order[1].order_number.size() == 0) {
      cooker = 1;
    } 

    else if (do_order[0].current_time <= minute[0]) {
      cooker = 0;
    }

    else if (do_order[1].current_time <= minute[0]) {
      cooker = 1;
    }

    // 都不閒置
    else if (do_order[0].current_time > minute[0] &&
             do_order[1].current_time > minute[0]) {
      // 都滿了
      if (do_order[0].order_number.size() == 3 &&
          do_order[1].order_number.size() == 3) {
        abort.SaveFile(order_number[0], minute[0], duration[0], delay[0]);
        abort.cid.push_back(0);
        abort.delay_order.push_back(0);
        abort.leave.push_back(this->minute[0]);
        PopHead();
        continue;
      }

      // 選比較短的，相同取一
      else if (do_order[0].order_number.size() <= 
               do_order[1].order_number.size()) {
        cooker = 0;
      }

      else if (do_order[0].order_number.size() > 
               do_order[1].order_number.size()) {
        cooker = 1;
      }
    }
 
    if(this->minute[0] <= do_order[cooker].current_time) {
      // 如果處理訂單未滿三筆，請讀入
      if (do_order[cooker].order_number.size() < 3) { 
        do_order[cooker].SaveFile(order_number[0], minute[0], duration[0], delay[0]);
        PopHead();
        continue;
      }

      // 如果滿三筆後續請棄單
      else { 
        abort.SaveFile(order_number[0], minute[0], duration[0], delay[0]);
        abort.cid.push_back(0);
        abort.delay_order.push_back(0);
        abort.leave.push_back(this->minute[0]);
        PopHead();
        continue;
      }
    }
    // 阿就真的沒有了，直接更新辣(都大)
    else if (this->minute[0] > do_order[0].current_time &&
             this->minute[0] > do_order[1].current_time) {
      // 開始處理訂單內容，如果current_time一發生變化就會返回讀資料
      while(do_order[0].current_time <= minute[0]) {
        do_order[0].DealWithOrder(abort, timeout, do_order[0].current_time, 1);
        if (do_order[0].order_number.empty()) {
          break;
        }
      }

      while(do_order[1].current_time <= minute[0]) {
        do_order[1].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
        if (do_order[1].order_number.empty()) {
          break;
        }
      }

      if (do_order[cooker].order_number.empty() &&
          do_order[cooker].current_time <= minute[0]) {
        do_order[cooker].current_time = minute[0] + duration[0];
      }

      if (!do_order[cooker].order_number.empty()) {
        do_order[cooker].SaveFile(order_number[0], minute[0], duration[0], delay[0]);
      }

      PopHead();
      continue;
    }

    else if (this->minute[0] > do_order[cooker].current_time) {
      // 開始處理訂單內容，如果current_time一發生變化就會返回讀資料
      while(do_order[cooker].current_time <= minute[0]) {
        do_order[cooker].DealWithOrder(abort, timeout, do_order[cooker].current_time, cooker + 1);
        if (do_order[cooker].order_number.empty()) {
          break;
        }
      }
        
      if (do_order[cooker].order_number.empty() &&
        do_order[cooker].current_time <= minute[0]) {
        do_order[cooker].current_time = minute[0] + duration[0];
      }

      else {
        do_order[cooker].SaveFile(order_number[0], minute[0], duration[0], delay[0]);
      }

      PopHead();
      continue;
    }

    // 開始處理訂單內容，如果current_time一發生變化就會返回讀資料
    do_order[0].DealWithOrder(abort, timeout, do_order[0].current_time, 1);
    do_order[1].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
  }

  // 依照時間序列處理
  while (!do_order[0].order_number.empty() || 
         !do_order[1].order_number.empty()) {

    if (do_order[0].order_number.empty()) {
      while(!do_order[1].order_number.empty()) {
        do_order[1].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
      }
    }

    else if (do_order[1].order_number.empty()) {
      while(!do_order[0].order_number.empty()) {
        do_order[0].DealWithOrder(abort, timeout, do_order[0].current_time, 1);
      }
    }

    else if (do_order[0].current_time <= 
             do_order[1].current_time) {
      do_order[0].DealWithOrder(abort, timeout, do_order[0].current_time, 1);
    }

    else if (do_order[1].current_time <
             do_order[0].current_time) {
      do_order[1].DealWithOrder(abort, timeout, do_order[1].current_time, 2);
    }

    if (do_order[0].order_number.empty() && 
        do_order[1].order_number.empty()) {
      break;
    }
  }

  OutputCookerList(abort, timeout, total_size, 3);
}

// 處理三筆order資料
void Data::DealWithOrder(Data &abort, Data &timeout, int &current_time, int cooker) {
  while(!order_number.empty()) {
    // 成功處理
    if(delay[0] >= current_time)  {
      // 逾時單

      if (current_time + duration[0] > delay[0]) {
        timeout.SaveFile(order_number[0],
                         minute[0],
                         duration[0],
                         delay[0]);
        timeout.cid.push_back(cooker);
        timeout.delay_order.push_back(current_time - this->minute[0]);
        timeout.leave.push_back(current_time + duration[0]);
      }
      // 更新current_time
      current_time = current_time + duration[0];
      PopHead();
      return;
    }

    // 棄單
    else {
      abort.SaveFile(order_number[0],
                     minute[0],
                     duration[0],
                     delay[0]);
      abort.cid.push_back(cooker);
      abort.delay_order.push_back(current_time - minute[0]);
      abort.leave.push_back(current_time);
      PopHead();
    }
  }
}

void Data::OutputCookerList(Data abort, Data timeout, float total, int cmd) {
  int delay_count = 0;
  int abort_count = 0;
  int total_delay = 0;
  float fail_data = abort.order_number.size() + timeout.order_number.size();
  std::string title;
  if (cmd ==2) {
    title = "one";
  }

  else if (cmd == 3) {
    title = "two";
  }
  
  std::ofstream outputFile(title + file_name + ".txt", std::ios::out);
  outputFile << "\t[Abort List]" << std::endl;
  outputFile << "\tOID";
  outputFile << "\tCID" ;
  outputFile << "\tDelay ";
  outputFile << "Abort" << std::endl;
  // 輸出abort list
  for (int i = 0; i < abort.order_number.size(); i++) {
    outputFile << "[" << i + 1 << "]";         // OID
    outputFile << '\t' << abort.order_number[i];
    outputFile << '\t' <<abort.cid[i];          // CID
    outputFile << '\t' <<abort.delay_order[i];        // Delay
    outputFile << '\t' <<abort.leave[i] << std::endl;  // Abort
    total_delay = total_delay + abort.delay_order[i];
  }

  outputFile << "\t[Timeout List]" << std::endl;
  outputFile << "\tOID ";
  outputFile << "\tCID " ;
  outputFile << "\tDelay ";
  outputFile << "\tDeparture" << std::endl;
  for (int i = 0; i < timeout.order_number.size(); i++) {
    outputFile << "[" << i + 1 << "]\t";
    outputFile << timeout.order_number[i]; 
    outputFile << '\t' <<timeout.cid[i]; 
    outputFile << '\t' <<timeout.delay_order[i];
    outputFile << '\t' <<timeout.leave[i] << std::endl;
    total_delay = total_delay + timeout.delay_order[i];
  }
  outputFile << "[Total Delay]" << std::endl;
  outputFile << total_delay << " min." << std::endl;
  outputFile << "[Failure Percentage]" << std::endl;
  //std::cout << total << " and " << fail_data << std::endl;
  float failure_rate = (fail_data / total) * 100;
  outputFile << std::fixed << std::setprecision(2) << failure_rate << " %";
  outputFile.close();
}

int main() {
  Data data;
  std::cout << "testing 402...." << std::endl;
  data.SetFileName("404");
  data.LoadSortedFile("404");
  data.DoubleCooker();
  std::cout << "finish!";
  return 0;
}
