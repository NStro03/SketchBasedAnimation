#include <vector>
#include "PlObject.h"
#include<algorithm> // for copy() and assign() 
#include<iterator> // for back_inserter

class Timeline
{
private:
    std::vector<PlObject> timeFrames[11];
    bool isTfFilled[11];
    int pixelObjectMap[11][961][721];

public:
    Timeline(){
        for (int i = 0; i < 11; i++){
            isTfFilled[i] = false;
        }
    }
    bool isEmpty(int pos){
        return !isTfFilled[pos];
    }

    void AddTimeFrame(int pos, std::vector<PlObject> PlObjs){
        if(pos>=11 || pos<0){
            std::cout<<"Invalid position to add time frame.\n";
            return;
        }
        timeFrames[pos].clear();
        copy(PlObjs.begin(), PlObjs.end(), back_inserter(timeFrames[pos]));
        
        if(timeFrames[pos].size() > 0){
            isTfFilled[pos] = true;
            // std::cout<<"TimeFrame added at position "<<pos<<"\n";
        }
    }
    void setPixelBuffer(int Time){
        std::vector<PlObject> PlObjects = timeFrames[Time];
        for(int j=0; j < PlObjects.size(); j++){
            for(int i=0; i < PlObjects[j].getSize(); i++){
                ImVec2 t;
                t.x = PlObjects[j].getPoint(i).x;
                t.y = PlObjects[j].getPoint(i).y;
                pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] = j+1;
            }
        }
    }
    int getObjectidonTime(int x, int y,int Time){
        int i = x-2, s=0,j=y-2;
        for(;i<=x+2;i++){
            for(;j<=y+2;j++)
                if(pixelObjectMap[Time][i][j]>s)
                    s=pixelObjectMap[Time][i][j];
        }
        return s;
    }
    std::vector<PlObject> getTimeFrame(int pos){
        int p = pos;
        if(isTfFilled[pos]){
            return timeFrames[pos];
        }
        while(p>0 && !isTfFilled[p-1]){
            p--;
        }
        AddTimeFrame(pos, timeFrames[p-1]);
        setPixelBuffer(pos);
        return timeFrames[pos];
    }
    void saveSelectedOnTime(int Time, int selected, PlObject selectedPlObj){
        int n = selectedPlObj.getSize();
        std::vector<PlObject> PlObjects = timeFrames[Time];
        for(int i=0; i<PlObjects[selected-1].getSize(); i++){
            ImVec2 t = PlObjects[selected-1].getPoint(i);
            if(pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] == selected){
                pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] = 0;
            }
            
        }

        if(!PlObjects[selected-1].clearPoints())
            std::cout<<"Not able to clear object points.";
        
        for(int i=0; i<n; i++){
            ImVec2 t;
            t.x = selectedPlObj.getPoint(i).x;
            t.y = selectedPlObj.getPoint(i).y;
            PlObjects[selected-1].addPoint(t);
            pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] = selected;
        }
        timeFrames[Time] = PlObjects;
    }
};

