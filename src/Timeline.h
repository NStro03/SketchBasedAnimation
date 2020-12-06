#include <vector>
#include "PlObject.h"
#include<algorithm> // for copy() and assign() 
#include<iterator> // for back_inserter

class Timeline
{
private:
    std::vector<PlObject> timeFrames[11];
    bool isTfFilled[11];

public:
    Timeline(){
        for (int i = 0; i < 11; i++)
            isTfFilled[i] = false;
    }

    void AddTimeFrame(int pos, std::vector<PlObject> PlObjs){
        if(pos>=11 || pos<0){
            std::cout<<"Invalid position to add time frame.\n";
            return;
        }
        
        copy(PlObjs.begin(), PlObjs.end(), back_inserter(timeFrames[pos]));
        
        if(timeFrames[pos].size() > 0){
            isTfFilled[pos] = true;
            std::cout<<"TimeFrame added at position "<<pos<<"\n";
        }
        else
        {
            std::cout<<"Nothing copied to timeFrames[pos].\n";
        }
        
    }

    std::vector<PlObject> getTimeFrame(int pos){
        return timeFrames[pos];
    }
};

