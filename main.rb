#!/usr/bin/env ruby
# frozen_string_literal: true

require_relative 'pathfinder'
require_relative 'genetic_algorithm'

# Main program to find best path through maze using genetic algorithm
def main
  if ARGV.length != 1
    puts 'Usage: ruby main.rb <map_file_path>'
    puts 'Example: ruby main.rb test_simple.txt'
    exit(1)
  end

  map_file = ARGV[0]

  unless File.exist?(map_file)
    puts "Error: Map file '#{map_file}' not found"
    exit(1)
  end

  puts "Loading map from: #{map_file}"
  puts "Starting genetic algorithm optimization...\n\n"

  # Create pathfinder
  pathfinder = Pathfinder.new(map_file)

  # Configure genetic algorithm
  ga_options = {
    population_size: 50,    # Population size
    num_generations: 100,   # Number of generations
    mutation_rate: 60,      # Mutation rate percentage
    num_bits: 80,           # Number of bits to optimize
    show_count: 1           # Show top solution per generation
  }

  ga = GeneticAlgorithm.new(ga_options)

  # Define fitness function
  fitness_fn = ->(bytes) { pathfinder.fitness(bytes) }

  # Run genetic algorithm
  ga.run(fitness_fn)

  # Display results
  puts "\n\n=== RESULTS ==="
  puts "Using #{ga.num_bytes} bytes:\n"
  puts ga.best_solution.join(' ')

  # Run pathfinder one more time with best solution to show final path
  puts "\n\nFinal map state:"
  pathfinder.fitness(ga.best_solution)
  pathfinder.display_map

  puts "\n\nOptimization complete!"
end

# Run main program
main if __FILE__ == $PROGRAM_NAME
