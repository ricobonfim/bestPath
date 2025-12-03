# frozen_string_literal: true

require_relative 'map_info'
require_relative 'position_stack'
require_relative 'direction_mask'

# Main pathfinding class using genetic algorithm masks
class Pathfinder
  GOAL_CHAR = 'y'
  START_CHAR = 'x'
  WALL_CHAR = '*'
  EMPTY_CHAR = ' '
  VISITED_CHAR = 'x'

  # Return codes for movement
  MOVE_FOUND_GOAL = 2
  MOVE_SUCCESS = 1
  MOVE_BLOCKED = 0
  MOVE_ALREADY_VISITED = -1

  def initialize(map_filename)
    @map_filename = map_filename
    @map_info = MapInfo.new
    @mask = DirectionMask.new
  end

  # Find start position (marked with 'x') in the map
  def find_start_position(stack)
    @map_info.rows.times do |row|
      @map_info.columns.times do |col|
        if @map_info.char_at(row, col) == START_CHAR
          stack.set_start_position(row, col)
          return true
        end
      end
    end
    false
  end

  # Move in specified direction and update stack
  def move(stack, direction)
    current_row, current_col = stack.current_position

    # Calculate new position based on direction
    new_row, new_col = case direction
                       when DirectionMask::DIRECTION_UP
                         [current_row - 1, current_col]
                       when DirectionMask::DIRECTION_DOWN
                         [current_row + 1, current_col]
                       when DirectionMask::DIRECTION_LEFT
                         [current_row, current_col - 1]
                       when DirectionMask::DIRECTION_RIGHT
                         [current_row, current_col + 1]
                       else
                         return MOVE_BLOCKED
                       end

    target_char = @map_info.char_at(new_row, new_col)

    case target_char
    when GOAL_CHAR
      MOVE_FOUND_GOAL
    when WALL_CHAR
      MOVE_BLOCKED
    when VISITED_CHAR
      MOVE_ALREADY_VISITED
    when EMPTY_CHAR
      @map_info.set_char_at(new_row, new_col, VISITED_CHAR)
      stack.push(new_row, new_col)
      MOVE_SUCCESS
    else
      @map_info.display
      puts "Unknown symbol in map: '#{target_char}'"
      exit(1)
    end
  end

  # Traverse the map using the direction mask
  def traverse_map(stack)
    mask_index = 0
    direction_index = 0
    failure_count = 0
    reset_counter = 0
    max_iterations = 50_000 # Prevent infinite loops
    iterations = 0

    loop do
      iterations += 1
      if iterations > max_iterations
        # Path not found within iteration limit
        return false
      end

      # Reset direction index if we've tried all 4 directions
      direction_index = 0 if direction_index >= 4

      # Reset step counts every 10 iterations if stuck
      if reset_counter >= 10
        @mask.reset_step_counts
        reset_counter = 0
        mask_index = 0
        next
      end

      # Skip masks with no steps remaining, wrap around to mask 0
      if mask_index > 8 || @mask.steps_for_mask(mask_index) == 0
        mask_index += 2
        if mask_index > 8
          mask_index = 0
        end
        reset_counter += 1
        next
      end

      # Get current direction and attempt to move
      current_direction = @mask.direction_at(mask_index, direction_index)
      result = move(stack, current_direction)

      case result
      when MOVE_FOUND_GOAL
        return true # Success!
      when MOVE_SUCCESS
        @mask.decrease_steps(mask_index)
        failure_count = 0
      when MOVE_BLOCKED, MOVE_ALREADY_VISITED
        failure_count += 1
        direction_index += 1
      end

      # If all 4 directions failed, backtrack
      if failure_count >= 4
        # Can't backtrack if we're at the start
        return false if stack.current_index <= 0

        stack.pop
        failure_count = 0
        direction_index = 0
      end
    end
  end  # Fitness function for genetic algorithm
  # Returns: 1 / (1 + number_of_steps)
  # Lower step count = higher fitness
  def fitness(genetic_bytes)
    # Reload map for fresh state
    @map_info.load_from_file(@map_filename)

    # Initialize stack
    stack = PositionStack.new

    # Find starting position
    unless find_start_position(stack)
      return 0.0
    end

    # Create direction mask from genetic algorithm bytes
    begin
      @mask.create_from_bytes(genetic_bytes)
    rescue StandardError => e
      return 0.0
    end

    # Traverse the map
    found_goal = false
    begin
      found_goal = traverse_map(stack)
    rescue StandardError => e
      return 0.0
    end

    # Calculate fitness: fewer steps = higher fitness
    # If goal not found, fitness is very low
    return 0.001 unless found_goal

    steps = stack.move_count
    1.0 / (1.0 + steps)
  end

  # Display final map state
  def display_map
    @map_info.display
  end
end
