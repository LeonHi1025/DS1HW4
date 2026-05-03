// 11327144 莊有隆 ＆ 11327158 謝亞諺

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
#include <vector>

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
  int idle_time = 0;

 public:
  bool LoadFile(std::string file_name);
  bool LoadSortedFile(std::string file_name);
  void OutputSortedFile(std::string file_name);
  void SaveFile(int t_ord, int t_min, int t_du, int t_de);
  void ClearFile(std::vector<int>);
  void PrintFile();
  void Shell_Sort();
  void PopHead();
  void SetFileName(std::string name);
  void InsertData(std::vector<int> &swap, int smaller, int front, int back);
  void Show_data_time();
  void DoubleCooker(int people);
  void OutputCookerList(Data abort, Data timeout, float total, int cmd);
  void DealWithOrder(Data &abort, Data &timeout, int &current_time, int cook);
  void AnyCook();
  int GetFileLength();
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

  file_name = file_name;
  ClearFile(order_number);
  ClearFile(minute);
  ClearFile(duration);
  ClearFile(delay);

  in >> oid >> arr >> dura >> time;
  int t_ord, t_min, t_du, t_de;

  // https://hackmd.io/@ndhu-programming-2021/BkZukG4jK#隱藏的-flag
  while(!in.eof()) {
    in >> t_ord >> t_min >> t_du >> t_de;
    if (!in.fail()){
      SaveFile(t_ord, t_min, t_du, t_de);
    }
  }
  this -> file_name = file_name;
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
  int t_ord, t_min, t_du, t_de;

  // https://hackmd.io/@ndhu-programming-2021/BkZukG4jK#隱藏的-flag
  while(!in.eof()) {
    in >> t_ord >> t_min >> t_du >> t_de;
    if (!in.fail()){
      SaveFile(t_ord, t_min, t_du, t_de);
    }
  }
  this -> file_name = file_name;
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
  std::cout << "\t" << oid << "\t";
  std::cout << arr << "\t";
  std::cout << dura << "\t";
  std::cout << time << std::endl;

  for (int i = 0; i < this->order_number.size(); i++) {
    std::cout << "(" << i + 1 << ") \t";
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
  std::cout << "Writing data: " << this->writing_data_time << " us." << std::endl;
  std::cout << std::endl;
}

void Data::DoubleCooker(int people) {
  float total_size = this->order_number.size();
  int total_delay = 0;
  std::vector<Data> do_order;
  Data in; 
  // 迴圈建立指定數量廚師
  do_order.resize(people);
  Data abort;    
  Data timeout;
  int cooker = 0;

  // 迴圈初始化
  for(int i = 0; i < people; i++) {
    do_order[i].current_time = minute[0] + duration[0];
    PopHead();
  }

  while(!order_number.empty()) {

    if (minute[0] + duration[0] > delay[0] || duration[0] <= 0) {
      total_size--;
      PopHead();
    }

    // 閒置和抵達一樣時間處理
    bool special = false;
    int special_cook = 0;
    for(int i = 0; i < people; i++) {
      if (this->minute[0] == do_order[i].current_time) {
        special = true;
        special_cook = i;
        break;
      }
    }

    int t_people = people;
    if (special) {
      for(int i = special_cook; i < t_people; i++) {
        if (this->minute[0] == do_order[i].current_time) {
          do_order[i].DealWithOrder(abort, timeout, do_order[i].current_time, i + 1);
        }

        if (special_cook + 1 == people) {
          special = 0;
          t_people = special;
        }
      }
    }

    // 兩個都空閒

    bool findcook = false;
    int usable_cook = 0;
    int first_usable = 0;
    
    // 一個是空閒且空空的
    for(int i = 0; i < people && !findcook; i++) {
      if (do_order[i].current_time <= minute[0] && 
        do_order[i].order_number.size() == 0) {
        cooker = i;
        findcook = true;
        break;
      }  
    }

    // 大家都很閒
    for (int i = 0; i < people && !findcook; i++) {
      if (do_order[i].current_time <= minute[0]) {
        usable_cook++;
        if (usable_cook == 1) {
          first_usable = i;
        }
      }

      if (usable_cook == 2) {
        findcook = true;
        cooker = first_usable;
        break;
      }
    }

    for(int i = 0; i < people && !findcook; i++) {
      if (do_order[i].current_time <= minute[0]) {
        cooker = i;
        findcook = true;
        break;
      }
    }

    // 都不閒置
    if (!findcook) {

      // 都滿了
      bool all_full = true;
      for (int i = 0; i < people; i++) {
        if(do_order[i].order_number.size() != 3) {
          all_full = false;
        }
      }

      if (all_full) {
        abort.SaveFile(order_number[0], minute[0], duration[0], delay[0]);
        abort.cid.push_back(0);
        abort.delay_order.push_back(0);
        abort.leave.push_back(this->minute[0]);
        PopHead();
        continue;
      }

      // 選比較短的，相同取一
      int shortest = do_order[0].order_number.size();
      cooker = 0;
      for (int i = 0; i < people; i++) {
        if (do_order[i].order_number.size() < shortest) {
          shortest = do_order[i].order_number.size();
          cooker = i;
        }
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
    bool arr_gt_curr = false;
    for (int i = 0; i < people; i++) {
      if (this->minute[0] > do_order[i].current_time) {
        arr_gt_curr = true;
        while(do_order[i].current_time <= minute[0]) {
          do_order[i].DealWithOrder(abort, timeout, do_order[i].current_time, i + 1);
          if (do_order[i].order_number.empty()) {
            break;
          }
        }
      }
    }
    
    if (arr_gt_curr) {
      if (!do_order[cooker].order_number.empty() || do_order[cooker].current_time > minute[0]) {
        do_order[cooker].SaveFile(order_number[0], minute[0], duration[0], delay[0]);
      }

      if (do_order[cooker].order_number.empty() && do_order[cooker].current_time <= minute[0]) {
        do_order[cooker].current_time = minute[0] + duration[0];
      }

      PopHead();
      continue;
    } 
  
    for(int i = 0; i < people; i++) {
      do_order[i].DealWithOrder(abort, timeout, do_order[i].current_time, i + 1);
    }
  }

  bool has_left_order = true;
  while(has_left_order) {
    int shortest_current_order = -1;
    for(int i = 0; i < people; i++) {
      if (!do_order[i].order_number.empty()) {
        shortest_current_order = i;
        has_left_order = true;
        break;
      }

      else {
        has_left_order = false;
      }
    }

    if (!has_left_order) {
      break;
    }

    // 選擇current_time最小的做
    for(int i = 0; i < people; i++) {
      if (do_order[i].order_number.size() != 0) {
        if (do_order[i].current_time < do_order[shortest_current_order].current_time) {
          shortest_current_order = i;
        }
      }
    }

    do_order[shortest_current_order].DealWithOrder(abort, timeout, do_order[shortest_current_order].current_time, shortest_current_order + 1);
  }

  OutputCookerList(abort, timeout, total_size, people);
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

void Data::OutputCookerList(Data abort, Data timeout, float total, int people) {
  int delay_count = 0;
  int abort_count = 0;
  int total_delay = 0;
  float fail_data = abort.order_number.size() + timeout.order_number.size();
  std::string title;
  if (people == 1) {
    title = "one";
  }

  else if (people == 2) {
    title = "two";
  }

  else {
    title = "any";
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

void Data::AnyCook() {
  int number_of_cook = 0 , temp_short = 0 , temp_small = 0; // 存最短索引，最小索引
  bool has_idle = false , need_to_queue = false , is_full = true , end = false;
  float total_size = this -> order_number.size();
  Data abort;    
  Data timeout;
  while (1) {
    std::cout << std::endl << "Input the number of queues: ";

    if (!(std::cin >> number_of_cook)) {
      std::cin.clear(); 
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    if (number_of_cook <= 19 && number_of_cook >= 1) {
      break;
    } else if (number_of_cook >= 0) {
      std::cout << std::endl << "### It is NOT in [1,19] ###" << std::endl;
    }
  }
  
  std::vector<Data> multi_cook(number_of_cook);
  while (!order_number.empty()) {

    // 看是否有閒置
    for (int i = 0 ; i < number_of_cook ; i++) {
      while (minute[0] + duration[0] > delay[0] || duration[0] <= 0) {
        total_size -= 1;
        PopHead();
      }
      if (multi_cook[i].idle_time <= minute[0] && multi_cook[i].GetFileLength() == 0) {
        if (order_number.empty()) {
          end = true;
          break;
        }
        multi_cook[i].idle_time =  minute[0] + duration[0];
        PopHead();
        break;
      }
    }
    if (end) {
      break;
    }

    need_to_queue = true;
    for (int i = 0 ; i < number_of_cook ; i++) {
      while (minute[0] + duration[0] > delay[0] || duration[0] <= 0) {
        total_size -= 1;
        PopHead();
      }
      if (order_number.empty()) {
        need_to_queue = false;
        break;
      }
      if (multi_cook[i].idle_time <= minute[0]) {
        need_to_queue = false;
        break;
      }
    }
    // 無閒置狀態 ， 排程
    while (need_to_queue) {
      // 找最短的佇列且數字最小
      temp_short = 0;
      for (int i = 0 ; i < number_of_cook ; i++) {
        if (multi_cook[i].GetFileLength() < multi_cook[temp_short].GetFileLength()) {
          temp_short = i;
        }
      }
      // 檢查queue是否已滿
      is_full = true;
      for (int i = 0 ; i < number_of_cook ; i++) {
        if (multi_cook[i].GetFileLength() < 3) {
          is_full = false;
          break;
        }
      }
      // 排程，檢查是否滿
      if (need_to_queue && is_full) {
        // abort
        abort.order_number.push_back(order_number[0]);
        abort.cid.push_back(0);
        abort.delay_order.push_back(0);
        abort.leave.push_back(minute[0]);
        PopHead();
      } else if (need_to_queue) {
        multi_cook[temp_short].SaveFile(order_number[0] , minute[0] , duration[0] , delay[0]);
        PopHead();
      }

      // 檢查是否需要queue
      for (int i = 0 ; i < number_of_cook ; i++) {
        while (minute[0] + duration[0] > delay[0] || duration[0] <= 0) {
          total_size -= 1;
          PopHead();
        }
        if (order_number.empty()) {
          need_to_queue = false;
          break;
        }
        if (multi_cook[i].idle_time <= minute[0]) {
          need_to_queue = false;
          break;
        }
      }
    }

    // 開始處理queue
    for (int i = 0 ; i < number_of_cook ; i++) {
      while (multi_cook[i].idle_time <= minute[0] || order_number.empty()) {
        if (multi_cook[i].order_number.size() == 0) {
          break;
        }
        if (multi_cook[i].delay[0] < multi_cook[i].idle_time) {
          // 「逾時時刻」<閒置時刻
          abort.order_number.push_back(multi_cook[i].order_number[0]);
          abort.cid.push_back(i + 1);
          abort.delay_order.push_back(multi_cook[i].idle_time - multi_cook[i].minute[0]);
          abort.leave.push_back(multi_cook[i].idle_time);
          multi_cook[i].PopHead();

        } else {
          if (multi_cook[i].delay[0] < multi_cook[i].idle_time + multi_cook[i].duration[0]) {
            // 「逾時時刻」<閒置時刻+「製作耗時」
            timeout.order_number.push_back(multi_cook[i].order_number[0]);
            timeout.cid.push_back(i + 1);
            timeout.delay_order.push_back(multi_cook[i].idle_time - multi_cook[i].minute[0]);
            multi_cook[i].idle_time = multi_cook[i].idle_time + multi_cook[i].duration[0];
            timeout.leave.push_back(multi_cook[i].idle_time);
            multi_cook[i].PopHead();

          } else {
            multi_cook[i].idle_time = multi_cook[i].idle_time + multi_cook[i].duration[0];
            multi_cook[i].PopHead();
          }
        }
      }
    }
  }
  this -> file_name = file_name;
  OutputCookerList(abort , timeout , total_size , number_of_cook);
}

int Data::GetFileLength() {
  return order_number.size();
}

void PrintInitial() {
  std::cout << "*** (^_^) Data Structure (^o^) ***" << std::endl;
  std::cout << "** Simulate FIFO Queues by SQF ***" << std::endl;
  std::cout << "* 0. Quit                        *" << std::endl;
  std::cout << "* 1. Sort a file                 *" << std::endl;
  std::cout << "* 2. Simulate one FIFO queue     *" << std::endl;
  std::cout << "* 3. Simulate two queues by SQF  *" << std::endl;
  std::cout << "* 4. Simulate some queues by SQF *" << std::endl;
  std::cout << "**********************************" << std::endl;
  std::cout << "Input a command(0, 1, 2, 3, 4): ";
}

int main() {
  int command = -1;
  std::string file_name;
  bool execute_2 = false;
  Data data;
  Data sorted_data;
  while (command != 0) {
    while (1) {
      PrintInitial();
      if (!(std::cin >> command)) {
        command = 0;
        break;
      } else if (command < 0) {
        std::cout << std::endl << "Command does not exist!" << std::endl << std::endl;
      } else {
        break;
      }
    }

    if (command == 1) {
      execute_2 = false;
      std::cout << std::endl << "Input a file number (e.g., 401, 402, 403, ...): ";
      std::cin >> file_name;
      if (data.LoadFile(file_name)) {
        std::cout << std::endl;
        data.PrintFile();
        data.Shell_Sort();
        data.OutputSortedFile(file_name);
        data.Show_data_time();
      } else {
        std::cout << std::endl << "### input" << file_name << ".txt does not exist! ###" << std::endl;
      }
    } else if (command == 2) {
      execute_2 = true;
      std::cout << std::endl << "Input a file number (e.g., 401, 402, 403, ...): ";
      std::cin >> file_name;
      if (sorted_data.LoadSortedFile(file_name)) {
        std::cout << std::endl;
        sorted_data.PrintFile();
        Data copy = sorted_data;
        sorted_data.DoubleCooker(1);
        sorted_data = copy;
      } else {
        std::cout << std::endl << "### sorted" << file_name << ".txt does not exist! ###" << std::endl << std::endl;
      }
    } else if (command == 3) {
      if (execute_2) {
        Data copy = sorted_data;
        sorted_data.DoubleCooker(2);
        sorted_data = copy;
        std::cout << std::endl;
      } else {
        std::cout << std::endl << "### Execute command 2 first! ###" << std::endl << std::endl;
      }
    } else if (command == 4) {
      if (execute_2) {
        Data copy = sorted_data;
        sorted_data.AnyCook();
        sorted_data = copy;
        std::cout << std::endl;
      } else {
        std::cout << std::endl << "### Execute command 2 first! ###" << std::endl << std::endl;
      }
    }
  }
  return 0;
}
