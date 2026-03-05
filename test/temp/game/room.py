'''
Room class
'''

__author__ = 'Parker Stacy'

import os
import sys
import copy
from abc import ABC, abstractmethod
from typing import List
from item import Item, Weapon, Armor, Potion, PotionHP, PotionStrength
from enemy import Enemy
from player import Player
from monster import MonsterList


class Room(ABC):
    '''
    Abstract base Room class
    '''

    def __init__(self, level: int, player: Player) -> None:
        '''
        constructor

        args:
            level: int - game level the room will be for
            player: Player - current player object
        '''
        self._level = level
        self._player = player

    @abstractmethod
    def play_room(self, player: Player) -> None:
        '''
        runs room logic (visit shop/fight monster)

        args:
            player: Player - current player object
        '''

    @abstractmethod
    def __str__(self) -> str:
        '''
        overload print method
        '''

    @property
    def level(self) -> int:
        '''
        getter for private level attribute
        '''
        return self._level


class Shop(Room):
    '''
    shop class
    '''
    # Could improve on shop system
    TEMP_SHOP: List[Item] = []

    TEMP_SHOP.append(Weapon("Steel Sword", 5, attack=20))
    TEMP_SHOP.append(Weapon("Mythril Sword", 50, attack=50))
    # TEMP_SHOP.append(Weapon("Bork's Sword", 1, attack=0))
    TEMP_SHOP.append(Armor("Steel Plate", 10, defense=10))
    TEMP_SHOP.append(Armor("Mythril Plate", 50, defense=25))
    TEMP_SHOP.append(PotionHP("Healing Potion", 25))
    TEMP_SHOP.append(PotionStrength("Strength Potion", 10))
    # TEMP_SHOP.append(Weapon("God Sword", 0, attack=10000000))

    def __init__(self, level: int, player: Player):
        '''
        constructor

        args:
            level: int - game level this shop is for
            player: Player - current player object
        '''
        super().__init__(level, player)
        self._items: List[Item] = Shop.TEMP_SHOP

    def play_room(self, player: Player) -> None:
        '''
        player enters shop and can choose to buy items

        args:
            player: Player - current player object
        '''
        os.system('clear')

        print("Welcome to Bork's $hop! It's totally not a scam!")
        print(self)

        print(f"You have {player.money}c")
        user_in = input("\nBork: Can I interest you in my wares? (y/n) ")

        if user_in == ('n' or 'N'):
            input("Bork: See you next time... I hope...    (Press Enter)")
            return
        if user_in == ('y' or 'Y'):
            self.buy_items(self._items)
        else:
            input("Invalid option!  (Press enter...) ")
            self.play_room(player)

    def buy_items(self, items: List[Item]) -> None:
        '''
        player buys items from shop

        args:
            items: List[Item] - item list containing shop inventory
        '''
        item = input("\nBork: What's catching your eye? (Enter item number...) ")

        try:
            item_num = int(item) - 1
            choice = items[item_num]
        except (IndexError, ValueError):
            input("Invalid option!  (Press enter...)")
            self.buy_items(items)

        if self._player.spend_money(choice.value):
            if isinstance(choice, Weapon):
                self._player.equip_weapon(choice)
                items.remove(choice)
            if isinstance(choice, Armor):
                self._player.equip_armor(choice)
                items.remove(choice)
            if isinstance(choice, Potion):
                new_potion = copy.copy(choice)  # Make a copy to allow multiple of same potion
                self._player.inventory.append(new_potion)

            os.system('clear')
            print(f"{self._player.name} bought a {choice.name}!\n")
            print(self)
            print(f"New balance: {self._player.money}c\n")
            user_in = input("Bork: Anything else? (y/n) ")
            if user_in == ('y' or 'Y'):
                os.system('clear')
                print(self)
                self.buy_items(items)
            else:
                input("\nBork: See ya, sucker!   (Press enter...)")
        else:
            input("\nBork: You can't afford that! Don't waste my time!\n"
                  f"{self._player.name} is escorted out by a gang of rat spiders  (Press enter...)")

    def __str__(self) -> str:
        '''
        prints shop inventory in readable format

        return:
            str - string containing formatted shop inventory
        '''
        rtn = ''
        for item in self._items:
            rtn += f"=== {self._items.index(item) + 1} - {item.name}: {item.value}c ===\n"

        return rtn


class Dungeon(Room):
    '''
    dungeon class
    '''

    def __init__(self, level: int, player: Player, monster_list: MonsterList) -> None:
        '''
        constructor

        args:
            level: int - game level this dungeon will be for
            player: Player - current player object
            monster_list: MonsterList - list of monsters to choose from
        '''
        super().__init__(level, player)
        self._curr_enemy = monster_list.get_monster(level)

    def play_room(self, player: Player) -> None:
        '''
        player fights a monster

        args:
            player: Player - current player object
        '''
        monster = self._curr_enemy

        os.system('clear')
        print(f"{player.name} enters the level {self.level} dungeon...")
        input(f"A {monster.name} has appeared! Time to throw hands!   (Press enter...)")
        os.system('clear')
        self.combat(player, monster)

    def combat(self, player: Player, enemy: Enemy) -> None:
        '''
        combat engine

        args:
            player: Player - current player object
            enemy: Enemy - current monster being fought
        '''
        while enemy.is_alive:
            print(f"===== {player.name} - {player.hp} HP - {player.attack} ATK - "
                  f"{player.defense} AMR =====")
            print(f"===== {enemy.name} - {enemy.hp} HP - {enemy.attack} ATK - "
                  f"{enemy.defense} AMR =====\n")

            options = ("Attack", "Use potion")
            for o in options:
                print(f"{options.index(o) + 1} - {o}")

            action = input(f"\nWhat will the brave {player.name} do? (Enter action number...) ")
            os.system('clear')

            if action == '1':
                dmg = player.attack_entity(enemy)
                print(f"{player.name} attacks for {dmg} damage!")

            if action == '2':
                self.use_potion(player)
                continue

            if enemy.is_alive:
                dmg = enemy.attack_entity(player)
                input(f"\nThe {enemy.name} attacks for {dmg} damage!    (Press enter...)")
                os.system('clear')

            if not player.is_alive:  # Need better game over but this will do for now
                os.system('clear')
                input("u ded")
                sys.exit()

        print(f"\nThe {enemy.name} has been defeated!")
        player.add_money(enemy.reward)
        input(f"{player.name} gained {enemy.reward}c!  (Press enter...)")

    def use_potion(self, player: Player) -> None:
        '''
        player uses a potion from their inventory

        args:
            player: Player - current player object
        '''
        if not player.inventory:
            input("No potions to use!   (Press enter...)")
            return

        potions: List[Potion] = []

        print("===== Iventory =====")
        for item in player.inventory:
            if isinstance(item, Potion):
                potions.append(item)

        for item in potions:
            print(f"{potions.index(item) + 1} - {item.name}")

        choice = input(f"Which item will {player.name} use? (Enter item number...) ")

        try:
            choice_num = int(choice) - 1
            player.use_potion(potions[choice_num])
            os.system('clear')
            print(f"{player.name} used a {potions[choice_num].name}")
        except (IndexError, ValueError):
            print("INVALID INPUT")
            self.use_potion(player)
        return

    def __str__(self) -> str:
        '''
        overload print method. Currently unused for Room type Dungeon
        '''
        return ''
