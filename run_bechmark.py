import glob
import os
import subprocess

import matplotlib.pyplot as plt
import numpy as np

def main(file_path):
    directory = 'tests/benchmark'
    pattern = os.path.join(directory, 'test_*.dat')
    test_files = glob.glob(pattern)
    test_files.sort()
    params = []

    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split()
            
            row = [int(part) for part in parts]
            
            params.append(f"{row[0]}:{row[1]}")

    programs = {
        'std::set' : [],
        'rbtree' : [],
    }

    command_set = ['./build/main_set', '-t', '-h']
    command_rbtree = ['./build/main_rbtree', '-t', '-h']
    for file_path in test_files:
        with open(file_path, 'r') as file:
            result = subprocess.run(command_rbtree, stdin=file, capture_output=True, text=True)
            programs['rbtree'].append(float(result.stdout))

        with open(file_path, 'r') as file:
            result = subprocess.run(command_set, stdin=file, capture_output=True, text=True)
            programs['std::set'].append(float(result.stdout))

    # Plotting the benchmark results
    bar_width = 0.35  # Width of the bars
    index = np.arange(len(params))  # X locations for the groups
    
    fig, ax = plt.subplots()
    
    # Create bars for program1 and program2
    bar1 = ax.bar(index, programs['rbtree'], bar_width, label='mydistance')
    bar2 = ax.bar(index + bar_width, programs['std::set'], bar_width, label='std::distance')
    
    # Labels and title
    ax.set_xlabel('k - number of keys : q - number of queries')
    ax.set_ylabel('Execution time (seconds)')
    ax.set_title('Benchmark of std::distance vs mydistance')
    ax.set_xticks(index + bar_width / 2)
    ax.set_xticklabels(params)
    ax.legend()
    
    # Show the plot
    plt.tight_layout()
    plt.savefig('result.png')

if __name__ == "__main__":
    file_path = "gen_params_benchmark.txt"
    main(file_path)
