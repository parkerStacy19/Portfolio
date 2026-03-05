% Zebra Puzzle solver
%   Parker Stacy

% Representing the houses as a list of 5 elements: [House1, House2, House3, House4, House5]
% Each house is a structure: house(Color, Nationality, Drink, Cigarette, Pet)

zebra_puzzle(Solution) :-
    Solution = [
        house(Color1, Nationality1, Drink1, Cigarette1, Pet1),
        house(Color2, Nationality2, Drink2, Cigarette2, Pet2),
        house(Color3, Nationality3, Drink3, Cigarette3, Pet3),
        house(Color4, Nationality4, Drink4, Cigarette4, Pet4),
        house(Color5, Nationality5, Drink5, Cigarette5, Pet5)
    ],

    % Constraints/Clues

    % 1: The Englishman lives in the red house
    member(house(red, englishman, _, _, _), Solution),

    % 2: The Spaniard owns dogs
    member(house(_, spaniard, _, _, dogs), Solution),

    % 3: The coffee is drunk in the green house
    member(house(green, _, coffee, _, _), Solution),

    % 4: The Ukrainian drinks tea
    member(house(_, ukrainian, tea, _, _), Solution),

    % 5: The green house is immediately to the right of the white house
    append(_, [house(green, _, _, _, _), house(white, _, _, _, _)], Solution),

    % 6: The Old Gold smoker owns snails
    member(house(_, _, _, old_gold, snails), Solution),

    % 7: Kools are smoked in the yellow house
    member(house(yellow, _, _, kools, _), Solution),

    % 8: Milk is drunk in the middle house
    Solution = [_, _, house(_, _, milk, _, _), _, _],

    % 9: The Norwegian lives in the first house
    Solution = [house(_, norwegian, _, _, _)|_],

    % 10: The man who smokes Chesterfields lives in the house next to the man with the fox
    next_to(house(_, _, _, chesterfields, _), house(_, _, _, _, fox), Solution),

    % 11: Kools are smoked in the house next to the house where the horse is kept
    next_to(house(_, _, _, kools, _), house(_, _, _, _, horse), Solution),

    % 12: The Lucky Strike smoker drinks orange juice
    member(house(_, _, orange_juice, lucky_strike, _), Solution),

    % 13: The Japanese smokes Parliaments
    member(house(_, japanese, _, parliaments, _), Solution),

    % 14: The Norwegian lives next to the blue house
    next_to(house(_, norwegian, _, _, _), house(blue, _, _, _, _), Solution),

    % Who owns the Zebra? (Establish zebra as a pet)
    member(house(_, _, _, _, zebra), Solution),

    % Who drinks the water? (Establish water as a drink)
    member(house(_, _, water, _, _), Solution).

% Helper predicate: next_to to check if homes are physically adjacent
next_to(A, B, List) :-
    append(_, [A, B|_], List);
    append(_, [B, A|_], List).
