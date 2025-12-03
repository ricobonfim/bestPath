# Best Path Finder - Ruby Implementation

A genetic algorithm implementation to find the optimal path through a maze from start ('x') to goal ('y').

## Overview

This project uses a genetic algorithm to evolve direction masks that guide a pathfinding agent through a maze. The algorithm optimizes for the shortest path by evolving strategies over multiple generations.

## Features

- **Genetic Algorithm**: Population-based optimization with crossover, mutation, and elitism
- **Direction Masks**: Encodes movement strategies as byte sequences
- **Pathfinding**: Explores maze using evolved direction priorities
- **Clean Architecture**: Separated concerns with dedicated classes for each component

## Components

### Core Classes

- **`MapInfo`**: Manages maze layout, dimensions, and character operations
- **`PositionStack`**: Tracks agent positions during pathfinding
- **`DirectionMask`**: Encodes and decodes genetic direction strategies
- **`Pathfinder`**: Main pathfinding logic using direction masks
- **`GeneticAlgorithm`**: Genetic algorithm implementation with selection, crossover, and mutation

## Usage

Run the program with a map file:

```bash
ruby main.rb test_simple.txt
```

### Map Format

Maps must follow these rules:

- **Start**: Marked with `x` (exactly one)
- **Goal**: Marked with `y` (exactly one)
- **Walls**: Marked with `*`
- **Empty spaces**: Marked with space ` `
- **Map must be fully enclosed** with walls

Example map:
```
*******
*x   y*
* *** *
*     *
*******
```

## Algorithm Details

### Direction Encoding

- **0**: Up
- **1**: Left
- **2**: Down
- **3**: Right

### Genetic Algorithm Parameters

- **Population Size**: 100 individuals
- **Generations**: 1000
- **Mutation Rate**: 60%
- **Chromosome Size**: 80 bits (10 bytes)

### Fitness Function

Fitness = 1 / (1 + steps_taken)

Lower step count = higher fitness

### Genetic Operations

1. **Selection**: Tournament selection with power bias (1.5) towards better fitness
2. **Crossover**: Single-point crossover at bit level
3. **Mutation**: Random bit flipping based on mutation rate
4. **Elitism**: Best solution always preserved in next generation

## Improvements Over Original C Code

### Readability
- Object-oriented design with clear separation of concerns
- Descriptive method and variable names in English
- Comprehensive comments explaining complex logic
- Constants for magic numbers
- Type-safe operations

### Bug Fixes
1. **Fixed operator precedence**: Changed `byte & 1 == 1` to `(byte & 1) == 1`
2. **Better bounds checking**: Added validation for array access
3. **Memory management**: Ruby's automatic garbage collection eliminates memory leaks
4. **Error handling**: Proper exception handling and validation

### Architecture
- Modular classes instead of global variables
- Encapsulated state management
- Reusable components
- Better testability

### Code Quality
- Consistent naming conventions
- DRY principle (Don't Repeat Yourself)
- Single Responsibility Principle
- Clear method signatures with explicit return values

## File Structure

```
bestpath/
├── main.rb                  # Entry point
├── genetic_algorithm.rb     # GA implementation
├── pathfinder.rb           # Pathfinding logic
├── direction_mask.rb       # Direction encoding/decoding
├── position_stack.rb       # Position tracking
├── map_info.rb            # Map management
├── test_simple.txt        # Sample map 1
├── test_medium.txt        # Sample map 2
└── README.md              # This file
```

## Requirements

- Ruby 2.7 or higher
- No external gems required (uses only standard library)

## Example Output

```
Loading map from: test_simple.txt
Starting genetic algorithm optimization...

=== RESULTS ===
Using 10 bytes:
145 67 89 123 45 78 156 234 12 90

Final map state:
[Map displayed with path marked]

Optimization complete!
```

## License

Open source - feel free to use and modify.
