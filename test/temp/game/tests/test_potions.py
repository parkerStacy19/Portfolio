'''
Unittest Item class

Only use() methods need to be tested for 100% test coverage

ChatGPT Free model used to generate starting code
https://chatgpt.com/share/69382299-f4cc-8001-8d0a-ab93c683f656
'''

__author__ = 'Parker Stacy'


import unittest
from unittest.mock import MagicMock

from item import PotionHP, PotionStrength


class TestPotions(unittest.TestCase):
    '''
    Unittest Potions class
    '''

    def test_potion_hp_use(self):
        """
        Test use method for HP potions using mock Mob object
        """
        mock_mob = MagicMock()
        potion = PotionHP("HP Potion", value=10, heal_value=75)

        potion.use(mock_mob)

        mock_mob.heal.assert_called_once_with(75)

    def test_potion_strength_use(self):
        '''
        Test use method for strength potions using mock Mob object
        '''
        mock_mob = MagicMock()
        mock_mob.strength_buff = False
        potion = PotionStrength("Strength Potion", value=20)

        potion.use(mock_mob)

        self.assertTrue(mock_mob.strength_buff)
