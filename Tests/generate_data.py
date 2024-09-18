import os
import random

def generate_random_hex_line(length):
    """Generate a string of random hexadecimal bytes with no spaces."""
    return ''.join(f'{random.randint(0, 255):02x}' for _ in range(length))

def generate_hex_file(filename, num_lines, line_length):
    """Generate a file with a specified number of lines, each containing random hex bytes."""
    with open(filename, 'w') as file:
        for _ in range(num_lines):
            line = generate_random_hex_line(line_length)
            file.write(line + '\n')

if __name__ == "__main__":
    output_file = 'tx_test_data.txt'
    lines_count = 100
    bytes_per_line = 250
    
    generate_hex_file(output_file, lines_count, bytes_per_line)
    print(f"File '{output_file}' with {lines_count} lines of {bytes_per_line} random hex bytes each has been created.")
