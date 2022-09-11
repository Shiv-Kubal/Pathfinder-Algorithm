#include <cstring>
#include <iostream>
#include <array>

#include "math.h"

#include <chrono>
#include <stack>
#include <tuple>
#include <queue>
#include <set>

using namespace std;

typedef pair<int, int> Coordinate;
typedef tuple<double, int, int> Tuple;


// A Function to check if the given cell is blocked

template <size_t ROW, size_t COL>
bool checkIfBlocked(const array<array<int, COL>, ROW>& grid,
				const Coordinate& point)
{
	// Returns true if the cell is not blocked else false
	return grid[point.first][point.second] == 1;
}

// A  Function to check whether destination cell has been reached or not

bool checkIfDestination(const Coordinate& position, const Coordinate& dest)
{
	return position == dest;
}

struct cell {
	Coordinate parent;
	double f, g, h;
	cell(): parent(-1, -1), f(-1), g(-1), h(-1){
	    
	}
};

// A Function to calculate the 'h' heuristics.
double calculateHValue(const Coordinate& src, const Coordinate& dest)
{
	return sqrt(pow((src.first - dest.first), 2.0) + pow((src.second - dest.second), 2.0));
}

// A Function to trace the Route from the source to destination
template <size_t ROW, size_t COL>
void traceRoute(
	const array<array<cell, COL>, ROW>& cellValues,
	const Coordinate& dest)
{
    printf("[");
	stack<Coordinate> Route;
	int steps = -1;

	int row = dest.first;
	int col = dest.second;
	Coordinate next_node = cellValues[row][col].parent;
	do {
		Route.push(next_node);
		next_node = cellValues[row][col].parent;
		row = next_node.first;
		col = next_node.second;
	} while (cellValues[row][col].parent != next_node);

	Route.emplace(row, col);
	while (!Route.empty()) {
		Coordinate p = Route.top();
		Route.pop();
		steps++;
		printf(" (%d,%d), ", p.first, p.second);
	}
	printf("]\n");
	printf("Number of steps taken: %d.", steps);
}

// A Function using A* Search Algorithm to find the shortest Route between a given
// source cell to a destination cell.

template <size_t ROW, size_t COL>
void aStar(const array<array<int, COL>, ROW>& grid,
				const Coordinate& src, const Coordinate& dest)
{

    if (!checkIfBlocked(grid, src) || !checkIfBlocked(grid, dest)) {
        printf("Source or the destination is blocked\n");
        return;
    }

	// Create a closed list and implemented as a boolean 2D array.
	
	bool closed_list[ROW][COL];
	memset(closed_list, false, sizeof(closed_list));

	array<array<cell, COL>, ROW> cellValues;

	int i, j;
	i = src.first, j = src.second;
	
	cellValues[i][j].f = 0.0;
	cellValues[i][j].g = 0.0;
	cellValues[i][j].h = 0.0;
	
	cellValues[i][j].parent = { i, j };

	/*
	Create an open list having information as-
	<f, <i, j>>
    f = g + h*/
	
	std::priority_queue<Tuple, std::vector<Tuple>, std::greater<Tuple> > open_list;

	open_list.emplace(0.0, i, j);
	
	while (!open_list.empty()) {
		const Tuple& p = open_list.top();
		i = get<1>(p); // second element of tuple
		j = get<2>(p); // third element of tuple

		open_list.pop();
		closed_list[i][j] = true;
		
		//Generate the possible 8 successor of this cell.
					
		for (int add_x = -1; add_x <= 1; add_x++) {
			for (int add_y = -1; add_y <= 1; add_y++) {
				Coordinate nearest(i + add_x, j + add_y);
					// Checking if the destination cell is the same
					// as the current successor
					if (checkIfDestination(nearest,dest)) { // Set the Parent of the destination cell
						cellValues[nearest.first][nearest.second].parent = { i, j };
						traceRoute(cellValues, dest);
						return;
					}
					// Check if successor is on the closed list or is blocked....
					else if (!closed_list[nearest.first][nearest.second]
							&& checkIfBlocked(grid,nearest)) {
						double gNew, hNew, fNew;
						gNew = cellValues[i][j].g + 1.0;
						hNew = calculateHValue(nearest,dest);
						fNew = gNew + hNew;

					
						if (cellValues[nearest.first][nearest.second].f == -1
							|| cellValues[nearest.first][nearest.second].f > fNew) {
							open_list.emplace(fNew, nearest.first,nearest.second);

							
							cellValues[nearest.first][nearest.second].g = gNew;
							cellValues[nearest.first][nearest.second].h= hNew;
							cellValues[nearest.first][nearest.second].f= fNew;
							cellValues[nearest.first][nearest.second].parent = { i, j };
						}
					}
			}
		}
	}

	// Bonus Section: failing to reach the delivery point due to blockages.
	printf("Unable to reach delivery point.\n");
}

int main()
{
	/*
	1--> The cell is not blocked
	0--> The cell is blocked */
	
	array<array<int, 10>, 10> grid{
{ { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 } },
  { { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 } },
  { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } } }
	};

	Coordinate src(0, 0);

	Coordinate dest(9, 9);

	aStar(grid, src, dest);

	return 0;
}


