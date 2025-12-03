# frozen_string_literal: true

# Represents direction masks for pathfinding
# Directions: 0=UP, 1=LEFT, 2=DOWN, 3=RIGHT
class DirectionMask
  attr_reader :masks

  DIRECTION_UP = 0
  DIRECTION_LEFT = 1
  DIRECTION_DOWN = 2
  DIRECTION_RIGHT = 3

  def initialize
    # masks[i][0-3] = 4 directions in order for mask i
    # masks[i+1][0] = number of steps to follow with mask[i]
    @masks = Array.new(10) { Array.new(4, 0) }
  end

  # Create mask from genetic algorithm bytes
  # Takes 10 bytes: pairs of (direction_byte, step_count)
  def create_from_bytes(bytes)
    raise ArgumentError, 'Expected 10 bytes' unless bytes.length == 10

    0.step(8, 2) do |i|
      direction_byte = bytes[i]
      step_count = bytes[i + 1]

      @masks[i + 1][0] = step_count # Store step count

      # Extract first direction from 2 most significant bits
      first_dir = (direction_byte >> 6) & 0x03

      # Extract second direction from bits 1-5, modulo 3
      second_dir_raw = ((direction_byte & 0x3E) >> 1) % 3
      second_dir = (first_dir == second_dir_raw) ? (first_dir + 1) % 4 : second_dir_raw

      @masks[i][0] = first_dir
      @masks[i][1] = second_dir

      # Track which directions have been used
      used_directions = 0
      used_directions |= (1 << first_dir)
      used_directions |= (1 << second_dir)

      # Validate we have exactly 2 directions set
      unless valid_used_directions?(used_directions)
        raise 'Error creating mask: invalid direction combination'
      end

      # Use least significant bit to determine order of remaining directions
      use_larger_first = (direction_byte & 1) == 1
      remaining = get_remaining_directions(used_directions, use_larger_first)

      @masks[i][2] = remaining[0]
      @masks[i][3] = remaining[1]
    end
  end

  # Get direction at specific mask and position
  def direction_at(mask_index, position)
    return 0 if mask_index >= @masks.length || position >= 4
    @masks[mask_index][position] || 0
  end

  # Get step count for mask
  def steps_for_mask(mask_index)
    return 0 if mask_index + 1 >= @masks.length
    @masks[mask_index + 1][0] || 0
  end

  # Decrease step count for mask
  def decrease_steps(mask_index)
    return if mask_index + 1 >= @masks.length
    @masks[mask_index + 1][0] -= 1 if @masks[mask_index + 1][0] && @masks[mask_index + 1][0] > 0
  end

  # Reset step counts
  def reset_step_counts
    [1, 3, 5, 7, 9].each do |i|
      @masks[i][0] = 255
    end
  end

  private

  # Check if used_directions represents exactly 2 valid directions
  def valid_used_directions?(used)
    # Valid combinations: 3, 5, 6, 9, 10, 12 (binary combinations of 2 bits set)
    [0b0011, 0b0101, 0b0110, 0b1001, 0b1010, 0b1100].include?(used)
  end

  # Get remaining 2 directions not yet used
  def get_remaining_directions(used, larger_first)
    remaining = []
    4.times do |dir|
      remaining << dir unless (used & (1 << dir)) != 0
    end

    larger_first ? remaining.reverse : remaining
  end
end
