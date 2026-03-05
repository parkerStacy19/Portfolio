'''
Unittesting Mob and Enemy class

ChatGPT Free model used to generate starting code:
https://chatgpt.com/share/69381543-8f78-8001-91b7-e351e55957a9
'''

__author__ = 'Parker Stacy'


import unittest
from unittest.mock import MagicMock
from mob import Mob
from enemy import Enemy


class TestMob(unittest.TestCase):
    '''
    Unittesting Mob class
    '''

    def test_constructor(self):
        '''
        Test constructor. Create dummy enemy and test attributes
        '''
        m = Mob("test mob", 10, 3, 1)
        self.assertEqual(m.name, "test mob")
        self.assertEqual(m.hp, 10)
        self.assertEqual(m.attack, 3)
        self.assertEqual(m.defense, 1)
        self.assertFalse(m.is_dead)
        self.assertEqual(m.strength_buff, 0)

    def test_attack_entity(self):
        '''
        Test attack_entity (basic damage).

        Use MagicMock to ensure method is only called once.
        '''
        attacker = Mob("A", 10, 5, 0)
        target = Mob("B", 10, 2, 1)

        target.take_damage = MagicMock()

        dmg = attacker.attack_entity(target)

        self.assertEqual(dmg, 4)  # 5 attack - 1 defense
        target.take_damage.assert_called_once_with(4)

    def test_attack_dead(self):
        '''
        Test attack_entity call that results in mob death
        '''
        attacker = Mob("A", 10, 5, 0)
        target = Mob("B", 10, 2, 1)
        attacker.is_dead = True

        dmg = attacker.attack_entity(target)

        self.assertEqual(dmg, 0)

    def test_take_damage(self):
        '''
        Test take_damage method (basic damage)
        '''
        m = Mob("A", 10, 3, 1)
        m.take_damage(4)
        self.assertEqual(m.hp, 6)

    def test_take_damage2(self):
        '''
        Test take_damage method with value <= 0
        '''
        m = Mob("A", 10, 3, 1)
        m.take_damage(-10)
        self.assertEqual(m.hp, 10)

    def test_take_damage_die(self):
        '''
        Test take_damage method call that results in mob death.

        Use MagicMock to ensure method is called only once.
        '''
        m = Mob("A", 5, 3, 1)
        m.die = MagicMock()

        m.take_damage(10)

        m.die.assert_called_once()

    def test_heal(self):
        '''
        Test heal function as intended
        '''
        m = Mob("A", 10, 3, 1)
        m.heal(5)
        self.assertEqual(m.hp, 15)

    def test_heal2(self):
        '''
        Test heal method for value <= 0
        '''
        m = Mob("A", 10, 3, 1)
        m.heal(0)
        self.assertEqual(m.hp, 10)

    def test_die(self):
        '''
        Test die method directly
        '''
        m = Mob("A", 10, 3, 1)
        m.die()
        self.assertTrue(m.is_dead)

    def test_is_alive(self):
        '''
        Test is_alive property
        '''
        m = Mob("A", 10, 3, 1)
        self.assertTrue(m.is_alive)
        m.is_dead = True
        self.assertFalse(m.is_alive)


class TestEnemy(unittest.TestCase):
    '''
    Docstring for TestEnemy
    '''

    def test_constructor(self):
        '''
        Test constructor with dummy enemy
        '''
        e = Enemy("test enemy", 10, 3, 1, 5)
        self.assertEqual(e.name, "test enemy")
        self.assertEqual(e.hp, 10)
        self.assertEqual(e.attack, 3)
        self.assertEqual(e.defense, 1)
        self.assertFalse(e.is_dead)
        self.assertEqual(e.strength_buff, 0)
        self.assertEqual(e.reward, 5)

    def test_give_reward(self):
        '''
        Test give_reward function
        '''
        e = Enemy("A", 10, 3, 1, 5)
        e.give_reward = MagicMock(wraps=e.give_reward)

        reward = e.give_reward()

        self.assertEqual(reward, 5)
        e.give_reward.assert_called_once()
