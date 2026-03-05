"""
Unittesting Game class
"""

__author__ = "Paul Connett"

import unittest
from unittest.mock import patch, MagicMock

from game import Game


class TestGame(unittest.TestCase):
    """
    Unittesting Game class
    """
    @patch("builtins.input", return_value="Player")
    def test_play_game(self, _mock_input) -> None:
        """
        Tests that play_game calls play_room
        """
        game = Game()
        test_room = MagicMock()
        game._rooms = [test_room]
        game.play_game()

        test_room.play_room.assert_called_once_with(game.player)

    @patch.object(Game, "play_game")
    @patch("builtins.input", return_value="Player")
    def test_main(self, _mock_input: MagicMock, mock_play_game: MagicMock,) -> None:
        """
        Tests main static method creates a Game instance and calls play_game()
        """
        Game.main()

        mock_play_game.assert_called_once()
