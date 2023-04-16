//
//  main.cpp
//  ConvexHull
//
//  Created by Maria on 13.04.2023.
//


#include <iostream>
#include <stack>
#include <stdlib.h>
#include <vector>
#include <cmath>
using namespace std;
 
struct Point
{
    double x, y;
};

 double orientation(Point p1, Point p2, Point p3) {
     return p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y);
}
// orient > 0 => counter-clockwise; orient < 0 => clockwise

vector<Point> convex_hull(vector<Point>& p) {
    Point p0 = *min_element(p.begin(), p.end(), [](Point a, Point b) {
        return make_pair(a.y, a.x) < make_pair(b.y, b.x);
    });
    sort(p.begin(), p.end(), [&p0](const Point& a, const Point& b) {
        int orient = orientation(p0, a, b);
        if (orient == 0)
            return (pow(p0.x-a.x,2) + pow(p0.y-a.y,2))
                < (pow(p0.x-b.x,2) + pow(p0.y-b.y,2));
        return orient < 0;
    });

    vector<Point> perem;
    for (int i = 0; i < (int)p.size(); i++) {
        while (perem.size() > 1 && orientation(perem[perem.size()-2], perem.back(), p[i]) >= 0)
            perem.pop_back();
        perem.push_back(p[i]);
    }

    return perem;
}
 
double distSq(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}
bool FindDist(Point p1,Point p2, Point p3){
    return (distSq(p1,p2)>=distSq(p1,p3) &&  distSq(p1,p2)>=distSq(p2,p3));
}
bool inter(Point p1,Point p2,Point p3, Point p4){
    double a1 = p2.y - p1.y;
    double b1 = p1.x - p2.x;
    double c1 = a1 * p1.x + b1* p1.y;
    double a2 = p4.y - p3.y;
    double b2 = p3.x - p4.x;
    double c2 = a2 * p3.x + b2* p3.y;
    double det = a1*b2 - a2*b1;

    if(det == 0) return false;
    else{
        Point params = {(b2*c1-b1*c2)/det,(a1*c2 - a2*c1)/det};
        return FindDist(p1, p2, params) && FindDist(p3, p4, params);
    }
}

pair<double,double> Find_Values(vector<Point>& S, char value){
    if(value == 'x'){
        auto values_S = minmax_element(S.begin(),S.end(),[](const Point& a, const Point& b){return a.x <b.x;});
        return {values_S.first->x,values_S.second->x};
    }
    else{
        auto values_S = minmax_element(S.begin(),S.end(),[](const Point& a, const Point& b){return a.y <b.y;});
        return {values_S.first->y,values_S.second->y};
    }

}

bool Intersection(vector<Point>& S1 , vector<Point>& S2){
    pair<double,double> y_1 = Find_Values(S1,'y');
    pair<double,double> y_2 = Find_Values(S2,'y');
    pair<double,double> x_1 = Find_Values(S1, 'x');
    pair<double,double> x_2 = Find_Values(S2, 'x');
    if((y_1.first >= y_2.second) || (y_2.first >= y_1.second) || (x_2.first >= x_1.second) ||(x_1.first >= x_2.second)){
        return false;
    }else
        if((y_1.first > y_2.first && y_1.second < y_2.second) || (y_1.first < y_2.first && y_1.second > y_2.second)){
            return true;
    }else{
        auto p_1 = find_if(S1.begin(), S1.end(), [x_1,x_2,y_1,y_2](const Point& value) {
            return (value.x >= max(x_1.first,x_2.first) &&
                     value.x <= min(x_1.second,x_2.second) &&
                     value.y >= max(y_1.first,y_2.first) &&
                     value.y <= min(y_1.second,y_2.second));
            });
        auto p_2 =  find_if(S2.begin(), S2.end(), [x_1,x_2,y_1,y_2](const Point& value) {
            return (value.x >= max(x_1.first,x_2.first) &&
                     value.x <= min(x_1.second,x_2.second) &&
                     value.y >= max(y_1.first,y_2.first) &&
                     value.y <= min(y_1.second,y_2.second));
            });
        int intersection = 0;
        int dist_1 =int(distance(S1.begin(),p_1))-2;
        int dist_2 = int(distance(S2.begin(),p_2))-2;
        if(p_1 != S1.end() || p_2 != S2.end()){
            S1.push_back(S1[0]);
            S2.push_back(S2[0]);
            for(auto i = max(dist_1,0); i < S1.size()-1; i++){
                if(intersection < 2){
                    for(auto j = max(dist_2,0); j < S2.size()-1; j++){
                        if(intersection < 2){
                            intersection += int(inter(S1[i],S1[i+1],S2[j],S2[j+1]));
                        }
                    }
                }
            }
        }
        return intersection == 2;
    }
}


int main()
{

    vector<Point> points_2 = {{1,1}, {4,1}, {0, 3}, {4,3}};
    vector<Point> points_1 = {{2,1}, {2, 5}, {6, 1}, {6, 5}};

    vector<Point> S1 = convex_hull(points_1);
    vector<Point> S2 = convex_hull(points_2);

    bool intersection = Intersection(S1,S2);
    
    cout <<boolalpha<<intersection <<endl;

    return 0;
}
