
#ifndef SMO_FUNCTIONALMODULE_HPP
#define SMO_FUNCTIONALMODULE_HPP

#include <memory>
#include <iostream>
#include <list>

#include "step.hpp"
#include "Handler.hpp"
#include "Buffer.hpp"
#include "Source.hpp"
static int count = 1;
//std::list<StepStructure> StepList;

struct sourceData {
  sourceData():
    refusedAppsCount(0),
    acceptedAppsCount(0),
    generatedAppsCount(0),
    bufferingTime(0),
    handlingTime(0)
  {}

  void cleanUp(){
    refusedAppsCount = 0;
    acceptedAppsCount = 0;
    generatedAppsCount = 0;
    bufferingTime = 0;
    handlingTime = 0;
  }

  size_t refusedAppsCount;
  size_t acceptedAppsCount;
  size_t generatedAppsCount;

  double bufferingTime;
  double handlingTime;
};

 struct outputFinish {

   size_t numSource;
   size_t numHand;
   double finishTime;
};

struct handlerData {
  handlerData():
    workingTime(0)
  {}

  void cleanUp() {
    workingTime = 0;
  }

  double workingTime;
};

struct simulationData {
  simulationData(const size_t& sourcesCount, const size_t& handlersCount)
  {
    timeNow = 0;
    sourcesData = std::vector<sourceData>(sourcesCount);
    handlersData = std::vector<handlerData>(handlersCount);
  }

  void cleanUp() {
    for (size_t i = 0; i < sourcesData.size(); ++i) {
      sourcesData[i].cleanUp();
    }
    for (size_t i = 0; i < handlersData.size(); ++i) {
      handlersData[i].cleanUp();
    }
    timeNow = 0;
  }

  double getAverageTimeInSystem(const size_t& sourceIndex) {
    return getAverageHandlingTime(sourceIndex) + getAverageBufferingTime(sourceIndex);
  }

  double getAverageHandlingTime(const size_t& sourceIndex) {
    return sourcesData[sourceIndex].handlingTime / sourcesData[sourceIndex].acceptedAppsCount;
  }

  double getAverageBufferingTime(const size_t& sourceIndex) {
    return sourcesData[sourceIndex].bufferingTime / sourcesData[sourceIndex].acceptedAppsCount;
  }

  double getProbabilityOfFailure(const size_t& sourceIndex) {
    return (double)sourcesData[sourceIndex].refusedAppsCount / sourcesData[sourceIndex].generatedAppsCount;
  }

  double getEmploymentRate(const size_t& handlerIndex) {
    double employmentRate = handlersData[handlerIndex].workingTime / timeNow;
    if (employmentRate > 1.0) {
      return 1;
    } else {
      return employmentRate;
    }
  }

  std::vector<sourceData> sourcesData;
  std::vector<handlerData> handlersData;
   double timeNow;
};

class FunctionalModule {
public:
  explicit FunctionalModule(std::vector<std::shared_ptr<Source>> sources,
                   std::shared_ptr<Buffer> buffer,
                   std::vector<std::shared_ptr<Handler>> handlers);

  void totalGeneratedAppsSimulation(int numSources, int numBufer, int numHandlerconstconst,const size_t &totalApps);
  void simulate(int numSources, int numBufer, int numHandlerconstconst, size_t &steps);
  void simulationStep(int h, size_t count, size_t numApplication, outputFinish *outputTime);
  void cleanUp();

  simulationData data_;
  std::vector<std::shared_ptr<Handler>> handlers_;


private:
  std::vector<std::shared_ptr<Source>> sources_;
  std::shared_ptr<Buffer> buffer_;
  size_t handlerPointer_;
  std::vector<outputFinish> outputTime;

  int getNextHandler(const double &timeNow);

  void postFirstApplications();

  int getEarliestSourceIndex();
  int getEarliestHandlerIndex();
  std::pair<bool, int> getEarliestEvent();

  void handleCreationOfNewApplication(const size_t &sourceGeneratedApplication);
  void handleEndOfHandlerWork(const size_t &handlerFinishedWork, int apps, int numApplication, outputFinish *outputTime) ;
};


#endif //SMO_FUNCTIONALMODULE_HPP
