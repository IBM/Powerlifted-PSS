#ifndef SEARCH_STRUCTURES_H
#define SEARCH_STRUCTURES_H

#include "hash_structures.h"

#include <string>
#include <utility>
#include <unordered_set>
#include <vector>

/**
 * @brief GroundAtom is an alias for vector of integers. It is represented
 * as a list of object indices.
 */
typedef std::vector<int> GroundAtom;


/**
 * @brief Represent a parameter for a given action schema.
 *
 * @var name: Name of the parameter in the action schema definition.
 * @var index: Index of this parameter in the list of parameters in the schema.
 * @var type: Type related to this parameter.
 * @var seed: bool indicating if the parameter is a seed or not.
 */
struct Parameter {
    Parameter(std::string name, int index, int type, bool seed)
        : name(std::move(name)), index(index), type(type), seed(seed) {}

    std::string name;
    int index;
    int type;
    bool seed;

    int get_index() {
        return index;
    }
};


/**
 * @brief Implements an argument composing an atom. It can be a free
 * variable or a constant.
 *
 * @var index: If the argument is a constant, then it represents the index of the object,
 * otherwise it represents the index of the free variable in the parameters of the action
 * schema.
 * @var constant: Indicates whether the argument is a constanst or not (free variable, then).
 *
 */
class Argument {
    int index;
    bool constant;
public:
    Argument(int index, bool constant) : index(index), constant(constant) {}

    int get_index() const {
        return index;
    }

    bool is_constant() const {
        return constant;
    }
};

/**
 * @brief Represent a relevant LMG that can be used to infer the non-seed parameters.
 *
 * @var parameters: Index of the non-seed parameter in the schema.
 * @var precondition: Index of the precondition in the schema.
 * @var counted_argument: Indexes of  argument in the precondition atom t.
 * @var fixed_argument: Indexes of the argument in the precondition atom.
 */
struct RelevantLMG {
    RelevantLMG() = default;
    RelevantLMG(int precondition, std::vector<int> parameters,
                std::vector<int> counted_argument, std::vector<int> fixed_argument)
        : precondition(precondition), parameters(parameters), counted_argument(counted_argument),
          fixed_argument(fixed_argument) {}

    int precondition;
    std::vector<int> parameters;
    std::vector<int> counted_argument;
    std::vector<int> fixed_argument;
};



/**
 * @brief A relation is a "table" with set of tuples corresponding to some
 * predicate in a state.
 *
 * @var predicate_symbol: Indicates its corresponding predicate.
 * @var tuples: Set of tuples (vectors) corresponding to the ground atoms in this relation.
 *
 */
struct Relation {
    Relation() = default;
    Relation(int predicate_symbol,
             std::unordered_set<GroundAtom, TupleHash> &&tuples)
        : predicate_symbol(predicate_symbol),
          tuples (std::move(tuples)) {}

    Relation(const Relation &) = default;


    bool operator==(const Relation &other) const {
        return predicate_symbol == other.predicate_symbol && tuples == other.tuples;
    }

    int predicate_symbol{};
    std::unordered_set<GroundAtom, TupleHash> tuples;
};

#endif //SEARCH_STRUCTURES_H
