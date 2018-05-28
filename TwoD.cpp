//A 2D Class demonstration
#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <random>
using namespace std;

//Class to represent 2D points
class Point2D {
private:
	//Function for question 2, customized comparator with the c++ built in sort algorithm
	static bool OriCompare(Point2D i, Point2D j) { return (i.dist2o() > j.dist2o()); }

	//Function which  returns the twice signed area between three points, part of question 3
	double TwiceSignedArea(Point2D a,  Point2D b, Point2D c) {
		return	((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x));
	}

public:
	int x;
	int y;

	//Initalisation
	Point2D(int a, int b) { x = a; y = b; };
	Point2D() { x = 0; y = 0; }

	//Distance between 2 points, function for question 1, basic pythagoras 
	double dist(Point2D other) {
		int x1 = x - other.x;
		int y1 = y - other.y;
		return sqrt(x1*x1 + y1*y1);
	}

	//Returns the distance to origin, part of question 2, may as well put into public
	double dist2o() { return dist(Point2D()); };

	//Sorts the array of 2D points by distance from origin, question 2
	void sortByDist2o(Point2D points[], int length){ return std::sort(points, points + length , OriCompare); }

	/*
	Determines the orientation of two connecting vectors defined by three points 
	0 for colinear, 1 for anti-clockwise, -1 for clockwise, question 3
	*/
	int orientation(Point2D& a, Point2D& b, Point2D& c) { 
		double val = TwiceSignedArea(a, b, c);
		if (val == 0)	return 0;
		return (val > 0) ? 1 : -1;
	}

	//Given three points a, b, c checks whether point a lines on the line segment 'bc', part of question 4
	bool onSegment(Point2D& a, Point2D& b, Point2D& c) {
		return((a.x <= max(b.x, c.x) && a.x >= min(b.x, c.x))
			&& (a.y <= max(b.y, c.y) && a.y >= min(b.y, c.y)));
	}
};


//Structure of segments, built upon the Point2D class
struct Segment {
	Point2D a, b;
	/*
	Function that determines whether the two segments, formed by s1 and s2 intersect
	Uses the orientation method below, modified to handle colinear cases
	Reference: www.cs.princeton.edu/~rs/AlgsDS07/16Geometric.pdf
	*/
	bool doIntersect(Segment& other) {
		int o1 = a.orientation(a, b, other.a);	int o2 = a.orientation(a, b, other.b);
		int o3 = a.orientation(other.a, other.b, a);	int o4 = a.orientation(other.a, other.b, b);
		//General case where segment (p1,q1) and segment (p2,q2) are not colinear
		if (o1 != o2 && o3 != o4)	return true;
		//Special cases when they are  colinear
		if (o1 == 0 && a.onSegment(other.a, a, b)) return true;
		if (o2 == 0 && a.onSegment(other.b, a, b)) return true;
		if (o3 == 0 && a.onSegment(a, other.a, other.b)) return true;
		if (o4 == 0 && a.onSegment(b, other.a, other.b)) return true;
		return false;
	}
};

/*
Checks whether a point p is inside a polygon of n number of vertices
Complexity O(n), utilises the ray-casting method found on wikipedia
Question 5
*/
bool isInside(Point2D p, Point2D polygon[], int n) {
	//at least 3 verticies in polygon[]
	if (n < 3) return false;
	//create a horizontal line from input point
	Point2D extreme = { INT_MAX, p.y };

	int count = 0, i = 0;
	do {
		int next = (i + 1) % n;
		Segment s1 = { polygon[i], polygon[next] }, s2 = { p, extreme };
		//check if the horizontal line intersects with the line segments formed from one to its next neighbour
		if (s1.doIntersect(s2)) {
			//If the point is colinear with the segment then check whether it is on the segment, if so then return true
			if (p.orientation(polygon[i], p, polygon[next]) == 0)
				return p.onSegment(p, polygon[i], polygon[next]);
			count++;
		}
		i = next;
	} while (i != 0);
	return count & 1; //bitwise test, true if counter is odd
}



int main()
{
	//Question 1 Demo, prints out distance between two points
	Point2D a = { 2,2 }, b = { 7,7 }, c = { 6,6 };
	cout << "Distance between a and b is " << a.dist(b) << endl;

	//Question 2 Demo, input a random array of 2D point objects, output an array of objects sorted from origin
	const int length = 100;
	Point2D points[length];
	//An Array of randomly generated 2D points as input
	cout << endl << "Input Random Array: " << endl;
	for (int i = 0; i < length; i++) {
		points[i] = { rand() % (length - 1), rand() % (length - 1) };
		cout << '(' << points[i].x << ',' << points[i].y << ')' << '	';
	}
	
	points->sortByDist2o(points, length);
	//Outputs the sorted array
	cout << endl << "Sorted Array: " << endl;
	for (int i = 0; i < length; i++) {
		cout << '(' << points[i].x << ',' << points[i].y << ')' << '	';
	}
	cout << endl;

	//Question 3 determine the orientation of line segments connected by points abc
	cout << endl << "Determine orientation of ab, bc; 0 for colinear, 1 for ACW, -1 for CW: " << endl << a.orientation(a, b, c) << endl;

	//Question 4 testing:  over lapping colinear case, non over lapping colinear case, 
	//non-colinear intersecting and non colinear non intersecting cases
	cout << endl << "Question 4, two segments intersection" << endl << "Testing cases:" << endl;
	Point2D a1 = { 1,1 }, a2 = { 5,5 }, b1 = { 4,4 }, b2 = { 6,6 };
	Segment s1 = { a1,a2 }, s2 = { b1,b2 };

	bool val = s1.doIntersect(s2);
	cout <<"Over lapping colinear case: "<< val << endl;


	a1 = { 1,1 }, a2 = { 2,2 }, b1 = { 3,3 }, b2 = { 4,4 };
	s1 = { a1,a2 }, s2 = { b1,b2 };
	val = s1.doIntersect(s2);
	cout << "non Over lapping colinear case: " << val << endl;


	a1 = { 10,0 }, a2 = { 0,10 }, b1 = { 0,0 }, b2 = { 10,10 };
	s1 = { a1,a2 }, s2 = { b1,b2 };
    val = s1.doIntersect(s2);
	cout << "Over lapping non colinear case: " << val << endl;


	a1 = { -5,5 }, a2 = { 0,0 }, b1 = { 1,1 }, b2 = { 10,10 };
	s1 = { a1,a2 }, s2 = { b1,b2 };
	val = s1.doIntersect(s2);
	cout << "non Over lapping non colinear case: " << val << endl;
	cout << endl;

	//Question 5, checking whether a point is within a polygon
	Point2D point = { 1,3 };
	Point2D weird[] = { {0,0}, { 2,0 }, { 2,2 }, { 10,10 }, {4,6},{2,10} };
	int n = sizeof(weird) / sizeof(weird[0]);
	bool test = isInside(point, weird, n);
	cout <<"Checking whether a point is inside a polygon: " <<test <<endl;

}