Module player
=============
Player class

Classes
-------

`Player(name: str = 'Player')`
:   Player class
    
    Constructor for mobs

    ### Ancestors (in MRO)

    * mob.Mob

    ### Methods

    `add_money(self, amount: int) ‑> None`
    :   Increase player's money

    `attack_entity(self, entity: Mob) ‑> int`
    :   Player attack
        Strength potion doubles next attack

    `equip_armor(self, armor: Armor) ‑> None`
    :   Equip new armor

    `equip_weapon(self, weapon: Weapon) ‑> None`
    :   Equip a new weapon

    `heal(self, amount: int) ‑> None`
    :   Heals player

    `spend_money(self, amount: int) ‑> bool`
    :   Checks if player has enough money
        Updates player's money
        Used for shop

    `use_potion(self, potion: Potion) ‑> None`
    :   Use a potion from the player's inventory