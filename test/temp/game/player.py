"""
Player class
"""

from __future__ import annotations

__author__ = "Paul Connett"

from mob import Mob
from item import Item, Weapon, Armor, Potion


class Player(Mob):
    """
    Player class
    """

    def __init__(self, name: str = "Player") -> None:
        super().__init__(name, hp=100, attack=0, defense=0)

        self.max_hp: int = 100
        self.money: int = 0
        self.inventory: list[Item] = []

        self.weapon: Weapon | None = None
        self.armor: Armor | None = None

        self.strength_buff: bool = False

        self._give_starting_equipment()

    def attack_entity(self, entity: Mob) -> int:
        """
        Player attack
        Strength potion doubles next attack
        """
        if self.is_dead:
            return 0

        if self.strength_buff:
            player_attack: int = self.attack * 2
            self.strength_buff = False
        else:
            player_attack = self.attack

        damage: int = max(0, player_attack - entity.defense)
        entity.take_damage(damage)
        return damage

    def heal(self, amount: int) -> None:
        """
        Heals player
        """
        self.hp = min(self.max_hp, self.hp + amount)

    def use_potion(self, potion: Potion) -> None:
        """Use a potion from the player's inventory
        """
        if potion not in self.inventory:
            return

        potion.use(self)
        self.inventory.remove(potion)

    def equip_weapon(self, weapon: Weapon) -> None:
        """Equip a new weapon
        """
        self.weapon = weapon
        self.attack = weapon.attack

    def equip_armor(self, armor: Armor) -> None:
        """Equip new armor
        """
        self.armor = armor
        self.defense = armor.defense

    def add_money(self, amount: int) -> None:
        """Increase player's money
        """
        if amount > 0:
            self.money += amount

    def spend_money(self, amount: int) -> bool:
        """Checks if player has enough money
        Updates player's money
        Used for shop
        """
        if amount < 0:
            return False

        if self.money >= amount:
            self.money -= amount
            return True

        return False

    def _give_starting_equipment(self) -> None:
        """set player starting items
        """
        starting_weapon = Weapon("Iron Sword", 0, attack=10)
        starting_armor = Armor("Iron Armor", 0, defense=5)

        self.equip_weapon(starting_weapon)
        self.equip_armor(starting_armor)
