#!/usr/bin/env python3
"""
Spooky2 to JSON Converter
Converts Spooky2 .txt preset files to JSON format for ESP32 RIFE device.
"""

import os
import re
import json
from pathlib import Path
from typing import List, Dict, Any

SPOOKY2_DIR = Path(r"C:\Spooky2\Preset Collections\JW_Peptides\Animals\Contact")
OUTPUT_FILE = Path(__file__).parent / "data" / "frequencies.json"

def parse_spooky2_file(filepath: Path) -> Dict[str, Any]:
    """Parse a single Spooky2 preset file."""
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    preset = {
        "name": "",
        "mode": "Contact",
        "category": "Animals",
        "description": "",
        "programs": []
    }
    
    # Extract preset name
    match = re.search(r'PresetName=(.+)', content)
    if match:
        preset["name"] = match.group(1).strip()
    
    # Extract notes/description
    match = re.search(r'Preset_Notes=(.+)', content, re.DOTALL)
    if match:
        preset["description"] = match.group(1).strip().replace('\n', ' ')
    
    # Extract animal category from filename
    filename = filepath.stem
    if "Dog" in filename:
        preset["animal_category"] = "Dog"
    elif "Cat" in filename:
        preset["animal_category"] = "Cat"
    elif "Chicken" in filename:
        preset["animal_category"] = "Chicken"
    elif "Horse" in filename:
        preset["animal_category"] = "Horse"
    elif "Cattle" in filename:
        preset["animal_category"] = "Cattle"
    elif "Sheep" in filename:
        preset["animal_category"] = "Sheep"
    elif "Rabbit" in filename:
        preset["animal_category"] = "Rabbit"
    elif "Bird" in filename:
        preset["animal_category"] = "Bird"
    else:
        preset["animal_category"] = "Other"
    
    # Extract loaded programs
    program_names = re.findall(r'Loaded_Programs=(.+)', content)
    
    # Extract frequency sets
    freq_matches = re.findall(r'Loaded_Frequencies=(.+)', content)
    
    for idx, freq_str in enumerate(freq_matches):
        program = {
            "id": f"p{idx+1:02d}",
            "name": program_names[idx] if idx < len(program_names) else f"Program {idx+1}",
            "frequencies": []
        }
        
        # Parse frequencies: M128.095=1 (offset 1), M156.101 (no offset), 0=4 (separator)
        freq_items = freq_str.split(',')
        for item in freq_items:
            item = item.strip()
            if not item or item == '0=4':
                continue
            
            # Match M frequency format
            match = re.match(r'M(\d+\.?\d*)=?(\d*)', item)
            if match:
                freq_value = float(match.group(1))
                offset = int(match.group(2)) if match.group(2) else 0
                program["frequencies"].append({
                    "hz": freq_value,
                    "offset": offset
                })
        
        if program["frequencies"]:
            preset["programs"].append(program)
    
    return preset

def categorize_presets(presets: List[Dict]) -> Dict[str, List[Dict]]:
    """Group presets by animal category."""
    categories = {}
    for preset in presets:
        cat = preset.get("animal_category", "Other")
        if cat not in categories:
            categories[cat] = []
        categories[cat].append(preset)
    return categories

def main():
    """Main conversion function."""
    print(f"Scanning: {SPOOKY2_DIR}")
    
    if not SPOOKY2_DIR.exists():
        print(f"Error: Directory not found: {SPOOKY2_DIR}")
        return
    
    # Find all .txt files
    txt_files = list(SPOOKY2_DIR.glob("*.txt"))
    print(f"Found {len(txt_files)} preset files")
    
    presets = []
    for filepath in txt_files:
        try:
            preset = parse_spooky2_file(filepath)
            if preset["programs"]:
                presets.append(preset)
                print(f"  + {preset['name']} ({len(preset['programs'])} programs)")
            else:
                print(f"  - {preset['name']} (no frequencies)")
        except Exception as e:
            print(f"  ! Error processing {filepath.name}: {e}")
    
    # Sort presets by name
    presets.sort(key=lambda p: p["name"])
    
    # Group by category
    categorized = categorize_presets(presets)
    
    # Create output structure
    output = {
        "version": "1.0",
        "source": "Spooky2 JW_Peptides Animals Contact",
        "total_presets": len(presets),
        "categories": list(categorized.keys()),
        "presets": presets
    }
    
    # Ensure output directory exists
    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    
    # Write JSON
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        json.dump(output, f, indent=2, ensure_ascii=False)
    
    print(f"\nGenerated: {OUTPUT_FILE}")
    print(f"Total presets: {len(presets)}")
    for cat, cat_presets in categorized.items():
        print(f"  {cat}: {len(cat_presets)}")

if __name__ == "__main__":
    main()
