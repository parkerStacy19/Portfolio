"""
Base class for items
"""

from __future__ import annotations

__author__ = "Paul Connett"

import abc

from mob import Mob


class Item:
    """
    Base class for items
    """

    def __init__(self, name: str, value: int) -> None:
        self.name: str = name
        self.value: int = value


class Weapon(Item):
    """
    Weapon item that increases player's attack
    """

    def __init__(self, name: str, value: int, attack: int) -> None:
        super().__init__(name, value)
        self.attack: int = attack


class Armor(Item):
    """
    Armor item that increases player's defense
    """

    def __init__(self, name: str, value: int, defense: int) -> None:
        super().__init__(name, value)
        self.defense: int = defense


class Potion(Item, abc.ABC):
    """
    Abstract base class for all potions.
    """

    @abc.abstractmethod
    def use(self, entity: Mob) -> None:
        """
        Abstract method for using potions
        """
        ...


class PotionHP(Potion):
    """
    Healing potion
    """

    def __init__(self, name: str, value: int, heal_value: int = 100) -> None:
        super().__init__(name, value)
        self.heal_value: int = heal_value

    def use(self, entity: Mob) -> None:
        entity.heal(self.heal_value)


class PotionStrength(Potion):
    """
    Strength potion
    """

    def __init__(self, name: str, value: int) -> None:
        super().__init__(name, value)

    def use(self, entity: Mob) -> None:
        entity.strength_buff = True
