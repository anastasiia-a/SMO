
#include "../headers/FunctionalModule.hpp"
#include <iostream>
//#include <windows.h>
#include "../headers/ringadd.h"
#include "step.hpp"
#include "stepwindow.h"

//static int count = 1;
std::list<StepStructure> StepLis;
std::list<StepStructure> *StepList=&StepLis;

void FunctionalModule::cleanUp() {
  data_.cleanUp();
  buffer_->cleanUp();
  for (auto &source : sources_) {
    source->cleanUp();
  }
  for (auto &handler : handlers_) {
    handler->cleanUp();
  }

  handlerPointer_ = 0;
}

void FunctionalModule::postFirstApplications() {
  for (size_t i = 0; i < sources_.size(); ++i) {
    sources_[i]->postApplication();
    //data_.sourcesData[i].generatedAppsCount++;
    //data_.sourcesData[i].acceptedAppsCount++;
    //std::cout << "Источник №" << i + 1<< " = " << sources_[i]->getPostTime() << '\n';
  }
}

int FunctionalModule::getEarliestSourceIndex() {
  double minTime = -1;
  int sourceIndex = -1;

  for (int i = 0; i < sources_.size(); i++) {
    if ((sourceIndex == -1) || (sources_[i]->getPostTime() < minTime)) {
      minTime = sources_[i]->getPostTime();
      sourceIndex = i;
    }
  }
  return sourceIndex;
}


int FunctionalModule::getEarliestHandlerIndex() {
  double minTime = -1;
  int handlerIndex = -1;

  for (int i = 0; i < handlers_.size(); i++) {
    if ((handlerIndex == -1) || (handlers_[i]->getFinishTime() < minTime)) {
      minTime = handlers_[i]->getFinishTime();
      handlerIndex = i;
    }
  }
  return handlerIndex;
}

std::pair<bool, int> FunctionalModule::getEarliestEvent() {
  // <true, index> - source with index
  // <false, index> - handler with index

  const int earliestSourceIndex = getEarliestSourceIndex();
  const double sourceTime = sources_[earliestSourceIndex]->getPostTime();

  const int earliestHandlerIndex = getEarliestHandlerIndex();
  const double handlerTime = handlers_[earliestHandlerIndex]->getFinishTime();

  if (sourceTime < handlerTime) {
    return {true, earliestSourceIndex};
  } else {
    return {false, earliestHandlerIndex};
  }
}

void FunctionalModule::handleCreationOfNewApplication(const size_t &sourceGeneratedApplication) {
  auto application = std::make_shared<Application>(sourceGeneratedApplication,  sources_[sourceGeneratedApplication]->getPostTime());

  std::cout<< "Шаг :" << count << '\n';
  count++;

  std::cout << "Источник №" << application->getSourceIndex() + 1 << " = " << sources_[application->getSourceIndex()]->getPostTime() << '\n';
  std::cout<< '\n';

  StepList->push_back( *(new StepStructure(0, application->getSourceIndex(),
                                          sources_[application->getSourceIndex()]->getPostTime(),
                                          count,
                                          application->getSourceIndex(),
                                          data_.sourcesData[application->getSourceIndex()].generatedAppsCount)) );
  std::cout <<"beg "<< StepList->front().time_ << '\n';
  std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
  std::cout <<"end "<< StepList->back().time_ << '\n';
  std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
  // Добавляем заявку в буфер, если есть место
  std::cout<< "Шаг :" << count << '\n';
  count++;
  const bool hasAdded = buffer_->addApplication(application);

  if (!hasAdded) {
    std::shared_ptr<Application> replacedApplication = buffer_->replaceApplication(application).first;
    size_t newPointer  = buffer_->replaceApplication(application).second;

    if (newPointer == buffer_->bufferSize_) {
      buffer_->additionStrategy_->getPointer(0);
    } else {
      buffer_->additionStrategy_->getPointer(newPointer + 1);
    }

    data_.sourcesData[replacedApplication->getSourceIndex()].refusedAppsCount++;

    //std::cout << "Отказ: " << "Источник №" << replacedApplication->getSourceIndex() + 1 << " = " << data_.timeNow << '\n';
    std::cout << "Отказ: " << "Источник №" << replacedApplication->getSourceIndex() + 1 << " = " << data_.timeNow << '\n';
    StepList->push_back(* (new StepStructure(3, 0,
                                            data_.timeNow,
                                            count,
                                            replacedApplication->getSourceIndex(),
                                            -1)) );
    std::cout <<"beg "<< StepList->front().time_ << '\n';
    std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
    std::cout <<"end "<< StepList->back().time_ << '\n';
    std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
    std::cout << '\n';
  }
  sources_[sourceGeneratedApplication]->postApplication();
  data_.sourcesData[sourceGeneratedApplication].generatedAppsCount ++;
}

void FunctionalModule::handleEndOfHandlerWork(const size_t &handlerFinishedWork, int apps, int numApplication, outputFinish *outputTime) {
  std::shared_ptr<Application> application;
  if (!buffer_->isEmpty()) {

    std::cout<< "Шаг :" << count << '\n';
    count++;
    application = buffer_->removeApplication();
    const int nextHandlerIndex = getNextHandler(handlers_[handlerFinishedWork]->getFinishTime());

    if (outputTime[nextHandlerIndex].finishTime != 0)
    {
        std::cout<< "Шаг :" << count << '\n';
        count++;
        StepList->push_back(* (new StepStructure(2, nextHandlerIndex,
                                                outputTime[nextHandlerIndex].finishTime,
                                                count,
                                                outputTime[nextHandlerIndex].numSource,
                                                -1)) );
        std::cout << "Прибор обработал №" << nextHandlerIndex + 1 << " Заявку от Источника №"
              << outputTime[nextHandlerIndex].numSource + 1 << " = "
              << outputTime[nextHandlerIndex].finishTime<< '\n';
         std::cout << '\n';
         std::cout <<"beg "<< StepList->front().time_ << '\n';
         std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
         std::cout <<"end "<< StepList->back().time_ << '\n';
         std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
         outputTime[nextHandlerIndex].finishTime = 0;

    }

    if (handlers_[nextHandlerIndex]->getFinishTime()!= 0)
    {
        std::cout<< "Шаг :" << count << '\n';
        count++;

        std::cout << "Работает Прибор №" << nextHandlerIndex + 1 << " Заявка от Источника №"
              << application->getSourceIndex() + 1 << " = "
              << handlers_[nextHandlerIndex]->getFinishTime()<< '\n';
         std::cout << '\n';
         StepList->push_back(* (new StepStructure(2, nextHandlerIndex,
                                                 handlers_[nextHandlerIndex]->getFinishTime(),
                                                 count,
                                                 application->getSourceIndex(),
                                                 -1)) );
         std::cout <<"beg "<< StepList->front().time_ << '\n';
         std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
         std::cout <<"end "<< StepList->back().time_ << '\n';
         std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
    }
    else
    {
        std::cout<< "Шаг :" << count << '\n';
        count++;
        std::cout << "Работает Прибор №" << nextHandlerIndex + 1 << " Заявка от Источника №"
              << application->getSourceIndex() + 1 << " = "
              << sources_[application->getSourceIndex()]->getPostTime()<< '\n';
         std::cout << '\n';
         StepList->push_back(* (new StepStructure(2, nextHandlerIndex,
                                                 sources_[application->getSourceIndex()]->getPostTime(),
                                                 count,
                                                 application->getSourceIndex(),
                                                 -1)) );
         std::cout <<"beg "<< StepList->front().time_ << '\n';
         std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
         std::cout <<"end "<< StepList->back().time_ << '\n';
         std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
    }
    const double timeInHandler = handlers_[nextHandlerIndex]->handleApplication(handlers_[handlerFinishedWork]->getFinishTime());


    data_.sourcesData[application->getSourceIndex()].handlingTime += timeInHandler;
    data_.sourcesData[application->getSourceIndex()].bufferingTime += (handlers_[handlerFinishedWork]->getFinishTime() - application->getTimeOfCreation());
    data_.handlersData[nextHandlerIndex].workingTime += timeInHandler;

    outputTime[nextHandlerIndex].finishTime = handlers_[nextHandlerIndex]->getFinishTime();
    outputTime[nextHandlerIndex].numSource = application->getSourceIndex() ;


  } else {
    const int earliestSourceIndex = getEarliestSourceIndex();
    application = std::make_shared<Application>(earliestSourceIndex, sources_[earliestSourceIndex]->getPostTime());

    std::cout<< "Шаг :" << count << '\n';
    count++;

    std::cout << "Источник №" << application->getSourceIndex() + 1 << " = " << sources_[application->getSourceIndex()]->getPostTime() << '\n';
     std::cout << '\n';
     StepList->push_back(* (new StepStructure(0, application->getSourceIndex(),
                                             sources_[application->getSourceIndex()]->getPostTime(),
                                             count,
                                             application->getSourceIndex(),
                                             data_.sourcesData[application->getSourceIndex()].generatedAppsCount)) );
     std::cout <<"beg "<< StepList->front().time_ << '\n';
     std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
     std::cout <<"end "<< StepList->back().time_ << '\n';
     std::cout <<"end "<< StepList->back().numSource_+1 << '\n';

    std::cout<< "Шаг :" << count << '\n';
    count++;
    buffer_->addApplication(application);

    std::cout<< "Шаг :" << count << '\n';
    count++;
    application = buffer_->removeApplication();

    const int nextHandlerIndex = getNextHandler(application->getTimeOfCreation());
    if (outputTime[nextHandlerIndex].finishTime != 0)
    {
        std::cout<< "Шаг :" << count << '\n';
        count++;
        std::cout << "Прибор обработал №" << nextHandlerIndex + 1 << " Заявку от Источника №"
              << outputTime[nextHandlerIndex].numSource + 1 << " = "
              << outputTime[nextHandlerIndex].finishTime<< '\n';
         std::cout << '\n';
         StepList->push_back(* (new StepStructure(2, nextHandlerIndex,
                                                 outputTime[nextHandlerIndex].finishTime,
                                                 count,
                                                 outputTime[nextHandlerIndex].numSource,
                                                 -1)) );
         std::cout <<"beg "<< StepList->front().time_ << '\n';
         std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
         std::cout <<"end "<< StepList->back().time_ << '\n';
         std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
         outputTime[nextHandlerIndex].finishTime = 0;


    }

    if (handlers_[nextHandlerIndex]->getFinishTime()!= 0)
    {
        std::cout<< "Шаг :" << count << '\n';
        count++;

        std::cout << "Работает Прибор №" << nextHandlerIndex + 1 << " Заявка от Источника №"
              << application->getSourceIndex() + 1 << " = "
              << handlers_[nextHandlerIndex]->getFinishTime()<< '\n';
         std::cout << '\n';
         StepList->push_back(* (new StepStructure(2, nextHandlerIndex,
                                                 handlers_[nextHandlerIndex]->getFinishTime(),
                                                 count,
                                                 application->getSourceIndex(),
                                                 -1)) );
         std::cout <<"beg "<< StepList->front().time_ << '\n';
         std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
         std::cout <<"end "<< StepList->back().time_ << '\n';
         std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
    }
    else
    {
        std::cout<< "Шаг :" << count << '\n';
        count++;

        std::cout << "Работает Прибор №" << nextHandlerIndex + 1 << " Заявка от Источника №"
              << application->getSourceIndex() + 1 << " = "
              << sources_[application->getSourceIndex()]->getPostTime()<< '\n';
         std::cout << '\n';
         StepList->push_back(* (new StepStructure(2, nextHandlerIndex,
                                                 sources_[application->getSourceIndex()]->getPostTime(),
                                                 count,
                                                 application->getSourceIndex(),
                                                 -1)) );
         std::cout <<"beg "<< StepList->front().time_ << '\n';
         std::cout <<"beg "<< StepList->front().numSource_ +1<< '\n';
         std::cout <<"end "<< StepList->back().time_ << '\n';
         std::cout <<"end "<< StepList->back().numSource_+1 << '\n';
    }

    const double timeInHandler = handlers_[nextHandlerIndex]->handleApplication(application->getTimeOfCreation());

    data_.sourcesData[application->getSourceIndex()].bufferingTime += 0;
    data_.sourcesData[application->getSourceIndex()].handlingTime += timeInHandler;
    data_.handlersData[nextHandlerIndex].workingTime += timeInHandler;

    outputTime[nextHandlerIndex].finishTime = handlers_[nextHandlerIndex]->getFinishTime();
    outputTime[nextHandlerIndex].numSource = application->getSourceIndex();

    sources_[earliestSourceIndex]->postApplication();
    data_.sourcesData[earliestSourceIndex].generatedAppsCount++;

  }

  data_.sourcesData[application->getSourceIndex()].acceptedAppsCount++;
}

int FunctionalModule::getNextHandler(const double &timeNow)
{
  /*for (size_t i = handlerPointer_; i < handlers_.size(); i++) {
    if (!handlers_[i]->isWorking(timeNow)) {
      if (i == handlers_.size()) {
        handlerPointer_ = 0;
      } else {
        handlerPointer_ = i + 1;
      }
      return static_cast<int>(i);
    }
  }

  for (size_t i = 0; i < handlerPointer_; i++) {
    if (!handlers_[i]->isWorking(timeNow)) {
      if (i == handlers_.size()) {
        handlerPointer_ = 0;
      } else {
        handlerPointer_ = i + 1;
      }
      return static_cast<int>(i);
    }
  }*/
   int min = handlers_.size() + 10;
   for (size_t i = 0; i < handlers_.size(); i++)
   {
     if (!handlers_[i]->isWorking(timeNow))
     {
       if (i < min)
       {
         min = i;
       }
       return static_cast<int>(min);
     }
 }
  return -1;
}

FunctionalModule::FunctionalModule(std::vector<std::shared_ptr<Source>> sources,
                                   std::shared_ptr<Buffer> buffer,
                                   std::vector<std::shared_ptr<Handler>> handlers):
  sources_(sources),
  buffer_(buffer),
  handlers_(handlers),
  handlerPointer_(0),
  data_(sources.size(), handlers.size())
{
  cleanUp();
}

/*void FunctionalModule::simulate(int numSources, int numBufer, int numHandlerconst, const size_t &steps) {
  cleanUp();
  postFirstApplications();
  for (int i = 0; i < steps; ++i) {
    simulationStep(numSources, numBufer, numHandlerconst);
  }
}*/

void FunctionalModule::simulationStep(int h, size_t countt, size_t numApplication, outputFinish *outputTime) {
  std::pair<bool, int> earliestEvent = getEarliestEvent();

  if (earliestEvent.first) {
    data_.timeNow = sources_[earliestEvent.second]->getPostTime();
    handleCreationOfNewApplication(earliestEvent.second);

  } else {
    data_.timeNow = handlers_[earliestEvent.second]->getFinishTime();
    handleEndOfHandlerWork(earliestEvent.second, count, numApplication, outputTime);

  }
}

void FunctionalModule::totalGeneratedAppsSimulation(int numSources, int numBufer, int numHandler, const size_t &totalApps) {
  cleanUp();
  postFirstApplications();

  outputFinish outputTime[numHandler];
  for (int i = 0; i < numHandler; i++)
  {
      outputTime[i].numHand = i;
      outputTime[i].finishTime = 0;
  }
  while (true) {

      size_t apps = 0;
    simulationStep(count, totalApps, apps, outputTime);
    //count++;
    /*for (size_t i = 0; i < sources_.size(); ++i) {
      std::cout << "Stats for " << i+ 1 << " source:\n";
      std::cout << "\t generatedAppsCount = " << data_.sourcesData[i].generatedAppsCount << '\n';
      std::cout << "\t refusedAppsCount = " << data_.sourcesData[i].refusedAppsCount << '\n';
      std::cout << "\t acceptedAppsCount = " << data_.sourcesData[i].acceptedAppsCount << '\n';
 }*/
    //size_t apps = 0;
    for (size_t i = 0; i < sources_.size(); ++i) {
      apps += data_.sourcesData[i].generatedAppsCount;

    }


    if (apps >= totalApps) {

      break;
    }
  }
}

