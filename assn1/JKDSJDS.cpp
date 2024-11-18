//
// Created by lasha on 10/3/24.
//

bool bs( int val){
    vector<int> v;
     int size = v.size();

    int l = 0;
    int r = size - 1;
    int idx;
    while(l < r){
      idx = (r - l) /2;
       int curr = v[idx];
       if(val < curr){
         r = idx;
         }else if(val > curr){
           l = idx;
         }else{
           return true;
         }
    }
return false;
}