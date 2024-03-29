
#ifndef SMO_ADDITIONSTRATEGY_HPP
#define SMO_ADDITIONSTRATEGY_HPP

#include <vector>
#include <memory>

#include "Application.hpp"

class AdditionStrategy {
public:
  //virtual bool addApplication(std::vector<std::shared_ptr<Application>> &applications,
                             // const std::shared_ptr<Application> &applicationToAdd) = 0;
    virtual bool addApplication(std::vector<std::shared_ptr<Application>>
                                &applications, const std::shared_ptr<Application> &applicationToAdd) = 0;
    virtual size_t getPointer(size_t p) = 0;
//public:
   //size_t elementPointer_;
};

#endif //SMO_ADDITIONSTRATEGY_HPP
