Module room
===========
Room class

Classes
-------

`Dungeon(level: int, player: player.Player, monster_list: monster.MonsterList)`
:   dungeon class
    
    constructor
    
    args:
        level: int - game level this dungeon will be for
        player: Player - current player object
        monster_list: MonsterList - list of monsters to choose from

    ### Ancestors (in MRO)

    * room.Room
    * abc.ABC

    ### Methods

    `combat(self, player: player.Player, enemy: enemy.Enemy) ‑> None`
    :   combat engine
        
        args:
            player: Player - current player object
            enemy: Enemy - current monster being fought

    `play_room(self, player: player.Player) ‑> None`
    :   player fights a monster
        
        args:
            player: Player - current player object

    `use_potion(self, player: player.Player) ‑> None`
    :   player uses a potion from their inventory
        
        args:
            player: Player - current player object

`Room(level: int, player: player.Player)`
:   Abstract base Room class
    
    constructor
    
    args:
        level: int - game level the room will be for
        player: Player - current player object

    ### Ancestors (in MRO)

    * abc.ABC

    ### Descendants

    * room.Dungeon
    * room.Shop

    ### Instance variables

    `level: int`
    :   getter for private level attribute

    ### Methods

    `play_room(self, player: player.Player) ‑> None`
    :   runs room logic (visit shop/fight monster)
        
        args:
            player: Player - current player object

`Shop(level: int, player: player.Player)`
:   shop class
    
    constructor
    
    args:
        level: int - game level this shop is for
        player: Player - current player object

    ### Ancestors (in MRO)

    * room.Room
    * abc.ABC

    ### Class variables

    `TEMP_SHOP: List[item.Item]`
    :

    ### Methods

    `buy_items(self, items: List[item.Item]) ‑> None`
    :   player buys items from shop
        
        args:
            items: List[Item] - item list containing shop inventory

    `play_room(self, player: player.Player) ‑> None`
    :   player enters shop and can choose to buy items
        
        args:
            player: Player - current player object