#include "fsm_state_interface.h"
#include "swganh_core/object/object.h"

#include <math.h>

using namespace glm;
using namespace swganh::spawn;
using namespace swganh::object;

/*
bool lineIntersectsLine(const Point& start1, const Point& end1, const vec3& start2, const vec3& end2)
{
	//Protect against division by 0
	double denom = ((end1.x() - start1.x()) * (end2.y - start2.y)) - ((end1.y() - start1.y()) * (end2.x - start2.x));
	if(denom == 0)
		return false;

	//Calculate the numerator for the line equations
	double numer = ((start1.y() - start2.y) * (end2.x - start2.x)) - ((start1.x() - start2.x) * (end2.y - start2.y));
	double numer2 = ((start1.y() - start2.y) * (end1.x() - start1.x())) - ((start1.x() - start2.x) * (end1.y() - start1.y()));

	//If the intersect outside the range we care about (most likely case)
	//Then we return false
    double r = numer / denom;
    double s = numer2 / denom;
	if ((r < 0 || r > 1) || (s < 0 || s > 1))
            return false;

	//All other cases are true
	return true;
}

bool lineIntersectsPolygon(const CollisionBox& polygon, const vec3& pt1, const vec3& pt2)
{
	auto itr = polygon.outer().cbegin();
	auto next = polygon.outer().cbegin()+1;
	auto end = polygon.outer().cend();

	for(; itr != end;)
	{
		//If the next is the end, 
		//we need to make it the beginning for this final edge
		if(next == end)
			next = polygon.outer().cbegin();

		//If the lines intersect, we know this polygon intersects
		if(lineIntersectsLine(*itr, *next, pt1, pt2))
			return true;

		//Prepare for the next iteration
		++itr; ++next;
	}
	return false;
}

int heuristicCostEstimate(const vec3& start, const vec3& end)
{
	//Return straightline distance
	return (int)sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2));
}

std::vector<vec3> generateValidNeighbors(std::shared_ptr<Object>& object, const vec3& point)
{
	std::vector<vec3> result;
	
	//Generate the basic N, S, E, W, NE, NW, SW, SE points
	for(int x=-1; x < 2; ++x)
	{
		for(int y=-1; y < 2; ++y)
		{
			bool is_valid = true;
			vec3 point2(point.x + PATH_STEP_CONSTANT*x, point.y + PATH_STEP_CONSTANT*y, point.z);

			//Make sure they don't intersect anywhere
			object->ViewAwareObjects([&is_valid, &point2] (std::shared_ptr<Object> object) {
				if(is_valid)
					is_valid = lineIntersectsPolygon(object->GetLocalCollisionBox(), object->GetAbsolutePosition(), point2);
			});

			if(is_valid)
				result.push_back(point2);
		}
	}

	return result;
}

struct vectorLess
{
	bool operator() (const vec3& a, const vec3& b) const
	{
		if(a.x == b.x)
		{
			if(a.y == b.y)
			{
				return a.z < b.z;
			}
			return a.y < b.y;
		}
		return a.x < b.x;
	}
};

std::vector<vec3> reconstructPath(const std::map<vec3, vec3, vectorLess>& came_from, const vec3& point)
{
	std::vector<vec3> result;

	auto itr = came_from.find(point);
	if(itr != came_from.end())
	{
		std::vector<vec3> sublist = reconstructPath(came_from, itr->second);
		result.insert(result.begin(), sublist.begin(), sublist.end());
	}
	result.push_back(point);

	return result;
}

bool FsmStateInterface::WithinLineOfSight(std::shared_ptr<Object> object, glm::vec3& point2)
{
	bool line_is_valid = true;
	object->ViewAwareObjects([&] (std::shared_ptr<Object> object) {
		if(line_is_valid)
		{
			line_is_valid = lineIntersectsPolygon(object->GetLocalCollisionBox(), object->GetAbsolutePosition(), point2);
		}
	});
	return line_is_valid;
}

std::vector<glm::vec3> FsmStateInterface::FindPath(std::shared_ptr<swganh::object::Object> object, glm::vec3& destination)
{
	vec3& start = object->GetAbsolutePosition();

	//Create the necessary data structures
	std::map<vec3, vec3, vectorLess> came_from;
	std::set<vec3, vectorLess> closedset, openset;
	std::multimap<int, vec3> fscore_queue;
	std::map<vec3, int, vectorLess> g_score, f_score;

	//Insert the initial point
	openset.insert(start);
	g_score.insert(std::make_pair(start, 0));

	int startHeuristic = heuristicCostEstimate(start, destination);
	f_score.insert(std::make_pair(start, startHeuristic));
	fscore_queue.insert(std::make_pair(startHeuristic, start));
	
	//Perform the loop
	while(openset.size() > 0)
	{
		//Get the point with the lowest f_score value
		vec3& current = fscore_queue.begin()->second;
		fscore_queue.erase(fscore_queue.begin());

		//If we're done lets return the proper path
		if(current == destination)
			return reconstructPath(came_from, destination);

		closedset.insert(current);

		for(auto& neighbor : generateValidNeighbors(object, current))
		{
			auto closed_itr = closedset.find(neighbor);
			if(closed_itr == closedset.end())
			{
				int tenative_g_score = g_score[current] + 1;
				auto open_itr = openset.find(neighbor);
				if(open_itr == openset.end() || tenative_g_score < g_score[neighbor])
				{
					if(open_itr == openset.end())
					{
						openset.insert(neighbor);
					}

					came_from[neighbor] = current;
					g_score[neighbor] = tenative_g_score;
					f_score[neighbor] = tenative_g_score + heuristicCostEstimate(neighbor, destination);
				}
			}
		}
	}

	//Return failure
	return std::vector<vec3>();
}
*/