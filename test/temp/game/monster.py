'''
MonsterList class
'''

__author__ = "Paul Connett", "Parker Stacy"

from typing import List
from enemy import Enemy


class MonsterList():
    '''
    class to manage list of monsters
    '''
    monster_list: List[Enemy] = [
        Enemy("Slime", 10, 5, 0, 10),
        Enemy("Wolf", 15, 7, 1, 15),
        Enemy("Goblin", 25, 10, 3, 20),
        Enemy("Skeleton", 40, 12, 2, 22),
        Enemy("Bandit", 60, 14, 4, 25),
        Enemy("Orc", 80, 16, 5, 30),
        Enemy("Wraith", 100, 18, 8, 35),
        Enemy("Ogre", 130, 20, 10, 40),
        Enemy("Giant", 160, 25, 12, 45),
        Enemy("Dragon", 200, 30, 15, 100)
    ]

    def __init__(self) -> None:
        '''
        constructor
        '''
        self._monster_list = MonsterList.monster_list

    def get_monster(self, index: int) -> Enemy:
        """
        returns monster from list at the given index
        """
        return self._monster_list[index - 1]
