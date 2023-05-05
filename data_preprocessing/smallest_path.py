import os
import sys

import pandas as pd


def save_best_result(csv_dir):
    output_path = os.path.join(csv_dir, "result.csv")    

    # Create an empty DataFrame to store the combined data
    df_combined = pd.DataFrame()

    # Loop through each file in the directory and append the data to the DataFrame
    for file in os.listdir(csv_dir):
        if file.endswith('.csv') and file.startswith('result'):
            df = pd.read_csv(os.path.join(csv_dir, file))
            df_combined = df_combined.append(df, ignore_index=True)

    # Group the data by a certain attribute and find the smallest value for each group
    df_grouped = df_combined.groupby('iteration')['pathLength'].min().reset_index()

    df_grouped.to_csv(output_path, index=True)


def main(): 
    if len(sys.argv) < 2:
        raise ValueError("Incorrect number of arguments. At least path of one directory should be passed.")

    for folder in sys.argv[1:]:
        save_best_result(folder)


if __name__ == "__main__":
    main()

