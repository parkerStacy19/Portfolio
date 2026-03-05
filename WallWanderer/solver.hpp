#pragma once

#include "My_EduCar_Params.h"
#include "maze.hpp"
#include "utils.hpp"
#include "drive.hpp"

const double des_dist = (190.0 - WHEEL_BASE_MM) / 5.0;

Node_Type do_node(uint8_t& x, uint8_t& y, Node* nodes, Bot& bot, Stack& stack) {
  static bool goal_reached = false;

  bot.serial_monitor.sendText("Entered do_node");
  bot.serial_monitor.sendNewline();

  bot.serial_monitor.sendText("X: ");
  bot.serial_monitor.sendIntegerValue(x);
  bot.serial_monitor.sendNewline();

  bot.serial_monitor.sendText("Y: ");
  bot.serial_monitor.sendIntegerValue(y);
  bot.serial_monitor.sendNewline();



  // First, check for goal
  // Second, find walls
  // Third, move and return now square result

  Node_Type res = Node_Type::GENERAL;

  Node& node = nodes[x + y * GRID_WIDTH];

  node.state |= NS_VISITED;

  /// Step 1 ///

  ctl::TapeColor cr;
  bot.cs_led.on();
  ctl::WaitMillis(1000);
  cr = bot.color_sensor.getTapeColor();
  bot.cs_led.off();

  bot.serial_monitor.sendText("Tape color: ");
  bot.serial_monitor.sendIntegerValue((uint16_t)cr);
  bot.serial_monitor.sendNewline();

  if (cr == ctl::TapeColor::white) {
    bot.serial_monitor.sendText("On start node");
    node.state |= NS_START;
    res = Node_Type::START;
    if (goal_reached) {
      for (uint8_t i = 0; i < 5; i++) {
        bot.buzzer.beep(200);
        ctl::WaitMillis(200);
      }


      while (true) {}
    }
  } else if (cr == ctl::TapeColor::red) {
    bot.serial_monitor.sendText("On goal node");
    node.state |= NS_GOAL;
    res = Node_Type::GOAL;
    goal_reached = true;
    bot.prox_led_right.on();
    bot.prox_led_left.on();
    bot.prox_led_back.on();
    bot.buzzer.beep(200);
    goto l_BACKTRACK;
  }

  /// Step 2 ///

  // Left wall
  if (bot.prox_left.closed()) {
    bot.serial_monitor.sendText("Left wall closed");
    bot.serial_monitor.sendNewline();
    switch (bot.dir) {
      case (Bot_Dir::NORTH): {
        node.state |= NS_WEST;
        break;
      }
      case (Bot_Dir::EAST): {
        node.state |= NS_NORTH;
        break;
      }
      case (Bot_Dir::SOUTH): {
        node.state |= NS_EAST;
        break;
      }
      case (Bot_Dir::WEST): {
        node.state |= NS_SOUTH;
        break;
      }
    }
  }
  else {
    bot.drive_train.leftTurn(45);
    MoveWithObstaclePauses(bot);

    bot.rs_servo.moveTo(45);
    double range_cm = bot.ultra.pingRangeCM();
    if (range_cm > 20) {
      //bot.rs_servo.moveTo(0);

      bot.drive_train.rightTurn(45);
      MoveWithObstaclePauses(bot);
    }
    else {
      switch (bot.dir) {
        case (Bot_Dir::NORTH): {
          node.state |= NS_WEST;
          break;
        }
        case (Bot_Dir::EAST): {
          node.state |= NS_NORTH;
          break;
        }
        case (Bot_Dir::SOUTH): {
          node.state |= NS_EAST;
          break;
        }
        case (Bot_Dir::WEST): {
          node.state |= NS_SOUTH;
          break;
        }
      }
      bot.drive_train.leftTurn(45);
      MoveWithObstaclePauses(bot);

      double mov_amount = fabs(range_cm - des_dist);

      bot.serial_monitor.sendText("Correction amount");
      bot.serial_monitor.sendDoubleValue(mov_amount);
      bot.serial_monitor.sendNewline();

      bot.drive_train.reverse(mov_amount);
      MoveWithObstaclePauses(bot);

      bot.drive_train.rightTurn(89);
      MoveWithObstaclePauses(bot);
    }
  }

  // Right wall
  if (bot.prox_right.closed()) {
    bot.serial_monitor.sendText("Right wall closed");
    bot.serial_monitor.sendNewline();
    switch (bot.dir) {
      case (Bot_Dir::NORTH): {
        node.state |= NS_EAST;
        break;
      }
      case (Bot_Dir::EAST): {
        node.state |= NS_SOUTH;
        break;
      }
      case (Bot_Dir::SOUTH): {
        node.state |= NS_WEST;
        break;
      }
      case (Bot_Dir::WEST): {
        node.state |= NS_NORTH;
        break;
      }
    }
  }
    else {
    bot.drive_train.rightTurn(45);
    MoveWithObstaclePauses(bot);

    bot.rs_servo.moveTo(135);
    double range_cm = bot.ultra.pingRangeCM();
    if (range_cm > 20) {
      //bot.rs_servo.moveTo(0);

      bot.drive_train.leftTurn(45);
      MoveWithObstaclePauses(bot);
    }
    else {
      switch (bot.dir) {
        case (Bot_Dir::NORTH): {
          node.state |= NS_EAST;
          break;
        }
        case (Bot_Dir::EAST): {
          node.state |= NS_SOUTH;
          break;
        }
        case (Bot_Dir::SOUTH): {
          node.state |= NS_WEST;
          break;
        }
        case (Bot_Dir::WEST): {
          node.state |= NS_NORTH;
          break;
        }
      }
      bot.drive_train.rightTurn(45);
      MoveWithObstaclePauses(bot);

      double mov_amount =  range_cm - des_dist;

      bot.serial_monitor.sendText("Correction amount");
      bot.serial_monitor.sendDoubleValue(mov_amount);
      bot.serial_monitor.sendNewline();

      bot.drive_train.forward(mov_amount);
      MoveWithObstaclePauses(bot);

      bot.drive_train.leftTurn(89);
      MoveWithObstaclePauses(bot);
    }
  }

  // Top wall
  bot.rs_servo.moveTo(90);
  auto range = bot.ultra.pingRangeCM();
  bot.serial_monitor.sendText("Ultrasonic value: ");
  bot.serial_monitor.sendDoubleValue(range);
  bot.serial_monitor.sendNewline();
  if (range < 10 && range > 1) {

    switch (bot.dir) {
      case (Bot_Dir::NORTH): {
        node.state |= NS_NORTH;
        break;
      }
      case (Bot_Dir::EAST): {
        node.state |= NS_EAST;
        break;
      }
      case (Bot_Dir::SOUTH): {
        node.state |= NS_SOUTH;
        break;
      }
      case (Bot_Dir::WEST): {
        node.state |= NS_WEST;
        break;
      }
    }
  }

  if (goal_reached) {
    bot.serial_monitor.sendText("Goal reached! Backtracking...");
    bot.serial_monitor.sendNewline();

    goto l_BACKTRACK;
  }

  /// Step 3 ///

  // Try and find a path we can go to here that we haven't visited before.
  // if that fails, backtrack until we find a node with a path we haven't
  // visited before
  //
  const double NODE_DIST = 195.0;



  // TODO: Function for move normal success and function for dead end.
  if (!straight_closed(nodes, x, y, bot.dir)) {
    Node* o_node = get_node_dir(nodes, x, y, bot.dir,
                                Bot_Dir::NORTH);  // get node straight ahead
    bot.serial_monitor.sendText("Straight wall open");
    bot.serial_monitor.sendNewline();

    if ((o_node->state & NS_VISITED) == 0) {
      bot.serial_monitor.sendText("Moving straight");
      bot.serial_monitor.sendNewline();

      switch (bot.dir) {
        case Bot_Dir::NORTH: {
          y++;
          break;
        }
        case Bot_Dir::EAST: {
          x++;
          break;
        }
        case Bot_Dir::SOUTH: {
          y--;
          break;
        }
        case Bot_Dir::WEST: {
          x--;
          break;
        }
      }

      bot.serial_monitor.sendText("Appending");
      bot.serial_monitor.sendIntegerValue((int)bot.dir);
      bot.serial_monitor.sendNewline();
      stack.append(bot.dir);
      bot.drive_train.reverse(NODE_DIST);
      MoveWithObstaclePauses(bot);
      return res;
    }
    else {
      bot.serial_monitor.sendText("Already visited foreward node");
      bot.serial_monitor.sendNewline();
    }
  }
  else {
    bot.serial_monitor.sendText("Straight wall closed");
    bot.serial_monitor.sendNewline();
  }
  if (!left_closed(nodes, x, y, bot.dir)) {
    Node* o_node = get_node_dir(nodes, x, y, bot.dir,
                                Bot_Dir::WEST);  // get node to the left
    bot.serial_monitor.sendText("Left wall open");
    bot.serial_monitor.sendNewline();

    // goto l_SKIP_LEFT_VISIT;
    if ((o_node->state & NS_VISITED) == 0) {

      l_SKIP_LEFT_VISIT:
      bot.serial_monitor.sendText("Moving left");
      bot.serial_monitor.sendNewline();

      bot.drive_train.leftTurn(90);
      MoveWithObstaclePauses(bot);

      switch (bot.dir) {
        case Bot_Dir::NORTH: {
          x--;
          break;
        }
        case Bot_Dir::EAST: {
          y++;
          break;
        }
        case Bot_Dir::SOUTH: {
          x++;
          break;
        }
        case Bot_Dir::WEST: {
          y--;
          break;
        }
      }

      bot.dir = (Bot_Dir)((((int8_t)bot.dir - 1 % 4) + 4) % 4);
      bot.serial_monitor.sendText("Appending");
      bot.serial_monitor.sendIntegerValue((int)bot.dir);
      bot.serial_monitor.sendNewline();
      stack.append(bot.dir);
      bot.drive_train.reverse(NODE_DIST);
      MoveWithObstaclePauses(bot);

      return res;
    }
    else {
      bot.serial_monitor.sendText("Already visited left node");
      bot.serial_monitor.sendNewline();
    }
  }
  else {
    bot.serial_monitor.sendText("Left wall closed");
    bot.serial_monitor.sendNewline();
  }

  if (!right_closed(nodes, x, y, bot.dir)) {
    Node* o_node = get_node_dir(nodes, x, y, bot.dir,
                                Bot_Dir::EAST);  // get node to the right
    bot.serial_monitor.sendText("Right wall open");
    bot.serial_monitor.sendNewline();
    if ((o_node->state & NS_VISITED) == 0) {
      bot.serial_monitor.sendText("Moving right");
        bot.serial_monitor.sendNewline();

      bot.drive_train.rightTurn(90);
      MoveWithObstaclePauses(bot);

      switch (bot.dir) {
        case Bot_Dir::NORTH: {
          x++;
          break;
        }
        case Bot_Dir::EAST: {
          y--;
          break;
        }
        case Bot_Dir::SOUTH: {
          x--;
          break;
        }
        case Bot_Dir::WEST: {
          y++;
          break;
        }
      }

      bot.dir = (Bot_Dir)((((int8_t)bot.dir + 1 % 4) + 4) % 4);
      bot.serial_monitor.sendText("Appending");
      bot.serial_monitor.sendIntegerValue((int)bot.dir);
      bot.serial_monitor.sendNewline();
      stack.append(bot.dir);  // Should update bot dir first!
      bot.drive_train.reverse(NODE_DIST);
      MoveWithObstaclePauses(bot);

      return res;
    }
    else {
      bot.serial_monitor.sendText("Already visited right node");
      bot.serial_monitor.sendNewline();
    }
  }
  else {
    bot.serial_monitor.sendText("Right wall closed");
    bot.serial_monitor.sendNewline();
  }

l_BACKTRACK:

  // If we get here, we need to backtrack and pop off the stack
  bot.serial_monitor.sendText("Fell through. Backtracking.");
  bot.serial_monitor.sendNewline();

  Bot_Dir last_bot_dir = stack.pop(); // Popped direction is the one we previously moved towards
  bot.serial_monitor.sendText("Last dir");
  bot.serial_monitor.sendIntegerValue((int)last_bot_dir);
  bot.serial_monitor.sendNewline();

  Bot_Dir new_bot_dir = (Bot_Dir)(((((int8_t)last_bot_dir - 2) % 4) + 4) % 4); // Calculate opposite direction for backtracking
  bot.serial_monitor.sendText("New direction: ");
  bot.serial_monitor.sendIntegerValue((int)new_bot_dir);
  bot.serial_monitor.sendNewline();

  uint8_t right_turns = (((((int8_t)new_bot_dir - (int8_t)bot.dir) % 4) + 4) % 4);
  bot.serial_monitor.sendText("Num right turns");
  bot.serial_monitor.sendIntegerValue(right_turns);
  bot.serial_monitor.sendNewline();

  bot.drive_train.rightTurn(90 * right_turns);
  MoveWithObstaclePauses(bot);

  bot.dir = new_bot_dir; // Update bot direction after turning

  // Adjust coordinates according to the new direction
  switch (new_bot_dir) {
    case Bot_Dir::NORTH:
      y++;
      break;
    case Bot_Dir::EAST:
      x++;
      break;
    case Bot_Dir::SOUTH:
      y--;
      break;
    case Bot_Dir::WEST:
      x--;
      break;
  }

  bot.drive_train.reverse(NODE_DIST);
  MoveWithObstaclePauses(bot);

  return res;
}
