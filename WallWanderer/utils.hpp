#pragma once

#include "maze.hpp"

// In this case desired dir is north=top east=right west=left south=bottom
Node* get_node_dir(Node* nodes, uint8_t x, uint8_t y, Bot_Dir dir,
                   Bot_Dir des_dir) {
  // Calculate the relative direction as an absolute direction
  Bot_Dir absolute_direction =
      static_cast<Bot_Dir>(((((uint8_t)dir + (uint8_t)des_dir) % 4) + 4) % 4);

  // Fetch the node based on the absolute direction
  switch (absolute_direction) {
    case Bot_Dir::NORTH:
      // Check bounds before accessing to prevent overflow/underflow
      return y + 1 < GRID_WIDTH ? &nodes[x + (y + 1) * GRID_WIDTH] : nullptr;
    case Bot_Dir::EAST:
      return x + 1 < GRID_WIDTH ? &nodes[(x + 1) + y * GRID_WIDTH] : nullptr;
    case Bot_Dir::SOUTH:
      return y > 0 ? &nodes[x + (y - 1) * GRID_WIDTH] : nullptr;
    case Bot_Dir::WEST:
      return x > 0 ? &nodes[(x - 1) + y * GRID_WIDTH] : nullptr;
    default:
      return nullptr;  // In case of an unexpected direction
  }
}

bool left_closed(Node* nodes, uint8_t x, uint8_t y, Bot_Dir dir) {
  /*
  Node* left_node = get_node_dir(nodes, x, y, dir, Bot_Dir::WEST); // Get the node to the left
  if (left_node == nullptr || left_node->state & NS_VISITED) { // Check if out of bounds or already visited
    return true;
  }
  */
  switch (dir) {
    case Bot_Dir::NORTH: return (nodes[x + y * GRID_WIDTH].state & NS_WEST) != 0;
    case Bot_Dir::EAST:  return (nodes[x + y * GRID_WIDTH].state & NS_NORTH) != 0;
    case Bot_Dir::SOUTH: return (nodes[x + y * GRID_WIDTH].state & NS_EAST) != 0;
    case Bot_Dir::WEST:  return (nodes[x + y * GRID_WIDTH].state & NS_SOUTH) != 0;
    default: {
      while (true) {}
      
      return true;
    }
  }
}

bool right_closed(Node* nodes, uint8_t x, uint8_t y, Bot_Dir dir) {
  /*
  Node* right_node = get_node_dir(nodes, x, y, dir, Bot_Dir::EAST); // Get the node to the right
  if (right_node == nullptr || right_node->state & NS_VISITED) { // Check if out of bounds or already visited
    return true;
  }
  */
  switch (dir) {
    case Bot_Dir::NORTH: return (nodes[x + y * GRID_WIDTH].state & NS_EAST) != 0;
    case Bot_Dir::EAST:  return (nodes[x + y * GRID_WIDTH].state & NS_SOUTH) != 0;
    case Bot_Dir::SOUTH: return (nodes[x + y * GRID_WIDTH].state & NS_WEST) != 0;
    case Bot_Dir::WEST:  return (nodes[x + y * GRID_WIDTH].state & NS_NORTH) != 0;
    default: return true;
  }
}

bool straight_closed(Node* nodes, uint8_t x, uint8_t y, Bot_Dir dir) {
  /*
  Node* straight_node = get_node_dir(nodes, x, y, dir, Bot_Dir::NORTH); // Get the node straight ahead
  if (straight_node == nullptr || straight_node->state & NS_VISITED) { // Check if out of bounds or already visited
    return true;
  }
  */
  switch (dir) {
    case Bot_Dir::NORTH: return (nodes[x + y * GRID_WIDTH].state & NS_NORTH) != 0;
    case Bot_Dir::EAST:  return (nodes[x + y * GRID_WIDTH].state & NS_EAST) != 0;
    case Bot_Dir::SOUTH: return (nodes[x + y * GRID_WIDTH].state & NS_SOUTH) != 0;
    case Bot_Dir::WEST:  return (nodes[x + y * GRID_WIDTH].state & NS_WEST) != 0;
    default: return true;
  }
}


