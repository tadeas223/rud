import subprocess
import time
import sys
import re

def parse_perf_output(stderr_text):
    """
    Parse perf stat output for cache-misses and branch-misses.
    Returns a tuple: (cache_misses, branch_misses)
    """
    cache_misses = None
    branch_misses = None

    # perf output lines look like: 
    #  1,234,567      cache-misses
    for line in stderr_text.splitlines():
        line = line.strip()
        match = re.match(r'([\d,]+)\s+([a-z-]+)', line)
        if match:
            count_str, event = match.groups()
            count = int(count_str.replace(',', ''))
            if event == "cache-misses":
                cache_misses = count
            elif event == "branch-misses":
                branch_misses = count
    return cache_misses, branch_misses

def run_cpp_benchmark(executable_path, iterations=10, use_perf=True):
    times = []
    total_cache_misses = 0
    total_branch_misses = 0
    perf_counts_collected = 0

    for i in range(iterations):
        if use_perf:
            perf_cmd = [
                "perf", "stat",
                "-e", "cache-misses,branch-misses",
                executable_path
            ]
            print(f"Iteration {i+1}/{iterations}: Running with perf...")
            result = subprocess.run(perf_cmd, capture_output=True, text=True)
            
            cache_misses, branch_misses = parse_perf_output(result.stderr)
            if cache_misses is not None and branch_misses is not None:
                total_cache_misses += cache_misses
                total_branch_misses += branch_misses
                perf_counts_collected += 1
            else:
                print("Warning: perf failed to report counts for this iteration.")

            # Measure wall-clock time separately
            start = time.perf_counter()
            subprocess.run([executable_path])
            end = time.perf_counter()
            times.append(end - start)
        else:
            start = time.perf_counter()
            subprocess.run([executable_path])
            end = time.perf_counter()
            times.append(end - start)

    avg_time = sum(times) / len(times)
    min_time = min(times)
    max_time = max(times)
    
    print("\n==== Benchmark Results ====")
    print(f"Runs: {iterations}")
    print(f"Average execution time: {avg_time:.6f} s")
    print(f"Min execution time: {min_time:.6f} s")
    print(f"Max execution time: {max_time:.6f} s")

    if use_perf and perf_counts_collected > 0:
        avg_cache_misses = total_cache_misses // perf_counts_collected
        avg_branch_misses = total_branch_misses // perf_counts_collected
        print(f"Average cache misses: {avg_cache_misses}")
        print(f"Average branch misses: {avg_branch_misses}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python benchmark.py <path_to_executable> [iterations]")
        sys.exit(1)

    exe_path = sys.argv[1]
    iterations = int(sys.argv[2]) if len(sys.argv) > 2 else 10

    run_cpp_benchmark(exe_path, iterations)

