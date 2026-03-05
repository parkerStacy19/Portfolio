'''
Unittesting Player class

ChatGPT Free model used to generate starting code:
https://chatgpt.com/share/69382263-fb70-8001-a8e3-a34ddf4021af
'''

__author__ = 'Parker Stacy'


import unittest
from unittest.mock import MagicMock

from player import Player
from mob import Mob


class TestPlayer(unittest.TestCase):
    '''
    Unittesting Player class
    '''

    def setUp(self):
        self.player = Player("TestPlayer")

    def test_constructor(self):
        '''
        Test constructor. Also tests give_starting_equipment by extension

        Starting equipment: Iron Sword (attack = 10), Iron Armor (defense = 5)
        '''
        self.assertEqual(self.player.max_hp, 100)
        self.assertEqual(self.player.money, 0)
        assert not self.player.inventory

        self.assertEqual(self.player.attack, 10)
        self.assertEqual(self.player.defense, 5)
        self.assertIsNotNone(self.player.weapon)
        self.assertIsNotNone(self.player.armor)

    def test_attack_entity(self):
        '''
        Test attack_entity as intended

        Use a mock Mob object to ensure method is called only once
        '''
        target = MagicMock(spec=Mob)
        target.defense = 3

        dmg = self.player.attack_entity(target)

        expected = max(0, 10 - 3)  # 10 attack - 3 defense
        self.assertEqual(dmg, expected)
        target.take_damage.assert_called_once_with(expected)

    def test_attack_entity_sb(self):
        '''
        Test attack_entity with strength buff active

        Use mock Mob object to ensure method is called only once with correct value
        '''
        target = MagicMock(spec=Mob)
        target.defense = 0

        self.player.strength_buff = True
        dmg = self.player.attack_entity(target)

        self.assertEqual(dmg, 20)
        target.take_damage.assert_called_once_with(20)
        self.assertFalse(self.player.strength_buff)  # ensure strength_buff resets after use

    def test_attack_dead(self):
        '''
        Test attack_entity when player is dead

        Use mock Mob object to ensure take_damage is not called
        '''
        target = MagicMock(spec=Mob)
        target.defense = 0
        self.player.is_dead = True

        dmg = self.player.attack_entity(target)

        self.assertEqual(dmg, 0)
        target.take_damage.assert_not_called()

    def test_heal(self):
        '''
        Test heal method as intended
        '''
        self.player.hp = 50
        self.player.heal(40)
        self.assertEqual(self.player.hp, 90)

    def test_heal_max(self):
        '''
        Test heal method when healing over maxHP
        '''
        self.player.hp = 95
        self.player.heal(50)
        self.assertEqual(self.player.hp, 100)

    def test_use_potion(self):
        '''
        Test use_potion method as intended

        Use MagicMock to ensure potion is used only once
        '''
        potion = MagicMock()
        self.player.inventory.append(potion)

        self.player.use_potion(potion)

        potion.use.assert_called_once_with(self.player)
        self.assertNotIn(potion, self.player.inventory)

    def test_use_potion_fail(self):
        '''
        Test use_potion method when potion is not in inventory

        Use MagicMock to ensure use method is not called
        '''
        potion = MagicMock()

        self.player.use_potion(potion)

        potion.use.assert_not_called()

    def test_equip_weapon(self):
        '''
        Test equip_weapon method

        Use MagicMock to simulate new Weapon object
        '''
        weapon = MagicMock()
        weapon.attack = 25

        self.player.equip_weapon(weapon)

        self.assertEqual(self.player.weapon, weapon)
        self.assertEqual(self.player.attack, 25)

    def test_equip_armor(self):
        '''
        Test equip_armor method

        Use MagicMock to simulate new Armor object
        '''
        armor = MagicMock()
        armor.defense = 12

        self.player.equip_armor(armor)

        self.assertEqual(self.player.armor, armor)
        self.assertEqual(self.player.defense, 12)

    def test_add_money(self):
        '''
        Test add_money method as intended
        '''
        self.player.add_money(50)
        self.assertEqual(self.player.money, 50)

    def test_add_money2(self):
        '''
        Test add_money with value <= 0
        '''
        self.player.add_money(-10)
        self.assertEqual(self.player.money, 0)

    def test_spend_money(self):
        '''
        Test spend_money method as intended
        '''
        self.player.money = 40
        result = self.player.spend_money(30)

        self.assertTrue(result)  # method returns true if successful
        self.assertEqual(self.player.money, 10)

    def test_spend_money2(self):
        '''
        Test spend_money method when player does not have enough money
        '''
        self.player.money = 10
        result = self.player.spend_money(20)

        self.assertFalse(result)
        self.assertEqual(self.player.money, 10)

    def test_spend_money3(self):
        '''
        Test spend_money method with value <= 0
        '''
        self.assertFalse(self.player.spend_money(-5))
