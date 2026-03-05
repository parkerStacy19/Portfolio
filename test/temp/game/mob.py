"""
Base class mobs
"""

from __future__ import annotations

__author__ = "Paul Connett"


class Mob:
    """
    Base class mobs
    """

    def __init__(self, name: str, hp: int, attack: int, defense: int) -> None:
        """Constructor for mobs
        """
        self.name: str = name
        self.hp: int = hp
        self.attack: int = attack
        self.defense: int = defense
        self.strength_buff = 0
        self.is_dead: bool = False

    def attack_entity(self, entity: "Mob") -> int:
        """Basic attack for mobs
        """
        if self.is_dead:
            return 0

        damage: int = max(0, self.attack - entity.defense)
        entity.take_damage(damage)
        return damage

    def take_damage(self, amount: int) -> None:
        """Reduce HP of mob
        """
        if amount <= 0:
            return

        self.hp -= amount
        if self.hp <= 0:
            self.die()

    def heal(self, amount: int) -> None:
        """Heals mob
        """
        if amount <= 0:
            return
        self.hp += amount

    def die(self) -> None:
        """Mark mob as dead
        """
        self.is_dead = True

    @property
    def is_alive(self) -> bool:
        """Returns True if mob is alive
        """
        return not self.is_dead
