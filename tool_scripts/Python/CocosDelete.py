import argparse
import sys
import json
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", type=str, required=True, help="Path to config json")
    args = parser.parse_args()
    config_file = args.config
   
    with open(config_file, "r", encoding="utf-8") as f:
        config = json.load(f)
    for value in config:
        os.remove(value)