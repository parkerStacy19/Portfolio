# Zebra Puzzle Solver

This Prolog project implements an expert system to solve the Zebra Puzzle, demonstrating advanced logic programming techniques. The solution uses constraint satisfaction and backtracking to deduce the correct arrangement of houses, nationalities, drinks, cigarettes, and pets based on the 15 given clues.

A screenshot of the solution and several queries to the expert system are found shown in the attached screenshot.

Context for the Zebra Puzzle can be found here: https://en.wikipedia.org/wiki/Zebra_Puzzle

## Code Highlights

### Data Respresentation

Defines the solution as a list of structures. The variables will be assigned specific values during constraint satisfaction.

```cpp
zebra_puzzle(Solution) :-
    Solution = [
        house(Color1, Nationality1, Drink1, Cigarette1, Pet1),
        house(Color2, Nationality2, Drink2, Cigarette2, Pet2),
        house(Color3, Nationality3, Drink3, Cigarette3, Pet3),
        house(Color4, Nationality4, Drink4, Cigarette4, Pet4),
        house(Color5, Nationality5, Drink5, Cigarette5, Pet5)
    ],
```

### Declarative Constraint Satisfaction

Instead of specifying how to find adjacent houses, we declare what adjacency means. Prolog uses backtracking to find assignments that satisfy all constraints simultaneously.

```cpp
next_to(A, B, List) :-
    append(_, [A, B|_], List);
    append(_, [B, A|_], List).
```
---

## Run instructions

This project comes with a ```.devcontainer``` folder that contains the necessary configurations and packages to run the program. Open the project folder in Visual Studio Code using the Dev Containers Extension. When prompted, reopen the folder in a dev container.

Once the container is running, run the program using:

```bash
swipl -s puzzle_solver.pl
```

At the prompt, you can view the full solution using:
```bash
zebra_puzzle(Solution).
```
Houses are stored using the structure:
```bash
house(color, nationality, drink, cigarette, pet)
```
To solve for specific information, such as "What pet does the man who drinks milk own?", you can:
- put blanks in the positions you do not care about
- put a variable in the position you are trying to find
- put the known information in the appropriate position.

For example:
```bash
zebra_puzzle(Solution), member(house(_, _, milk, _, PET), Solution).
```
will return ```PET = snails``` as well as a display of the full solution.