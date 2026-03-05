'''
Game driver class
'''

__author__ = 'Parker Stacy'

import os
from typing import List
from room import Room, Shop, Dungeon
from player import Player
from monster import MonsterList


class Game():
    '''
    game driver class
    '''
    NUM_LEVELS = 10  # Number of shops and dungeons

    def __init__(self) -> None:
        '''
        constructor
        '''
        # Maybe put name in a different function
        os.system('clear')
        player_name = input("Bork: Who dares enter my dungeon?   (Enter your name...) ")
        self._player: Player = Player(player_name)
        self._rooms: List[Room] = []
        self._monster_list: MonsterList = MonsterList()

        for i in range(Game.NUM_LEVELS):  # Build dungeon
            shop = Shop(i + 1, self.player)
            self._rooms.append(shop)
            dun = Dungeon(i + 1, self.player, self._monster_list)
            self._rooms.append(dun)

    def play_game(self) -> None:
        '''
        runs game logic
        '''
        # Maybe print this in a seperate function
        os.system('clear')
        print(f"Welcome, {self.player.name}, to Bork's Dungeon! >:)")
        print(f"\nBork has {Game.NUM_LEVELS} monsters for you to fight.")
        print("You can buy items at the shop before each encounter.")
        input("\nGood luck!  (Press Enter to begin...)")

        rooms: List[Room] = self._rooms  # Iterate through rooms
        for curr_room in rooms:
            curr_room.play_room(self.player)

        os.system('clear')  # You win
        input(f"Congratulations, {self.player.name}, you have bested Bork's dungeon.")
        input("\nBork: Let me just go get your prize...")

    @property
    def player(self) -> Player:
        '''
        returns the current player object
        '''
        return self._player

    @staticmethod
    def main() -> None:
        '''
        main method
        '''
        game = Game()
        game.play_game()


if __name__ == '__main__':
    Game.main()  # pragma: no cover
