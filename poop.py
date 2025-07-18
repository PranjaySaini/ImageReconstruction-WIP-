import sys
import uproot
import numpy as np
import matplotlib.pyplot as plt
import os
import math

def main(run_id):
    run_dir = f"build/Run_{run_id}"
    root_file = os.path.join(run_dir, f"output_run_{run_id}.root")

    with uproot.open(root_file) as file:
        # Get detector histogram keys like 'Detector_0;1'
        raw_keys = [k for k in file.keys() if k.startswith("Detector_")]
        # Keep only latest version for each detector (strip ;1 etc.)
        unique_keys = {}
        for k in raw_keys:
            base = k.split(";")[0]
            if base not in unique_keys:
                unique_keys[base] = k  # Save full key (with version)

        # Sort by detector index
        sorted_keys = sorted(unique_keys.items(), key=lambda x: int(x[0].split("_")[1]))

        edeps = []
        for base_name, full_key in sorted_keys:
            try:
                hist = file[full_key]
                # Get bin contents (skip underflow/overflow)
                values = hist.values(flow=False)
                total_energy = np.sum(values)
                edeps.append(total_energy)
            except Exception as e:
                print(f"Error reading {full_key}: {e}")
                edeps.append(0)

        n = len(edeps)
        grid_size = int(math.ceil(math.sqrt(n)))

        if grid_size * grid_size != n:
            print("Warning: Detectors do not form a square grid. Padding with zeros.")
            padded_edeps = edeps + [0] * (grid_size**2 - n)
            data = np.array(padded_edeps).reshape((grid_size, grid_size))
            label_source = padded_edeps
        else:
            data = np.array(edeps).reshape((grid_size, grid_size))
            label_source = edeps

        # Plot the heatmap
        plt.figure(figsize=(6, 5))
        plt.imshow(data, cmap='plasma', interpolation='nearest')
        plt.title(f"Energy Deposition Heatmap - Run {run_id}")
        plt.axis("off")  # Hide axes for a cleaner look
        plt.colorbar(label="Total Energy Deposited (MeV)")

        # Annotate each cell with the detector number
        for idx, energy in enumerate(label_source):
            row = idx // grid_size
            col = idx % grid_size
            plt.text(col, row, str(idx), ha='center', va='center', color='white',
                     fontsize=8, fontweight='bold')

        plt.tight_layout()
        output_path = os.path.join(run_dir, f"heatmap_run_{run_id}.png")
        plt.savefig(output_path)
        print(f"Heatmap with detector numbers saved to {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 generate_heatmap.py <run_id>")
    else:
        main(sys.argv[1])

