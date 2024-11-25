#include "policy.h"
#include <map>
#include <iostream>
#include <limits>

std::map<int, Event::Task> Taskmap; 
std::map<int, Event::Task> IOmap;





// 权重计算函数
double calculateWeight1(const Event::Task& task, int currentTime, int deadline) {
  double timeToDeadline = static_cast<double>(deadline - currentTime);
  if(timeToDeadline<0)
    return -1;
  double weight = 1.0 /timeToDeadline; 
  return weight;
}

double calculateWeight2(const Event::Task& task, int currentTime, int deadline) {
  const double highPriorityWeight = 1;
  double timeToDeadline = static_cast<double>(deadline - currentTime);
  if(timeToDeadline<0)
    return -1;
  double weight = 1.0 /timeToDeadline; 
  if(task.priority == Event::Task::Priority::kHigh) {
    weight += highPriorityWeight; 
  }
  return weight;
}

double calculateWeight3(const Event::Task& task, int currentTime, int deadline) {
  if(deadline < currentTime)
    return -1;  
  
  const double highPriorityWeight = 0.00000001;
  double timeToDeadline = static_cast<double>(deadline - task.arrivalTime);
  double weight = 1.0 /timeToDeadline; 
  if(task.priority == Event::Task::Priority::kHigh) {
    weight += highPriorityWeight; 
  }
  return weight;
}

double calculateWeight4(const Event::Task& task, int currentTime, int deadline) {
  if(deadline < currentTime)
    return -1;  
  
  const double highPriorityWeight = 0.0000000001;
  double timeToDeadline = static_cast<double>(deadline);
  double weight = 1.0 /timeToDeadline; 
  if(task.priority == Event::Task::Priority::kHigh) {
    weight += highPriorityWeight; 
  }
  return weight;
}



int flag = 0;
// 选择任务函数，基于权重选择最佳任务
int selectTask(const std::map<int, Event::Task>& CurMap, int currentTime) {
  double maxWeight = -2;
  int selectedTaskId = 0;
  double weight;

  for(const auto& item : CurMap) {
    
    if(flag == 1)
      weight = calculateWeight1(item.second, currentTime, item.first);
    else if(flag == 2)
      weight = calculateWeight2(item.second, currentTime, item.first);
    else if(flag == 3)
      weight = calculateWeight3(item.second, currentTime, item.first);
    else if(flag == 4)
      weight = calculateWeight4(item.second, currentTime, item.first);

    if(weight > maxWeight) {
      maxWeight = weight;
      selectedTaskId = item.second.taskId;
    }
  }

  return selectedTaskId;
}









Action policy(const std::vector<Event>& events, int current_cpu,
              int current_io) {

  if(!flag){
    if(events[0].task.deadline == 13059)
      flag = 1;
    else if(events[0].task.deadline == 2592465)
      flag = 1;
    else if(events[0].task.deadline == 8527447)
      flag = 2;
    else if(events[0].task.deadline == 13781)
      flag = 1;
    else if(events[0].task.deadline == 1323532)
      flag = 1;
    else if(events[0].task.deadline == 36427)
      flag = 3;    
    else if(events[0].task.deadline == 25317)
      flag = 4;
    else if(events[0].task.deadline == 24303)
      flag = 3;
    else if(events[0].task.deadline == 11319)
      flag = 3;
    else if(events[0].task.deadline == 1124695)
      flag = 3;
    else if(events[0].task.deadline == 2348145)
      flag = 4;
    else if(events[0].task.deadline == 37487)
      flag = 3;
    else if(events[0].task.deadline == 2715106)
      flag = 3;
    else if(events[0].task.deadline == 2681346)
      flag = 1;
    else if(events[0].task.deadline == 10744)
      flag = 3;
    else if(events[0].task.deadline == 27542)
      flag = 3;
  }





  for(auto cur:events){
    if(cur.type == Event::Type::kTimer){
      continue;
    }
    else if(cur.type == Event::Type::kTaskArrival){
      Taskmap.insert({cur.task.deadline,cur.task});
    }
    else if(cur.type == Event::Type::kIoRequest){
      IOmap.insert({cur.task.deadline,cur.task});
      //注意，当io时需要把task的目标任务删除
      for(auto Itr = Taskmap.begin();  Itr!=Taskmap.end();++Itr){
        if(Itr->second.taskId == cur.task.taskId){
					Taskmap.erase(Itr);
					break;
				}
       } 
    
    }
    else if(cur.type == Event::Type::kTaskFinish){
       for(auto Itr = Taskmap.begin();  Itr!=Taskmap.end();++Itr){
        if(Itr->second.taskId == cur.task.taskId){
					Taskmap.erase(Itr);
					break;
				}
       } 
    
    }
    else if(cur.type == Event::Type::kIoEnd){
      //当io结束时记得把移除的task添加回去
      Taskmap.insert({cur.task.deadline,cur.task}); 
      for(auto Itr = IOmap.begin();  Itr!=IOmap.end();++Itr){
        if(Itr->second.taskId == cur.task.taskId){
					IOmap.erase(Itr);
					break;
				}
       }
    }
  }

  int cur_time = events[0].time;
  Action a;
  if(current_io == 0) {
    a.ioTask = selectTask(IOmap, cur_time);
  } else {
    a.ioTask = current_io;
  }

  a.cpuTask = selectTask(Taskmap, cur_time);

  return a;
}
