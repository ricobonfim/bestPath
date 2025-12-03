# frozen_string_literal: true

# Represents information about a map including its layout and dimensions
class MapInfo
  attr_accessor :path, :rows, :columns

  MAX_ROWS = 100
  MAX_COLUMNS = 100

  def initialize
    @path = Array.new(MAX_ROWS) { Array.new(MAX_COLUMNS) }
    @rows = 0
    @columns = 0
  end

  # Load map from file
  def load_from_file(filename)
    @rows = 0
    @columns = 0

    File.open(filename, 'r') do |file|
      file.each_line do |line|
        @path[@rows] = line.chomp.chars
        @rows += 1
      end
    end

    @columns = @path[0].length if @rows > 0
  end

  # Get character at position
  def char_at(row, col)
    return nil if row < 0 || row >= @rows || col < 0 || col >= @columns
    @path[row][col]
  end

  # Set character at position
  def set_char_at(row, col, char)
    return false if row < 0 || row >= @rows || col < 0 || col >= @columns
    @path[row][col] = char
    true
  end

  # Print the map to console
  def display
    system('clear') || system('cls')
    @rows.times do |i|
      puts "\t\t\t\t\t\t#{@path[i].join}"
    end
  end
end
