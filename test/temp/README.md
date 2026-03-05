# Bork's Dungeon - A Text-Based Dungeon Crawler

A simple CLI adventure game demonstrating object-oriented design principles, clean architecture, and robust unit testing practices.

## Project Overview

Bork's Dungeon is an interactive text-based dungeon crawler where players navigate through multiple levels of shops and combat encounters. This project showcases professional software engineering practices including inheritance hierarchies, polymorphism, abstract base classes, and comprehensive unit testing using the pytest framework.

This was a collaborative project with Paul Connett.

**Key Features:**
- 10 procedurally generated dungeon levels with shop systems
- Turn-based combat with player and enemy AI
- Dynamic item system (weapons, armor, potions)
- Nearly full test coverage
- Type checking and static analysis support using mypy
- Professional code documentation

---

## Object-Oriented Design Architecture

### 1. Inheritance Hierarchy

The project employs an inheritance structure to allow for code reuse and maintainability.

#### Base Class: `Mob`
All combat entities inherit from the `Mob` base class, establishing core combat mechanics:

```python
class Mob:
    """Base class for all entities in the game"""
    
    def __init__(self, name: str, hp: int, attack: int, defense: int) -> None:
        self.name: str = name
        self.hp: int = hp
        self.attack: int = attack
        self.defense: int = defense
        self.is_dead: bool = False

    def attack_entity(self, entity: "Mob") -> int:
        """Calculate and apply damage"""
        if self.is_dead:
            return 0
        damage: int = max(0, self.attack - entity.defense)
        entity.take_damage(damage)
        return damage

    def take_damage(self, amount: int) -> None:
        """Reduce HP and trigger death if necessary"""
        if amount <= 0:
            return
        self.hp -= amount
        if self.hp <= 0:
            self.die()
            
    def die(self) -> None:
        """Mark entity as dead"""
        self.is_dead = True
```

**Design Pattern**: Template Method Pattern - Core behavior defined in base class with customization in subclasses.

#### Specialized Classes: `Player` and `Enemy`

**Player** extends `Mob` with player-specific features:

```python
class Player(Mob):
    """Player extends Mob with inventory and equipment management"""
    
    def __init__(self, name: str = "Player") -> None:
        super().__init__(name, hp=100, attack=0, defense=0)
        self.max_hp: int = 100
        self.money: int = 0
        self.inventory: list[Item] = []
        self.weapon: Weapon | None = None
        self.armor: Armor | None = None
        self.strength_buff: bool = False
        self._give_starting_equipment()

    def attack_entity(self, entity: Mob) -> int:
        """Override: double damage when strength buff is active"""
        if self.is_dead:
            return 0
        
        attack_power = self.attack * 2 if self.strength_buff else self.attack
        if self.strength_buff:
            self.strength_buff = False  # Buff consumed
            
        damage: int = max(0, attack_power - entity.defense)
        entity.take_damage(damage)
        return damage
```

**Enemy** extends `Mob` with reward mechanics:

```python
class Enemy(Mob):
    """Enemy extends Mob with reward system"""
    
    def __init__(self, name: str, hp: int, attack: int, 
                 defense: int, reward: int) -> None:
        super().__init__(name, hp, attack, defense)
        self.reward: int = reward

    def give_reward(self) -> int:
        """Return money reward to player upon defeat"""
        return self.reward
```

**Design Pattern**: Inheritance with Specialization - Base class provides common interface; subclasses customize behavior.

---

### 2. Polymorphism with Abstract Base Classes

The item system leverages abstract base classes and polymorphism:

```python
class Item:
    """Base class for all items"""
    def __init__(self, name: str, value: int) -> None:
        self.name: str = name
        self.value: int = value


class Weapon(Item):
    """Weapon increases player attack"""
    def __init__(self, name: str, value: int, attack: int) -> None:
        super().__init__(name, value)
        self.attack: int = attack


class Armor(Item):
    """Armor increases player defense"""
    def __init__(self, name: str, value: int, defense: int) -> None:
        super().__init__(name, value)
        self.defense: int = defense


class Potion(Item, abc.ABC):
    """Abstract base class for potions with polymorphic use() method"""
    
    @abc.abstractmethod
    def use(self, entity: Mob) -> None:
        """Subclasses implement specific potion effects"""
        ...


class PotionHP(Potion):
    """Healing potion that restores HP"""
    
    def __init__(self, name: str, value: int, heal_value: int = 100) -> None:
        super().__init__(name, value)
        self.heal_value: int = heal_value

    def use(self, entity: Mob) -> None:
        entity.heal(self.heal_value)


class PotionStrength(Potion):
    """Strength potion that doubles next attack"""
    
    def __init__(self, name: str, value: int) -> None:
        super().__init__(name, value)

    def use(self, entity: Mob) -> None:
        entity.strength_buff = True
```

**Design Pattern**: Strategy Pattern + Template Method - Abstract `Potion` class enforces interface; concrete subclasses implement different strategies with the `use()` method.

---

### 3. Game State Management

The `Game` class is the main driver and manages game flow:

```python
class Game:
    """Main game driver coordinating all game elements"""
    NUM_LEVELS = 10
    
    def __init__(self) -> None:
        self._player: Player = Player(player_name)
        self._rooms: List[Room] = []
        self._monster_list: MonsterList = MonsterList()
        
        # Build dungeon structure
        for i in range(Game.NUM_LEVELS):
            shop = Shop(i + 1, self.player)
            self._rooms.append(shop)
            dun = Dungeon(i + 1, self.player, self._monster_list)
            self._rooms.append(dun)

    def play_game(self) -> None:
        """Orchestrate game flow through rooms"""
        for curr_room in self._rooms:
            curr_room.play_room(self.player)

    @staticmethod
    def main() -> None:
        """Entry point"""
        game = Game()
        game.play_game()
```

**Design Pattern**: Facade Pattern + Composition - `Game` class provides simplified interface to rooms, items, and entities.

---

## Unit Testing Strategy

The project demonstrates comprehensive unit testing covering all major components.

### 1. Mock Objects

Tests use `unittest.mock.MagicMock` to isolate components, verify behavior, and ensure isolated unit tests independent of other components.

```python
from unittest.mock import MagicMock

class TestPlayer(unittest.TestCase):
    
    def test_attack_entity(self):
        """Verify attack damage calculation using mock"""
        self.player = Player("TestPlayer")
        target = MagicMock(spec=Mob)  # Mock prevents dependencies
        target.defense = 3
        
        dmg = self.player.attack_entity(target)
        
        expected = max(0, 10 - 3)  # 10 attack - 3 defense
        self.assertEqual(dmg, expected)
        target.take_damage.assert_called_once_with(expected)  # Verify interaction
```

### 2. Testing Inheritance and Polymorphism

Verify that subclass behavior correctly overrides base class behavior:

```python
class TestPlayerAttack(unittest.TestCase):
    
    def test_attack_entity_with_strength_buff(self):
        """Test specialized Player.attack_entity override"""
        player = Player("TestPlayer")
        target = MagicMock(spec=Mob)
        target.defense = 0
        
        # Enable strength buff (potion effect)
        player.strength_buff = True
        dmg = player.attack_entity(target)
        
        self.assertEqual(dmg, 20)  # Double damage: 10 * 2
        self.assertFalse(player.strength_buff)  # Buff consumed
        target.take_damage.assert_called_once_with(20)
```

### 3. Testing Polymorphic Behavior

Verify abstract base classes work correctly across implementations:

```python
class TestPotions(unittest.TestCase):
    
    def test_potion_hp_use(self):
        """Test polymorphic behavior of Potion subclasses"""
        mock_mob = MagicMock()
        potion = PotionHP("HP Potion", value=10, heal_value=75)
        
        potion.use(mock_mob)
        
        mock_mob.heal.assert_called_once_with(75)
    
    def test_potion_strength_use(self):
        """Different potion type, different behavior"""
        mock_mob = MagicMock()
        potion = PotionStrength("Strength Potion", value=20)
        
        potion.use(mock_mob)
        
        self.assertTrue(mock_mob.strength_buff)
```

### 4. Testing Base Class Behavior

Comprehensive testing of fundamental mechanics:

```python
class TestMob(unittest.TestCase):
    
    def test_damage_reduction_by_defense(self):
        """Verify armor/defense reduces damage"""
        attacker = Mob("A", 10, 5, 0)
        target = Mob("B", 10, 2, 1)  # defense=1
        
        target.take_damage = MagicMock()
        dmg = attacker.attack_entity(target)
        
        self.assertEqual(dmg, 4)  # 5 attack - 1 defense
        target.take_damage.assert_called_once_with(4)
    
    def test_take_damage_death(self):
        """Verify death condition triggers correctly"""
        mob = Mob("A", 5, 3, 1)
        mob.die = MagicMock()
        
        mob.take_damage(10)  # Damage exceeds HP
        
        mob.die.assert_called_once()
```

### Running Tests

```bash
# Run all tests with coverage report
pytest --cov=game tests/

# Run specific test file
pytest tests/test_player.py -v

# Run with detailed output
pytest -vv
```

---

## Code Quality & Standards

**Type Safety:**
- Full type hints throughout codebase
- Static analysis with `mypy` for type checking

**Code Style:**
- PEP 8 compliant via `flake8`
- Automatic formatting with `autopep8`

**Documentation:**
- Comprehensive docstrings following Google style
- Generated API documentation in `docs/code-docs/`

**Configuration:**
- `.mypy.ini` - Type checker configuration
- `.flake8` - Style enforcement rules

---

## Project Structure

```
BorksDungeon/
├── game/                     # Main game source code
    ├── player.py             # Player class (Mob subclass)
    ├── enemy.py              # Enemy class (Mob subclass)
    ├── mob.py                # Base class for all entities
    ├── item.py               # Item hierarchy (weapons, armor, potions)
    ├── room.py               # Room types (Shop, Dungeon)
    ├── game.py               # Game orchestrator
    ├── monster.py            # Monster generation
    └── tests/                # Unit tests
    │    ├── test_player.py
    │    ├── test_mobs.py
    │    ├── test_potions.py
    │    ├── test_room.py
    │    └── test_game.py
    └──docs/code-docs/        # Generated API docs
    └──uml/                   # Generated UML diagrams
```
---
## How to Play the Game

A Makefile was provided to compile and run the progrm, perform unit tests, and create documentation automatically. To run the program, clone the repository and navigate to the root directory:
```bash
git clone https://github.com/paulconnett/oop-group-final
cd oop-group-final
```

Use ```make play``` to run the game. ```make``` will run all unit tests and create documentation. Several individual rules are included to test for style and types, calculate test coverage, and create documentation.
