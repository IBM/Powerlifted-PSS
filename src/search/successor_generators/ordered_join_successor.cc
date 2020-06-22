#include "ordered_join_successor.h"

#include "../action.h"
#include "../task.h"

#include "../database/table.h"
#include "../database/hash_join.h"
#include "../states/state.h"

#include <cassert>
#include <vector>

using namespace std;

template <typename OrderT>
bool OrderedJoinSuccessorGenerator<OrderT>::parse_precond_into_join_program(
    const PrecompiledActionData &adata, const DBState &state, std::vector<Table>& tables)
{
    // TODO This is highly inefficient. See issue #6:
    //      https://github.com/abcorrea/powerlifted/issues/6
    bool res = GenericJoinSuccessor::parse_precond_into_join_program(adata, state, tables);
    if (!res) return false;

    return true;
}

template <typename OrderT>
OrderedJoinSuccessorGenerator<OrderT>::OrderedJoinSuccessorGenerator(const Task &task)
    : GenericJoinSuccessor(task) {
    vector<pair<int,int>> to_sort;
    int a_idx = 0;
    assert(action_data.size() == task.actions.size());
    precondition_to_order.resize(action_data.size());
    for (auto &a : action_data) {
        to_sort.clear();
        int k = 0;
        for (const auto & i : a.precompiled_db) {
            /*const Atom &atom = a.relevant_precondition_atoms[i];
            int n_free_vars = 0;
            for (const auto &c : atom.arguments)
                if (!c.constant) ++n_free_vars;*/
            to_sort.emplace_back(i.tuple_index.size(), k++);
        }
        std::sort(to_sort.begin(), to_sort.end(), OrderT());
        for (auto p : to_sort) {
            int idx = p.second;
            precondition_to_order[a_idx].push_back(idx);
        }
        ++a_idx;
    }
}

template<typename OrderT>
Table OrderedJoinSuccessorGenerator<OrderT>::instantiate(const ActionSchema &action,
                                                         const DBState &state) {

    vector<int> order = precondition_to_order[action.get_index()];

    if (action.is_ground()) {
        throw std::runtime_error("Shouldn't be calling instantiate() on a ground action");
    }

    const auto& actiondata = action_data[action.get_index()];

    vector<Table> tables(0);
    auto res = parse_precond_into_join_program(actiondata, state, tables);

    if (!res) return Table::EMPTY_TABLE();

    assert(!tables.empty());
    assert(tables.size() == actiondata.relevant_precondition_atoms.size());

    Table &working_table = tables[order[0]];
    for (size_t i = 1; i < tables.size(); ++i) {
        hash_join(working_table, tables[order[i]]);
        // Filter out equalities
        filter_inequalities(action, working_table);
        if (working_table.tuples.empty()) {
            return working_table;
        }
    }

    return working_table;

    return GenericJoinSuccessor::instantiate(action, state);
}

// explicit template instantiations
template class OrderedJoinSuccessorGenerator<OrderTable>;
template class OrderedJoinSuccessorGenerator<InverseOrderTable>;