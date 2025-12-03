# frozen_string_literal: true

# Stack to track positions during pathfinding
class PositionStack
  attr_accessor :current_index, :move_count
  attr_reader :rows, :columns

  MAX_SIZE = 10_000 # Maximum stack size

  def initialize
    @current_index = 0
    @move_count = 0
    @rows = Array.new(MAX_SIZE, 0)
    @columns = Array.new(MAX_SIZE, 0)
  end

  # Get current position
  def current_position
    [@rows[@current_index], @columns[@current_index]]
  end

  # Set initial position
  def set_start_position(row, col)
    @rows[0] = row
    @columns[0] = col
    @current_index = 0
    @move_count = 0
  end

  # Push new position onto stack
  def push(row, col)
    @current_index += 1
    @rows[@current_index] = row
    @columns[@current_index] = col
    @move_count += 1
  end

  # Pop position from stack (backtrack)
  def pop
    @current_index -= 1 if @current_index > 0
  end

  # Get position at specific index
  def position_at(index)
    [@rows[index], @columns[index]]
  end
end
