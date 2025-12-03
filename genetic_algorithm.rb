# frozen_string_literal: true

# Genetic Algorithm implementation for optimization
class GeneticAlgorithm
  attr_reader :best_solution, :num_bytes

  def initialize(options = {})
    @population_size = options[:population_size] || 100
    @num_generations = options[:num_generations] || 1000
    @mutation_rate = options[:mutation_rate] || 60 # Percentage
    @num_bits = options[:num_bits] || 80
    @show_count = options[:show_count] || 0

    @num_bytes = (@num_bits / 8.0).ceil
    @population = []
    @fitness_values = []
    @sorted_indices = []
    @best_solution = nil
  end

  # Run genetic algorithm
  def run(fitness_function)
    initialize_population
    evaluate_fitness(fitness_function)
    sort_by_fitness
    display_generation(0) if @show_count > 0

    @num_generations.times do |generation|
      crossover
      mutate
      evaluate_fitness(fitness_function)
      sort_by_fitness
      display_generation(generation + 1) if @show_count > 0
    end

    @best_solution = @population[@sorted_indices[0]]
    self
  end

  private

  # Initialize random population
  def initialize_population
    @population = Array.new(@population_size) do
      Array.new(@num_bytes) { rand(256) }
    end
  end

  # Evaluate fitness for entire population
  def evaluate_fitness(fitness_function)
    @fitness_values = @population.map do |individual|
      fitness_function.call(individual)
    end
  end

  # Sort population by fitness (descending)
  def sort_by_fitness
    @sorted_indices = (0...@population_size).to_a.sort_by do |i|
      -@fitness_values[i] # Negative for descending sort
    end
  end

  # Select individual using tournament selection with bias towards better fitness
  def select_parent
    # Use power function to bias selection towards better individuals
    normalized = (rand * 1_000_000.0 / 1_000_001.0) ** 1.5
    index = (normalized * @population_size).to_i
    @sorted_indices[index]
  end

  # Crossover operation to create new population
  def crossover
    new_population = []
    elite = @population[@sorted_indices[0]].dup # Keep best solution (elitism)

    (@population_size / 2).times do
      # Select two different parents
      parent1_idx = select_parent
      parent2_idx = select_parent
      parent2_idx = select_parent while parent1_idx == parent2_idx

      parent1 = @population[parent1_idx]
      parent2 = @population[parent2_idx]

      # Random crossover point
      crossover_bit = rand(@num_bits)
      crossover_byte = crossover_bit / 8
      crossover_bit_in_byte = crossover_bit % 8

      child1 = []
      child2 = []

      @num_bytes.times do |byte_idx|
        if byte_idx < crossover_byte
          child1 << parent1[byte_idx]
          child2 << parent2[byte_idx]
        elsif byte_idx > crossover_byte
          child1 << parent2[byte_idx]
          child2 << parent1[byte_idx]
        else
          # Split at bit level for crossover byte
          mask = (1 << crossover_bit_in_byte) - 1
          child1_byte = (parent1[byte_idx] & mask) | (parent2[byte_idx] & ~mask)
          child2_byte = (parent1[byte_idx] & ~mask) | (parent2[byte_idx] & mask)
          child1 << child1_byte
          child2 << child2_byte
        end
      end

      new_population << child1
      new_population << child2
    end

    # Apply elitism: replace first individual with elite
    new_population[0] = elite
    @population = new_population
  end

  # Mutation operation
  def mutate
    num_mutations = ((@population_size * @num_bytes * @mutation_rate) / 100.0).to_i

    num_mutations.times do
      # Random byte in population
      byte_position = rand(@population_size * @num_bytes)
      individual_idx = byte_position / @num_bytes
      byte_idx = byte_position % @num_bytes

      # Random bit to flip
      bit_position = rand(8)

      # Flip the bit
      @population[individual_idx][byte_idx] ^= (1 << bit_position)
    end
  end

  # Display best individuals from current generation
  def display_generation(generation)
    puts "\n=== Generation #{generation} ==="
    [@show_count, @population_size].min.times do |i|
      idx = @sorted_indices[i]
      fitness = @fitness_values[idx]
      individual = @population[idx]

      puts format('%3d %5.2f - %s', idx, fitness, individual.join(' '))
    end
  end
end
