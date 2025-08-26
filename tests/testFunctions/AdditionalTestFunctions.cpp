#include "AdditionalTestFunctions.hpp"
bool equalMatchesList(MatchesList input,MatchesList expected) {
    if (input.matches.size() != expected.matches.size()) {
        return false;
    }
    int valuesFound=0;
    for (auto value:expected.matches) {
        for (int i=0; i<input.matches.size(); i++) {
            if (input.matches[i] == value) {
                valuesFound++;
                input.matches.erase(input.matches.begin()+i);
            }
        }

    }
    return valuesFound==expected.matches.size();
}


bool equalVectorPtr(std::vector<int*> input,std::vector<int*> expected) {
  if (input.size() != expected.size()) {
    return false;
  }
  int valuesFound=0;
  for (auto value:expected) {
    for (int i=0; i<input.size(); i++) {
      if (input[i] == value) {
        valuesFound++;
        input.erase(input.begin()+i);
      }
    }

  }
  return valuesFound==expected.size();
}
