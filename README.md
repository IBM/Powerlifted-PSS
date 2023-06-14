# Powerlifted-PSS Planner

Powerlifted-PSS is a domain-independent classical lifted planner that uses seed parameters of an action to efficiently identify the applicable actions.


This code uses [Powerlifted Planner](https://github.com/abcorrea/powerlifted) for lifted planning. The majority of the code is reproduced from their repository with required alterations to accomodate for the parameter seeds.

## Usage

The `powerlifted.py` script solves a PDDL task provided as input.

Example usage:

```$ python ./powerlifted.py -d ./dev/domains/blocks/domain.pddl -i ./dev/domains/blocks/probBLOCKS-4-0.pddl -s astar -e ff -g  yannakakis --state sparse --use-parameter-seed --build```


Here, `--use-parameter-seed` is an optional parameter. When provided, the script would obtain the [parameter seeds](https://github.com/IBM/Parameter-Seed-Set) for each lifted action and use that information to generate successor states. By default, the script solved PDDL task without using the parameter seed information. Currently, the `--use-parameter-seed` flag is only supported for `yannakakis` generator. 

## Requirements for Planning with Parameter Seeds 

 - A C++17-compliant compiler
 - CMake 3.9+
 - Python 3.5+
 - Boost
 - [Tarski](https://tarski.readthedocs.io/en/latest/)
 - [Parameter Seed Set](https://github.com/IBM/Parameter-Seed-Set)


## Reproducibility 

To replicate the results from the main paper, compare the table sizes from the logs. Table sizes are recorded for every state when applicable actions are queried.


For instance,
```
Table size before join: clear : 1
Table size before join: holding : 1
Table size before join: type@object : 4
Table size before join: type@object : 4
```



## Citation

If you build on this code or the ideas of this paper, please use the following citation.

```
@inproceedings{KokelLKSS23,
 	title={Action Space Reduction for Planning Domains},
 	journal={IJCAI},
 	author={Kokel, Harsha and Lee, Junkyu and Katz, Michael and Srinivas, Kavitha and Sohrabi, Shirin},
 	year={2023}
}
```


## LICENSE

This code is licensed under the [GNU GENERAL PUBLIC LICENSE Version 3 (GPL v 3.0)](./LICENSE).
