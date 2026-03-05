Module mob
==========
Base class mobs

Classes
-------

`Mob(name: str, hp: int, attack: int, defense: int)`
:   Base class mobs
    
    Constructor for mobs

    ### Descendants

    * enemy.Enemy
    * player.Player

    ### Instance variables

    `is_alive: bool`
    :   Returns True if mob is alive

    ### Methods

    `attack_entity(self, entity: "'Mob'") ‑> int`
    :   Basic attack for mobs

    `die(self) ‑> None`
    :   Mark mob as dead

    `heal(self, amount: int) ‑> None`
    :   Heals mob

    `take_damage(self, amount: int) ‑> None`
    :   Reduce HP of mob