#ifndef SEARCH_SEARCH_H
#define SEARCH_SEARCH_H

#include <utility>
#include <vector>

#include "../action_schema.h"
#include "../action.h"
#include "../state.h"
#include "../structures.h"
#include "../task.h"
#include "../successor_generators/successor_generator.h"
#include "../heuristics/heuristic.h"

#define SOLVED 0
#define NOT_SOLVED 1
#define DEBUG_GRACEFUL_EXIT 0

class Node {
public:
    Node(int g, int h, int id) : g(g), h(h), id(id) {}

    int g;
    int h;
    int id;
};

struct NodeComparison {
    bool operator()(const Node &n, const Node &m) const {
        if (n.h != m.h) return n.h > m.h;
        else return n.g < m.g;
    }
};

class Search {

public:
    Search() = default;

    int getNumberExploredStates() const;

    int getNumberGeneratedStates() const;

    virtual const int search(const Task &task,
                             SuccessorGenerator *generator,
                             Heuristic &heuristic) const = 0;

    bool is_goal(const State &state, const GoalCondition &goal) const;

    void extract_goal(int state_counter, int generations, State state,
                      unordered_map<int, pair<int, Action>> &cheapest_parent,
                      unordered_map<State, int, boost::hash<State>> &visited,
                      unordered_map<int, State> &index_to_state, const Task &task) const;

    static void extract_plan(unordered_map<int, pair<int, Action>> &cheapest_parent,
                             State state,
                             unordered_map<State, int, boost::hash<State>> &visited,
                             unordered_map<int, State> &index_to_state,
                             const Task &task);


    std::vector<Action> plan;
private:
    int number_explored_states = 0;
    int number_generated_states = 0;


};


#endif //SEARCH_SEARCH_H
