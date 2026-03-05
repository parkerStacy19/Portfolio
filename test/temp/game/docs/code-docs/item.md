Module item
===========
Base class for items

Classes
-------

`Armor(name: str, value: int, defense: int)`
:   Armor item that increases player's defense

    ### Ancestors (in MRO)

    * item.Item

`Item(name: str, value: int)`
:   Base class for items

    ### Descendants

    * item.Armor
    * item.Potion
    * item.Weapon

`Potion(name: str, value: int)`
:   Abstract base class for all potions.

    ### Ancestors (in MRO)

    * item.Item
    * abc.ABC

    ### Descendants

    * item.PotionHP
    * item.PotionStrength

    ### Methods

    `use(self, entity: Mob) ‑> None`
    :   Abstract method for using potions

`PotionHP(name: str, value: int, heal_value: int = 100)`
:   Healing potion

    ### Ancestors (in MRO)

    * item.Potion
    * item.Item
    * abc.ABC

`PotionStrength(name: str, value: int)`
:   Strength potion

    ### Ancestors (in MRO)

    * item.Potion
    * item.Item
    * abc.ABC

`Weapon(name: str, value: int, attack: int)`
:   Weapon item that increases player's attack

    ### Ancestors (in MRO)

    * item.Item