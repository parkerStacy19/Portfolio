"""
Enemy class
"""

from __future__ import annotations

__author__ = "Paul Connett"

from mob import Mob


class Enemy(Mob):
    """
    Enemy class
    """

    def __init__(
        self,
        name: str,
        hp: int,
        attack: int,
        defense: int,
        reward: int
    ) -> None:
        super().__init__(name, hp, attack, defense)
        self.reward: int = reward

    def give_reward(self) -> int:
        """Return reward money
        """
        return self.reward
